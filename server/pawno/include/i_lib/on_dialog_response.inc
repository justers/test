public OnDialogResponse ( playerid,  dialogid, response, listitem, inputtext[])
{
	strreplace(inputtext, '%', '#') ;
	strreplace(inputtext, '\\', '#') ;
	p_s_info [ playerid ] [ p_dialog ] = false;
	switch(dialogid)
	{
		case d_none:
		{
			if ( GetPVarInt ( playerid, "profile_enter_td_opened" ) == 1 )return SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
		}
		case d_profile_name:
		{
			if ( !response )return SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
			if ( !regex_match ( inputtext, "[[:word:]]{3,13}" ) )return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина логина не должна быть менее 3 и более 12 символов\n- Логин может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
			for(new i = strlen(inputtext) ; i != 0; --i)
			{
				switch(inputtext[i])
				{
					case 'А'..'Я', 'а'..'я', ' ': return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина логина не должна быть менее 6 и более 12 символов\n- Логин может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
				}
			}	
 			SetPVarString ( playerid, "profile_name", inputtext ) ;
			
			PlayerTextDrawSetString ( playerid, profile_pl_textdraws [ playerid ] [ 0 ], inputtext ) ;
			SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
			return 1;
		}
		case d_profile_password:
		{
			if ( !response )return SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
			if ( !regex_match ( inputtext, "[[:word:]]{6,13}" ) )return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина пароля не должна быть менее 6 и более 12 символов\n- Пароль может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
			for(new i = strlen(inputtext) ; i != 0; --i)
			{
				switch(inputtext[i])
				{
					case 'А'..'Я', 'а'..'я', ' ': return show_dialog ( playerid, d_none, DIALOG_STYLE_MSGBOX, "{2E8B57}Примечание", "\n{FFFFFF}- Длина пароля не должна быть менее 6 и более 12 символов\n- Пароль может включать только буквы, цифры и знаки подчёркивания", "Назад", "" ) ;
				}
			}	
 
			PlayerTextDrawSetString ( playerid, profile_pl_textdraws [ playerid ] [ 1 ], inputtext ) ;

			SetPVarString ( playerid, "profile_password", inputtext ) ;
			SelectTextDraw ( playerid, 0xAFAFAFAA ) ;
			return 1;
		}
		case d_list_character:
		{
			if ( !response ) 
			{
				new query_string [ 168 ];
				mysql_format ( sql_connection, query_string, sizeof ( query_string ), "SELECT * FROM `c_login` WHERE `c_profile` = '%d' LIMIT 3", profile_info [ playerid ] [ profile_increment ] ) ;
				mysql_tquery ( sql_connection, query_string, "load_characters", "i", playerid ) ;
				return 1;
			}
			if ( listitem < GetPVarInt ( playerid, "characters_rows" ) )
			{
				new character_name [ 32 ], query_string [ 168 ], mysql_name [ 32 ];
				format ( character_name, sizeof ( character_name ), "character_name_%d", listitem ) ;
				GetPVarString ( playerid, character_name, mysql_name, sizeof ( mysql_name ) ) ;
				mysql_format ( sql_connection, query_string, sizeof ( query_string ), "SELECT * FROM `characters` WHERE `c_login` = '%e' LIMIT 1", mysql_name ) ;
				mysql_tquery ( sql_connection, query_string, "login_character", "i", playerid ) ;
				return 1;
			}
			else
			{
				SetPVarInt ( playerid, "reg_spawn", 1 ) ;
				TogglePlayerSpectating ( playerid, false ) ;
				SetSpawnInfo ( playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) ;
				SpawnPlayer ( playerid ) ;
				SetPVarInt ( playerid, "create_character_step", 1 ) ;
				create_character_td ( playerid, 1, true ) ;
				set_skin ( playerid, 79 ) ;
			}
		}
		case d_character_age:
		{
			if ( !response ) return 1 ;
			if ( !strval ( inputtext ) ) return 1 ;
			if(strval(inputtext) < 16 || strval(inputtext) > 120)return SendClientMessage ( playerid, COLOR_GRAY, "Возраст не может быть меньше 16 и не больше 120" ), show_dialog ( playerid, d_character_age, DIALOG_STYLE_INPUT, "{2E8B57}Возраст персонажа", "{FFFFFF}Введите возраст Вашего персонажа:", "Далее", "Назад" ) ;
			p_info [ playerid ][ age ] = strval ( inputtext ) ;
			new td_string [ 32 ] ;
			format ( td_string, sizeof ( td_string ), "%d", p_info [ playerid ] [ age ] ) ;
			PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], td_string ) ;
		}
		case d_character_name:
		{
			if ( !response ) return 1 ;
			if ( !regex_match ( inputtext, "([A-Z]{1,1})[a-z]{2,9}+_([A-Z]{1,1})[a-z]{2,9}" ) )return SendClientMessage ( playerid, COLOR_RED, "Вы ввели Non-rp имя персонажа. Формат: Имя_Фамилия" ), show_dialog ( playerid, d_character_name, DIALOG_STYLE_INPUT, "{2E8B57}Имя персонажа", "{FFFFFF}Введите имя Вашего персонажа:", "Принять", "Назад" ) ;
			new query_string [ 168 ] ;
			mysql_format ( sql_connection, query_string, sizeof ( query_string ), "SELECT * FROM `characters` WHERE `c_login` = '%e' LIMIT 1", inputtext ) ;
			mysql_tquery ( sql_connection, query_string, "check_free_name", "is", playerid, inputtext ) ;
		}
		case d_character_gender:
		{
			if ( !response ) return 1 ;
			p_info [ playerid ][ gender ] = listitem + 1;
			if ( p_info [ playerid ][ gender ] == 1 ) PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], "male" ) ;
			else if ( p_info [ playerid ][ gender ] == 2 ) PlayerTextDrawSetString ( playerid, create_step1_pl_td [ playerid ] [ 0 ], "female" ) ;
			set_skin ( playerid, reg_skins [ random ( 3 ) ] [ p_info [ playerid ] [ gender ] - 1] ) ;
		}
		case d_give_object_id:
		{
			if ( !response ) return SelectTextDraw ( playerid, 0x2641FEAA ) , DeletePVar ( playerid, "give_pl_id" ), DeletePVar ( playerid, "sell_or_dare" ), DeletePVar ( playerid, "obj_amount" ) ;
			if ( !strval ( inputtext ) ) return 1;
			if ( strval ( inputtext ) < 0 || strval ( inputtext ) > MAX_PLAYERS || p_s_info [ strval ( inputtext ) ] [ p_logged ] == false ) return SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] Игрок не в сети!" ) ;
			SetPVarInt ( playerid, "give_pl_id", strval ( inputtext ) ) ;
			if ( GetPVarInt ( playerid, "sell_or_dare" ) == 1 )show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для продажи:", "Далее", "Назад" ) ;
			else show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для передачи:", "Принять", "Назад" ) ;
		}
		case d_give_object_amount:
		{
			if ( !response )
			{
				if ( GetPVarInt ( playerid, "sell_or_dare" ) == 1 )show_dialog ( playerid, d_give_object_id, DIALOG_STYLE_INPUT, "{2E8B57}Продать предмет", "{FFFFFF}Введите ID игрока:", "Далее", "Закрыть" ) ;
				else show_dialog ( playerid, d_give_object_id, DIALOG_STYLE_INPUT, "{2E8B57}Передать предмет", "{FFFFFF}Введите ID игрока:", "Далее", "Закрыть" ) ;
			}
			if ( !strval ( inputtext ) )
			{
				if ( GetPVarInt ( playerid, "sell_or_dare" ) == 1 )show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для продажи:", "Далее", "Назад" ) ;
				else show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для передачи:", "Принять", "Назад" ) ;
				return 1;
			}
			if ( strval ( inputtext ) < 0 || p_info  [ playerid ] [ inventory_amount ] [ clicked_item [ playerid ] ] < strval ( inputtext ) )
			{			
				SendClientMessage ( playerid, COLOR_GRAY, "[Ошибка] У вас нет такого колличества предмета!" ) ;
				if ( GetPVarInt ( playerid, "sell_or_dare" ) == 1 )show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для продажи:", "Далее", "Назад" ) ;
				else show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для передачи:", "Принять", "Назад" ) ;
				return 1;
			}

			if ( GetPVarInt ( playerid, "sell_or_dare" ) == 1 )
			{
				p_info  [ playerid ] [ inventory_amount ] [ clicked_item [ playerid ] ] -= strval ( inputtext ) ;
				give_inventory ( GetPVarInt ( playerid, "give_pl_id"), p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ], false, 0, strval ( inputtext ) ) ;
				if ( p_info  [ playerid ] [ inventory_amount ] [ clicked_item [ playerid ] ] == 0) p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] = 0;
				SelectTextDraw ( playerid, 0x2641FEAA ) ;
				DeletePVar ( playerid, "give_pl_id" ) ;
				DeletePVar ( playerid, "sell_or_dare" ) ;
				PlayerTextDrawBackgroundColor( playerid, inv_player_td [ playerid ] [ clicked_item [ playerid ] ], 103) ;
				update_inventory ( playerid, clicked_item [ playerid ] , p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ]) ;
				clicked_item [ playerid ] = -1;

			}
			else 
			{
				show_dialog ( playerid, d_sell_object_price, DIALOG_STYLE_INPUT, "{2E8B57}Цена предмета", "{FFFFFF}Введите цену предмета:", "Принять", "Назад" ) ;
				SetPVarInt ( playerid, "obj_amount", strval ( inputtext ) ) ;
				
			}
		}	
		case d_sell_object_price:
		{
			if ( !response )return show_dialog ( playerid, d_give_object_amount, DIALOG_STYLE_INPUT, "{2E8B57}Колличество предмета", "{FFFFFF}Введите колличество предмета для продажи:", "Далее", "Назад" ) ;
			if ( !strval ( inputtext ) )return show_dialog ( playerid, d_sell_object_price, DIALOG_STYLE_INPUT, "{2E8B57}Цена предмета", "{FFFFFF}Введите цену предмета:", "Принять", "Назад" ) ;
			if ( strval ( inputtext ) < 0 )return show_dialog ( playerid, d_sell_object_price, DIALOG_STYLE_INPUT, "{2E8B57}Цена предмета", "{FFFFFF}Введите цену предмета:", "Принять", "Назад" ) ;
			if ( !give_inventory ( GetPVarInt ( playerid, "give_pl_id"), p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ], false, 0, GetPVarInt ( playerid, "obj_amount" ) ) ) return SendClientMessage(playerid,COLOR_GRAY,"[Ошибка] В инвентаре игрока нет места.") ;
			new dialog_string [ 256 ] ;
			SetPVarInt ( playerid, "price_for_obj", strval ( inputtext ) ) ;
			SetPVarInt ( GetPVarInt ( playerid, "give_pl_id"), "seller_id", playerid ) ;
			SetPVarInt ( GetPVarInt ( playerid, "give_pl_id"), "seller_increment",  p_info [ playerid ] [ id ] ) ;
			format ( dialog_string, sizeof ( dialog_string ), "{2E8B57}%s {FFFFFF}предлагает Вам купить у него {2E8B57}%s {FFFFFF}(%d шт.).\nСтоимость: {2E8B57}%d{FFFFFF}$", p_info [ playerid ] [ name ], item_info [ p_info  [ playerid ] [ inventory_slot ] [ clicked_item [ playerid ] ] ] [ item_name ], GetPVarInt ( playerid, "obj_amount" ), strval ( inputtext )) ;
			show_dialog ( GetPVarInt ( playerid, "give_pl_id"), d_accept_object_buy, DIALOG_STYLE_MSGBOX, "{2E8B57}Покупка предмета", dialog_string, "Принять", "Отмена" ) ;
		}
		case d_accept_object_buy:
		{
			if ( !response )
			{
				new pl_id = GetPVarInt ( playerid, "seller_id" ) ;
				SendClientMessage ( pl_id,COLOR_ADM, "Игрок отказался от покупки!" ) ; 
				DeletePVar ( pl_id, "give_pl_id" ) ;
				DeletePVar ( pl_id, "price_for_obj" ) ;
				DeletePVar ( pl_id, "obj_amount" ) ;
				DeletePVar ( pl_id, "sell_or_dare" ) ;
				DeletePVar ( playerid, "seller_id" ) ;
				DeletePVar ( playerid, "seller_increment" ) ;
				return 1;
			}
			if ( p_s_info [ GetPVarInt ( playerid, "seller_id" ) ] [ p_logged ] == false ) return SendClientMessage ( playerid,COLOR_ADM, "Продавец вышел из игры!" ) ; 
			if ( p_info [ GetPVarInt ( playerid, "seller_id" ) ] [ id ] != GetPVarInt ( playerid, "seller_increment" ) ) return SendClientMessage ( playerid,COLOR_ADM, "Продавец вышел из игры!" ) ; 
			new pl_id = GetPVarInt ( playerid, "seller_id" ) ;
			give_money ( pl_id, GetPVarInt ( pl_id, "price_for_obj") ) ;
			give_money ( playerid, - GetPVarInt ( pl_id, "price_for_obj") ) ;
			
			p_info  [ pl_id ] [ inventory_amount ] [ clicked_item [ pl_id ] ] -= GetPVarInt ( pl_id, "obj_amount" ) ;
			give_inventory ( playerid, p_info  [ pl_id ] [ inventory_slot ] [ clicked_item [ pl_id ] ], false, 0, GetPVarInt ( pl_id, "obj_amount" ) ) ;
			if ( p_info  [ pl_id ] [ inventory_amount ] [ clicked_item [ pl_id ] ] == 0) p_info  [ pl_id ] [ inventory_slot ] [ clicked_item [ pl_id ] ] = 0;
			PlayerTextDrawBackgroundColor( pl_id, inv_player_td [ pl_id ] [ clicked_item [ pl_id ] ], 103) ;
			update_inventory ( pl_id, clicked_item [ pl_id ] , p_info  [ pl_id ] [ inventory_slot ] [ clicked_item [ pl_id ] ]) ;
			clicked_item [ pl_id ] = -1;
			for ( new j = 0; j < 10; j++ )
			{
				update_inventory ( playerid, j , p_info  [ playerid ] [ inventory_slot ] [ j ]) ;
			}	
			DeletePVar ( pl_id, "give_pl_id" ) ;
			DeletePVar ( pl_id, "price_for_obj" ) ;
			DeletePVar ( pl_id, "obj_amount" ) ;
			DeletePVar ( pl_id, "sell_or_dare" ) ;
			DeletePVar ( playerid, "seller_id" ) ;
			DeletePVar ( playerid, "seller_increment" ) ;
		}
	}		
	return 1;
}
