#pragma warning (disable:4018)

#include "Application.h"
#include "LogConstants.h"
#include "SyntacticSpellerImp.h"
#include "SemanticSpellerImp.h"
#include "Speller.h"
#include "AutoMapToolkit.h"
#include "Ignore.h"
#include "LetterSetSimilarity.h"
#include "LinearSimilarity.h"
#include "SyntacticSimilarity.h"
#include "DictionaryIndex.h"
#include "KeysSwappingRules.h"
#include "WordsMappingRules.h"
#include "LettersMappingRules.h"
#include "LetterSetIndex.h"
#include "NGramIndex.h"
#include "SentenceNGramIndex.h"
#include "ContainersManipulators.h"
#include "StringManipulators.h" 

#include <algorithm>

Application::Application(const typo_string &installDir,  const typo_string &runningMode,
									 const typo_string &compFuncName, const typo_string &perfRep, const typo_string &output_type): 
INSTALL_DIR(installDir),_contCompFuncName(compFuncName), 
_perfRepFilePath(perfRep), _output_type(output_type)
{
	_pl_ssi_containerNamesListWrapper = &_l_ssi_containerNamesListWrapper;
	_pContainerPointersList = &_containerPointersList;
	readIniFiles();
	if ((runningMode == TESTING) || (runningMode == PRODUCTION)) 
		init();
}

Application::Application(const Application& c){
	app_inifile = c.app_inifile;
	dict_inifile = c.dict_inifile;
	paramSet_inifile = c.paramSet_inifile;
	username_inifile = c.username_inifile;
	report_inifile = c.report_inifile;

	INSTALL_DIR=c.INSTALL_DIR;

	_pLog=c._pLog;
	_contCompFuncName=c._contCompFuncName;
	_spellingsSuggestionsListSize=c._spellingsSuggestionsListSize;
	_NGramLength=c._NGramLength;
	_perfRepFilePath=c._perfRepFilePath;
	_dictIniFilePath=c._dictIniFilePath;
	_parametersSetIniFilePath=c._parametersSetIniFilePath;
	_UI_Language=c._UI_Language; 
	_Current_Input_Language=c._Current_Input_Language;
	_Current_Output_Language=c._Current_Output_Language;
	_currentDictIndexFilePath=c._currentDictIndexFilePath;
	_maxDictSize=c._maxDictSize;
	_output_type=c._output_type;

	_TopSim=c._TopSim;
}

Application& Application::operator =(const Application & c)
{
	if (this != &c) // protect against invalid self-assignment
	{
		app_inifile = c.app_inifile;
		dict_inifile = c.dict_inifile;
		paramSet_inifile = c.paramSet_inifile;
		username_inifile = c.username_inifile;
		report_inifile = c.report_inifile;

		INSTALL_DIR=c.INSTALL_DIR;

		_pLog=c._pLog;
		_contCompFuncName=c._contCompFuncName;
		_spellingsSuggestionsListSize=c._spellingsSuggestionsListSize;
		_NGramLength=c._NGramLength;
		_perfRepFilePath=c._perfRepFilePath;
		_dictIniFilePath=c._dictIniFilePath;
		_parametersSetIniFilePath=c._parametersSetIniFilePath;
		_UI_Language=c._UI_Language; 
		_Current_Input_Language=c._Current_Input_Language;
		_Current_Output_Language=c._Current_Output_Language;
		_currentDictIndexFilePath=c._currentDictIndexFilePath;
		_maxDictSize=c._maxDictSize;
		_output_type=c._output_type;

		_TopSim=c._TopSim;
	}
	return *this;
}

int Application::init()
{
	if (createAlgorithmsList() != RET_FFAILURE)
		return loadContainers();
	_pLog->report(MSG_00020);
	throw runtime_error(MSG_00005);
}

