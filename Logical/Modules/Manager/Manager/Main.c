
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	// Initializiation of Master MpAxis
	TheConveyor.Devices.Axis.MpLink = &Master;
	TheConveyor.Devices.Axis.Parameters = &TheConveyor.Par.AxisPar;
	TheConveyor.Devices.Axis.Enable = 1;
	
	TheConveyor.Par.AxisPar.Velocity = 55;
	TheConveyor.Par.AxisPar.Acceleration = 1000;
	TheConveyor.Par.AxisPar.Deceleration = 1000;
	
	// Initialization of Slicer MpAxis
	TheSlicer.Devices.Axis.MpLink = &Slave;
	TheSlicer.Devices.Axis.Parameters = &TheSlicer.Par.AxisPar;
	TheSlicer.Devices.Axis.Enable = 1;
	
	// Initialization of the Sequencer
	TheSequencer.Sequencer.MpLinkMaster = &Master;
	TheSequencer.Sequencer.MpLink = &Slave;
	TheSequencer.Sequencer.Parameters = &TheSequencer.Par;
	TheSequencer.Sequencer.Enable = 1;

	// Initialization of TouchProbe for mark detection
	TheConveyor.Devices.Probe.Axis = &Master;
	TheConveyor.Devices.Probe.TriggerInput.EventSource = mcEVENT_SRC_TRIGGER1;
	TheConveyor.Devices.Probe.Period = 70;
	TheConveyor.Devices.Probe.WindowNegative = 5.0;
	TheConveyor.Devices.Probe.WindowPositive = 5.0;
	TheConveyor.Devices.Probe.AdvancedParameters.UseFirstTriggerPosition = 1;
	TheConveyor.Devices.Probe.Mode = mcTP_MODE_SHIFT_FROM_RESULT;
	
	// Initializatin of TouchProbe for successful cut detection
	TheSlicer.Devices.Probe.Axis = &Slave;
	TheSlicer.Devices.Probe.TriggerInput.EventSource = mcEVENT_SRC_TRIGGER1;

}

void _CYCLIC ProgramCyclic(void)
{
	// Power on conveyor and slicer if FUBs are active
	if (!TheConveyor.Devices.Axis.PowerOn && TheConveyor.Devices.Axis.Active) {
		TheConveyor.Devices.Axis.Power = 1;
	}
	
	if (!TheSlicer.Devices.Axis.PowerOn && TheSlicer.Devices.Axis.Active) {
		TheSlicer.Devices.Axis.Power = 1;
	}
	
	// Home conveyor and slicer if they are powered
	if (TheConveyor.Devices.Axis.PowerOn && !TheConveyor.Devices.Axis.IsHomed) {
		TheConveyor.Devices.Axis.Home = 1;
	}

	if (TheSlicer.Devices.Axis.PowerOn && !TheSlicer.Devices.Axis.IsHomed) {
		TheSlicer.Devices.Axis.Home = 1;
	}
	
	// Move velocity conveyor once homed
	if (TheConveyor.Devices.Axis.IsHomed && TheSlicer.Devices.Axis.IsHomed) {
		TheConveyor.Devices.Axis.MoveVelocity = 1;
		TheSequencer.Sequencer.StartSequence = 1;
		TheConveyor.Devices.Probe.Enable = 1;
		TheSlicer.Devices.Probe.Enable = 1;
	}
	
	// If a new hole is detected send a "pulse" to Signal #.
	if (TheConveyor.Devices.Probe.ValidTriggerCount > TheConveyor.Par.HolesCounted) {
		
		// Evalute current sequencer state, enabled/disable respective signals
		switch (TheSequencer.Sequencer.ActualStateIndex) {
			// In state 1, enable signal 1 to transition to state 2. Disable signal 3 as a transition-from-state-5 remnant.
			case 1:
				TheSequencer.Sequencer.Signal1 = 1;
				TheSequencer.Sequencer.Signal3 = 0;
				break;
			// In state 2, enable signal 2 to transition to state 3. Disable signal 1 as a transition-from-state-1 remnant.
			case 2:
				TheSequencer.Sequencer.Signal2 = 1;
				TheSequencer.Sequencer.Signal1 = 0;
				break;
			// In state 3, enable signal 3 to transition to state 4. Will bounce back and forth between states 3 and 4.
			case 3:
				TheSequencer.Sequencer.Signal3 = 1;
				TheSequencer.Sequencer.Signal4 = 0;
				TheSequencer.Sequencer.Signal2 = 0;
				break;
			// In state 4, enable signal 4 to transition to state 3.
			case 4:
				TheSequencer.Sequencer.Signal4 = 1;
				TheSequencer.Sequencer.Signal3 = 0;
				break;
		}
	}
	
	// Assignments
	TheSlicer.Par.SuccessfulCuts = TheSlicer.Devices.Probe.ValidTriggerCount;
	TheConveyor.Par.HolesCounted = TheConveyor.Devices.Probe.ValidTriggerCount;
	
	// FUBs calls
	MpAxisBasic(&TheConveyor.Devices.Axis);
	MpAxisBasic(&TheSlicer.Devices.Axis);
	MpAxisCamSequencer(&TheSequencer.Sequencer);
	MC_BR_TouchProbe(&TheConveyor.Devices.Probe);
	MC_BR_TouchProbe(&TheSlicer.Devices.Probe);
}

void _EXIT ProgramExit(void)
{

}

