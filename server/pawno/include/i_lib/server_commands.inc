public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if ( p_s_info [ playerid ] [ p_logged ] == false ) return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Вы не авторизовались!" ) ? 1 : 1,false ;
	if ( p_s_info [ playerid ] [ p_dialog ] ) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Нельзя использовать команды, пока открыт диалог!"),false;
    return 1;
}

CMD:serverinfo(playerid, params[]) 
{
	SendClientMessage ( playerid, COLOR_LIGHTBLUE, "------[SERVER INFO]------" ) ;
	new text_string [ 128 ];
	format ( text_string, sizeof ( text_string ), "Колличество телепортов: %d", tp_areas_amount ) ;
	SendClientMessage ( playerid, COLOR_YELLOW, text_string ) ;
	return 1;
}
CMD:veh(playerid, params[])
{
    if ( GetPVarInt ( playerid, "admin_vehicle" ) ) return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Вы уже создавали транспорт!" ) ;
	if ( GetPlayerState ( playerid ) == PLAYER_STATE_DRIVER ) return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Нельзя использовать в транспорте!") ;
	if ( sscanf ( params, "ddd", params [ 0  ], params [ 1  ], params [ 2 ] ) ) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Введите: /veh [id транспорта] [Цвет №1] [Цвет №2]") ;
	if ( params [ 0 ] < 400 || params [ 0 ] > 611) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Введите: /veh [id транспорта] [Цвет №1] [Цвет №2]") ;
	if ( params [ 1 ] < 0 || params [ 1 ] > 126) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Введите: /veh [id транспорта] [Цвет №1] [Цвет №2]") ;
	if ( params [ 2 ] < 0 || params [ 2 ] > 126) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Введите: /veh [id транспорта] [Цвет №1] [Цвет №2]") ;
	new Float:player_pos [ 3 ];
	GetPlayerPos ( playerid, player_pos [ 0  ], player_pos [ 1  ], player_pos [ 2 ] ) ;
	new adm_veh = AddStaticVehicleEx ( params [ 0  ], player_pos [ 0  ], player_pos [ 1  ], player_pos [ 2  ], 0.0, params [ 1  ], params [ 2  ], 60000) ;
	if ( in_velo ( adm_veh ) )
	{
		new engine,lights,alarm,doors,bonnet,boot,objective;
		GetVehicleParamsEx ( adm_veh, engine, lights, alarm, doors, bonnet, boot, objective ) ;
		SetVehicleParamsEx ( adm_veh, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective ) ;
	}
	SetPVarInt ( playerid, "admin_vehicle", adm_veh ) ;
	veh_info [ adm_veh ] [ v_fuel ] = 60.0;
	PutPlayerInVehicle ( playerid, adm_veh, 0) ;
	SetPlayerArmedWeapon(playerid,0) ;
	new text_string [ 128 ];
	format ( text_string, sizeof ( text_string ), "Транспорт создан. ID: %d | Model: %d", adm_veh, params [ 0 ] ) ;
	SendClientMessage ( playerid, COLOR_GREEN, text_string ) ;
	return true;
}
CMD:delveh(playerid, params[])
{
    if ( !GetPVarInt ( playerid, "admin_vehicle" ) ) return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Вы не создавали транспорт!" ) ;
	DestroyVehicle ( GetPVarInt ( playerid, "admin_vehicle" ) ) ;
	DeletePVar ( playerid, "admin_vehicle" ) ; 
	SendClientMessage ( playerid, COLOR_GREEN, "Транспорт успешно удален!" ) ;
	return true;
}
CMD:createarea(playerid, params[]) 
{ 
	new Float: create_pos [ 4 ];
	GetPlayerPos ( playerid, create_pos [ 0  ], create_pos [ 1  ], create_pos [ 2 ] ) ;
	GetPlayerFacingAngle ( playerid, create_pos [ 3 ] ) ;
	for ( new i = 0; i < 4; i ++ )
	{
		new pvar_string [ 28 ];
		format ( pvar_string, sizeof ( pvar_string ), "area_pos_%d", i ) ;
		SetPVarFloat ( playerid, pvar_string, create_pos [ i ] ) ;
	}
	SendClientMessage ( playerid, COLOR_GREEN, "Переходите к следующему этапу создания (( /endarea ))" ) ;
	return true; 
}

