
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	Alarm.Core.MpLink = &gAlarmXCore;
	Alarm.ListUI.MpLink = &gAlarmXCore;
	Alarm.History.MpLink = &gAlarmXCore;
}

void _CYCLIC ProgramCyclic(void)
{
	if (!Alarm.ListUI.Active) {
		Alarm.ListUI.Enable = 1;
		Alarm.ListUI.UISetup = Alarm.ListUISetup;
		Alarm.ListUI.UIConnect = &Alarm.ListUIConnect;
	}
	
	if (!Alarm.Core.Active) {
		Alarm.Core.Enable = 1;
	}
	
	Alarm.History.Enable = 1;
	Alarm.History.DeviceName = &"AlarmHistory";
	Alarm.History.FileName = &Alarm.HistoryName;
		

	// Call FUBs
	MpAlarmXCore(&Alarm.Core);
	MpAlarmXListUI(&Alarm.ListUI);
	MpAlarmXHistory(&Alarm.History);
}

void _EXIT ProgramExit(void)
{

}

