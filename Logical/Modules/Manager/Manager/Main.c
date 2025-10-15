
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	
	Conveyor.MpLink = &Master;
	Conveyor.Parameters = &ConveyorPar;
	Conveyor.Enable = 1;
	
	ConveyorPar.Velocity = 65;
	ConveyorPar.Acceleration = 1000;
	ConveyorPar.Deceleration = 1000;
	
	Slicer.MpLink = &Slave;
	Slicer.Parameters = &SlicerPar;
	Slicer.Enable = 1;
	
	Sequencer.MpLinkMaster = &Master;
	Sequencer.MpLink = &Slave;
	Sequencer.Parameters = &SequencerPar;
	Sequencer.Enable = 1;

	TouchProbe.Axis = &Master;
	TouchProbe.TriggerInput.EventSource = mcEVENT_SRC_TRIGGER1;
	TouchProbe.Period = 140;
	TouchProbe.WindowPositive = 135;
	TouchProbe.Mode = mcTP_MODE_SHIFT_FROM_RESULT;

}

void _CYCLIC ProgramCyclic(void)
{
	// Power on conveyor and slicer if FUBs are active
	if (!Conveyor.PowerOn && Conveyor.Active) {
		Conveyor.Power = 1;
	}
	
	if (!Slicer.PowerOn && Slicer.Active) {
		Slicer.Power = 1;
	}
	
	// Home conveyor and slicer if they are powered
	if (Conveyor.PowerOn && !Conveyor.IsHomed) {
		Conveyor.Home = 1;
	}

	if (Slicer.PowerOn && !Slicer.IsHomed) {
		Slicer.Home = 1;
	}
	
	// Move velocity conveyor once homed
	if (Conveyor.IsHomed && Slicer.IsHomed) {
		Conveyor.MoveVelocity = 1;
		Sequencer.StartSequence = 1;
	}
	
	if (TouchProbe.ValidTriggerCount > PrevCounter) {
		Pulse = 1;
		
		switch (Sequencer.ActualStateIndex)
		{
			case 0:
				Sequencer.Signal1 = 1;
				break;
			case 1:
				Sequencer.Signal2 = 1;
				break;
			case 2:
				Sequencer.Signal3 = 1;
				break;
			
		}
	} else {
		Pulse = 0;
	}

	if (Sequencer.ActualStateIndex = 2 && Sequencer.InCompensation) {
		Sequencer.Signal3 = 0;
	}
	
	// Assignments
	PrevCounter = TouchProbe.ValidTriggerCount;
	MpAxisBasic(&Conveyor);
	MpAxisBasic(&Slicer);
	MpAxisCamSequencer(&Sequencer);
	MC_BR_TouchProbe(&TouchProbe);
}

void _EXIT ProgramExit(void)
{

}

