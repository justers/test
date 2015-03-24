stock print_log ( print_text[] )
{
	if ( debug_mode ) return printf ( print_text ) ;
	return 1;
}
stock show_dialog ( playerid, dialogid,style,caption[],info[],button1[],button2[])
{
	p_s_info [ playerid ] [ p_dialog ] = true;
	return ShowPlayerDialog ( playerid, dialogid,style,caption,info,button1,button2) ;
}
stock kick_player ( playerid )
{
	new ms = GetPlayerPing ( playerid ) > 200 ? 200 : GetPlayerPing ( playerid ) + 50 ;
	return SetTimerEx ( "fixed_kick", ms , 0, "i", playerid ) ;
}
callback: fixed_kick ( playerid ) { Kick ( playerid ) ;}

stock strreplace(string[], find, replace)
{
	for(new i=0; string[i ] ; i++)
	{
		if(string[i] == find)string[i] = replace;
	}
}

public OnQueryError(errorid, error[], callback[], query[], connectionHandle)return print_log(error) ;

stock clear_player_data ( playerid )
{
	p_s_info [ playerid ] [ p_logged ] = false;
	p_s_info [ playerid ] [ p_dialog ] = false;

	p_info [ playerid ] [ level ] = 0;
	p_info [ playerid ] [ age ] = 0;
	p_info [ playerid ] [ gender ] = 0;
	p_info [ playerid ] [ skin ] = 0;
	p_info [ playerid ] [ exp ] = 0;
	p_info [ playerid ] [ money ] = 0;
	
	profile_info [ playerid ] [ profile_increment ] = 0;
	clicked_item[ playerid ]  = -1;
	return 1;
}
stock set_skin ( playerid, skinid )
{
	p_info [ playerid ] [ skin ] = skinid ;
	update_int_sql ( playerid, "c_skin", p_info [ playerid ] [ skin ] ) ;
	return SetPlayerSkin ( playerid, p_info [ playerid ] [ skin ] ) ;
}
callback: connect_waiting ( playerid )
{
	for ( new i = 0; i < 12; i ++ )SendClientMessage ( playerid, -1, " " ) ;
	InterpolateCameraPos ( playerid, 1719.8282, -808.3868, 73.1169, 1719.8282, -808.3868, 73.1169, 10000, CAMERA_MOVE ) ;
	InterpolateCameraLookAt ( playerid, 1719.3439, -809.2669, 73.2269, 1719.3439, -809.2669, 73.2269, 10000, CAMERA_MOVE ) ;
	profile_enter_td ( playerid, true ) ;
	return 1;
}

callback: profile_reg_check ( playerid )
{
	new rows, fields ;
	cache_get_data ( rows, fields ) ;
	if ( rows ) return CancelSelectTextDraw ( playerid ), show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{B22222}Ошибка", "\n{FFFFFF}Профиль с данным логином уже существует! Попробуйте другой!", "Назад", "" ) ;
	if ( !can_register ) return CancelSelectTextDraw ( playerid ), show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{B22222}Ошибка", "{FFFFFF}Регистрация в данный момент закрыта, ведётся разработка!", "Назад", "" ), kick_player ( playerid ) ;
	new text_string [ 188  ], profile_name_string [ 13 + 1  ], profile_password_string [ 13 + 1  ] ;
	
	GetPVarString ( playerid, "profile_name", profile_name_string, sizeof ( profile_name_string ) ) ;
	GetPVarString ( playerid, "profile_password", profile_password_string, sizeof ( profile_password_string ) ) ;
	if ( !regex_match ( profile_name_string, "[[:word:]]{3,13}" ) )return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина логина не должна быть менее 3 и более 12 символов\n- Логин может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
	for(new i = strlen(profile_name_string) ; i != 0; --i)
	{
		switch(profile_name_string[i])
		{
			case 'А'..'Я', 'а'..'я', ' ': return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина логина не должна быть менее 6 и более 12 символов\n- Логин может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
		}
	}	
	if ( !regex_match ( profile_password_string, "[[:word:]]{6,13}" ) )return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина пароля не должна быть менее 6 и более 12 символов\n- Пароль может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
	for(new i = strlen(profile_password_string) ; i != 0; --i)
	{
		switch(profile_password_string[i])
		{
			case 'А'..'Я', 'а'..'я', ' ': return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина пароля не должна быть менее 6 и более 12 символов\n- Пароль может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
		}
	}	
 	format ( text_string, 96, "Профиль %s успешно создан в базе данных.", profile_name_string ) ;
	SendClientMessage ( playerid, COLOR_GREEN, text_string ) ;
	
	mysql_format ( sql_connection_cross_server, text_string, sizeof ( text_string ), "INSERT INTO `profiles` ( `p_login`, `p_password`, `p_ip_registered`, `p_date_registered` ) VALUES ( '%s', '%s', '%s', '%d', NOW() )", profile_name_string, profile_password_string, p_s_info [ playerid ] [ p_ip ] ) ;
	mysql_query ( sql_connection_cross_server, text_string) ;
	
	profile_enter_td ( playerid, false ) ;
	mysql_format ( sql_connection_cross_server, text_string, sizeof ( text_string ), "SELECT * FROM `profiles` WHERE `p_login` = '%e' LIMIT 1", profile_name_string ) ;
	mysql_tquery ( sql_connection_cross_server, text_string, "profile_login", "i", playerid ) ;
	DeletePVar ( playerid, "profile_name" ), DeletePVar ( playerid, "profile_password" ) ;
	return 1;
}
callback: profile_login ( playerid )
{
	new rows, fields ;
	cache_get_data ( rows, fields ) ;
	if ( rows )
	{
		new temp [ 36  ] ;
		cache_get_field_content ( 0, "p_login", temp ) , sscanf ( temp, "s[32]", profile_info [ playerid ] [ profile_name ] ) ;
		new is_online = cache_get_field_content_int ( 0, "p_is_online" ) ;
		if ( is_online )
		{	
			new query_string [ 98 ] ;
			CancelSelectTextDraw ( playerid ) ;		
			format ( query_string, sizeof(query_string), "{FFFFFF}Профиль %s уже находится в сети. ", profile_info [ playerid ] [ profile_name ] ) ;
			show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{B22222}Ошибка", query_string, "Назад", "" ) ;
			return 1;
		}	
		profile_info [ playerid ] [ profile_increment ] = cache_get_field_content_int ( 0, "p_increment" ) ;
		new query_string [ 168 ] ;
		DeletePVar ( playerid, "profile_name" ), DeletePVar ( playerid, "profile_password" ) ;
		format(query_string,sizeof(query_string),"UPDATE `profiles` SET `p_is_online` = '%d' WHERE `p_increment` = '%d'", server_number, profile_info [ playerid ] [ profile_increment ] ) ;
		mysql_query ( sql_connection_cross_server, query_string  ) ;
		
		format ( query_string, sizeof(query_string), "Профиль %s успешно загружен из базы данных. ", profile_info [ playerid ] [ profile_name ] ) ;
		SendClientMessage ( playerid, 0xFFE330FF, query_string ) ;
		
		mysql_format ( sql_connection, query_string, sizeof ( query_string ), "SELECT `c_login` FROM `characters` WHERE `c_profile` = '%d' LIMIT 3", profile_info [ playerid ] [ profile_increment ] ) ;
		mysql_tquery ( sql_connection, query_string, "load_characters", "i", playerid ) ;
		profile_enter_td ( playerid, false ) ;

	}
	else
	{
		show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{B22222}Ошибка", "{FFFFFF}Профиль с таким паролем не найден", "Назад", "" ) ;
	}
	return 1;
}

