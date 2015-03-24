/*
	Daniel's Command Processing Plugin
	© 2012-2013, Daniel_Cortez

You may use this plugin however you want.
Just don't claim this work as yours and/or try do decompile/disassemble it, etc.
*/


native DC_CMD(playerid, cmdtext[]);
native DC_CMD_Init();
native RegisterAlt(const command[], alt1[], ...);


#define CMD:%0(%1)	\
			public cmd_%0(%1);	\
			public cmd_%0(%1)

#define COMMAND:%1(%2)	\
			CMD:%1(%2)

#define DC_CMD:%1(%2)	\
			CMD:%1(%2)

#define ALT:%0:%1; \
			public cmd_%1(playerid, params[]);	\
			public cmd_%1(playerid, params[])	\
				return cmd_%0(playerid, params);

#define ALTX:%0(%1);	\
	public alt_%0();	\
	public alt_%0()	\
		RegisterAlt("/"#%0, %1);

#define	cmd::%0(%1)	\
			cmd_%0(%1)

#if !defined isnull
	#define isnull(%1) \
				(((%1[0]) == 0) || ((%1[0]) == '\1') && ((%1[1]) == 0))
#endif

public OnPlayerCommandText(playerid, cmdtext[]){
	return DC_CMD(playerid, cmdtext);
}
#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif
#if defined dc_cmd_OnPlayerCommandText
	forward dc_cmd_OnPlayerCommandText(playerid, cmdtext[]);
#endif
#define	OnPlayerCommandText dc_cmd_OnPlayerCommandText


#if defined FILTERSCRIPT
	public OnFilterScriptInit(){
		DC_CMD_Init();
		#if defined dc_cmd_OnFilterScriptInit
			dc_cmd_OnFilterScriptInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#if	defined	dc_cmd_OnFilterScriptInit
		forward dc_cmd_OnFilterScriptInit();
	#endif
	#define	OnFilterScriptInit dc_cmd_OnFilterScriptInit
#else
	public OnGameModeInit(){
		DC_CMD_Init();
		#if defined dc_cmd_OnGameModeInit
			dc_cmd_OnGameModeInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#if	defined	dc_cmd_OnGameModeInit
		forward dc_cmd_OnGameModeInit();
	#endif
	#define	OnGameModeInit dc_cmd_OnGameModeInit
#endif


#if	defined	OnPlayerCommandReceived
	OnPlayerCommandReceived(playerid, cmdtext[]);
#endif
#if	defined	OnPlayerCommandPerformed
	forward OnPlayerCommandPerformed(playerid, cmdtext[], success);
#endif