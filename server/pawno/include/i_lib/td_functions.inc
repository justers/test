new Text: profile_textdraws [ 14 ] ;
new PlayerText: profile_pl_textdraws [ MAX_PLAYERS ] [ 2 ] ;

new Text: td_veh_shop [ 42 ];
new Text: td_veh_shop_selection [ 7  ];
new PlayerText: td_veh_info [ MAX_PLAYERS ] ;

new PlayerText: create_step1_pl_td [ MAX_PLAYERS ] [ 2 ] ;
new Text: create_step1_td [ 13 ] ;
new PlayerText: create_step4_pl_td [ MAX_PLAYERS ] [ 4 ] ;

new Text: inv_global_td [ 11 ] ;
new PlayerText: inv_player_td [ MAX_PLAYERS ] [ 10 ] ;
new PlayerText: inv_skin_show [ MAX_PLAYERS ] ;

new PlayerText: speed_td [ MAX_PLAYERS ] [ 7 ] ;
new Text: speed_global_td [ 18 ] ;

public OnPlayerClickTextDraw ( playerid, Text:clickedid )
{
	if ( GetPVarInt ( playerid, "profile_enter_td_opened" ) == 1 )
	{
		if(!(_:clickedid ^ 0xFFFF))return SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
	}
	if ( GetPVarInt ( playerid, "create_character_step" ) )
	{
		if(!(_:clickedid ^ 0xFFFF))return SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
	}
	if ( GetPVarInt ( playerid, "in_salon" ) )
	{
		if(!(_:clickedid ^ 0xFFFF))
		{
			veh_salon_td ( playerid, false ) ;
			new salon_id = GetPVarInt ( playerid, "in_salon" ) - 1;
			DestroyVehicle( GetPVarInt ( playerid, "buy_vehicle" ) ) ;
			set_pos ( playerid, veh_exit_coordinate [ salon_id ] [ 0 ],veh_exit_coordinate [ salon_id ] [ 1 ],veh_exit_coordinate [ salon_id ] [ 2 ], veh_exit_coordinate [ salon_id ] [ 3 ],0,0) ;
			TogglePlayerControllable ( playerid, true ) ;
			SetCameraBehindPlayer(playerid) ;
			DeletePVar ( playerid, "car_list" );
			DeletePVar ( playerid, "buy_vehicle" );
		}
	}
	if(_:clickedid == INVALID_TEXT_DRAW)
    { 
        if(GetPVarInt(playerid, "inventory_opened") == 1 && p_s_info [ playerid ] [ p_dialog ] == false) 
        { 
            if(clicked_item [ playerid ] != -1) 
            { 
                PlayerTextDrawBackgroundColor ( playerid, inv_player_td [ playerid ] [ clicked_item [ playerid ] ], 103) ;
            } 
            clicked_item [ playerid ] = -1;
            inventory_use ( playerid, false ) ;
        } 
    } 
	if ( clickedid == td_veh_shop_selection [ 2 ] )
	{
		new salon_id = GetPVarInt ( playerid, "in_salon" ) - 1;
		new salon_list = GetPVarInt ( playerid, "car_list" ) ;
		if( cars_for_sale[ salon_list + 1 ] [ salon_id ] == 0)return 1;
		else salon_list ++;
		DestroyVehicle ( GetPVarInt ( playerid, "buy_vehicle" ) ) ;
		
		new buy_vehicle = CreateVehicle( cars_for_sale[ salon_list ] [ salon_id ], veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ], veh_coordinate [ salon_id ] [ 3 ], car_color [ random ( sizeof ( car_color ) ) ], car_color [ random ( sizeof ( car_color ) ) ], 60000 ) ;
		SetPVarInt ( playerid, "buy_vehicle", buy_vehicle ) ;

		RepairVehicle ( buy_vehicle ) ;
		SetVehicleHealth ( buy_vehicle, 1000 ) ;
		SetVehicleVirtualWorld ( buy_vehicle, playerid ) ;
		SetPlayerVirtualWorld ( playerid, playerid ) ;
		PutPlayerInVehicle ( playerid, buy_vehicle, 0) ;
		SetPlayerArmedWeapon ( playerid, 0 ) ;
		TogglePlayerControllable ( playerid, false ) ;
		SetPlayerCameraPos ( playerid, veh_coordinate [ salon_id ] [ 4 ], veh_coordinate [ salon_id ] [ 5 ], veh_coordinate [ salon_id ] [ 6 ] ) ;
		SetPlayerCameraLookAt ( playerid, veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ] ) ;

		SetVehiclePos ( buy_vehicle, veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ] ) ;
		SetVehicleZAngle ( buy_vehicle, veh_coordinate [ salon_id ] [ 3 ] ) ;
		
		SelectTextDraw(playerid, 0xAFAFAFAA) ;
		new buy_veh_model = GetVehicleModel( buy_vehicle ) ;
		new td_string [ 128 ];
		format ( td_string, sizeof td_string, "                ~b~Shop~n~~n~~n~~b~  model:~w~%s~n~~n~~b~  Cost:~w~%d$",get_vehicle_name ( buy_veh_model ), get_vehicle_price ( buy_veh_model )  ) ;
		PlayerTextDrawSetString( playerid, td_veh_info [ playerid ],td_string) ;
		PlayerTextDrawShow( playerid, td_veh_info [ playerid ] ) ;
		
		SetPVarInt ( playerid, "car_list", salon_list ) ;
		return 1;
    }
	if ( clickedid == td_veh_shop_selection [ 3 ] )
	{
		new salon_id = GetPVarInt ( playerid, "in_salon" ) - 1;
		new salon_list = GetPVarInt ( playerid, "car_list" ) ;
		if( salon_list - 1 < 0)return 1;
		else salon_list --;
		DestroyVehicle ( GetPVarInt ( playerid, "buy_vehicle" ) ) ;
		
		new buy_vehicle = CreateVehicle( cars_for_sale[ salon_list ] [ salon_id ], veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ], veh_coordinate [ salon_id ] [ 3 ], car_color [ random ( sizeof ( car_color ) ) ], car_color [ random ( sizeof ( car_color ) ) ], 60000 ) ;
		SetPVarInt ( playerid, "buy_vehicle", buy_vehicle ) ;

		RepairVehicle ( buy_vehicle ) ;
		SetVehicleHealth ( buy_vehicle, 1000 ) ;
		SetVehicleVirtualWorld ( buy_vehicle, playerid ) ;
		SetPlayerVirtualWorld ( playerid, playerid ) ;
		PutPlayerInVehicle ( playerid, buy_vehicle, 0) ;
		SetPlayerArmedWeapon ( playerid, 0 ) ;
		TogglePlayerControllable ( playerid, false ) ;
		SetPlayerCameraPos ( playerid, veh_coordinate [ salon_id ] [ 4 ], veh_coordinate [ salon_id ] [ 5 ], veh_coordinate [ salon_id ] [ 6 ] ) ;
		SetPlayerCameraLookAt ( playerid, veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ] ) ;

		SetVehiclePos ( buy_vehicle, veh_coordinate [ salon_id ] [ 0 ], veh_coordinate [ salon_id ] [ 1 ], veh_coordinate [ salon_id ] [ 2 ] ) ;
		SetVehicleZAngle ( buy_vehicle, veh_coordinate [ salon_id ] [ 3 ] ) ;
		
		SelectTextDraw(playerid, 0xAFAFAFAA) ;
		new buy_veh_model = GetVehicleModel( buy_vehicle ) ;
		new td_string [ 128 ];
		format ( td_string, sizeof td_string, "                ~b~Shop~n~~n~~n~~b~  model:~w~%s~n~~n~~b~  Cost:~w~%d$",get_vehicle_name ( buy_veh_model ), get_vehicle_price ( buy_veh_model )  ) ;
		PlayerTextDrawSetString( playerid, td_veh_info [ playerid ],td_string) ;
		PlayerTextDrawShow( playerid, td_veh_info [ playerid ] ) ;
		
		SetPVarInt ( playerid, "car_list", salon_list ) ;
 		return 1;
    }	
	if ( clickedid == td_veh_shop_selection [ 4 ] )
	{
		new color [ 2 ], vehicleid = GetPlayerVehicleID ( playerid );
		GetVehicleColor ( vehicleid, color [ 0 ] , color [ 1 ] ) ;
		new sql_string [ 256 ];

		new salon_id = GetPVarInt ( playerid, "in_salon" ) - 1;
		new veh_model = GetVehicleModel ( vehicleid ) ;
		format ( sql_string, sizeof sql_string, "INSERT INTO `ownable_veh_system` ( `v_model`, `v_owner`, `v_pos_x`, `v_pos_y`, `v_pos_z`, `v_pos_c`, `	v_color_one`, `	v_color_two`, `v_millage`) VALUES ('%d','%d','%f','%f','%f','%f','%d','%d','0.000')", veh_model, p_info [ playerid ] [ id ], veh_exit_coordinate [ salon_id ] [ 4 ],
		veh_exit_coordinate [ salon_id ] [ 5 ], veh_exit_coordinate [ salon_id ] [ 6 ], veh_exit_coordinate [ salon_id ] [ 7 ], color [ 0 ], color [ 1 ] ) ;
		mysql_query ( sql_connection, sql_string ) ;
		
		if ( p_info [ playerid ] [ money ] < get_vehicle_price ( veh_model ) )return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Недостаточно средств!") ;
		
		give_money ( playerid, - get_vehicle_price ( veh_model ) ) ;
		set_pos ( playerid, veh_exit_coordinate [ salon_id ] [ 0 ],veh_exit_coordinate [ salon_id ] [ 1 ],veh_exit_coordinate [ salon_id ] [ 2 ], veh_exit_coordinate [ salon_id ] [ 3 ],0,0) ;
		veh_salon_td ( playerid, false ) ;
		DestroyVehicle( GetPVarInt ( playerid, "buy_vehicle" ) ) ;

		format ( sql_string, sizeof sql_string, "SELECT * FROM `ownable_veh_system` WHERE `v_owner` = '%d', `v_model` = '%d', `v_load_this` = '1' LIMIT 1", p_info [ playerid ] [ id ], veh_model ) ;
		mysql_tquery ( sql_connection, sql_string, "load_ownable_veh_system", "") ;
		DeletePVar ( playerid, "car_list" );
		DeletePVar ( playerid, "buy_vehicle" );

		return 1;
     }
	if ( clickedid == profile_textdraws [ 8 ] )
	{
		new profile_name_string [ 13 + 1 ], query_string [ 168 ] ;
		GetPVarString ( playerid, "profile_name", profile_name_string, sizeof(profile_name_string) ) ;
				
		mysql_format ( sql_connection_cross_server, query_string, sizeof ( query_string ), "SELECT * FROM `profiles` WHERE `p_login` = '%e' LIMIT 1", profile_name_string  ) ;
		mysql_tquery ( sql_connection_cross_server, query_string, "profile_reg_check", "i", playerid ) ;
		return 1;
	}
	if ( clickedid == profile_textdraws [ 13 ] )
	{
		CancelSelectTextDraw ( playerid ) ;		
		new profile_name_string [ 13 + 1 ], profile_password_string [ 13 + 1 ], query_string [ 208 ] ;
		GetPVarString ( playerid, "profile_name", profile_name_string, sizeof(profile_name_string) ) ;
		GetPVarString ( playerid, "profile_password", profile_password_string, sizeof(profile_password_string) ) ;
				
		mysql_format ( sql_connection_cross_server, query_string, sizeof ( query_string ), "SELECT * FROM `profiles` WHERE `p_login` = '%e' AND `p_password` = '%e' LIMIT 1", profile_name_string, profile_password_string  ) ;
		mysql_tquery ( sql_connection_cross_server, query_string, "profile_login", "i", playerid ) ;
		return 1;
	}
	if ( clickedid == profile_textdraws [ 13 ] )
	{
		CancelSelectTextDraw ( playerid ) ;		
		new profile_name_string [ 13 + 1 ], profile_password_string [ 13 + 1 ], query_string [ 208 ] ;
		GetPVarString ( playerid, "profile_name", profile_name_string, sizeof(profile_name_string) ) ;
		GetPVarString ( playerid, "profile_password", profile_password_string, sizeof(profile_password_string) ) ;
				
		mysql_format ( sql_connection_cross_server, query_string, sizeof ( query_string ), "SELECT * FROM `profiles` WHERE `p_login` = '%e' AND `p_password` = '%e' LIMIT 1", profile_name_string, profile_password_string  ) ;
		mysql_tquery ( sql_connection_cross_server, query_string, "profile_login", "i", playerid ) ;
		return 1;
	}

	if ( clickedid == create_step1_td [ 7 ] )
	{
		switch ( GetPVarInt ( playerid, "create_character_step" ) )
		{
			case 1:
			{
				if ( !regex_match ( p_info [ playerid ] [ name ], "([A-Z]{1,1})[a-z]{2,9}+_([A-Z]{1,1})[a-z]{2,9}" ) )return SendClientMessage ( playerid, COLOR_RED, "Вы не ввели Role Play имя Вашего персонажа!" ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], "gender" ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 1 ], "  Select gender" ) ;
				SetPVarInt ( playerid, "create_character_step", 2 ) ;
			}
			case 2:
			{
				if ( !p_info [ playerid ][ gender ] )return SendClientMessage ( playerid, COLOR_RED, "Вы не выбрали пол вашего персонажа!" ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], "age" ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 1 ], " enter your age" ) ;
				SetPVarInt ( playerid, "create_character_step", 3 ) ;
			}
			case 3:
			{
				if ( !p_info [ playerid ][ age ] )return SendClientMessage ( playerid, COLOR_RED, "Вы не ввели возраст персонажа!" ) ;
				create_character_td ( playerid, 1, false ) ;
				create_character_td ( playerid, 4, true ) ;

				SetPVarInt ( playerid, "create_character_step", 4 ) ;
			}
			case 4:
			{
				DeletePVar ( playerid, "create_character_step" ) ;
				DeletePVar ( playerid, "reg_spawn" ) ;
				create_character_td ( playerid, 4, false ) ;
				new text_string [ 168 ] ;
				mysql_format ( sql_connection, text_string, sizeof ( text_string ), "INSERT INTO `characters` ( `c_profile`, `c_login`, `c_age`, `c_gender`, `c_skin` ) VALUES ( '%d', '%e', '%d', '%d', '%d')", profile_info [ playerid ] [ profile_increment ], p_info [ playerid ] [ name ], p_info [ playerid ] [ age ], p_info [ playerid ] [ gender ], p_info [ playerid ] [ skin ] ) ;
				mysql_query ( sql_connection, text_string) ;
				mysql_format ( sql_connection, text_string, sizeof ( text_string ), "SELECT * FROM `characters` WHERE `c_login` = '%e' LIMIT 1", p_info [ playerid ] [ name ] ) ;
				mysql_tquery ( sql_connection, text_string, "login_character", "i", playerid ) ;
			}
		}
		return 1;
	}	
	if ( clickedid == create_step1_td [ 9 ] )
	{
		switch ( GetPVarInt ( playerid, "create_character_step" ) )
		{
			case 1:
			{
				InterpolateCameraPos ( playerid, 1719.8282, -808.3868, 73.1169, 1719.8282, -808.3868, 73.1169, 10000, CAMERA_MOVE ) ;
				InterpolateCameraLookAt ( playerid, 1719.3439, -809.2669, 73.2269, 1719.3439, -809.2669, 73.2269, 10000, CAMERA_MOVE ) ;
				create_character_td ( playerid, 1, false ) ;
				new query_string [ 168 ];
				mysql_format ( sql_connection, query_string, sizeof ( query_string ), "SELECT `c_login` FROM `characters` WHERE `c_profile` = '%d' LIMIT 3", profile_info [ playerid ] [ profile_increment ]  ) ;
				mysql_tquery ( sql_connection, query_string, "load_characters", "i", playerid ) ;
			}
			case 2:
			{
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], p_info [ playerid ] [ name ] ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 1 ], "Character name" ) ;
				SetPVarInt ( playerid, "create_character_step", 1 ) ;
			}
			case 3:
			{
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], "gender" ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 1 ], "  Select gender" ) ;
				SetPVarInt ( playerid, "create_character_step", 2 ) ;
			}
			case 4:
			{
				create_character_td ( playerid, 4, false ) ;
				create_character_td ( playerid, 1, true ) ;
				new td_string [ 32 ] ;
				format ( td_string, sizeof ( td_string ), "%d", p_info [ playerid ] [ age ] ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], td_string ) ;
				PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 1 ], " enter your age" ) ;
				SetPVarInt ( playerid, "create_character_step", 3 ) ;
			}
		}
		return 1;
	}
	if ( clickedid == inv_global_td [ 0 ] )
	{
		if ( clicked_item [ playerid ] != - 1 && p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] != 0 )
		{
			switch ( p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] )
			{
				case 1:
				{
					SendClientMessage ( playerid, -1, "Вы сожрали какую-то хуйню" ) ;
				}
			}
			p_info  [ playerid ] [ inventory_amount ] [ clicked_item [ playerid ] ] --;
			if ( p_info  [ playerid ] [ inventory_amount ] [ clicked_item [ playerid ] ] == 0 ) p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] = 0;
			PlayerTextDrawBackgroundColor( playerid, inv_player_td [ playerid ] [ clicked_item [ playerid ] ], 103) ;
			update_inventory ( playerid, clicked_item [ playerid ] , p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ]) ;
			clicked_item [ playerid ] = -1;
		}
		else return 1;
		return 1;
	}
	if ( clickedid == inv_global_td [ 8 ] )
	{
		if ( clicked_item [ playerid ] != - 1 && p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] != 0 )
		{
			drop_object(playerid, item_info [ p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] ][ item_id ], p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] ) ;
			PlayerTextDrawBackgroundColor( playerid, inv_player_td [ playerid ] [ clicked_item [ playerid ] ], 103) ;
			update_inventory ( playerid, clicked_item [ playerid ] , p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ]) ;
			clicked_item [ playerid ] = -1;
		}
		else return 1;
		return 1;
	}
	if ( clickedid == inv_global_td [ 9 ] )
	{
		if ( GetPVarInt ( playerid, "sell_or_dare" ) ) return SendClientMessage ( playerid, COLOR_GRAY, "Невозможно использовать сейчас!" ) ;
		if ( clicked_item [ playerid ] != - 1 && p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] != 0 )
		{
			CancelSelectTextDraw ( playerid ) ;
			show_dialog ( playerid, d_give_object_id, DIALOG_STYLE_INPUT, "{2E8B57}Передать предмет", "{FFFFFF}Введите ID игрока:", "Далее", "Закрыть" ) ;
			SetPVarInt ( playerid, "sell_or_dare", 2 ) ;
		}
		else return 1;
		return 1;
	}
	if ( clickedid == inv_global_td [ 10 ] )
	{
		if ( GetPVarInt ( playerid, "sell_or_dare" ) ) return SendClientMessage ( playerid, COLOR_GRAY, "Невозможно использовать сейчас!" ) ;
		if ( clicked_item [ playerid ] != - 1 && p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] != 0 )
		{
			CancelSelectTextDraw ( playerid ) ;
			show_dialog ( playerid, d_give_object_id, DIALOG_STYLE_INPUT, "{2E8B57}Продать предмет", "{FFFFFF}Введите ID игрока:", "Далее", "Закрыть" ) ;
			SetPVarInt ( playerid, "sell_or_dare", 1 ) ;
		}
		else return 1;
		return 1;
	}
	for(new i = 0; i < 43; i++)
	{
		if ( clickedid == td_veh_shop [ i ] )
		{
		    ChangeVehicleColor ( GetPlayerVehicleID(playerid), car_color [ i ], car_color [ i ]) ;
		    break;
        }
	}
	return 1;
}