callback: load_characters ( playerid )
{
	new rows, fields;
	cache_get_data ( rows, fields ) ;
	if ( rows )
	{
		SetPVarInt ( playerid, "characters_rows", rows ) ;
		new pvar_name [ 32  ], character_name [ MAX_PLAYER_NAME + 1  ] ;
		new dialog_string [ 144  ] ;
		for ( new i = 0; i < rows; i ++ )
		{
			new temp [ 43  ] ;
			format ( pvar_name, sizeof ( pvar_name ), "character_name_%d", i ) ;
			cache_get_field_content (i, "c_login", temp ) , sscanf ( temp, "s[32]", character_name ) ;
			SetPVarString ( playerid, pvar_name, character_name ) ;
			format ( temp, sizeof ( temp ), "{2E8B57}%d.{FFFFFF} %s\n", i + 1, character_name ) ;
			strcat ( dialog_string, temp ) ;
		}	
		if ( rows < 3 ) strcat ( dialog_string, "-- Создать персонажа") ;
		show_dialog ( playerid, d_list_character, DIALOG_STYLE_LIST, "{2E8B57}Выбор персонажа", dialog_string, "Выбрать", "" ) ;
	}
	else show_dialog ( playerid, d_list_character, DIALOG_STYLE_LIST, "{2E8B57}Выбор персонажа", "{FFFFFF}-- Создать персонажа", "Выбрать", "" ) ;
	return 1;
}
callback: player_timer ( playerid )
{
	
	if( GetPlayerState ( playerid ) == PLAYER_STATE_DRIVER )
	{
		new engine, lights, alarm, doors, bonnet, boot, objective, vehicleid = GetPlayerVehicleID ( playerid ) ;
		if ( in_boat ( vehicleid ) || in_bike( vehicleid ) || in_plane( vehicleid ) ) return 1;
		GetVehicleParamsEx ( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective ) ;
		new Float:veh_health;
		GetVehicleHealth ( vehicleid, veh_health ) ;
		if(veh_health < 310.0) SetVehicleParamsEx ( vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective ) ;
		if(veh_health < 300.0) SetVehicleHealth ( vehicleid, 305.0 ) ;
		if ( engine == VEHICLE_PARAMS_ON )
		{
			veh_info [ vehicleid ] [ v_fuel ] -= 0.020;
			if ( veh_info [ vehicleid ] [ v_fuel ] < 1 )SetVehicleParamsEx ( vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective ) ;
			new Float:X, Float:Y,Float:A;
			Y = 416.062500,X = 527.000000;
			A = veh_info [ vehicleid ] [ v_fuel ] * 0.7;
			X += A;

			PlayerTextDrawHide(playerid, speed_td [ playerid ] [ 6 ] ) ;
			PlayerTextDrawDestroy(playerid, speed_td [ playerid ] [ 6 ] ) ;

			speed_td [ playerid ] [ 6 ] = CreatePlayerTextDraw(playerid, X, Y, "LD_SPAC:white") ;
			PlayerTextDrawLetterSize(playerid, speed_td [ playerid ] [ 6 ], 0.000000, 0.000000) ;
			PlayerTextDrawTextSize(playerid, speed_td [ playerid ] [ 6 ], 1.000000, 7.000000) ;
			PlayerTextDrawAlignment(playerid, speed_td [ playerid ] [ 6 ], 1) ;
			PlayerTextDrawColor(playerid, speed_td [ playerid ] [ 6 ], -16776961) ;
			PlayerTextDrawSetShadow(playerid, speed_td [ playerid ] [ 6 ], 0) ;
			PlayerTextDrawSetOutline(playerid, speed_td [ playerid ] [ 6 ], 0) ;
			PlayerTextDrawFont(playerid, speed_td [ playerid ] [ 6 ], 4) ;
			PlayerTextDrawShow(playerid, speed_td [ playerid ] [ 6 ] ) ;
		}
		new Float:dist = veh_info [ vehicleid ] [ v_millage ] / 500;

		new string[68];
		if(dist >= 0.000 && dist <= 9.999)format ( string, sizeof string,"000000%.0f km", dist ) ;
		else if(dist >= 10.000 && dist <= 99.999)format ( string, sizeof string,"00000%.0f km", dist ) ;
		else if(dist >= 100.000 && dist <= 999.99)format ( string, sizeof string,"0000%.0f km", dist ) ;
		else if(dist >= 1000.00 && dist <= 9999.99)format ( string, sizeof string,"000%.0f km", dist ) ;
		else if(dist >= 10000.00 && dist <= 99999.99)format ( string, sizeof string,"00%.0f km", dist ) ;
		PlayerTextDrawSetString ( playerid, speed_td [ playerid ] [ 0  ], string ) ;
		new info[3][12];
		if(lights == VEHICLE_PARAMS_UNSET || lights == VEHICLE_PARAMS_OFF) info[0] = "~r~off";
		else info[0] = "~g~on";
		if(veh_info [ vehicleid ] [ v_locked ] == false || doors == VEHICLE_PARAMS_OFF) info[1] = "~g~unlock";
		else info[1] = "~r~lock";
		if(engine == VEHICLE_PARAMS_UNSET || engine == VEHICLE_PARAMS_OFF) info[2] = "~r~off";
		else info[2] = "~g~on";
		format ( string, sizeof string, "Lite:%s  ~w~DS:%s  ~w~Engine:%s",info[0 ],info[1 ],info[2]  ) ;
		PlayerTextDrawSetString ( playerid, speed_td [ playerid ] [ 1  ], string ) ;
	}	
	return 1;
}
callback: update_player_speedometr(playerid)
{
	if(GetPlayerState( playerid ) == PLAYER_STATE_DRIVER)
	{
		new Float:X, Float:Y,Float:A;
		X = 400.500000, Y = 394.187500;
		A = get_vehicle_speed ( playerid ) / 2.222;
		X += A;
		for( new i = 2;i< 6;i++)
		{
			if(speed_td [ playerid ] [ i ] != (PlayerText:-1))
			{
				PlayerTextDrawHide(playerid, speed_td [ playerid ] [ i ] ) ;
				PlayerTextDrawDestroy(playerid, speed_td [ playerid ] [ i ] ) ;
			}
		}
		speed_td [ playerid ] [ 2 ] = CreatePlayerTextDraw(playerid, X, Y, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td [ playerid ] [ 2 ], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td [ playerid ] [ 2 ], 6.500000, 0.875000) ;
		PlayerTextDrawAlignment(playerid, speed_td [ playerid ] [ 2 ], 1) ;
		PlayerTextDrawColor(playerid, speed_td [ playerid ] [ 2 ], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td [ playerid ] [ 2 ], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td [ playerid ] [ 2 ], 0) ;
		PlayerTextDrawFont(playerid, speed_td [ playerid ] [ 2 ], 4) ;

		speed_td [ playerid ] [ 3 ] = CreatePlayerTextDraw(playerid, X, Y, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td [ playerid ] [ 3 ], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td [ playerid ] [ 3 ], 3.500000, 10.062500) ;
		PlayerTextDrawAlignment(playerid, speed_td [ playerid ] [ 3 ], 1) ;
		PlayerTextDrawColor(playerid, speed_td [ playerid ] [ 3 ], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td [ playerid ] [ 3 ], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td [ playerid ] [ 3 ], 0) ;
		PlayerTextDrawFont(playerid, speed_td [ playerid ] [ 3 ], 4) ;

		Y = 402.937500;

		speed_td [ playerid ] [ 4 ] = CreatePlayerTextDraw(playerid, X, Y, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td [ playerid ] [ 4 ], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td [ playerid ] [ 4 ], 6.500000, 1.312500) ;
		PlayerTextDrawAlignment(playerid, speed_td [ playerid ] [ 4 ], 1) ;
		PlayerTextDrawColor(playerid, speed_td [ playerid ] [ 4 ], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td [ playerid ] [ 4 ], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td [ playerid ] [ 4 ], 0) ;
		PlayerTextDrawFont(playerid, speed_td [ playerid ] [ 4 ], 4) ;

		Y = 394.625000,X = 405.500000,X += A;

		speed_td [ playerid ] [ 5 ] = CreatePlayerTextDraw(playerid, X, Y, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td [ playerid ] [ 5 ], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td [ playerid ] [ 5 ], 1.000000, 9.625000) ;
		PlayerTextDrawAlignment(playerid, speed_td [ playerid ] [ 5 ], 1) ;
		PlayerTextDrawColor(playerid, speed_td [ playerid ] [ 5 ], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td [ playerid ] [ 5 ], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td [ playerid ] [ 5 ], 0) ;
		PlayerTextDrawFont(playerid, speed_td [ playerid ] [ 5 ], 4) ;

		for( new i = 2;i< 6;i++)
		{
			PlayerTextDrawShow(playerid, speed_td [ playerid ] [ i  ] ) ;
		}
		new engine, lights, alarm, doors, bonnet, boot, objective, vehicleid = GetPlayerVehicleID ( playerid ) ;
		GetVehicleParamsEx ( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective ) ;
		if ( engine == VEHICLE_PARAMS_ON )
		{
		    if (  veh_info [ vehicleid ] [ v_now_pos ] [ 0 ] != 0 && veh_info [ vehicleid ] [ v_now_pos ] [ 1 ] != 0 && veh_info [ vehicleid ] [ v_now_pos ] [ 2 ] != 0 && get_vehicle_speed ( playerid ) > 2)
			{
				new Float:distance = GetVehicleDistanceFromPoint ( vehicleid, veh_info [ vehicleid ] [ v_now_pos ] [ 0  ], veh_info [ vehicleid ] [ v_now_pos ] [ 1  ], veh_info [ vehicleid ] [ v_now_pos ] [ 2 ] ) ;
				veh_info [ vehicleid ] [ v_millage ] += distance;
			}
			GetVehiclePos ( vehicleid, veh_info [ vehicleid ] [ v_now_pos ] [ 0  ], veh_info [ vehicleid ] [ v_now_pos ] [ 1  ], veh_info [ vehicleid ] [ v_now_pos ] [ 2 ] ) ;
		}
	}
	else
	{
	    for ( new i = 0; i != 7 ; i++ ) PlayerTextDrawHide ( playerid, speed_td [ playerid ] [ i ] ) ;
		for ( new i = 0; i != 18 ; i++ )TextDrawHideForPlayer ( playerid, speed_global_td [ i ] ) ;
		speedometr_td ( playerid, false ) ;
		KillTimer ( p_info [ playerid ] [ speed_timer ] ) ;
		SetPlayerDrunkLevel ( playerid, 0 ) ;
	}
 	return true;
}
callback: login_character ( playerid )
{
	new rows, fields, temp [ 32 ] ;
	cache_get_data ( rows, fields ) ;
	if ( rows )
	{
		cache_get_field_content ( 0, "c_id", temp ) , p_info [ playerid ] [ id ] = strval ( temp ) ;
		cache_get_field_content ( 0, "c_skin", temp ) , p_info [ playerid ] [ skin ] = strval ( temp ) ;
		cache_get_field_content ( 0, "c_age", temp ) , p_info [ playerid ] [ age ] = strval ( temp ) ;		
		cache_get_field_content ( 0, "c_gender", temp ) , p_info [ playerid ] [ gender ] = strval ( temp ) ;		
		cache_get_field_content ( 0, "c_level", temp ) , p_info [ playerid ] [ level ] = strval ( temp ) ;		
		cache_get_field_content ( 0, "c_exp", temp ) , p_info [ playerid ] [ exp ] = strval ( temp ) ;	
		cache_get_field_content ( 0, "c_money", temp ) , p_info [ playerid ] [ money ] = strval ( temp ) ;	
		cache_get_field_content ( 0, "c_login", temp ) , sscanf ( temp, "s[32]", p_info [ playerid ] [ name ] ) ;
		TogglePlayerSpectating ( playerid, false ) ;
		
		SetSpawnInfo ( playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) ;
		ResetPlayerMoney ( playerid ) ;	
		GivePlayerMoney ( playerid, p_info [ playerid ] [ money ] ) ;
		p_info [ playerid ] [ main_timer ] = SetTimerEx ( "player_timer", 1000, 1, "i", playerid ) ;
		p_s_info [ playerid ] [ p_logged ] = true ;
		SetPlayerColor ( playerid, COLOR_LOGGED ) ;
		SetPlayerName(playerid, p_info [ playerid ] [ name  ] ) ;
		
		
		SpawnPlayer ( playerid ) ;
		
		new sql_string [ 128 ] ;
		format ( sql_string, sizeof sql_string, "SELECT * FROM `ownable_veh_system` WHERE `v_owner` = '%d'", p_info [ playerid ] [ id ] ) ;
		mysql_tquery ( sql_connection, sql_string, "load_ownable_veh_system", "") ;

		ApplyAnimation(playerid,"BOMBER","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"RAPPING","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"SHOP","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"BEACH","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"SMOKING","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"ON_LOOKERS","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"DEALER","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"CRACK","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"CARRY","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"COP_AMBIENT","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"PARK","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"INT_HOUSE","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"FOOD","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"CRIB","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"ROB_BANK","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"JST_BUISNESS","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"PED","null",0.0,0,0,0,0,0) ;
		ApplyAnimation(playerid,"OTB","null",0.0,0,0,0,0,0) ;
		
		return 1;
	}
	return 1;
}