Application::~Application()
{
	list<IAbstractProduct *>::iterator it = _containerPointersList.begin();
	for (;it != _containerPointersList.end(); ++it)
		delete (*(it));
		//_containerPointersList.erase(_containerPointersList.begin(), _containerPointersList.end());
	delete app_inifile;
	delete dict_inifile;
	delete paramSet_inifile;
	delete username_inifile;
	delete report_inifile;
	app_inifile = dict_inifile = paramSet_inifile = username_inifile = report_inifile = NULL;
	dict_inifile = username_inifile = paramSet_inifile = NULL;
	delete _pLog;
	_pLog = NULL;
}

IAbstractProduct* Application::makeComparisonFunction(const typo_string &id)
{
	try
	{
		if (id == LINEAR_SIMILARITY) return new LinearSimilarity(this);
		if (id == SYNTACTIC_SIMILARITY) return new SyntacticSimilarity(this);
//		if (id == NGRAM_SIMILARITY) return new NGramSimilarity(this);
//		if (id == LETTERSET_SIMILARITY) return new LetterSetSimilarity(this);
		//if (id == LEVINSTEIN_EDIT_DISTANCE) return new LevinsteinEditDistance(this);
	}
	catch(std::exception){
		_pLog->report(MSG_00022, id);
		throw runtime_error(MSG_00005);
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00022, id);
		throw runtime_error(msg);
	}
	catch(...){	throw runtime_error(MSG_00007); }

	_pLog->report(MSG_00022, id);
	throw runtime_error(MSG_00022);
	return NULL;
}

IAbstractProduct* Application::loadContainer(const typo_string &id, const typo_string &compFuncName=S_EMPTY)
{
	_currentDictIndexFilePath = curentDictionaryFileName + _UI_Language + TXT_EXT;

	try
	{	
		if (id == DICTIONARY_INDEX) return new DictionaryIndex(this, _currentDictIndexFilePath, compFuncName);
		if (id == NGRAM_INDEX )return new NGramIndex(this, _NGramLength, nGramFileName + _UI_Language + TXT_EXT);
		if (id == SENTENCE_NGRAM_INDEX )return new SentenceNGramIndex(this, _NGramLength, sentencenGramFileName + _UI_Language + TXT_EXT);
		if (id == SYNONYMS_MAP )return new SynonymsMap(this, synonymsFileName + _UI_Language + TXT_EXT);
		if (id == LETTER_SET_INTERSECTION_LIST )return new LetterSetIndex(this, _currentDictIndexFilePath, LetterSetFileName + _UI_Language + TXT_EXT);
		if (id == WORDS_MAPPING_RULES_LIST )return new WordsMappingRules(this, wordsMappingFileName + _UI_Language + TXT_EXT);
		if (id == KEYBOARD_RULES_LIST )return new KeysSwappingRules(this, keyboardRulesFileName + _UI_Language + TXT_EXT);
		if (id == LETTERS_MAPPING_RULES_LIST )return new LettersMappingRules(this, lettersMappingFileName + _UI_Language + TXT_EXT);
	}

	catch(const typo_string& msg){	
		_pLog->report(MSG_00023, msg);
		_pLog->report(MSG_00023, id);
		throw runtime_error(msg);
	}
	catch(std::exception){
		_pLog->report(MSG_00023, id);
		throw runtime_error(MSG_00005);
	}
	catch(...){	throw runtime_error(MSG_00007); }
	return NULL;
}

//IAbstractProduct* Application::makeSpellChecker(const typo_string &id)
IAbstractProduct* Application::makeSpellChecker()
{
	typo_string id = (*username_inifile)[USER_PROFILE_APP_DATA_SECTION][SPELLCHECKERTYPE];
	try
	{
		if (id == SPELLER) return new Speller(this);
		//		if (id == AUTO_MAP_TOOLKIT) return new AutoMapToolkit(this);
		//		if (id == IGNORE) return new Ignore();
	}
	catch(std::exception){
		_pLog->report(MSG_00024, id);
		throw runtime_error(MSG_00005);
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00024, id);
		throw runtime_error(msg);
	}
	catch(...){	throw runtime_error(MSG_00007); }
	return NULL;
}

