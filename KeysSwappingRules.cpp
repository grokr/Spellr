/*! \class KeysSwappingRules
 *  \brief KeysSwappingRules: A container, implemented as map<typo_string, typo_string>
 *  \author Oxana Rybalko
 *  \version 0.0.3
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */
#include "KeysSwappingRules.h"
#include "SpellerTrainer.h"
#include "ContainersManipulators.h"
#include "LogConstants.h"

#include <fstream>
#include <algorithm>

KeysSwappingRules::KeysSwappingRules(Application *mf, const typo_string &d):_pSimplePipeBuilder(mf), 
	_kmpath(d), _GUID(KEYBOARD_RULES_LIST), _pDict(NULL)
{
	_kmpath = (_pSimplePipeBuilder->INSTALL_DIR + _kmpath).c_str();
	_MaxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
	_only_exact_match_counts = atoi((*_pSimplePipeBuilder->paramSet_inifile)[KEYBOARDRULESLISTCONSTANTS_SECTION][ONLY_EXACT_MATCH_COUNTS].c_str());

	try{
		load_KeysSwappingRules();
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}

	_pDict = static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
	
}


int KeysSwappingRules::load_KeysSwappingRules()
{
	ifstream is;
	is.open(_kmpath.c_str());
	if ( false == is.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, _kmpath.c_str());	throw runtime_error(MSG_00006);	}

	typo_char c; typo_string  word;
	
	if (false != is.good()){
		while (!is.eof() && ((int)_keys_map.size() < _MaxDictSize)){
			is >> c; is >> word;
			_keys_map.insert(cs_pair(c, word));	
		}
		is.close();

		return RET_FSUCCESS;
	}
	else{
		_pSimplePipeBuilder->getLog()->report(MSG_00018, _kmpath);
		return RET_FFAILURE;
	}
}

//CREATES A PLAIN LIST OF CANDIDATES
//DOES NOT USE ANY COMPARISON FUNCTION TO CHOOSE MOST PROBABLE,
//IS INTENDED TO BE USED WITH OTHER ALGORITHMS, FOLLOWED BY THIS

//TODO: add depth, so that 2 and more misspellings will be fixed
int KeysSwappingRules::getSuggestions(const typo_string &in, vector<ds_pair> &res, Filter &filter)
{
	typo_string lin = in;

	ContainersManipulators cm;
	typo_long_num sim = D_HUNDRED;

	for (size_t i = 0; i < in.size(); i++){ 
		map<typo_char, typo_string>::iterator it = _keys_map.find(lin[i]); 
		if (it == _keys_map.end())
			continue;
		typo_string s = (*it).second;
		for (size_t j = 0; j < s.size(); j++){
			typo_string cand = in;
			cand.replace(i, 1, 1, s[j]);

			if (_pDict->find(cand) != RET_FIND_NOT_FOUND){
				if (_only_exact_match_counts){
					res.push_back(ds_pair(D_HUNDRED, cand));
					return RET_FSUCCESS;
				}
				if (cm.findIndex(res, cand) == RET_FFAILURE)
						res.push_back(ds_pair(D_HUNDRED, cand));
			}
			else
			{
				if (_only_exact_match_counts) continue;
				res.push_back(ds_pair(D_ZERO, cand));
			}
		}
	}
	return RET_FSUCCESS;
}