callback: load_ownable_veh_system ( )
{
	new rows, fields, temp [ 32 ] ;
	cache_get_data ( rows, fields ) ;

	new CarId [ MAX_PLAYER_VEHICLES ] ;
	new j = 0;
	if ( rows )
	{
  		for(new i=1, rows2 = rows; i < MAX_VEHICLES; i++)
		{
		    if(rows2 == 0) break;
			if(IsValidVehicle(i)) continue;
			while(j < MAX_PLAYER_VEHICLES)
			{
			    if(CarId[j] == i && CarId[j] != 0) continue;
			    if(rows2 == 0) break;
			    CarId[j] = i;
                j++;
				break;
			}
			rows2 --;
		}
		for ( new t = 0; t < rows ; t++ )
        {

			cache_get_field_content ( t, "v_id", temp ) , veh_info [ CarId[t] ] [ v_id ] = strval ( temp ) ;
			cache_get_field_content ( t, "v_owner", temp ) , veh_info [ CarId[t] ] [ v_owner ] = strval ( temp ) ;

			cache_get_field_content ( t, "v_model", temp ) , veh_info [ CarId[t] ][ v_model ] = strval ( temp ) ;

			cache_get_field_content ( t, "v_plate", temp ), sscanf ( temp, "s[24]", veh_info [ CarId[t] ] [ v_plate ] ) ;
			cache_get_field_content ( t, "v_pos_x", temp ) , veh_info [ CarId[t] ] [ v_pos ] [ 0 ] = floatstr ( temp ) ;
			cache_get_field_content ( t, "v_pos_y", temp ) , veh_info [ CarId[t] ] [ v_pos ] [ 1 ] = floatstr ( temp ) ;
			cache_get_field_content ( t, "v_pos_z", temp ) , veh_info [ CarId[t] ] [ v_pos ] [ 2 ] = floatstr ( temp ) ;
			cache_get_field_content ( t, "v_pos_c", temp ) , veh_info [ CarId[t] ] [ v_pos ] [ 3 ] = floatstr ( temp ) ;
			cache_get_field_content ( t, "v_color_one", temp ) , veh_info [ CarId[t] ] [ v_color ] [ 0 ] = strval ( temp ) ;
			cache_get_field_content ( t, "v_color_two", temp ) , veh_info [ CarId[t] ] [ v_color ] [ 1 ] = strval ( temp ) ;
			cache_get_field_content ( t, "v_millage", temp ) , veh_info [ CarId[t] ] [ v_millage ] = floatstr ( temp ) ;
			cache_get_field_content ( t, "v_fuel", temp ) , veh_info [ CarId[t] ] [ v_fuel ] = floatstr ( temp ) ;

			veh_info [ CarId[t] ] [ v_vehicle ] = CreateVehicle  ( veh_info [ CarId[t] ] [ v_model  ], veh_info [ CarId[t] ] [ v_pos ] [ 0  ], veh_info [ CarId[t] ] [ v_pos ] [ 1  ], veh_info [ CarId[t] ] [ v_pos ] [ 2  ], veh_info [ CarId[t] ] [ v_pos ] [ 3  ], veh_info [ CarId[t] ] [ v_color ] [ 0  ], veh_info [ CarId[t] ] [ v_color ] [ 1  ], 90 * 30 ) ;
			Iter_Add(ownable_vehicles,veh_info [ CarId[t] ] [ v_vehicle  ] ) ;
			veh_info [ CarId[t] ] [ v_locked ] = false;
			RepairVehicle(veh_info [ CarId[t] ] [ v_vehicle  ] ) ;
			SetVehicleHealth(veh_info [ CarId[t] ] [ v_vehicle  ], 1000) ;
			
			new lights, alarm, bonnet, boot, objective, engine, lock;
			GetVehicleParamsEx ( veh_info [ CarId[t] ] [ v_vehicle ], engine, lights, alarm, lock, bonnet, boot, objective ) ;
			SetVehicleParamsEx ( veh_info [ CarId[t] ] [ v_vehicle ], false, lights, alarm, lock, bonnet, boot, objective ) ;
			SetVehicleNumberPlate ( veh_info [ CarId[t] ] [ v_vehicle ], veh_info [ CarId[t] ] [ v_plate ] ) ;

			new sql_string [ 128 ];
			format ( sql_string, sizeof sql_string, "UPDATE `vehicles` SET `v_load_this` = '0' WHERE `v_id` = '%d' LIMIT 1", veh_info [ CarId[t] ] [ v_id ] ) ;
			mysql_query ( sql_connection, sql_string ) ;

		}
	}
	else return true;
	return 1;
}
stock save_vehicle ( vehicleid )
{
	new query_str [ 256 ] ;
	format ( query_str, sizeof query_str, "UPDATE `vehicles` SET \
	`v_millage` = '%f',\
	`v_fuel` = '%f', \
	`v_color_one` = '%d',\
	`v_color_two` = '%d' WHERE `v_id` = '%d' LIMIT 1",
	veh_info [ vehicleid ] [ v_millage  ], veh_info [ vehicleid ] [ v_fuel  ], veh_info [ vehicleid ] [ v_color ] [ 0  ], veh_info [ vehicleid ] [ v_color ] [ 1  ], veh_info [ vehicleid ] [ v_id ]) ;
	mysql_query ( sql_connection, query_str ) ;
	DestroyVehicle ( vehicleid ) ;
	return 1;
}
callback: check_free_name ( playerid, inputtext [ 32 ] )
{
	new rows, fields ;
	cache_get_data ( rows, fields ) ;
	if ( rows )return SendClientMessage ( playerid, COLOR_RED, "Персонаж с таким именем и фамилией уже существует!" ) ;
	format ( p_info [ playerid ] [ name  ], 24,"%s", inputtext ) ;
	PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0  ], p_info [ playerid ] [ name ] ) ;
	return 1;
}
new tp_areas_amount;