IAbstractProduct* Application::makeSpellCheckerImplementation(const typo_string &id)
{ 
	try
	{
		if (id == SYNTACTIC_SPELLER_IMP) return new SyntacticSpellerImp(this);
		//		if (id == SYNTACTIC_SPELLER_IMP) return new SyntacticSpellerImp(static_cast<IAbstractBuilder *>(this));

		if (id == SEMANTIC_SPELLER_IMP) return new SemanticSpellerImp(this);
		//		if (id == SYN_SEM_SPELLER_IMP) return new SyntacticoSemanticSpellerImp(this);
	}
	catch(std::exception){
		_pLog->report(MSG_00025, id);
		throw runtime_error(MSG_00005);
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00025, id);
		throw runtime_error(msg);
	}
	catch(...){	throw runtime_error(MSG_00007); }
	return NULL;
}

int Application::createAlgorithmsList(){

	typo_string algSeq = _contCompFuncName;
	if (algSeq != S_EMPTY) return _l_ssi_containerNamesListWrapper.loadList(algSeq, C_SEM);

	algSeq = (*username_inifile)[USER_PROFILE_APP_DATA_SECTION][SPELLCHECKERDEFINEDALGSEQUENCE];
	if (algSeq != S_EMPTY) return _l_ssi_containerNamesListWrapper.loadList(algSeq, C_SEM);

	algSeq = (*username_inifile)[USER_PROFILE_APP_DATA_SECTION][STATISTICSANALIZERDEFINEDALGORITHMSSEQUENCE];
	if (algSeq != S_EMPTY) return _l_ssi_containerNamesListWrapper.loadList(algSeq, C_SEM);

	algSeq = (*username_inifile)[USER_PROFILE_APP_DATA_SECTION][USERDEFINEDALGORITHMSSEQUENCE]; 
	if (algSeq != S_EMPTY) return _l_ssi_containerNamesListWrapper.loadList(algSeq, C_SEM);

	constructAlgList(algSeq); //[USER_PROFILE_APP_DATA_SECTION][TARGETGROUP] may contain a mixed user type
	if (algSeq != S_EMPTY) return _l_ssi_containerNamesListWrapper.loadList(algSeq, C_SEM);

	return RET_FFAILURE;
}

///if TARGETGROUP is General;LanguageLearners the output is the chain of their algorithms
int Application::constructAlgList(typo_string& algSeq)
{
	typo_string targetGroup = (*username_inifile)[USER_PROFILE_APP_DATA_SECTION][TARGETGROUP]; 
	if (targetGroup == S_EMPTY)
		return RET_FFAILURE;
	vector<typo_string> vsTGroup;
	StringManipulators sm;
	sm.parseStringAppendToVector(targetGroup, C_SEM, vsTGroup);

	for (vector<typo_string>::iterator tgit = vsTGroup.begin(); tgit!= vsTGroup.end(); tgit++){
		algSeq += (*dict_inifile)[ALGORITHMS_TGOL][*tgit];
	}

	return RET_FSUCCESS;
}

int Application::loadContainers() 
{
	try{
		ContainersManipulators cm; 
		if (getContainerNamesList()->empty())
			return RET_FFAILURE;
		bool needsDict = false;
		typo_string dictCompFuncName=S_EMPTY;

		list<ssi_pair>::iterator it=getContainerNamesList()->begin();
		for(it=getContainerNamesList()->begin(); it !=getContainerNamesList()->end(); ++it){ 
			typo_string contName = ((*it).first).first;
			if ((contName == NGRAM_INDEX) || (contName == LETTER_SET_INTERSECTION_LIST) ||
				(contName == KEYBOARD_RULES_LIST) || (contName == LETTERS_MAPPING_RULES_LIST))
			{
				dictCompFuncName = ((*it).first).second;
				IDictionary *dict=static_cast<IDictionary *>(getContainer(DICTIONARY_INDEX, dictCompFuncName));//;
				if (false == cm.find<IAbstractProduct *>(_containerPointersList, dict))
					_containerPointersList.push_back(dict);
			}
			IDictionary *next=static_cast<IDictionary *>(getContainer(contName, S_EMPTY));
			if (NULL == next){ 
				_pLog->report(MSG_00026, contName);
				throw runtime_error(MSG_00005);
			}
			if (false == cm.find<IAbstractProduct *>(_containerPointersList, next))
				_containerPointersList.push_back(next);
		}

		_iNextContainerPtr=_containerPointersList.begin();
		return RET_FSUCCESS;
	}
	catch(const typo_string& msg){
		_pLog->report(msg, (*report_inifile)[TEXT_CONSTANTS_SECTION][TXT_ID_00005]); 
		throw runtime_error(msg);
	}
	catch(...){	throw runtime_error(MSG_00007); }
}

