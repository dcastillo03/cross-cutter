
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	// MpLink assignments
	Recipe.Core.MpLink = &gRecipeXml;
	Recipe.Register.MpLink = &gRecipeXml;
	Recipe.UI.MpLink = &gRecipeXml;
}

void _CYCLIC ProgramCyclic(void)
{
	// FUB Setup
	Recipe.Core.Enable = 1;
	Recipe.Core.DeviceName = &"Recipes";
	Recipe.Core.FileName = &Recipe.Name;
		
	Recipe.Register.Enable = 1;
	Recipe.Register.PVName = &"::Manager:DataObjectName";
		
	Recipe.UI.Enable = 1;
	Recipe.UI.DeviceName = &"Recipes";
	Recipe.UI.UISetup = Recipe.UISetup;
	Recipe.UI.UIConnect = &Recipe.UIConnect;
		
	MpRecipeXml(&Recipe.Core);
	MpRecipeUI(&Recipe.UI);
	MpRecipeRegPar(&Recipe.Register);
}

void _EXIT ProgramExit(void)
{

}

