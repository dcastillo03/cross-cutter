
TYPE
	AlarmType : 	STRUCT  (*Alarm FUBs*)
		ListUI : MpAlarmXListUI;
		ListUISetup : MpAlarmXListUISetupType;
		ListUIConnect : MpAlarmXListUIConnectType;
		Core : MpAlarmXCore;
		History : MpAlarmXHistory;
		HistoryName : STRING[50];
	END_STRUCT;
END_TYPE