IAbstractProduct* Application::getContainer(const typo_string &GUID, const typo_string &compFuncName) 
{
	list<IAbstractProduct *>::const_iterator pit;
	for ( pit = _containerPointersList.begin(); pit!= _containerPointersList.end(); pit++){
		if (static_cast<IDictionary *>((*pit))->getID() == GUID)
			return *pit;
	}
	IDictionary *pC=static_cast<IDictionary *>(loadContainer(GUID, compFuncName));
	if (pC) _containerPointersList.push_back(pC);
	return pC;
	return NULL;
}

void Application::resetContainerPtr()
{
	_iNextContainerPtr = _containerPointersList.begin();
}

int Application::getSize(const typo_string &GUID)
{
	list<IAbstractProduct *>::iterator pit;
	for ( pit = _containerPointersList.begin(); pit!= _containerPointersList.end(); pit++){
		if (static_cast<IDictionary *>((*pit))->getID() == GUID)
			return static_cast<IIndexedDictionary *>((*pit))->getSize();
	}
	return RET_FFAILURE;
}



void Application::getComparisonFunctionName(const typo_string &key, typo_string &value)
{ 
	if (value != S_EMPTY) return;
	if (false == _l_ssi_containerNamesListWrapper.getComparisonFunction(key, value))
		value = (*paramSet_inifile)[CONTAINER_TO_COMPARISON_MAP_SECTION][key];
}

int Application::getIniFile(const typo_string &name, IniFile &inifile)
{
	if (name == APP){ inifile=*app_inifile;				return RET_FSUCCESS;}
	if (name == DICT){ inifile=*dict_inifile;			return RET_FSUCCESS;}
	if (name == USERNAME){ inifile=*username_inifile;	return RET_FSUCCESS;}
	if (name == REPORT){ inifile=*report_inifile;		return RET_FSUCCESS;}
	if (name == PARAM_SET){ inifile=*paramSet_inifile;	return RET_FSUCCESS;}
	return RET_FFAILURE;
}

bool ContainerNamesListWrapper::getComparisonFunction(const typo_string &key, typo_string &value) 
{
	list<ssi_pair>::const_iterator it=_containerNamesList.begin();
	for(it=_containerNamesList.begin(); it != _containerNamesList.end(); ++it){ 
		if ((*it).first.first == key) {
			value = (*it).first.second;
			return true;
		}
	}
	return false;
}

bool ContainerNamesListWrapper::getRating(const ss_pair &key, int &value) 
{
	list<ssi_pair>::const_iterator it=_containerNamesList.begin();
	for(it=_containerNamesList.begin(); it != _containerNamesList.end(); ++it){ 
		if ((*it).first == key) {
			value = (*it).second;
			return true;
		}
	}
	return false;
}

bool ContainerNamesListWrapper::increaseRating(const ss_pair &key) 
{
	list<ssi_pair>::iterator it=_containerNamesList.begin();
	for(; it != _containerNamesList.end(); ++it){ 
		if ((*it).first == key) {
			(*it).second++;
			return true;
		}
	}
	return false;
}

int ContainerNamesListWrapper::loadList(const typo_string &algList, const typo_char& DELIMITER)
{
	StringManipulators sm;
	typo_string next;
	int offset=0;
	while(sm.getNext(algList, DELIMITER, offset, next)){
		ss_pair ss = ss_pair(sm.getLeft(next, C_COLON), sm.getRight(next, C_COLON));
		if ((S_EMPTY != ss.first) && (S_EMPTY != ss.second))
			_containerNamesList.push_back(ssi_pair(ss, 0));
	}

	return RET_FSUCCESS;
}

