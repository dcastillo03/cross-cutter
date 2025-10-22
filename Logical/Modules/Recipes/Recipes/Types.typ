
TYPE
	RecipeType : 	STRUCT  (*Recipe FUB instances*)
		Core : MpRecipeXml;
		Register : MpRecipeRegPar;
		UI : MpRecipeUI;
		UISetup : MpRecipeUISetupType;
		UIConnect : MpRecipeUIConnectType;
		Name : STRING[50];
	END_STRUCT;
END_TYPE