callback: areas_loading ()
{
	new rows, fields, temp [ 32 ];
	cache_get_data ( rows, fields ) ;
	tp_areas_amount = rows;
	if ( rows )
	{
       	for ( new t = 0; t < rows ; t++ )
        {

			cache_get_field_content (t, "area_id", temp ) , area_info [ t ] [ a_id ] = strval ( temp ) ;
			cache_get_field_content (t, "area_x", temp ) , area_info [ t ] [ a_pos ] [ 0 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_y", temp ) , area_info [ t ] [ a_pos ] [ 1 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_z", temp ) , area_info [ t ] [ a_pos ] [ 2 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_c", temp ) , area_info [ t ] [ a_pos ] [ 3 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_to_x", temp ) , area_info [ t ] [ a_to_pos ] [ 0 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_to_y", temp ) , area_info [ t ] [ a_to_pos ] [ 1 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_to_z", temp ) , area_info [ t ] [ a_to_pos ] [ 2 ] = floatstr ( temp ) ;
			cache_get_field_content (t, "area_to_c", temp ) , area_info [ t ] [ a_to_pos ] [ 3 ] = floatstr ( temp ) ;

			cache_get_field_content (t, "area_virtual_world", temp ) , area_info [ t ] [ a_virt_world ] [ 0 ] = strval ( temp ) ;
			cache_get_field_content (t, "area_to_virtual_world", temp ) , area_info [ t ] [ a_virt_world ] [ 1 ] = strval ( temp ) ;

			cache_get_field_content (t, "area_interior", temp ) , area_info [ t ] [ a_interior ] [ 0 ] = strval ( temp ) ;
			cache_get_field_content (t, "area_to_interior", temp ) , area_info [ t ] [ a_interior ] [ 1 ] = strval ( temp ) ;
			
			CreateDynamicPickup ( 1318, 23, area_info [ t ] [ a_pos ] [ 0 ], area_info [ t ] [ a_pos ] [ 1 ], area_info [ t ] [ a_pos ] [ 2 ], area_info [ t ] [ a_virt_world ] [ 0 ] ) ;

			area_info [ t ] [ a_area ] [ 0 ] = CreateDynamicSphere ( area_info [ t ] [ a_pos ] [ 0 ], area_info [ t ] [ a_pos ] [ 1 ], area_info [ t ] [ a_pos ] [ 2 ], 1.0 ) ;
 
			CreateDynamicPickup ( 1318, 23, area_info [ t ] [ a_to_pos ] [ 0 ], area_info [ t ] [ a_to_pos ] [ 1 ], area_info [ t ] [ a_to_pos ] [ 2 ], area_info [ t ] [ a_virt_world ] [ 1 ] ) ;

			area_info [ t ] [ a_area ] [ 1 ] = CreateDynamicSphere ( area_info [ t ] [ a_to_pos ] [ 0 ], area_info [ t ] [ a_to_pos ] [ 1 ], area_info [ t ] [ a_to_pos ] [ 2 ], 1.0 ) ;
	 	}
		printf ( "[AREAS] Loaded %d areas.", tp_areas_amount ) ;
    }
	else print ( "[AREAS] No areas was loaded." ) ;
	for(new i = 0 ; i < 5 ; i++)
	{
		g_areas [ veh_shop ] [ i ] = CreateDynamicSphere ( veh_exit_coordinate [ i ] [ 0 ], veh_exit_coordinate [ i ] [ 1 ], veh_exit_coordinate [ i ] [ 2 ], 1.5 ) ;
		new obj = CreateObject ( 1317, veh_exit_coordinate [ i ] [ 0 ], veh_exit_coordinate [ i ] [ 1 ], veh_exit_coordinate [ i ] [ 2 ] - 1.6, 5, 0.0, 0.0, 0.0 ) ;
		SetObjectMaterial ( obj, 0, 18646, "matcolours", "red-4", 0x99FF0000 ) ;
		CreateDynamic3DTextLabel ( "{FFAB4A}----[Покупка транспорта]----", -1, veh_exit_coordinate [ i ] [ 0 ], veh_exit_coordinate [ i ] [ 1 ], veh_exit_coordinate [ i ] [ 2 ], 7.6, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0 ) ;
	}
	return 1;
}

