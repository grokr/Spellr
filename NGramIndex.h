#ifndef __SENTENCE_NGRAM_H__
#define __SENTENCE_NGRAM_H__

#include "IDictionary.h"
#include "Application.h"
#include "Filter.h"
#include "IOManipulators.h"
#include "DictionaryIndex.h"

namespace SpellChecker
{

//k from n: 
	//3 from 26: 3!*26!/23!=6x 24x 25x 26 = 93.600 bytes  100K+25%=120KB
	//4 from 26: 24x23x24x25x26=8.611.200 bytes 8MB
	//look up O(1)
class NGramIndex : public IIndexedDictionary
{
		Application *_pSimplePipeBuilder; 
public: 

	NGramIndex(Application *mf, int n, const typo_string &filename);

	virtual ~NGramIndex(){
		if (!_NGram.empty())
		_NGram.erase(_NGram.begin(), _NGram.end());
		//last change
		delete _pComp;
		_pComp = NULL;
	}
	virtual typo_string getID(){return _GUID;}
	virtual int getSize(){return _NGram.size();} //used
	virtual int find(typo_string &s){return RET_FSUCCESS;}
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);

	int encode(const typo_string &s, typo_string &to_fix, typo_long_num &sim);
	int simple_encode(typo_string &to_fix);
	int simple_encode_imporoved(const typo_string &to_fix, typo_string &fixed, typo_long_num &sim);

	//return frequency in dictionary for a given ngram
	typo_long_num getNGramFrequency(const typo_string &s) const;
	//return frequency in dictionary for a similar ngram
	typo_long_num getNGramFrequency(const typo_string &s, typo_long_num sim, typo_string &similar);
private:

	map<typo_string, typo_long_num>::iterator findSimilar(const typo_string &s, typo_long_num sim);


	//int run_NGram(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim);

private:
	typo_long_num sum_of_next_n(vector <typo_long_num>::const_iterator& it);
	typo_long_num sum_of_probabilities(const vector <typo_long_num>& v);

	DictionaryIndex *dict;
	int _n;
	typo_string _filename;
	typo_string _GUID;
	int _size;
	map<typo_string, typo_long_num> _NGram;
	IAbstractProduct* _pComp;

	typo_string name;
	typo_string description;
	typo_string domain;
	typo_long_num _meaningfulFrequency;
	typo_long_num _warp;
//	typo_long_num NGramStarValue;
	typo_long_num _nGramStarMargin;
};

};

#endif //__SENTENCE_NGRAM_H__