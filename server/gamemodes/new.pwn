#include 									<a_samp>
#include 									<a_mysql>
#include 									<streamer>
#include 									<sscanf2>
#include 									<dc_cmd>
#include 									<regex>
#include 									<foreach>
//==============================================================================
#include 									<i_lib/text_colors>
#include 									<i_lib/macroses>
#include 									<i_lib/veh_colors>

//==============================================================================

main(){}
//==============================================================================
new bool:debug_mode = true ;
new sql_connection, sql_connection_cross_server ;
new server_number = 1 ;
new can_register = 1;

new Iterator:streamed_players[MAX_PLAYERS]<MAX_PLAYERS-1>;
new Iterator:ownable_vehicles<1000>;

//==============================================================================
#include <i_lib/player_vars>

#include <i_lib/td_functions>
#include <i_lib/server_functions>

public OnGameModeInit()
{
//==============================================================================
/*	sql_connection = mysql_connect ( "triniti.ru-hoster.com","kybikxTl","kybikxTl","3gKTc63b5b" ) ;
	sql_connection_cross_server = mysql_connect ( "triniti.ru-hoster.com","kybikReD","kybikReD","5zQDsep028" ) ;*/
	sql_connection = mysql_connect ( "127.0.0.1","root","user","" ) ;
	sql_connection_cross_server = mysql_connect ( "127.0.0.1","root_cross_server","user","" ) ;

	mysql_log(LOG_ALL) ;

	mysql_tquery ( sql_connection_cross_server, "SELECT * FROM `teleport_areas`", "areas_loading" ) ;
//==============================================================================
	SetNameTagDrawDistance ( 25.0 ) ;
	EnableStuntBonusForAll ( false ) ;
	DisableInteriorEnterExits ( ) ;
	ManualVehicleEngineAndLights ( ) ;
	AllowInteriorWeapons ( true ) ;
	LimitPlayerMarkerRadius ( 30.0 ) ;
	SetGameModeText ( "v.0.3.1" ) ;
	ShowPlayerMarkers ( PLAYER_MARKERS_MODE_STREAMED ) ;
	ShowNameTags ( true ) ;
	#include <i_lib/global_td_create>
	#include <i_lib/create_objects>
//==============================================================================
	Iter_Init(streamed_players) ;
	return 1;
}


public OnGameModeExit()
{
	mysql_close ( ) ;
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if ( p_s_info [ playerid ] [ p_logged ] == false ) return false ;
	SetSpawnInfo ( playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) ;
	SpawnPlayer ( playerid ) ;
	return 1;
}


