#ifndef __SPELLCHECKERFACTORY_H__
#define __SPELLCHECKERFACTORY_H__

#include "IAbstractProduct.h"
#include "AbstractApplicationInterface.h"
#include "IniFileHandler.h"
#include "Log.h"
#include "DictionaryIniConstants.h"

#include <list>

#pragma warning(push)
#pragma warning(disable:4251)

using namespace IniFileNamespace;

namespace SpellChecker 
{
	typedef list<ssi_pair> ContainerNamesList;
	typedef list<IAbstractProduct *> ContainerPointersList;

class TYPODLL_API ContainerNamesListWrapper
{

	ContainerNamesList _containerNamesList; 
	ContainerNamesList *_pContainerNamesList; 
public:
	
	ContainerNamesListWrapper(){
		_pContainerNamesList = &_containerNamesList;
	}
	~ContainerNamesListWrapper(){
		_containerNamesList.erase(_containerNamesList.begin(), _containerNamesList.end());
		//delete _pContainerNamesList;
		_pContainerNamesList = NULL;
	}
	bool getComparisonFunction(const typo_string &key, typo_string &value);
	bool getRating(const ss_pair &key, int &value);
	bool increaseRating(const ss_pair &key);
	int loadList(const typo_string &algList, const typo_char& delimiter);
	ContainerNamesList* getContainerNamesList(){ return _pContainerNamesList;} 
};


class TYPODLL_API Application : public AbstractApplicationInterface{

public:

	Application(const typo_string &installDir, const typo_string &runningMode,
		const typo_string &compFuncName=S_EMPTY, 
		const typo_string &perfRep=S_EMPTY, const typo_string &output_type=S_EMPTY);

	Application(const Application& c);

	virtual ~Application();
	
	//virtual 
		IAbstractProduct *makeSpellCheckerImplementation(const typo_string &id);
	//virtual 
		IAbstractProduct *makeComparisonFunction(const typo_string &id);
	virtual IAbstractProduct* makeSpellChecker();

	//virtual 
		IAbstractProduct* loadContainer(const typo_string &id, const typo_string &compFuncName);

	virtual void buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type)
	{
		//return mf->buildContainer(input, output, dict_type);
	}

	virtual 
		int add_correction(const typo_string &w, const typo_string &f);
	virtual 
		int add_correction(const typo_string &file);

//	virtual 
	IAbstractProduct* getContainer(const typo_string &id, const typo_string &compFuncName);
public:

	typo_string INSTALL_DIR; //TBD

	void getInstallDir(typo_string& inst_dir){ inst_dir = INSTALL_DIR;}
	ContainerNamesList *getContainerNamesList(){ return _pl_ssi_containerNamesListWrapper->getContainerNamesList();}
	void getComparisonFunctionName(const typo_string &key, typo_string &value);
	void resetContainerPtr();
	int getSize(const typo_string &GUID);
	ErrorReport *getLog(){ return _pLog;}
	int getSpellingsSuggestionsListSize() const { return _spellingsSuggestionsListSize;}
	void getPerfReportFilePath(typo_string &file) { file = _perfRepFilePath;}
	void getParamSetIniFilePath(typo_string &filepath) { filepath = _parametersSetIniFilePath;}
	
	//hide this:
	IniFile *app_inifile;
	IniFile *dict_inifile;
	IniFile *paramSet_inifile;
	IniFile *username_inifile;
	IniFile *report_inifile;

public: //self-learning
	ContainerNamesListWrapper *getContainerNamesListWrapper(){ return _pl_ssi_containerNamesListWrapper;}
	ContainerPointersList *getContainerPointersList(){ return _pContainerPointersList;}

public: //accessors

	void getUILanguage(typo_string &lang){lang = _UI_Language;} //used for messages.ini , Log
	void getCurrentInputLanguage(typo_string &lang){lang = _Current_Input_Language;}
	void getCurrentOutputLanguage(typo_string &lang){lang = _Current_Output_Language;}
//	void getOutputType(typo_string &output_type){output_type = _output_type;} 
	bool isStdOut(){  
		if (_output_type == STD_OUT)
			return true;
		return false;
	}

	Application &operator=(const Application& c);

	//hide this:
	int _TopSim;

private:
	
	int loadContainers();
//	void unloadContainers();
	void getDictIniFilePath(typo_string &filepath) { filepath = _dictIniFilePath;}
	int getIniFile(const typo_string &name, IniFile &inifile);

private: //init stage

	void readUserIni();
	void readAlgoritmsIni();
	void readParametersIni();
	void readMessagesIni();
	void readApplicationIni();
	void readIniFiles();

	
	int init();
	int constructAlgList(typo_string& algSeq);
	int createAlgorithmsList();
	int saveStatisticsAnalizer();
	int analizeSession();

private: //data

	list<IAbstractProduct *>::iterator _iNextContainerPtr;
	ContainerNamesListWrapper _l_ssi_containerNamesListWrapper; 
	ContainerNamesListWrapper *_pl_ssi_containerNamesListWrapper;
	ContainerPointersList _containerPointersList;
	ContainerPointersList *_pContainerPointersList;

	ErrorReport *_pLog;
	typo_string _contCompFuncName; 
	int _spellingsSuggestionsListSize;
	int _NGramLength;
	typo_string _perfRepFilePath;
	typo_string _dictIniFilePath;
	typo_string _parametersSetIniFilePath;
	typo_string _UI_Language; 
	typo_string _Current_Input_Language;
	typo_string _Current_Output_Language;
	typo_string _currentDictIndexFilePath;
	int _maxDictSize;
	typo_string _output_type;
};


};

#pragma warning(pop)

#endif //__SPELLCHECKERFACTORY_H__