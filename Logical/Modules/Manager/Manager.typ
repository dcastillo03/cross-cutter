
TYPE
	ConveyorType : 	STRUCT 
		Devices : DevicesType; (*Contains either the axis or the probe.*)
		Par : ConveyorParType; (*Contains the axis parameters and other misc. parameters.*)
	END_STRUCT;
	SlicerType : 	STRUCT 
		Devices : DevicesType; (*Contains either the axis or the probe.*)
		Par : SlicerParType; (*Contains the axis parameters and other misc. parameters.*)
	END_STRUCT;
	DevicesType : 	STRUCT 
		Axis : MpAxisBasic; (*MpAxisBasic FUB for master/slave*)
		Probe : MC_BR_TouchProbe; (*TouchProbe FUB*)
	END_STRUCT;
	ConveyorParType : 	STRUCT 
		HolesCounted : UDINT; (*Integer value of all the holes accounted for but not yet sliced*)
		AxisPar : MpAxisBasicParType; (*Contains MpAxisBasic parameters for the conveyor*)
	END_STRUCT;
	SlicerParType : 	STRUCT 
		SuccessfulCuts : UDINT; (*Integer value of all the holes successfully sliced at*)
		AxisPar : MpAxisBasicParType; (*Contains MpAxisBasic parameters for the slicer*)
	END_STRUCT;
	SequencerType : 	STRUCT 
		Sequencer : MpAxisCamSequencer; (*MpAxisCamSequencer FUB*)
		Par : MpAxisCamSequencerParType; (*Parameters for MpAxisCamSequencer*)
	END_STRUCT;
END_TYPE
