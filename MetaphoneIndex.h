#ifndef __PHONETIC_UTILITIES_H__
#define __PHONETIC_UTILITIES_H__

#include "IDictionary.h"
#include "Filter.h"
#include "data_types.h"
#include "SimplePipeBuilder.h"

namespace SpellChecker
{

class MetaphoneIndex : public IIndexedDictionary
{
		SimplePipeBuilder *_pSimplePipeBuilder; 
public:
	MetaphoneIndex(SimplePipeBuilder *mf, const typo_string &d, const typo_string &m);

	virtual ~MetaphoneIndex(){
			if (!_vss.empty())
		_vss.erase(_vss.begin(), _vss.end());
					//last change
		delete _pComp;
		_pComp = NULL;
	}
	virtual typo_string getID(){return _GUID;}
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);
	virtual int getSize(){return _vss.size();}
	virtual int find(typo_string &s){return RET_FSUCCESS;}

private:
	int load_metaphone_map();

	typo_string dpath;
	typo_string mpath;
	typo_string _GUID;
	vector <ss_pair> _vss;
	AbstractProduct* _pComp;

	typo_string name;
	typo_string description;
	typo_string domain;
	int _TopSim;
	int _MaxDictSize;
};

};

#endif //__PHONETIC_UTILITIES_H__