CMD:endarea(playerid, params[]) 
{ 
	if ( tp_areas_amount >= sizeof ( area_info ) ) return SendClientMessage ( playerid, COLOR_ADM, "Лимит телепортов превышен! Измените размер массива area_info!" ) ;	
	if ( sscanf(params, "dddd", params [ 0  ], params [ 1  ], params [ 2  ], params [ 3 ] ) ) return SendClientMessage(playerid, COLOR_GRAY, "[Ошибка] Введите: /endarea [virtual_world] [interior] [to_vw] [to_int]" ) ;
	new Float: create_pos [ 4 ];
	GetPlayerPos ( playerid, create_pos [ 0  ], create_pos [ 1  ], create_pos [ 2 ] ) ;
	GetPlayerFacingAngle ( playerid, create_pos [ 3 ] ) ;
	tp_areas_amount ++;
	area_info [ tp_areas_amount - 1 ] [ a_id ] = tp_areas_amount - 1;
	for ( new i = 0; i < 4; i ++ )
	{
		new pvar_string [ 28 ];
		format ( pvar_string, sizeof ( pvar_string ), "area_pos_%d", i ) ;
		area_info [ tp_areas_amount - 1 ] [ a_pos ] [ i ] = GetPVarFloat ( playerid, pvar_string ) ;
		DeletePVar ( playerid, pvar_string ) ;
		area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ i ] = create_pos [ i ];
	}
	area_info [ tp_areas_amount - 1 ] [ a_virt_world ] [ 0 ] = params [ 0 ];
	area_info [ tp_areas_amount - 1 ] [ a_virt_world ] [ 1 ] = params [ 2 ];

	area_info [ tp_areas_amount - 1 ] [ a_interior ] [ 0 ] = params [ 1 ];
	area_info [ tp_areas_amount - 1 ] [ a_interior ] [ 1 ] = params [ 3 ];

	CreateDynamicPickup ( 1318, 23, area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 2  ], area_info [ tp_areas_amount - 1 ] [ a_virt_world ] [ 0 ] ) ;
	area_info [ tp_areas_amount - 1 ] [ a_area ] [ 0 ] = CreateDynamicSphere ( area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 2  ], 1.0 ) ;
 	CreateDynamicPickup ( 1318, 23, area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 2  ], area_info [ tp_areas_amount - 1 ] [ a_virt_world ] [ 1 ] ) ;
	area_info [ tp_areas_amount - 1 ] [ a_area ] [ 1 ] = CreateDynamicSphere ( area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 2  ], 1.0 ) ;

	new insert_string [ 512 ];
	format ( insert_string, sizeof insert_string, "INSERT INTO `teleport_areas` ( `area_x`, `area_y`, `area_z`, `area_c`, `area_to_x`, `area_to_y`, `area_to_z`, `area_to_c`, `area_virtual_world`, `area_to_virtual_world`, `area_interior`, `area_to_interior` ) VALUES ( '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%d', '%d', '%d', '%d' )",
	area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 2  ], area_info [ tp_areas_amount - 1 ] [ a_pos ] [ 3  ],
	area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 0  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 1  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 2  ], area_info [ tp_areas_amount - 1 ] [ a_to_pos ] [ 3  ],
	params [ 0  ], params [ 2  ], params [ 1  ], params [ 3 ] ) ;
	mysql_query ( sql_connection_cross_server, insert_string ) ;
	
	SendClientMessage ( playerid, COLOR_GREEN, "Телепорт создан!" ) ;
	return true; 
}