public OnPlayerClickPlayerTextDraw ( playerid, PlayerText:playertextid )
{
	if ( playertextid == profile_pl_textdraws [ playerid ] [ 0 ] )CancelSelectTextDraw ( playerid ), show_dialog ( playerid, d_profile_name, DIALOG_STYLE_INPUT, "{2E8B57}Логин учётной записи", "{FFFFFF}Введите логин учётной записи:", "Принять", "Назад" ) ;
	else if ( playertextid == profile_pl_textdraws [ playerid ] [ 1 ] )CancelSelectTextDraw ( playerid ), show_dialog ( playerid, d_profile_password, DIALOG_STYLE_INPUT, "{2E8B57}Пароль учётной записи", "{FFFFFF}Введите пароль учётной записи:", "Принять", "Назад" ) ;
	else if ( playertextid == create_step1_pl_td [ playerid ] [ 0 ] )
	{
		switch ( GetPVarInt ( playerid, "create_character_step" ) )
		{
			case 1:return show_dialog ( playerid, d_character_name, DIALOG_STYLE_INPUT, "{2E8B57}Имя персонажа", "{FFFFFF}Введите имя Вашего персонажа:", "Принять", "Назад" ) ;
			case 2:return show_dialog ( playerid, d_character_gender, DIALOG_STYLE_LIST, "{2E8B57}Пол персонажа", "{FFFFFF}Мужчина\nЖенщина", "Далее", "Назад" ) ;
			case 3:return show_dialog ( playerid, d_character_age, DIALOG_STYLE_INPUT, "{2E8B57}Возраст персонажа", "{FFFFFF}Введите возраст Вашего персонажа:", "Далее", "Назад" ) ;
		}
	}	
	else if ( playertextid == create_step4_pl_td [ playerid ] [ 0 ] ) set_skin ( playerid, reg_skins [ 0 ] [ p_info [ playerid ] [ gender ] - 1] ) ;
	else if ( playertextid == create_step4_pl_td [ playerid ] [ 1 ] ) set_skin ( playerid, reg_skins [ 1 ] [ p_info [ playerid ] [ gender ] - 1] ) ;
	else if ( playertextid == create_step4_pl_td [ playerid ] [ 2 ] ) set_skin ( playerid, reg_skins [ 2 ] [ p_info [ playerid ] [ gender ] - 1] ) ;
	else if ( playertextid == create_step4_pl_td [ playerid ] [ 3 ] ) set_skin ( playerid, reg_skins [ 3 ] [ p_info [ playerid ] [ gender ] - 1] ) ;
	for(new j = 0; j < 10; j++)
    { 
        if(playertextid == inv_player_td [ playerid ] [ j ]) 
        { 
            if(clicked_item [ playerid ] == -1)
            { 
                if(p_info  [ playerid ] [ inventory_slot ] [ j ] != 0)
                { 
                    clicked_item [ playerid ] = j;
                    PlayerTextDrawBackgroundColor ( playerid, inv_player_td [ playerid ] [ j ], 0x00000034) ;
                    PlayerTextDrawHide ( playerid, inv_player_td [ playerid ] [ j ]) ;
                    PlayerTextDrawShow ( playerid, inv_player_td [ playerid ] [ j ]) ; 
					break;
                } 
            } 
            else 
            { 
                if(p_info  [ playerid ] [ inventory_slot ] [ j ] == 0) 
                { 
                    p_info  [ playerid ] [ inventory_slot ] [ j ] = p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] ; 
                    p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] = 0; 
                    PlayerTextDrawBackgroundColor ( playerid, inv_player_td [ playerid ] [ clicked_item [ playerid ] ], 103) ;
                    update_inventory(playerid, clicked_item [ playerid ], p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] ) ; 
                    update_inventory(playerid, j, p_info  [ playerid ] [ inventory_slot ] [ j ] ) ; 
                    clicked_item [ playerid ] = -1;
					break;
                } 
                if(j == clicked_item [ playerid ])
                { 
                    clicked_item [ playerid ] = -1; 
                    PlayerTextDrawBackgroundColor ( playerid, inv_player_td [ playerid ] [ j ], 103) ;
                    PlayerTextDrawHide ( playerid, inv_player_td [ playerid ] [ j ]) ; 
                    PlayerTextDrawShow ( playerid, inv_player_td [ playerid ] [ j ]) ; 
 					break;
               } 
            } 
        } 
    }  
	return 1;
}
stock create_character_td ( playerid, step, bool:status )
{
	switch ( step )
	{
		case 1:
		{
			if ( status == true )
			{
				SetPVarInt ( playerid, "create_character_step", 1 ) ;
			
				SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
				for ( new i = 0; i < 10; i ++ ) TextDrawShowForPlayer ( playerid, create_step1_td [ i ] ) ;
				
				create_step1_pl_td [ playerid ] [ 0 ] = CreatePlayerTextDraw(playerid, 209.000000, 355.000000, "Name") ;
				PlayerTextDrawBackgroundColor(playerid, create_step1_pl_td [ playerid ] [ 0 ], 255) ;
				PlayerTextDrawFont(playerid, create_step1_pl_td [ playerid ] [ 0 ], 2) ;
				PlayerTextDrawLetterSize(playerid, create_step1_pl_td [ playerid ] [ 0 ], 0.470000, 1.500000) ;
				PlayerTextDrawColor(playerid, create_step1_pl_td [ playerid ] [ 0 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step1_pl_td [ playerid ] [ 0 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step1_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step1_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawSetSelectable(playerid, create_step1_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawTextSize ( playerid, create_step1_pl_td [ playerid ] [ 0 ], 367.000000, 14 ) ;

				create_step1_pl_td [ playerid ] [ 1 ] = CreatePlayerTextDraw(playerid, 223.000000, 326.000000, "Character name") ;
				PlayerTextDrawBackgroundColor(playerid, create_step1_pl_td [ playerid ] [ 1 ], 255) ;
				PlayerTextDrawFont(playerid, create_step1_pl_td [ playerid ] [ 1 ], 2) ;
				PlayerTextDrawLetterSize(playerid, create_step1_pl_td [ playerid ] [ 1 ], 0.519999, 1.200000) ;
				PlayerTextDrawColor(playerid, create_step1_pl_td [ playerid ] [ 1 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step1_pl_td [ playerid ] [ 1 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step1_pl_td [ playerid ] [ 1 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step1_pl_td [ playerid ] [ 1 ], 1) ;
				PlayerTextDrawSetSelectable(playerid, create_step1_pl_td [ playerid ] [ 1 ], 0) ;

				PlayerTextDrawShow ( playerid, create_step1_pl_td [ playerid ] [ 0 ] ) ;
				PlayerTextDrawShow ( playerid, create_step1_pl_td [ playerid ] [ 1 ] ) ;
				TextDrawShowForPlayer ( playerid, create_step1_td [ 10 ] ) ;
				return 1;
			}
			else if ( status == false )
			{
				for ( new i = 0; i < 10; i ++ ) TextDrawHideForPlayer ( playerid, create_step1_td [ i ] ) ;
				TextDrawHideForPlayer ( playerid, create_step1_td [ 10 ] ) ;
				PlayerTextDrawDestroy ( playerid, create_step1_pl_td [ playerid ] [ 0 ] ) ;
				PlayerTextDrawDestroy ( playerid, create_step1_pl_td [ playerid ] [ 1 ] ) ;
				create_step1_pl_td [ playerid ] [ 1 ] = PlayerText:INVALID_TEXT_DRAW;
				create_step1_pl_td [ playerid ] [ 0 ] = PlayerText:INVALID_TEXT_DRAW;
			}
		}	
		case 4:
		{
			if ( status == true )
			{
				create_step4_pl_td [ playerid ] [ 0 ] = CreatePlayerTextDraw(playerid, 265.000000, 350.000000, "skin_1") ;
				PlayerTextDrawBackgroundColor(playerid, create_step4_pl_td [ playerid ] [ 0 ], 0) ;
				PlayerTextDrawFont(playerid, create_step4_pl_td [ playerid ] [ 0 ], 5) ;
				PlayerTextDrawLetterSize(playerid, create_step4_pl_td [ playerid ] [ 0 ], 0.500000, 1.000000) ;
				PlayerTextDrawColor(playerid, create_step4_pl_td [ playerid ] [ 0 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step4_pl_td [ playerid ] [ 0 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step4_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step4_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawBoxColor(playerid, create_step4_pl_td [ playerid ] [ 0 ], 255) ;
				PlayerTextDrawTextSize(playerid, create_step4_pl_td [ playerid ] [ 0 ], 55.000000, 59.000000) ;
				PlayerTextDrawSetSelectable(playerid, create_step4_pl_td [ playerid ] [ 0 ], 1) ;
				PlayerTextDrawSetPreviewModel(playerid, create_step4_pl_td [ playerid ] [ 0 ], reg_skins [ 0 ] [ p_info [ playerid ] [ gender ] - 1]) ;

				create_step4_pl_td [ playerid ] [ 1 ] = CreatePlayerTextDraw(playerid, 321.000000, 350.000000, "skin_1") ;
				PlayerTextDrawBackgroundColor(playerid, create_step4_pl_td [ playerid ] [ 1 ], 0) ;
				PlayerTextDrawFont(playerid, create_step4_pl_td [ playerid ] [ 1 ], 5) ;
				PlayerTextDrawLetterSize(playerid, create_step4_pl_td [ playerid ] [ 1 ], 0.500000, 1.000000) ;
				PlayerTextDrawColor(playerid, create_step4_pl_td [ playerid ] [ 1 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step4_pl_td [ playerid ] [ 1 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step4_pl_td [ playerid ] [ 1 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step4_pl_td [ playerid ] [ 1 ], 1) ;
				PlayerTextDrawBoxColor(playerid, create_step4_pl_td [ playerid ] [ 1 ], 255) ;
				PlayerTextDrawTextSize(playerid, create_step4_pl_td [ playerid ] [ 1 ], 55.000000, 59.000000) ;
				PlayerTextDrawSetSelectable(playerid, create_step4_pl_td [ playerid ] [ 1 ], 1) ;
				PlayerTextDrawSetPreviewModel(playerid, create_step4_pl_td [ playerid ] [ 1 ], reg_skins [ 1 ] [ p_info [ playerid ] [ gender ] - 1]) ;

				create_step4_pl_td [ playerid ] [ 2 ] = CreatePlayerTextDraw(playerid, 375.000000, 350.000000, "skin_1") ;
				PlayerTextDrawBackgroundColor(playerid, create_step4_pl_td [ playerid ] [ 2 ], 0) ;
				PlayerTextDrawFont(playerid, create_step4_pl_td [ playerid ] [ 2 ], 5) ;
				PlayerTextDrawLetterSize(playerid, create_step4_pl_td [ playerid ] [ 2 ], 0.500000, 1.000000) ;
				PlayerTextDrawColor(playerid, create_step4_pl_td [ playerid ] [ 2 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step4_pl_td [ playerid ] [ 2 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step4_pl_td [ playerid ] [ 2 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step4_pl_td [ playerid ] [ 2 ], 1) ;
				PlayerTextDrawBoxColor(playerid, create_step4_pl_td [ playerid ] [ 2 ], 255) ;
				PlayerTextDrawTextSize(playerid, create_step4_pl_td [ playerid ] [ 2 ], 55.000000, 59.000000) ;
				PlayerTextDrawSetSelectable(playerid, create_step4_pl_td [ playerid ] [ 2 ], 1) ;
				PlayerTextDrawSetPreviewModel(playerid, create_step4_pl_td [ playerid ] [ 2 ], reg_skins [ 2 ] [ p_info [ playerid ] [ gender ] - 1]) ;

				create_step4_pl_td [ playerid ] [ 3 ] = CreatePlayerTextDraw(playerid, 208.000000, 350.000000, "skin_1") ;
				PlayerTextDrawBackgroundColor(playerid, create_step4_pl_td [ playerid ] [ 3 ], 0) ;
				PlayerTextDrawFont(playerid, create_step4_pl_td [ playerid ] [ 3 ], 5) ;
				PlayerTextDrawLetterSize(playerid, create_step4_pl_td [ playerid ] [ 3 ], 0.500000, 1.000000) ;
				PlayerTextDrawColor(playerid, create_step4_pl_td [ playerid ] [ 3 ], -1) ;
				PlayerTextDrawSetOutline(playerid, create_step4_pl_td [ playerid ] [ 3 ], 0) ;
				PlayerTextDrawSetProportional(playerid, create_step4_pl_td [ playerid ] [ 3 ], 1) ;
				PlayerTextDrawSetShadow(playerid, create_step4_pl_td [ playerid ] [ 3 ], 1) ;
				PlayerTextDrawBoxColor(playerid, create_step4_pl_td [ playerid ] [ 3 ], 255) ;
				PlayerTextDrawTextSize(playerid, create_step4_pl_td [ playerid ] [ 3 ], 55.000000, 59.000000) ;
				PlayerTextDrawSetSelectable(playerid, create_step4_pl_td [ playerid ] [ 3 ], 1) ;
				PlayerTextDrawSetPreviewModel(playerid, create_step4_pl_td [ playerid ] [ 3 ], reg_skins [ 3 ] [ p_info [ playerid ] [ gender ] - 1]) ;

				PlayerTextDrawShow ( playerid, create_step4_pl_td [ playerid ] [ 0 ] ) ;
				PlayerTextDrawShow ( playerid, create_step4_pl_td [ playerid ] [ 1 ] ) ;
				PlayerTextDrawShow ( playerid, create_step4_pl_td [ playerid ] [ 2 ] ) ;
				PlayerTextDrawShow ( playerid, create_step4_pl_td [ playerid ] [ 3 ] ) ;
				for ( new i = 0; i < 12; i ++ )
				{	
					if ( i == 2 || i == 3 || i == 4 ) continue;
					TextDrawShowForPlayer ( playerid, create_step1_td [ i ] ) ;
				}
			}
			else if ( status == false )
			{
				PlayerTextDrawDestroy ( playerid, create_step4_pl_td [ playerid ] [ 0 ] ) ;
				PlayerTextDrawDestroy ( playerid, create_step4_pl_td [ playerid ] [ 1 ] ) ;
				PlayerTextDrawDestroy ( playerid, create_step4_pl_td [ playerid ] [ 2 ] ) ;
				PlayerTextDrawDestroy ( playerid, create_step4_pl_td [ playerid ] [ 3 ] ) ;
				create_step4_pl_td [ playerid ] [ 0 ] = PlayerText:INVALID_TEXT_DRAW;
				create_step4_pl_td [ playerid ] [ 1 ] = PlayerText:INVALID_TEXT_DRAW;
				create_step4_pl_td [ playerid ] [ 2 ] = PlayerText:INVALID_TEXT_DRAW;
				create_step4_pl_td [ playerid ] [ 3 ] = PlayerText:INVALID_TEXT_DRAW;

				for ( new i = 0; i < 12; i ++ )
				{	
					if ( i == 2 || i == 3 || i == 4 ) continue;
					TextDrawHideForPlayer ( playerid, create_step1_td [ i ] ) ;
				}
			}
		}
	}
	return 1;
}
stock profile_enter_td ( playerid, bool:status )
{
	if ( status == true )
	{
	
		SetPVarInt ( playerid, "profile_enter_td_opened", 1 ) ;
		SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
		

		for ( new i = 0; i < 14; i ++ ) TextDrawShowForPlayer ( playerid, profile_textdraws [ i ] ) ;
		
	
		profile_pl_textdraws [ playerid ] [ 0 ] = CreatePlayerTextDraw(playerid,258.000000, 180.000000, "	         Profile" ) ;
		PlayerTextDrawBackgroundColor ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 255 ) ;
		PlayerTextDrawFont ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 2  ) ;
		PlayerTextDrawLetterSize ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 0.289999, 2.099999 ) ;
		PlayerTextDrawColor ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], -1 ) ;
		PlayerTextDrawSetOutline ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 0 ) ;
		PlayerTextDrawSetProportional ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 1 ) ;
		PlayerTextDrawSetShadow ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 1 ) ;
		PlayerTextDrawTextSize ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 367.000000, 14 ) ;
		PlayerTextDrawSetSelectable ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], 1 ) ;

		profile_pl_textdraws [ playerid ] [ 1 ] = CreatePlayerTextDraw(playerid,258.000000, 223.000000, "	       Password" ) ;
		PlayerTextDrawBackgroundColor ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 255 ) ;
		PlayerTextDrawFont ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 2 ) ;
		PlayerTextDrawLetterSize ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 0.289999, 2.099999 ) ;
		PlayerTextDrawColor ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], -1 ) ;
		PlayerTextDrawSetOutline ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 0 ) ;
		PlayerTextDrawSetProportional ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 1 ) ;
		PlayerTextDrawSetShadow ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 1 ) ;
		PlayerTextDrawTextSize ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 367.000000, 14 ) ;
		PlayerTextDrawSetSelectable ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], 1 ) ;

		
		PlayerTextDrawShow ( playerid, profile_pl_textdraws [ playerid ] [ 0 ] ) ;
		PlayerTextDrawShow ( playerid, profile_pl_textdraws [ playerid ] [ 1 ] ) ;
	}
	else
	{
		DeletePVar ( playerid, "profile_enter_td_opened" ) ;
		for ( new i = 0; i < 14; i ++ ) TextDrawHideForPlayer ( playerid, profile_textdraws [ i ] ) ;
		PlayerTextDrawDestroy ( playerid, profile_pl_textdraws [ playerid ] [ 0 ] ) ;
		PlayerTextDrawDestroy ( playerid, profile_pl_textdraws [ playerid ] [ 1 ] ) ;
		profile_pl_textdraws [ playerid ] [ 0 ] = PlayerText:INVALID_TEXT_DRAW;
		profile_pl_textdraws [ playerid ] [ 1 ] = PlayerText:INVALID_TEXT_DRAW;

		CancelSelectTextDraw ( playerid ) ;
	}
	return 1;
}
stock inventory_use ( playerid, bool:status )
{
	if ( status == true )
	{
	
		SetPVarInt ( playerid, "inventory_opened", 1 ) ;
		
		inv_player_td  [ playerid ] [ 0 ] = CreatePlayerTextDraw(playerid, 216.000000, 324.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 0 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 0 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 0 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 0 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 0 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 0 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 0 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 0 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 0 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 0 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 0 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 0 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 0 ], 1) ;

		inv_player_td  [ playerid ] [ 1 ] = CreatePlayerTextDraw(playerid, 258.000000, 324.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 1 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 1 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 1 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 1 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 1 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 1 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 1 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 1 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 1 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 1 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 1 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 1 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 1 ], 1) ;

		inv_player_td  [ playerid ] [ 2 ] = CreatePlayerTextDraw(playerid, 300.000000, 324.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 2 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 2 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 2 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 2 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 2 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 2 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 2 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 2 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 2 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 2 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 2 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 2 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 2 ], 1) ;

		inv_player_td  [ playerid ] [ 3 ] = CreatePlayerTextDraw(playerid, 342.000000, 324.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 3 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 3 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 3 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 3 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 3 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 3 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 3 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 3 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 3 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 3 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 3 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 3 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 3 ], 1) ;

		inv_player_td  [ playerid ] [ 4 ] = CreatePlayerTextDraw(playerid, 384.000000, 324.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 4 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 4 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 4 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 4 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 4 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 4 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 4 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 4 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 4 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 4 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 4 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 4 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 4 ], 1) ;

		inv_player_td  [ playerid ] [ 5 ] = CreatePlayerTextDraw(playerid, 216.000000, 367.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 5 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 5 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 5 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 5 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 5 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 5 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 5 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 5 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 5 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 5 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 5 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 5 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 5 ], 1) ;

		inv_player_td  [ playerid ] [ 6 ] = CreatePlayerTextDraw(playerid, 258.000000, 367.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 6 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 6 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 6 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 6 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 6 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 6 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 6 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 6 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 6 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 6 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 6 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 6 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 6 ], 1) ;

		inv_player_td  [ playerid ] [ 7 ] = CreatePlayerTextDraw(playerid, 300.000000, 367.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 7 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 7 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 7 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 7 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 7 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 7 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 7 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 7 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 7 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 7 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 7 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 7 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 7 ], 1) ;

		inv_player_td  [ playerid ] [ 8 ] = CreatePlayerTextDraw(playerid, 342.000000, 367.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 8 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 8 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 8 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 8 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 8 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 8 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 8 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 8 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 8 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 8 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 8 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 8 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 8 ], 1) ;

		inv_player_td  [ playerid ] [ 9 ] = CreatePlayerTextDraw(playerid, 384.000000, 367.000000, "td_inv_icon") ;
		PlayerTextDrawBackgroundColor(playerid, inv_player_td  [ playerid ] [ 9 ], 103) ;
		PlayerTextDrawFont(playerid, inv_player_td  [ playerid ] [ 9 ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_player_td  [ playerid ] [ 9 ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_player_td  [ playerid ] [ 9 ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_player_td  [ playerid ] [ 9 ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_player_td  [ playerid ] [ 9 ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_player_td  [ playerid ] [ 9 ], 1) ;
		PlayerTextDrawUseBox(playerid, inv_player_td  [ playerid ] [ 9 ], 1) ;
		PlayerTextDrawBoxColor(playerid, inv_player_td  [ playerid ] [ 9 ], 0) ;
		PlayerTextDrawTextSize(playerid, inv_player_td  [ playerid ] [ 9 ], 41.000000, 42.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_player_td  [ playerid ] [ 9 ], 1649) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_player_td  [ playerid ] [ 9 ], -16.000000, 180.000000, 0.000000, 0.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_player_td  [ playerid ] [ 9 ], 1) ;
		
		inv_skin_show  [ playerid ] = CreatePlayerTextDraw(playerid, 248.000000, 137.000000, "skin") ;
		PlayerTextDrawBackgroundColor(playerid, inv_skin_show  [ playerid ], 0) ;
		PlayerTextDrawFont(playerid, inv_skin_show  [ playerid ], 5) ;
		PlayerTextDrawLetterSize(playerid, inv_skin_show  [ playerid ], 0.500000, 1.000000) ;
		PlayerTextDrawColor(playerid, inv_skin_show  [ playerid ], -1) ;
		PlayerTextDrawSetOutline(playerid, inv_skin_show  [ playerid ], 0) ;
		PlayerTextDrawSetProportional(playerid, inv_skin_show  [ playerid ], 1) ;
		PlayerTextDrawSetShadow(playerid, inv_skin_show  [ playerid ], 0) ;
		PlayerTextDrawUseBox(playerid, inv_skin_show  [ playerid ], 0) ;
		PlayerTextDrawBoxColor(playerid, inv_skin_show  [ playerid ], 255) ;
		PlayerTextDrawTextSize(playerid, inv_skin_show  [ playerid ], 144.000000, 184.000000) ;
		PlayerTextDrawSetPreviewModel(playerid, inv_skin_show  [ playerid ], p_info [ playerid ] [ skin ]) ;
		PlayerTextDrawSetPreviewRot(playerid, inv_skin_show  [ playerid ], 0.000000, 0.000000, 0.000000, 1.000000) ;
		PlayerTextDrawSetSelectable(playerid, inv_skin_show  [ playerid ], 0) ;
		
		for(new j = 0; j < 10; j++) 
		{ 
			PlayerTextDrawSetPreviewModel ( playerid, inv_player_td [ playerid ][ j ], item_info [ p_info [ playerid ] [ inventory_slot ] [ j ] ] [item_id] ) ; 
			PlayerTextDrawSetPreviewRot ( playerid, inv_player_td [ playerid ] [ j ], item_info [ p_info [ playerid ] [ inventory_slot ] [ j ] ] [rot_x], item_info [ p_info [ playerid ] [ inventory_slot ] [ j ] ] [rot_y], item_info [ p_info [ playerid ] [ inventory_slot ] [ j ] ] [rot_z], item_info [ p_info [ playerid ] [ inventory_slot ] [ j ] ] [zoom] ) ; 
		} 
		for(new i; i < sizeof(inv_global_td) ; i++)
		{ 
			TextDrawShowForPlayer(playerid,inv_global_td [ i ]) ;
		} 
		for(new i; i < 10; i++)
		{ 
			PlayerTextDrawShow ( playerid, inv_player_td [ playerid ] [ i ]) ;
			
		} 
		PlayerTextDrawShow ( playerid, inv_skin_show  [ playerid ]) ;
		SelectTextDraw(playerid,0x2641FEAA) ;
	}
	else
	{
	    for(new i; i < sizeof(inv_global_td) ; i++) 
		{ 
			TextDrawHideForPlayer ( playerid, inv_global_td [ i ]) ; 
		} 
		for(new i; i < 10; i++) 
		{ 
			PlayerTextDrawDestroy ( playerid, inv_player_td [ playerid ] [ i ]) ; 
			inv_player_td [ playerid ] [ i ] = PlayerText:INVALID_TEXT_DRAW;

		} 
		CancelSelectTextDraw ( playerid ) ; 
		PlayerTextDrawHide ( playerid, inv_skin_show  [ playerid ]) ;
		PlayerTextDrawDestroy ( playerid, inv_skin_show [ playerid ]) ; 
		inv_skin_show  [ playerid ] = PlayerText:INVALID_TEXT_DRAW;
		DeletePVar ( playerid, "inventory_opened" ) ;
	}
	return 1;
}
stock speedometr_td ( playerid, bool:status)
{
	if ( status == true )
	{
		speed_td[playerid][0] = CreatePlayerTextDraw(playerid, 505.500000, 381.062500, " ") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][0], 0.321999, 1.245625) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][0], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][0], -1) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][0], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][0], 1) ;
		PlayerTextDrawBackgroundColor(playerid, speed_td[playerid][0], 51) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][0], 2) ;
		PlayerTextDrawSetProportional(playerid, speed_td[playerid][0], 1) ;

		speed_td[playerid][1] = CreatePlayerTextDraw(playerid, 396.000000, 416.062500, " ") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][1], 0.185000, 1.004999) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][1], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][1], -1) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][1], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][1], 1) ;
		PlayerTextDrawBackgroundColor(playerid, speed_td[playerid][1], 51) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][1], 2) ;
		PlayerTextDrawSetProportional(playerid, speed_td[playerid][1], 1) ;

		speed_td[playerid][2] = CreatePlayerTextDraw(playerid, 400.500000, 394.187500, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][2], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td[playerid][2], 6.500000, 0.875000) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][2], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][2], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][2], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][2], 0) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][2], 4) ;

		speed_td[playerid][3] = CreatePlayerTextDraw(playerid, 400.500000, 394.187500, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][3], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td[playerid][3], 3.500000, 10.062500) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][3], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][3], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][3], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][3], 0) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][3], 4) ;

		speed_td[playerid][4] = CreatePlayerTextDraw(playerid, 400.500000, 402.937500, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][4], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td[playerid][4], 6.500000, 1.312500) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][4], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][4], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][4], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][4], 0) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][4], 4) ;

		speed_td[playerid][5] = CreatePlayerTextDraw(playerid, 405.500000, 394.625000, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][5], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td[playerid][5], 1.000000, 9.625000) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][5], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][5], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][5], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][5], 0) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][5], 4) ;

		speed_td[playerid][6] = CreatePlayerTextDraw(playerid, 527.000000, 416.062500, "LD_SPAC:white") ;
		PlayerTextDrawLetterSize(playerid, speed_td[playerid][6], 0.000000, 0.000000) ;
		PlayerTextDrawTextSize(playerid, speed_td[playerid][6], 1.000000, 7.000000) ;
		PlayerTextDrawAlignment(playerid, speed_td[playerid][6], 1) ;
		PlayerTextDrawColor(playerid, speed_td[playerid][6], -16776961) ;
		PlayerTextDrawSetShadow(playerid, speed_td[playerid][6], 0) ;
		PlayerTextDrawSetOutline(playerid, speed_td[playerid][6], 0) ;
		PlayerTextDrawFont(playerid, speed_td[playerid][6], 4) ;
	}
	else
	{
		for ( new i = 0 ; i != 7; i++ )PlayerTextDrawDestroy ( playerid, speed_td [ playerid ] [ i ] ),speed_td [ playerid ] [ i ] = PlayerText:INVALID_TEXT_DRAW;
	}
}
stock veh_salon_td ( playerid, bool:status)
{
	if ( status == true )
	{

		for(new i; i != 7 ; i++)
		{ 
			TextDrawShowForPlayer(playerid, td_veh_shop_selection [ i ]) ;
		} 
		for(new i; i != 42 ; i++)
		{ 
			TextDrawShowForPlayer(playerid, td_veh_shop [ i ]) ;
		}
	}
	else 
	{
		for(new i; i != 7 ; i++)
		{ 
			TextDrawHideForPlayer ( playerid, td_veh_shop_selection [ i ] ) ;
		} 
		for(new i; i != 42 ; i++)
		{ 
			TextDrawHideForPlayer ( playerid, td_veh_shop [ i ] ) ;
		} 
		PlayerTextDrawDestroy ( playerid, td_veh_info [ playerid ] ) ;
		CancelSelectTextDraw(playerid) ;
		td_veh_info [ playerid ] = PlayerText:INVALID_TEXT_DRAW;
	}
	return 1;
}



//SendClientMessage ( playerid, COLOR_RED, "Если Вы желаете выйти из меню создания персонажа нажмите `Esc`" ) ;