stock update_inventory ( playerid, td, jdk )
{ 
    PlayerTextDrawSetPreviewModel ( playerid, inv_player_td [ playerid ][ td  ], item_info [ jdk ] [item_id ] ) ; 
    PlayerTextDrawSetPreviewRot ( playerid, inv_player_td [ playerid ] [ td  ], item_info [ jdk ] [rot_x ], item_info [ jdk ] [rot_y ], item_info [ jdk ] [rot_z ], item_info [ jdk ] [zoom ] ) ; 
    if(GetPVarInt(playerid, "inventory_opened") == 1) 
    { 
        PlayerTextDrawHide ( playerid, inv_player_td [ playerid ] [ td  ] ) ; 
        PlayerTextDrawShow ( playerid, inv_player_td [ playerid ] [ td  ] ) ; 
    } 
    return true; 
}  

stock give_inventory ( playerid, itemid, bool:drop, ids, amount )
{
    new lf_free_slot;
    for ( new j = 0; j < 10; j++ )
    {
        if ( p_info [ playerid ] [ inventory_slot ] [ j ] == 0)
        {
            if(drop == true)
            {
                drop_obj_info [ ids ] [ d_pos ] [ 0 ]= 0.0;
                drop_obj_info [ ids ] [ d_pos ] [ 1 ]= 0.0;
                drop_obj_info [ ids ] [ d_pos ] [ 2 ]= 0.0;
                drop_obj_info [ ids ] [ d_model ] = 0; 
                DestroyDynamicObject ( drop_obj_info [ ids ] [ d_object  ] ) ;
                DestroyDynamic3DTextLabel ( drop_obj_info [ ids ] [ d_label ] ) ;
                ApplyAnimation ( playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0 ) ;
            }
            p_info [ playerid ] [ inventory_slot ] [ j ] = itemid;
            p_info [ playerid ] [ inventory_amount ] [ j ] = amount;
            if ( GetPVarInt(playerid, "inventory_opened") == 1 ) update_inventory ( playerid, j, j ) ;
            break;
        }
        else lf_free_slot++;
    }
    if(lf_free_slot == 10) return SendClientMessage(playerid,COLOR_GRAY,"[Ошибка] В инвентаре нет места."), false;
    return true;
}  

