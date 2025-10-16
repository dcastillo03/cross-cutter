
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	// Initializiation of Conveyor MpAxis
	Conveyor.MpLink = &Master;
	Conveyor.Parameters = &ConveyorPar;
	Conveyor.Enable = 1;
	
	ConveyorPar.Velocity = 55;
	ConveyorPar.Acceleration = 1000;
	ConveyorPar.Deceleration = 1000;
	
	// Initialization of Slicer MpAxis
	Slicer.MpLink = &Slave;
	Slicer.Parameters = &SlicerPar;
	Slicer.Enable = 1;
	
	Sequencer.MpLinkMaster = &Master;
	Sequencer.MpLink = &Slave;
	Sequencer.Parameters = &SequencerPar;
	Sequencer.Enable = 1;

	// Initialization of TouchProbe for mark detection
	TouchProbe.Axis = &Master;
	TouchProbe.TriggerInput.EventSource = mcEVENT_SRC_TRIGGER1;
	TouchProbe.Period = 70.0;
	TouchProbe.WindowNegative = 5.0;
	TouchProbe.WindowPositive = 5.0;
	TouchProbe.AdvancedParameters.UseFirstTriggerPosition = 1;
	TouchProbe.Mode = mcTP_MODE_SHIFT_FROM_RESULT;
	
	// Initializatin of TouchProbe for successful cut detection
	SlicerProbe.Axis = &Slave;
	SlicerProbe.TriggerInput.EventSource = mcEVENT_SRC_TRIGGER1;

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
		TouchProbe.Enable = 1;
		SlicerProbe.Enable = 1;
	}
	
	// If a new hole is detected send a "pulse" to Signal#, else send no pulse.
	if (TouchProbe.ValidTriggerCount > PrevCounter) {
		Pulse = 1;
		
		// Evalute current sequencer state, enabled/disable respective signals
		switch (Sequencer.ActualStateIndex) {
			// In state 1, enable signal 1 to transition to state 2. Disable signal 3 as a transition-from-state-5 remnant.
			case 1:
				Sequencer.Signal1 = 1;
				Sequencer.Signal3 = 0;
				break;
			// In state 2, enable signal 2 to transition to state 3. Disable signal 1 as a transition-from-state-1 remnant.
			case 2:
				Sequencer.Signal2 = 1;
				Sequencer.Signal1 = 0;
				break;
			// In state 3, enable signal 3 to transition to state 4. Will bounce back and forth between states 3 and 4.
			case 3:
				Sequencer.Signal3 = 1;
				Sequencer.Signal4 = 0;
				Sequencer.Signal2 = 0;
				break;
			// In state 4, enable signal 4 to transition to state 3.
			case 4:
				Sequencer.Signal4 = 1;
				Sequencer.Signal3 = 0;
				break;
		}
	} else {
		Pulse = 0;
	}
	
	// Assignments
	SuccessfulCuts = SlicerProbe.ValidTriggerCount;
	PrevCounter = TouchProbe.ValidTriggerCount;
	
	// FUBs calls
	MpAxisBasic(&Conveyor);
	MpAxisBasic(&Slicer);
	MpAxisCamSequencer(&Sequencer);
	MC_BR_TouchProbe(&TouchProbe);
	MC_BR_TouchProbe(&SlicerProbe);
}

void _EXIT ProgramExit(void)
{

}

