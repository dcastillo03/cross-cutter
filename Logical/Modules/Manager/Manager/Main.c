
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
	
	

}

void _CYCLIC ProgramCyclic(void)
{
	Conveyor;
	
}

void _EXIT ProgramExit(void)
{

}

