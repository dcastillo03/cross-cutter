
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	
	Conveyor.MpLink = &Master;
	Conveyor.Parameters = &ConveyorPar;
	Conveyor.Enable = 1;
	
	Slicer.MpLink = &Slave;
	Slicer.Parameters = &SlicerPar;
	Slicer.Enable = 1;
	
	Sequencer.MpLinkMaster = &Master;
	Sequencer.MpLink = &Slave;
	Sequencer.Parameters = &SequencerPar;
	Sequencer.Enable = 1;
	
	TouchProbe.Enable = 1;
	TouchProbe.Axis = &Master;
	

}

void _CYCLIC ProgramCyclic(void)
{
//	if (!Conveyor.PowerOn && Conveyor.Active) {
//		Conveyor.Power = 1;
//	}
//	
//	if (!Slicer.PowerOn && Slicer.Active) {
//		Slicer.Power = 1;
//	}
	

	
	MpAxisBasic(&Conveyor);
	MpAxisBasic(&Slicer);
	MpAxisCamSequencer(&Sequencer);
	MC_BR_TouchProbe(&TouchProbe);
}

void _EXIT ProgramExit(void)
{

}

