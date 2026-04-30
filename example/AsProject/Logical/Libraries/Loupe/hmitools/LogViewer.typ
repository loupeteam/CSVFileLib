(*
* File: LogViewer.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of HMITools, licensed under the MIT License.
* 
*)

TYPE
	LogView_HMI_typ : 	STRUCT 
		ListBox : HMI_Listbox_typ;
		GoToTopButton : HMI_Button_typ;
		PageUpButton : HMI_Button_typ;
		PageDownButton : HMI_Button_typ;
		UpButton : HMI_Button_typ;
		DownButton : HMI_Button_typ;
		Text : STRING[LIST_BOX_STRING_SIZE];
		FilterString : STRING[80];
		MaxAlarms : UDINT;
		AlarmList : UDINT;
		internal : LogView_HMI_internal_typ;
	END_STRUCT;
	LogView_HMI_internal_typ : 	STRUCT 
		Offset : DINT;
		SelectedAlarmIndex : UDINT;
		SelectedListIndex : UINT;
		oSelectedListIndex : UINT;
		NumberAlarms : UINT;
	END_STRUCT;
	LogView_typ : 	STRUCT 
		IN : LogView_IN_typ;
		OUT : LogView_OUT_typ;
		Internal : LogView_Internal_typ;
	END_STRUCT;
	LogView_IN_typ : 	STRUCT  (*Logger Variables*)
		PAR : LogView_PAR_typ;
		CMD : LogView_CMD_typ;
		CFG : LogView_CFG_typ;
	END_STRUCT;
	LogView_PAR_typ : 	STRUCT  (*Logger Variables*)
		LoggerName : STRING[80];
		Offset : DINT;
	END_STRUCT;
	LogView_CMD_typ : 	STRUCT  (*Logger Variables*)
		GoToTop : BOOL;
	END_STRUCT;
	LogView_CFG_typ : 	STRUCT  (*Logger Variables*)
		MaxLogs : UINT;
	END_STRUCT;
	LogView_OUT_typ : 	STRUCT  (*Logger Variables*)
		STAT : LogView_STAT_typ;
		NumberAlarms : UDINT;
	END_STRUCT;
	LogView_STAT_typ : 	STRUCT  (*Logger Variables*)
		Done : BOOL;
		Busy : BOOL;
		Error : BOOL;
		ErrorID : DINT;
		ErrorString : STRING[80];
	END_STRUCT;
	LogView_Internal_typ : 	STRUCT  (*Logger Variables*)
		GetIdent: ArEventLogGetIdent;
		GetRecordID: ArEventLogGetLatestRecordID;
		GetTopAlarm : LogView_GetAlarm;
		GetBottomAlarm : LogView_GetAlarm;
		Buffer : Buffer_typ;
		OffsetIndex : UDINT;
		LoggerName : STRING[80];
		Ident : UDINT;
	END_STRUCT;
	ArEventLogREAD: 	STRUCT  (*Header data for log entry*)
		lenAscii : UDINT; (*Length of the ASCII log data*)
		errornr : UDINT; (*Error number of the entry*)
		errTime : DTStructure;
	END_STRUCT;
	LogView_GetAlarm : 	STRUCT 
		ReadItem : ArEventLogRead; (*ArEventLog: ArEventLogRead FUB*)
		ReadAddData: ArEventLogReadAddData; (*For reading the message text*)
		ReadData : ArEventLogREAD;
		convertUTC : UtcDT_TO_LocalDTStructure;
		BinaryData : ARRAY[0..255]OF USINT;
		ASCIIData : ARRAY[0..255]OF USINT;
	END_STRUCT;
	LogData : 	STRUCT 
		String : STRING[LIST_BOX_STRING_SIZE];
		Index : UDINT;
	END_STRUCT;
END_TYPE
