#ifndef __KEYBOARDRULESLISTALG_H__
#define __KEYBOARDRULESLISTALG_H__

#include "IDictionary.h"
#include "IOManipulators.h"
#include "Application.h"
#include "DictionaryIndex.h"

namespace SpellChecker
{


class KeysSwappingRules: public IRulesList{

	Application *_pSimplePipeBuilder; 
public:
	

	KeysSwappingRules(Application *mf, const typo_string &d);

	virtual ~KeysSwappingRules(){
	//	iom.io_write(upads._KeyboardFrequencyFileName, _freq_map, S_SPACE);
		if (!_keys_map.empty())
		_keys_map.erase(_keys_map.begin(), _keys_map.end());
//		_freq_map.erase(_freq_map.begin(), _freq_map.end()); 
	}
	virtual typo_string getID(){return _GUID;}
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);
	virtual int add_correction(const typo_string &w, const typo_string &f){return RET_FFAILURE;};
//	virtual int remove_correction(const typo_string &w, const typo_string &f){return RET_FFAILURE;};
//	virtual int update_frequency(const int &index){}
//	int virtual getSuggestions(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim, 
//		computeSimilarityRatioType comp_f){
		//call default impl
//	int virtual find(typo_string &s){return 0;}
//	int virtual getSize(){return _keys_map.size();}

private:

	int _only_exact_match_counts;
//	int _max_misspellings_number;


	DictionaryIndex *_pDict;
	int load_KeysSwappingRules();

	typo_string _kmpath;
	typo_string _GUID;
	map<typo_char, typo_string> _keys_map;
	typo_string name;
	typo_string description;
	typo_string domain;
	int _MaxDictSize;
};
};





#endif //__KEYBOARDRULESLISTALG_H__
