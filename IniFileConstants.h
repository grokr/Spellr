#ifndef __INIFILE_CONSTANTS_H__
#define __INIFILE_CONSTANTS_H__

#include "data_types.h"
//////////////////////////////////////////////////////////////
//INI FILES SECTIONS
//////////////////////////////////////////////////////////////

const typo_string ACTIVE_SECTIONS=_T("ActiveSections");

//////////////////////////////////////////////////////////////////
///APP.INI SECTIONS
//////////////////////////////////////////////////////////////////
const typo_string APP_DATA_SECTION=_T("ApplicationData");
const typo_string GUI_SECTION=_T("Gui");
const typo_string TRACE_SECTION=_T("Trace");
const typo_string USER_DATA_SECTION=_T("UserData");
const typo_string PATH_SECTION=_T("Path");
//////////////////////////////////////////////////////////////////
///APP.INI [ApplicationData] KEY NAMES 
//////////////////////////////////////////////////////////////////
const typo_string PRODUCT_VERSION=_T("product_version");
const typo_string _UI_LANGUAGE = _T("UI_Language");
const typo_string _CURRENT_INPUT_LANGUAGE = _T("Current_Input_Language");
const typo_string _CURRENT_OUTPUT_LANGUAGE = _T("Current_Output_Language");

//////////////////////////////////////////////////////////////////
///APP.INI [Path] KEY NAMES 
//////////////////////////////////////////////////////////////////
const typo_string DICT_INI_FILENAME_SECTION=_T("DictionariesFileName");
const typo_string PARAMETERS_SET_INI_FILENAME_SECTION=_T("ParametersSetFileName");
//const typo_string ERRORDEFINITIONSFILENAME=_T("ErrorDefinitionsFileName");
const typo_string LASTUSERCONFIGFILEPATH=_T("LastUserConfigFilePath");

//////////////////////////////////////////////////////////////////
///APP.INI [UserData] KEY NAMES 
//////////////////////////////////////////////////////////////////
const typo_string CURRENTUSERNAME=_T("CurrentUserName");


//////////////////////////////////////////////////////////////////
///GROKER.INI SECTIONS
//////////////////////////////////////////////////////////////////
const typo_string USER_PROFILE_APP_DATA_SECTION=_T("UserConfiguration");
const typo_string STATISTICS_ANALIZER_PROBLEM_FREQUENCIES=_T("StatisticsAnalizerProblemFrequencies");



//////////////////////////////////////////////////////////////////
///GROKER.INI KEY VALUES
//////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
///[UserConfiguration] KEY NAMES
//////////////////////////////////////////////////////////////////

const typo_string COMPARISONFUNCTIONS=_T("ComparisonFunctions");
const typo_string LANGUAGE=_T("Language");
const typo_string SPELLCHECKERDEFINEDALGSEQUENCE=_T("SpellCheckerDefinedAlgSequence");
const typo_string SPELLCHECKERIMPTYPE=_T("SpellCheckerImpType");
const typo_string SPELLCHECKERTYPE=_T("SpellCheckerType");
const typo_string STATISTICSANALIZERDEFINEDALGORITHMSSEQUENCE=_T("StatisticsAnalizerDefinedAlgorithmsSequence");
const typo_string TARGETGROUP=_T("TargetGroup");
const typo_string USERDEFINEDALGORITHMSSEQUENCE=_T("UserDefinedAlgorithmsSequence");
const typo_string USERDEFINEDPROBLEMTYPES=_T("UserDefinedProblemTypes");
const typo_string UserName=_T("UserName");

//////////////////////////////////////////////////////////////////
///MESSAGES.INI CONSTANTS 
//////////////////////////////////////////////////////////////////
const typo_string MESSAGES_SECTION=_T("Messages");
const typo_string SEVERITIES_SECTION=_T("Severities");
const typo_string CONDITION_CODES_SECTION=_T("ConditionCodes");

//////////////////////////////////////////////////////////////////
///MESSAGES.INI [Trace] KEY NAMES 
//////////////////////////////////////////////////////////////////

const typo_string DEFAULTSEVERITYLEVEL=_T("DefaultSeverityLevel");
const typo_string ENABLED=_T("Enabled");
const typo_string SMAXFILESIZE=_T("MaxFileSize");
const typo_string RECYCLINGMETHOD=_T("RecyclingMethod");
const typo_string RECYCLINGMETHODS=_T("RecyclingMethods");
const typo_string SEVERITYLEVELS=_T("SeverityLevels");
const typo_string REPORTTYPE=_T("ReportType");
const typo_string FILENAME=_T("FileName");
//////////////////////////////////////////////////////////////////
///MESSAGES.INI [Trace] KEY VALUES 
//////////////////////////////////////////////////////////////////
const typo_string INFORMATION=_T("INFORMATION");
const typo_string WARNING=_T("WARNING");
const typo_string EXCEPTION=_T("EXCEPTION");
const typo_string ERR = _T("ERR");
const typo_string TYPO_LOG = _T("Typo_log.txt");
const int MAXFILESIZE=10000;
const typo_string RESTART=_T("RESTART");
const typo_string FILES_ROUND=_T("FILES_ROUND");
const typo_string CONSOLE_REPORT=_T("CONSOLE_REPORT");
const typo_string LOG_REPORT=_T("LOG_REPORT");
const typo_string GUI_REPORT=_T("GUI_REPORT");

//////////////////////////////////////////////////////////////////
///TEXT_STRINGS_EN.INI [TEXT_CONSTANTS] KEY NAMES
//////////////////////////////////////////////////////////////////
const typo_string TEXT_CONSTANTS_SECTION=_T("TEXT_CONSTANTS");

const typo_string TXT_ID_00000=_T("TXT_ID_00000");
const typo_string TXT_ID_00001=_T("TXT_ID_00001");
const typo_string TXT_ID_00002=_T("TXT_ID_00002");
const typo_string TXT_ID_00003=_T("TXT_ID_00003");
const typo_string TXT_ID_00004=_T("TXT_ID_00004"); 
const typo_string TXT_ID_00005=_T("TXT_ID_00005"); 
const typo_string TXT_ID_00006=_T("TXT_ID_00006"); 
const typo_string TXT_ID_00007=_T("TXT_ID_00007"); 

#endif //__INIFILE_CONSTANTS_H__