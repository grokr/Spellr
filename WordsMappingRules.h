#ifndef __WORDSMAPPINGRULESALG_H__
#define __WORDSMAPPINGRULESALG_H__

#include "IDictionary.h"
#include "IOManipulators.h"
#include "SpellerTrainer.h"
#include "IAbstractProduct.h"
#include "Application.h"
#include "LogConstants.h"
#include "StringManipulators.h"


namespace SpellChecker
{


class WordsMappingRules: public IRulesList{
	
	Application *_pSimplePipeBuilder; 
public:

	WordsMappingRules(Application *mf, const typo_string &d):_pSimplePipeBuilder(mf), _wmpath(d), _changed(false), 
		_GUID(WORDS_MAPPING_RULES_LIST)
	{
		//_pComp is not used in WordsMappingRules
		try{
			_MaxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
			load_WordsMappingRules();
		}
		catch(...){
			throw runtime_error(MSG_00006);
		}
	}

	virtual ~WordsMappingRules();
	virtual typo_string getID(){return _GUID;}
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);
	virtual int add_correction(const typo_string &w, const typo_string &f);
	virtual int add_correction(const typo_string &file);
	virtual int remove_correction(const typo_string &w, const typo_string &f);

private:

	WordsMappingRules &operator=(const WordsMappingRules& d){}
	int load_WordsMappingRules();
	int load_letterSetIndex();

private:

	int _MaxDictSize;
	typo_string _wmpath;
	bool _changed;
	typo_string _GUID;	
	map<typo_string, vector<typo_string>> _wm;
	//IAbstractProduct* _pComp;
	StringManipulators _sm;

	typo_string name;
	typo_string description;
	typo_string domain;
	typo_long_num _TopSim;

};
};

#endif //__WORDSMAPPINGRULESALG_H__
