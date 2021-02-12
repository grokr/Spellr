#ifndef __LETTERSETINTERSECTIONALG_H__
#define __LETTERSETINTERSECTIONALG_H__

#include "IDictionary.h"
#include "IAbstractProduct.h"
#include "Application.h"
#include "DictionaryIndex.h"
#include "StringManipulators.h"
#include "ContainersManipulators.h"

namespace SpellChecker
{

class LetterSetIndex: public IIndexedDictionary{
	Application *_pSimplePipeBuilder; 
public:

	LetterSetIndex(Application *mf, const typo_string &d, const typo_string &ls);

	virtual ~LetterSetIndex(){
		if (!_letterSet.empty())
		_letterSet.erase(_letterSet.begin(), _letterSet.end());
		//last change
		delete _pComp;
		_pComp = NULL;
	}
	virtual typo_string getID(){return _GUID;}
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);
	virtual int find(typo_string &s){return RET_FSUCCESS;}
	virtual int getSize(){return _letterSet.size();}
	virtual int add_word(const typo_string &w, const typo_string &f);
	virtual int remove_word(const typo_string &w, const typo_string &f);

private:

	int load_letterSetIndex();
	int getSectionEnd(int size);
	int getSectionBeginning(int size);
//	void getLetterIntersectionIndexMapping(int index, typo_string &s, int &freq);

	int _min_sim;
	int _only_exact_match_counts;
	int _MaxDictSize;
	typo_string _dpath;
	typo_string _lspath;
	typo_string _GUID;

	map<typo_string, typo_string> _letterSet;
	map<int, pair<int, int>> length_map;

	IAbstractProduct* _pComp;
	DictionaryIndex *_pDict;
	IAbstractProduct* _pComp_LetterSetSimilarity;

	ContainersManipulators _cm;
	StringManipulators _sm;

	typo_string name;
	typo_string description;
	typo_string domain;

};
};
#endif //__LETTERSETINTERSECTIONALG_H__