void Application::readIniFiles()
{
	readApplicationIni();
	readMessagesIni();
	readParametersIni();
	readAlgoritmsIni();
	readUserIni(); //i.e groker.ini 
}

void Application::readApplicationIni()
{
	try{ 
		app_inifile = new IniFile(INSTALL_DIR + APP_INI_FILENAME, false, true); 
	}
	catch(const typo_string& msg){	throw runtime_error(msg); }
	catch(...){	throw runtime_error(MSG_00005);}
}

void Application::readMessagesIni()
{
	try{ 
		_UI_Language = (*app_inifile)[APP_DATA_SECTION][_UI_LANGUAGE]; 
		_Current_Input_Language = (*app_inifile)[APP_DATA_SECTION][_CURRENT_INPUT_LANGUAGE]; 
		_Current_Output_Language = (*app_inifile)[APP_DATA_SECTION][_CURRENT_OUTPUT_LANGUAGE]; 
		typo_string messagesIniFileName = INSTALL_DIR + ErrorDefinitionsFileName + _UI_Language + INI_EXT;
		report_inifile = new IniFile(messagesIniFileName.c_str(), false, true); 

		typo_string logtype=(*app_inifile)[APP_DATA_SECTION][LOGTYPE]; 
		if (logtype == FILE_REPORT)
			_pLog = new LogReport(INSTALL_DIR, report_inifile);
		else
			_pLog = new ConsoleReport(INSTALL_DIR, report_inifile);
	}
	catch(const typo_string& msg){	throw runtime_error(msg); }
	catch(...){	throw runtime_error(MSG_00005);}
}

void Application::readParametersIni()
{
	_parametersSetIniFilePath = INSTALL_DIR + (*app_inifile)[PATH_SECTION][PARAMETERS_SET_INI_FILENAME_SECTION];
	try{ 
		paramSet_inifile = new IniFile(_parametersSetIniFilePath, false, true); 
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00019, _parametersSetIniFilePath);
		throw runtime_error(msg); 
	}
	catch(...){	throw runtime_error(MSG_00005); }
}

void Application::readAlgoritmsIni()
{
	_dictIniFilePath = INSTALL_DIR + (*app_inifile)[PATH_SECTION][DICT_INI_FILENAME_SECTION];
	try{ 
		dict_inifile = new IniFile(_dictIniFilePath, false, true); 
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00019, _dictIniFilePath);
		throw runtime_error(msg); 
	}
	catch(...){	throw runtime_error(MSG_00005); }
	_NGramLength = atoi((*paramSet_inifile)[NGRAMCONSTANTS_SECTION][NGRAM_LENGTH].c_str());
	_TopSim = atoi((*paramSet_inifile)[ALG_CONSTANTS_SECTION][TOPSIM].c_str());
	_maxDictSize = atoi((*paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
	_spellingsSuggestionsListSize= atoi((*paramSet_inifile)[ALG_CONSTANTS_SECTION][SPELLINGSSUGGESTIONSLISTSIZE].c_str());
}

void Application::readUserIni()
{
	typo_string lastUserConfigFilePath = INSTALL_DIR + (*app_inifile)[PATH_SECTION][LASTUSERCONFIGFILEPATH];
	try{ 
		username_inifile = new IniFile(lastUserConfigFilePath.c_str() , false, true); 
	}
	catch(const typo_string& msg){	
		_pLog->report(MSG_00019, lastUserConfigFilePath.c_str());
		throw runtime_error(msg);
	}
	catch(...){	throw runtime_error(MSG_00005); }
}

//TBD updates UserProfileAppDataSection::_StatisticsAnalizerDefinedAlgorithmsSequence	
int Application::saveStatisticsAnalizer()
{  
	return RET_FSUCCESS;
}

//TBD updates _mAlgRating
int Application::analizeSession() 
{
	return RET_FSUCCESS;
}


int Application::add_correction(const typo_string &w, const typo_string &f)
{

	return RET_FSUCCESS;
}

int Application::add_correction(const typo_string &file){
	return RET_FSUCCESS;
}