stock drop_object ( playerid, model, ids )
{
    new t3d_string [ 128 ];
    for(new i = 0; i < sizeof ( drop_obj_info ) ; i++)
    {
        if(drop_obj_info [ i ] [ d_pos ] [ 0 ] == 0.0 && drop_obj_info [ i ] [ d_pos ] [ 1 ] == 0.0 && drop_obj_info [ i ] [ d_pos ] [ 2 ] == 0.0)
        {
            drop_obj_info [ i ] [ d_id ] = ids; 
            drop_obj_info [ i ] [ d_model ] = model;
            drop_obj_info [ i ] [ d_virtual_world ] = GetPlayerVirtualWorld(playerid) ; 
            drop_obj_info [ i ] [ d_interior ] = GetPlayerInterior(playerid) ;
			p_info [ playerid ] [ inventory_amount ] [ i ] = 0;
            p_info [ playerid ] [ inventory_slot ] [ i ] = 0;

			update_inventory ( playerid, clicked_item [ playerid ] , p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ]  ] ) ;

			clicked_item [ playerid ] = -1;

			
			GetPlayerPos(playerid, drop_obj_info [ i ] [ d_pos ] [ 0  ], drop_obj_info [ i ] [ d_pos ] [ 1  ], drop_obj_info [ i ] [ d_pos ] [ 2  ] ) ;
            format(t3d_string, sizeof(t3d_string), "%s\n{408080}Чтобы поднять нажмите клавишу C",item_info [ ids ] [ item_name  ] ) ;
            switch(model)
            {
                case 2223: drop_obj_info [ i ] [ d_object ] = CreateDynamicObject(drop_obj_info [ i ] [ d_model  ], drop_obj_info [ i ] [ d_pos ] [ 0  ], drop_obj_info [ i ] [ d_pos ] [ 1  ], drop_obj_info [ i ] [ d_pos ] [ 2 ]-0.91, 0.0, 0.0, 0.0, drop_obj_info [ i ] [ d_virtual_world  ], drop_obj_info [ i ] [ d_interior  ], -1, 25.0) ;
            }
            drop_obj_info [ i ] [ d_label] = CreateDynamic3DTextLabel(t3d_string, 0xFFFFFFFF, drop_obj_info [ i ] [ d_pos ] [ 0  ], drop_obj_info [ i ] [ d_pos ] [ 1  ], drop_obj_info [ i ] [ d_pos ] [ 2 ]-0.91, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, drop_obj_info [ i ] [ d_virtual_world  ], drop_obj_info [ i ] [ d_interior  ], -1, 7.5) ;
            me_action ( playerid, "положил(а) что-то на землю" ) ;
            ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0) ;
            return true;
        }
    }
    return true;
}  
stock send_world_message ( pointplayerid, Float:max_radius, text[], color1, color2, color3)
{
    new Float:px,Float:py,Float:pz,Float:distance;
    GetPlayerPos(pointplayerid,px,py,pz) ;
    SendClientMessage(pointplayerid,color1,text) ;
    if(Iter_Count(streamed_players[pointplayerid]) == 0) return 1;
    foreach(new i :streamed_players[pointplayerid])
    {
        if(IsPlayerInRangeOfPoint(i,max_radius,px,py,pz) && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(pointplayerid))
        {
            distance = GetPlayerDistanceFromPoint(i,px,py,pz) ;
            if(distance >= 0.0 && distance < max_radius/3) SendClientMessage(i,color1,text) ;
            if(distance >= max_radius/3 && distance < max_radius/3*2) SendClientMessage(i,color2,text) ;
            if(distance >= max_radius/3*2 && distance <= max_radius) SendClientMessage(i,color3,text) ;
        }
    }
    return 1;
}

stock me_action(playerid,action[],Float:distance = 13.0)
{
	new text_string [ 128 ] ;
	format ( text_string, sizeof(text_string), "%s %s", p_info [ playerid ][ name  ], action ) ;
	send_world_message ( playerid, distance, text_string, COLOR_ME, COLOR_ME, COLOR_ME ) ;
	return 1;
}


stock do_action(playerid,action[],Float:distance = 13.0)
{
	new text_string [ 128 ] ;
	format( text_string, sizeof( text_string ),"%s (%s)", action, p_info [ playerid ][ name  ] ) ;
	send_world_message ( playerid, distance, text_string, COLOR_ME, COLOR_ME, COLOR_ME ) ;
	return 1;
}
stock try_action(playerid,action[],Float:distance = 13.0)
{
	new luck[24];
	if(RandomEx(0,2) == 1) strcat(luck,"{00ff00}Удачно") ;
	else strcat(luck,"{ff7518}Неудачно") ;
	new text_string [ 128 ] ;
	format ( text_string, sizeof( text_string ),"%s %s | %s", p_info [ playerid ][ name  ], action, luck ) ;
	send_world_message ( playerid, distance, text_string, COLOR_ME, COLOR_ME, COLOR_ME ) ;
	return 1;
}

stock give_money ( playerid, amount )
{
	p_info [ playerid ] [ money ] += amount;
	GivePlayerMoney(playerid, amount) ;
	new local_string[18];
	if(amount < 0)
	{
		format ( local_string, sizeof ( local_string ), "~r~$%i", amount) ;
		GameTextForPlayer ( playerid, local_string, 3000, 1) ;
	}
	else if(amount > 0)
	{
		format ( local_string, sizeof ( local_string ), "~g~+$%i", amount ) ;
		GameTextForPlayer ( playerid, local_string, 3000, 1 ) ;
	}
	update_int_sql ( playerid, "c_money", p_info [ playerid ] [ money ] ) ;
	return 1;
}

stock get_vehicle_speed( player )
{
	new Float:x, Float:y, Float:z;
	if ( IsPlayerInAnyVehicle ( player ) )GetVehicleVelocity ( GetPlayerVehicleID ( player ), x, y, z ) ;
	else GetPlayerVelocity ( player, x, y, z ) ;

	return floatround ( floatsqroot ( x * x + y * y + z * z ) * 310  ) ;
}
stock update_int_sql ( playerid, field[], data )
{
	new query_string[ 128 ];
	format ( query_string,sizeof(query_string),"UPDATE `characters` SET `%s` = '%d' WHERE `c_id` = '%d' LIMIT 1", field, data, p_info [ playerid ] [ id  ] ) ;
	mysql_query ( sql_connection, query_string ) ;
	return true;
}
stock toggle_lights ( vehicleid )
{
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx ( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective ) ;
	SetVehicleParamsEx ( vehicleid, engine, ( lights == VEHICLE_PARAMS_UNSET || lights == VEHICLE_PARAMS_OFF ) ? VEHICLE_PARAMS_ON : VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective ) ;
	return 1;
}

stock toggle_engine ( playerid, vid )
{
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx ( vid, engine, lights, alarm, doors, bonnet, boot, objective ) ;
	if ( engine == VEHICLE_PARAMS_UNSET || engine == VEHICLE_PARAMS_OFF )
	{
		TogglePlayerControllable ( playerid, 0 ) ;
		GameTextForPlayer ( playerid, "~g~ENGINE STARTED...", 3000, 4 ) ;
		SetTimerEx ( "start_engine", 1000, 0, "ii", playerid, vid ) ;
	}
	else
	{
		GameTextForPlayer ( playerid, "~r~ENGINE OFF", 3000, 4 ) ;
		SetVehicleParamsEx ( vid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective ) ;
	}
	return 1;
}

callback: start_engine ( playerid, vid )
{
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx ( vid, engine, lights, alarm, doors, bonnet, boot, objective ) ;
	TogglePlayerControllable ( playerid, 1 ) ;
	if ( veh_info [ vid ] [ v_fuel ] < 1 )SetVehicleParamsEx ( vid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective ), GameTextForPlayer ( playerid, "~r~NOT ENOUGH FUEL...", 3000, 4 ) ;
	else SetVehicleParamsEx ( vid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective ) ;
	return 1;
}
stock in_boat(vehicleid)
{
	switch(GetVehicleModel(vehicleid))
	{
		case 472,473,493,595,484,430,432,452..454,446: return true;
	}
	return false;
}
stock in_bike(vehicleid)
{
	switch(GetVehicleModel(vehicleid))
	{
		case 448,435,449,450,457,462,464,465,481,485,501,509,510,530,564,569,570,584,594,606,607,608,610,611: return true;
	}
	return false;
}
stock in_plane(vehicleid)
{
	switch(GetVehicleModel(vehicleid))
	{
		case 592,577,511,512,593,520,553,476,519,460,513:  return true;
	}
	return false;
}
stock in_velo(vehicleid)
{
	switch(GetVehicleModel(vehicleid))
	{
		case 481,509,510:return true;
	}
	return false;
}
stock set_pos ( playerid, Float:x, Float:y, Float:z, Float:angle, interior, world )
{
	if ( interior != -1 ) SetPlayerInterior ( playerid, interior ) ;
	if ( world != -1 ) SetPlayerVirtualWorld ( playerid, world ) ;
	SetPlayerPos ( playerid, x, y, z ) ;
	SetPlayerFacingAngle ( playerid, angle ) ;
	ClearAnimations ( playerid, 1 ) ;
	SetCameraBehindPlayer ( playerid ) ;
	TogglePlayerControllable ( playerid, 0 ) ;
	SetTimerEx ( "toggle_controlable", 1500, 0, "ii", playerid, 1 ) ;
	return 1;
}
callback: toggle_controlable ( playerid, toggle ) return TogglePlayerControllable ( playerid, toggle ) ;
callback: clear_anim ( playerid ) return ApplyAnimation ( playerid, "CARRY", "crry_prtial",4.0,0,0,0,0,0,1 ) ;

