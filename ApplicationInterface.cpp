#include "ApplicationInterface.h"
#include "Application.h"

ApplicationInterface::ApplicationInterface(const typo_string &installDir, const typo_string &runningMode,
										   const typo_string &compFuncName, 
										   const typo_string &perfRep, const typo_string &output_type)
{
	mf = new Application(installDir, runningMode, compFuncName, perfRep, output_type);			
}