public OnPlayerConnect(playerid)
{
//================================================================================
	RemoveBuildingForPlayer(playerid, 5024, 1748.8438, -1883.0313, 14.1875, 0.25) ;
//================================================================================
	SetPlayerColor ( playerid, COLOR_NOLOGGED ) ;
	clear_player_data ( playerid ) ;
	
	GetPlayerIp( playerid, p_s_info [ playerid ] [ p_ip ], 32 ) ;
	TogglePlayerSpectating ( playerid, true ) ;

	SetTimerEx ( "connect_waiting", 500, false, "d", playerid ) ;
	
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if ( profile_info [ playerid ] [ profile_increment ] != 0 )
	{
		new query_string [ 168 ] ;
		format(query_string,sizeof(query_string),"UPDATE `profiles` SET `p_is_online` = '0',`p_date_last` = NOW(),`p_ip_last` = '%s' WHERE `p_increment` = '%d'", p_s_info [ playerid ] [ p_ip ], profile_info [ playerid ] [ profile_increment ] ) ;
		mysql_query ( sql_connection_cross_server, query_string  ) ;
	}
	if ( p_s_info [ playerid ] [ p_logged ] == false ) return 1 ;
	if ( Iter_Count(streamed_players[playerid]) != 0 ) Iter_Clear(streamed_players[playerid]) ;
    foreach(new v : ownable_vehicles)
    {
		if( veh_info [ v ][ v_owner ] == p_info [ playerid ] [ id ])
		{
		    save_vehicle ( v ) ;
		}
	}
	KillTimer ( p_info [ playerid ] [ main_timer ] ) ;
	
	if ( GetPVarInt ( playerid, "in_salon" ) )
	{
		veh_salon_td ( playerid, false ) ;
		DestroyVehicle( GetPVarInt ( playerid, "buy_vehicle" ) ) ;
		DeletePVar ( playerid, "car_list" );
		DeletePVar ( playerid, "buy_vehicle" );
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if ( GetPVarInt ( playerid, "reg_spawn" ) == 1)
	{
		TogglePlayerControllable ( playerid, true ) ;
		SetPlayerVirtualWorld ( playerid, playerid ) ;
		SetPlayerPos ( playerid, 346.24319, 1687.58423, -10.73690) ;
		SetPlayerFacingAngle ( playerid, 219 ) ;
		TogglePlayerControllable ( playerid, 0 ) ;
		set_skin ( playerid, reg_skins [ 0 ] [ p_info [ playerid ] [ gender ] ] ) ;
		SetPlayerCameraPos ( playerid, 350.3323, 1683.6161, -10.4817 ) ;
		SetPlayerCameraLookAt ( playerid, 349.6181, 1684.3156, -10.5267 ) ;
		return 1;
	}
	set_pos ( playerid, 2228.0400, -1160.4053, 25.7803, 91.3840, 0, 0 ) ;
	set_skin ( playerid, p_info [ playerid ] [ skin ] ) ;
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	new lights, alarm, bonnet, boot, objective, engine, lock;
	GetVehicleParamsEx ( vehicleid, engine, lights, alarm, lock, bonnet, boot, objective ) ;
	SetVehicleParamsEx ( vehicleid, false, lights, alarm, false, bonnet, boot, objective ) ;
	SetVehicleNumberPlate ( vehicleid, veh_info [ vehicleid ] [ v_plate ] ) ;
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if ( p_s_info [ playerid ] [ p_logged ] == false ) return 1 ;
	new text_string [ 128 ] ;
	format( text_string, sizeof ( text_string ), "%s[%d] говорит: %s", p_info [ playerid ] [ name ], playerid, text ) ;
    send_world_message ( playerid, 10.0, text_string, COLOR_WHITE, COLOR_WHITE, COLOR_GRAY) ;
    ApplyAnimation ( playerid, "PED", "IDLE_CHAT", 4.1, 0, 1, 1, 1, 1, 1), SetTimerEx ( "clear_anim", 2000, false, "d", playerid ) ;
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	switch ( newstate )
	{
		case PLAYER_STATE_DRIVER:
		{
			if ( GetPVarInt ( playerid, "in_salon" ) > 0 ) return 1;
			new veh_id = GetPlayerVehicleID ( playerid ) ;
			
			if ( !in_boat ( veh_id ) && !in_bike( veh_id ) && !in_plane( veh_id ) )
			{
				if ( !p_info [ playerid ] [ license ] [ 0 ] ) SetPlayerDrunkLevel ( playerid, 6000 ), SendClientMessage ( playerid, COLOR_ADM,"Вы не умеете водить автомобиль! Советуем Вам выйти из автомобиля!") ;
				SendClientMessage ( playerid, 0x1faee9FF, "Для того чтобы завести или выключить двигатель нажмите {ffffff}'ALT'") ;
				SendClientMessage ( playerid, 0x1faee9FF, "Для того чтобы завести или выключить габариты нажмите {ffffff}'CTRL'") ;
				speedometr_td ( playerid, true ) ;
				for ( new i = 0; i != 7 ; i++ )PlayerTextDrawShow ( playerid, speed_td [ playerid ] [ i ] ) ;
				for ( new i = 0; i != 18 ; i++ )TextDrawShowForPlayer ( playerid, speed_global_td [ i ] ) ;
				p_info [ playerid ] [ speed_timer ] = SetTimerEx("update_player_speedometr", 150, 1, "d", playerid) ;
			}
		}
	}	
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	switch ( GetPlayerState ( playerid ) )
	{
		case PLAYER_STATE_ONFOOT:
		{
			if ( newkeys == KEY_YES )return inventory_use ( playerid, true ) ;
		}
		case PLAYER_STATE_DRIVER:
		{
			if(newkeys & KEY_FIRE)
			{
				new vehicleid = GetPlayerVehicleID ( playerid ) ;
				if ( in_velo ( vehicleid ) ) return 1;
				toggle_engine ( playerid, vehicleid ) ;
				return 1;
			}
			if(newkeys & KEY_ACTION)
			{
				new vehicleid = GetPlayerVehicleID ( playerid ) ;
				if ( in_velo ( vehicleid ) ) return 1;
				toggle_lights ( vehicleid ) ;
				return 1;
			}
		}
	} 
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	if(areaid >= area_info [ 0 ] [ a_area ] [ 0 ] && areaid <= area_info [ tp_areas_amount - 1 ] [ a_area ] [ 1 ] && GetPlayerState ( playerid ) == PLAYER_STATE_ONFOOT )
	{
		if ( GetPVarInt ( playerid, "tp_area_used" ) )return DeletePVar ( playerid, "tp_area_used" ) ;
		new ars_id, sec_id;
		for ( new i; i < tp_areas_amount; i++ )
		{
			if ( areaid == area_info [ i ] [ a_area ] [ 0 ])
			{	
				ars_id = i;
				sec_id = 1;
				break;
			}	
			if ( areaid == area_info [ i ] [ a_area ] [ 1 ])
			{	
				ars_id = i;
				sec_id = 0;
				break;
			}
		}
		if ( area_info [ ars_id ] [ a_to_pos ] [ 0 ] == 0 ) return 1;
		if ( sec_id ) set_pos ( playerid, area_info [ ars_id ] [ a_to_pos ] [ 0 ], area_info [ ars_id ] [ a_to_pos ] [ 1 ], area_info [ ars_id ] [ a_to_pos ] [ 2 ], area_info [ ars_id ] [ a_to_pos ] [ 3 ], area_info [ ars_id ] [ a_interior ] [ sec_id ], area_info [ ars_id ] [ a_virt_world ] [ sec_id ]) ;
		else set_pos ( playerid, area_info [ ars_id ] [ a_pos ] [ 0 ], area_info [ ars_id ] [ a_pos ] [ 1 ], area_info [ ars_id ] [ a_pos ] [ 2 ], area_info [ ars_id ] [ a_pos ] [ 3 ], area_info [ ars_id ] [ a_interior ] [ sec_id ], area_info [ ars_id ] [ a_virt_world ] [ sec_id ]) ;
		SetPVarInt ( playerid, "tp_area_used", 1 ) ;
	}
	if(areaid >= g_areas [ veh_shop ] [ 0 ] && areaid <= g_areas [ veh_shop ] [ 4 ] && GetPlayerState ( playerid ) == PLAYER_STATE_ONFOOT )
	{
		if ( GetPVarInt ( playerid, "in_salon" ) > 0) return DeletePVar ( playerid, "in_salon" );
		new ars_id;
		for ( new i; i < 5; i++ )
		{
			if ( areaid == g_areas [ veh_shop ] [ i ])
			{	
				ars_id = i;
				break;
			}	
		}
		SetPVarInt ( playerid, "in_salon", ars_id + 1) ;
	
		new buy_vehicle = CreateVehicle( cars_for_sale[ 0 ] [ ars_id ], veh_coordinate [ ars_id ] [ 0 ], veh_coordinate [ ars_id ] [ 1 ], veh_coordinate [ ars_id ] [ 2 ], veh_coordinate [ ars_id ] [ 3 ], car_color [ random ( sizeof ( car_color ) ) ], car_color [ random ( sizeof ( car_color ) ) ], 60000 ) ;
		SetPVarInt ( playerid, "buy_vehicle", buy_vehicle ) ;
		SetPVarInt ( playerid, "car_list", 0 ) ;

		RepairVehicle ( buy_vehicle ) ;
		SetVehicleHealth ( buy_vehicle, 1000 ) ;
		SetVehicleVirtualWorld ( buy_vehicle, playerid ) ;
		SetPlayerVirtualWorld ( playerid, playerid ) ;
		PutPlayerInVehicle ( playerid, buy_vehicle, 0) ;
		SetPlayerArmedWeapon ( playerid, 0 ) ;
		TogglePlayerControllable ( playerid, false ) ;
		SetPlayerCameraPos ( playerid, veh_coordinate [ ars_id ] [ 4 ], veh_coordinate [ ars_id ] [ 5 ], veh_coordinate [ ars_id ] [ 6 ] ) ;
		SetPlayerCameraLookAt ( playerid, veh_coordinate [ ars_id ] [ 0 ], veh_coordinate [ ars_id ] [ 1 ], veh_coordinate [ ars_id ] [ 2 ] ) ;

		SetVehiclePos ( buy_vehicle, veh_coordinate [ ars_id ] [ 0 ], veh_coordinate [ ars_id ] [ 1 ], veh_coordinate [ ars_id ] [ 2 ] ) ;
		SetVehicleZAngle ( buy_vehicle, veh_coordinate [ ars_id ] [ 3 ] ) ;
		
		SelectTextDraw(playerid, 0xAFAFAFAA) ;
		new buy_veh_model = GetVehicleModel( buy_vehicle ) ;
		veh_salon_td ( playerid, true ) ;
		new td_string [ 128 ];

		format ( td_string, sizeof td_string, "                ~b~Shop~n~~n~~n~~b~  model:~w~%s~n~~n~~b~  Cost:~w~%d$",get_vehicle_name ( buy_veh_model ), get_vehicle_price ( buy_veh_model )  ) ;
		td_veh_info [ playerid ] = CreatePlayerTextDraw(playerid, 445.000000, 135.000000, td_string );
		PlayerTextDrawBackgroundColor(playerid, td_veh_info [ playerid ], 255);
		PlayerTextDrawFont(playerid, td_veh_info [ playerid ], 2);
		PlayerTextDrawLetterSize(playerid, td_veh_info [ playerid ], 0.360000, 1.900000);
		PlayerTextDrawColor(playerid, td_veh_info [ playerid ], -1);
		PlayerTextDrawSetOutline(playerid, td_veh_info [ playerid ], 0);
		PlayerTextDrawSetProportional(playerid, td_veh_info [ playerid ], 1);
		PlayerTextDrawSetShadow(playerid, td_veh_info [ playerid ], 1);
		PlayerTextDrawSetSelectable(playerid, td_veh_info [ playerid ], 0);
		PlayerTextDrawShow( playerid, td_veh_info [ playerid ] ) ;
	}
	return 1;
}
public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	return 1;
}
public OnPlayerStreamIn(playerid, forplayerid)
{
	Iter_Add(streamed_players[forplayerid], playerid ) ;
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	Iter_Remove(streamed_players[forplayerid], playerid ) ;
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

#include <i_lib/on_dialog_response>
#include <i_lib/server_commands>

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
//	© Leo Juster, 2015. Все права защищены.