stock get_vehicle_price ( model )
{
 	new cost;
 	switch ( model )
 	{
		case 400: cost = 15000;
		case 401: cost = 7500;
		case 402: cost = 100000;
		case 404: cost = 7500;
		case 405: cost = 9000;
		case 410: cost = 7500;
		case 411: cost = 100000;
		case 412: cost = 55000;
		case 415: cost = 100000;
		case 419: cost = 55000;
		case 421: cost = 40000;
		case 422: cost = 12000;
		case 424: cost = 95000;
		case 426: cost = 25000;
		case 429: cost = 100000;
		case 434: cost = 950000;
		case 436: cost = 7500;
		case 439: cost = 60000;
		case 445: cost = 45000;
		case 446: cost = 245000;
		case 451: cost = 100000;
		case 452: cost = 135000;
		case 453: cost = 115000;
		case 454: cost = 220000;
		case 458: cost = 30000;
		case 461: cost = 35000;
		case 462: cost = 20000;
		case 463: cost = 50000;
		case 466: cost = 55000;
		case 467: cost = 55000;
		case 468: cost = 40000;
		case 471: cost = 35000;
		case 473: cost = 30000;
		case 472: cost = 40000;
		case 474: cost = 55000;
		case 475: cost = 55000;
		case 477: cost = 75000;
		case 478: cost = 7500;
		case 479: cost = 9000;
		case 480: cost = 100000;
		case 481: cost = 1000;
		case 483: cost = 45000;
		case 484: cost = 235000;
		case 489: cost = 45000;
		case 491: cost = 60000;
		case 492: cost = 55000;
		case 493: cost = 315000;
		case 496: cost = 75000;
		case 500: cost = 85000;
		case 505: cost = 45000;
		case 506: cost = 100000;
		case 507: cost = 25000;
		case 509: cost = 700;
		case 510: cost = 2000;
		case 516: cost = 20000;
		case 517: cost = 45000;
		case 518: cost = 40000;
		case 521: cost = 55000;
		case 522: cost = 60000;
		case 526: cost = 30000;
		case 527: cost = 30000;
		case 529: cost = 20000;
		case 533: cost = 75000;
		case 534: cost = 65000;
		case 535: cost = 65000;
		case 536: cost = 70000;
		case 540: cost = 40000;
		case 541: cost = 100000;
		case 542: cost = 120000;
		case 543: cost = 7500;
		case 545: cost = 40000;
		case 546: cost = 25000;
		case 547: cost = 25000;
		case 549: cost = 35000;
		case 550: cost = 65000;
		case 551: cost = 25000;
		case 554: cost = 45000;
		case 555: cost = 65000;
		case 558: cost = 35000;
		case 559: cost = 95000;
		case 560: cost = 100000;
		case 561: cost = 65000;
		case 562: cost = 95000;
		case 565: cost = 65000;
		case 566: cost = 45000;
		case 567: cost = 55000;
		case 575: cost = 90000;
		case 579: cost = 85000;
		case 580: cost = 90000;
		case 581: cost = 50000;
		case 585: cost = 35000;
		case 586: cost = 40000;
		case 587: cost = 70000;
		case 589: cost = 65000;
		case 595: cost = 56000;
		case 600: cost = 40000;
		case 602: cost = 75000;
		case 603: cost = 90000;
		default:  cost = 0;
 	}
 	return cost;
}
stock get_vehicle_name ( model )
{
 	new veh_name [ 32 ] ;
 	switch ( model )
 	{
		case 400: veh_name = "Stalker";
		case 401: veh_name = "Bravura";
		case 402: veh_name = "Buffalo";
		case 403: veh_name = "Linerunner";
		case 404: veh_name = "Perenniel";
		case 405: veh_name = "Sentinel";
		case 406: veh_name = "Dumper";
		case 407: veh_name = "Firetruck";
		case 408: veh_name = "Trashmaster";
		case 409: veh_name = "Stretch";
		case 410: veh_name = "Manana";
		case 411: veh_name = "Infernus";
		case 412: veh_name = "Voodoo";
		case 413: veh_name = "Pony";
		case 414: veh_name = "Mule";
		case 415: veh_name = "Cheetah";
		case 416: veh_name = "Ambulance";
		case 417: veh_name = "Leviathan";
		case 418: veh_name = "Moonbeam";
		case 419: veh_name = "Esperanto";
		case 420: veh_name = "Taxi";
		case 421: veh_name = "Washington";
		case 422: veh_name = "Bobcat";
		case 423: veh_name = "Mr Whoopee";
		case 424: veh_name = "BF Injection";
		case 425: veh_name = "Hunter";
		case 426: veh_name = "Premier";
		case 427: veh_name = "Enforcer";
		case 428: veh_name = "Securicar";
		case 429: veh_name = "Banshee";
		case 430: veh_name = "Predator";
		case 431: veh_name = "Bus";
		case 432: veh_name = "Rhino";
		case 433: veh_name = "Barracks";
		case 434: veh_name = "Hotknife";
		case 435: veh_name = "Article Trailer";
		case 436: veh_name = "Previon";
		case 437: veh_name = "Coach";
		case 438: veh_name = "Cabbie";
		case 439: veh_name = "Stallion";
		case 440: veh_name = "Rumpo";
		case 441: veh_name = "RC Bandit";
		case 442: veh_name = "Romero";
		case 443: veh_name = "Packer";
		case 444: veh_name = "Monster";
		case 445: veh_name = "Admiral";
		case 446: veh_name = "Squallo";
		case 447: veh_name = "Seasparrow";
		case 448: veh_name = "Pizzaboy";
		case 449: veh_name = "Tram";
		case 450: veh_name = "Article Trailer 2";
		case 451: veh_name = "Turismo";
		case 452: veh_name = "Speeder";
		case 453: veh_name = "Reefer";
		case 454: veh_name = "Tropic";
		case 455: veh_name = "Flatbed";
		case 456: veh_name = "Yankee";
		case 457: veh_name = "Caddy";
		case 458: veh_name = "Solair";
		case 459: veh_name = "Berkley's RC Van";
		case 460: veh_name = "Skimmer";
		case 461: veh_name = "PCJ-600";
		case 462: veh_name = "Faggio";
		case 463: veh_name = "Freeway";
		case 464: veh_name = "RC Baron";
		case 465: veh_name = "RC Raider";
		case 466: veh_name = "Glendale";
		case 467: veh_name = "Oceanic";
		case 468: veh_name = "Sanchez";
		case 469: veh_name = "Sparrow";
		case 470: veh_name = "Patriot";
		case 471: veh_name = "Quad";
		case 472: veh_name = "Coastguard";
		case 473: veh_name = "Dinghy";
		case 474: veh_name = "Hermes";
		case 475: veh_name = "Sabre";
		case 476: veh_name = "Rustler";
		case 477: veh_name = "ZR-350";
		case 478: veh_name = "Walton";
		case 479: veh_name = "Regina";
		case 480: veh_name = "Comet";
		case 481: veh_name = "BMX";
		case 482: veh_name = "Burrito";
		case 483: veh_name = "Camper";
		case 484: veh_name = "Marquis";
		case 485: veh_name = "Baggage";
		case 486: veh_name = "Dozer";
		case 487: veh_name = "Maverick";
		case 488: veh_name = "SAN News Maverick";
		case 489: veh_name = "Rancher";
		case 490: veh_name = "FBI Rancher";
		case 491: veh_name = "Virgo";
		case 492: veh_name = "Greenwood";
		case 493: veh_name = "Jetmax";
		case 494: veh_name = "Hotring Racer";
		case 495: veh_name = "Sandking";
		case 496: veh_name = "Blista Compact";
		case 497: veh_name = "Police Maverick";
		case 498: veh_name = "Boxville";
		case 499: veh_name = "Benson";
		case 500: veh_name = "Mesa";
		case 501: veh_name = "RC Goblin";
		case 502: veh_name = "Hotring Racer";
		case 503: veh_name = "Hotring Racer";
		case 504: veh_name = "Bloodring Banger";
		case 505: veh_name = "Rancher";
		case 506: veh_name = "Super GT";
		case 507: veh_name = "Elegant";
		case 508: veh_name = "Journey";
		case 509: veh_name = "Bike";
		case 510: veh_name = "Mountain Bike";
		case 511: veh_name = "Beagle";
		case 512: veh_name = "Cropduster";
		case 513: veh_name = "Stuntplane";
		case 514: veh_name = "Tanker";
		case 515: veh_name = "Roadtrain";
		case 516: veh_name = "Nebula";
		case 517: veh_name = "Majestic";
		case 518: veh_name = "Buccaneer";
		case 519: veh_name = "Shamal";
		case 520: veh_name = "Hydra";
		case 521: veh_name = "FCR-900";
		case 522: veh_name = "NRG-500";
		case 523: veh_name = "HPV1000";
		case 524: veh_name = "Cement Truck";
		case 525: veh_name = "Towtruck";
		case 526: veh_name = "Fortune";
		case 527: veh_name = "Cadrona";
		case 528: veh_name = "FBI Truck";
		case 529: veh_name = "Willard";
		case 530: veh_name = "Forklift";
		case 531: veh_name = "Tractor";
		case 532: veh_name = "Combine Harvester";
		case 533: veh_name = "Feltzer";
		case 534: veh_name = "Remington";
		case 535: veh_name = "Slamvan";
		case 536: veh_name = "Blade";
		case 537: veh_name = "Freight";
		case 538: veh_name = "Brownstreak";
		case 539: veh_name = "Vortex";
		case 540: veh_name = "Vincent";
		case 541: veh_name = "Bullet";
		case 542: veh_name = "Clover";
		case 543: veh_name = "Sadler";
		case 544: veh_name = "Firetruck LA";
		case 545: veh_name = "Hustler";
		case 546: veh_name = "Intruder";
		case 547: veh_name = "Primo";
		case 548: veh_name = "Cargobob";
		case 549: veh_name = "Tampa";
		case 550: veh_name = "Sunrise";
		case 551: veh_name = "Merit";
		case 552: veh_name = "Utility Van";
		case 553: veh_name = "Nevada";
		case 554: veh_name = "Yosemite";
		case 555: veh_name = "Windsor";
		case 556: veh_name = "Monster A";
		case 557: veh_name = "Monster B";
		case 558: veh_name = "Uranus";
		case 559: veh_name = "Jester";
		case 560: veh_name = "Sultan";
		case 561: veh_name = "Stratum";
		case 562: veh_name = "Elegy";
		case 563: veh_name = "Raindance";
		case 564: veh_name = "RC Tiger";
		case 565: veh_name = "Flash";
		case 566: veh_name = "Tahoma";
		case 567: veh_name = "Savanna";
		case 568: veh_name = "Bandito";
		case 569: veh_name = "Freight Flat Trailer";
		case 570: veh_name = "Streak Trailer";
		case 571: veh_name = "Kart";
		case 572: veh_name = "Mower";
		case 573: veh_name = "Dune";
		case 574: veh_name = "Sweeper";
		case 575: veh_name = "Broadway";
		case 576: veh_name = "Tornado";
		case 577: veh_name = "AT400";
		case 578: veh_name = "DFT-30";
		case 579: veh_name = "Huntley";
		case 580: veh_name = "Stafford";
		case 581: veh_name = "BF-400";
		case 582: veh_name = "Newsvan";
		case 583: veh_name = "Tug";
		case 584: veh_name = "Petrol Trailer";
		case 585: veh_name = "Emperor";
		case 586: veh_name = "Wayfarer";
		case 587: veh_name = "Euros";
		case 588: veh_name = "Hotdog";
		case 589: veh_name = "Club";
		case 590: veh_name = "Freight Box Trailer";
		case 591: veh_name = "Article Trailer 3";
		case 592: veh_name = "Andromada";
		case 593: veh_name = "Dodo";
		case 594: veh_name = "RC Cam";
		case 595: veh_name = "Launch";
		case 596: veh_name = "Police Car (LSPD)";
		case 597: veh_name = "Police Car (SFPD)";
		case 598: veh_name = "Police Car (LVPD)";
		case 599: veh_name = "Police Ranger";
		case 600: veh_name = "Picador";
		case 601: veh_name = "S.W.A.T.";
		case 602: veh_name = "Alpha";
		case 603: veh_name = "Phoenix";
		case 604: veh_name = "Glendale Shit";
		case 605: veh_name = "Sadler Shit";
		case 606: veh_name = "Baggage Trailer A";
		case 607: veh_name = "Baggage Trailer B";
		case 608: veh_name = "Tug Stairs Trailer";
		case 609: veh_name = "Boxville";
		case 610: veh_name = "Farm Trailer";
		case 611: veh_name = "Utility Trailer";
		default:  veh_name = "No-ne";
 	}
 	return veh_name;
}
