#ifndef __SENTENCENGRAMINDEX_H__
#define __SENTENCENGRAMINDEX_H__

#include "IDictionary.h"
#include "Application.h"
#include "Filter.h"
#include "IOManipulators.h"
#include "DictionaryIndex.h"
#include "ContainersManipulators.h"
#include "SynonymsMap.h"

#include <sstream>

namespace SpellChecker
{


//k from n: 
	//3 from 26: 3!*26!/23!=6x 24x 25x 26 = 93.600 bytes  100K+25%=120KB
	//4 from 26: 24x23x24x25x26=8.611.200 bytes 8MB
	//look up O(1)
class SentenceNGramIndex : public IIndexedDictionary
{
		Application *_pSimplePipeBuilder; 
public: 

	SentenceNGramIndex(Application *mf, int n, const typo_string &filename);

	virtual ~SentenceNGramIndex(){
		if (!_SentenceNGram.empty())
		_SentenceNGram.erase(_SentenceNGram.begin(), _SentenceNGram.end());
		//last change
		delete _pComp;
		_pComp = NULL;
	}
	virtual typo_string getID(){return _GUID;}
	virtual int getSize(){return _SentenceNGram.size();} //used
	virtual int find(typo_string &s){return RET_FSUCCESS;}
	
	//is not in use:
	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter); //getSynonyms
	
	/** for use after another algorithms: updates cand_list similarities occording to _SentenceNGram **/
	int getSuggestions(const sss_pair &currentElement, vector<ds_pair> &cand_list);

	/** for use after another algorithms: updates cand_list similarities occording to Synonyms **/
	int filterOutNonSynonyms(const typo_string &to_fix,  vector<ds_pair> &cand_list);
	
	/** for use as a first algorithm in a pipe: builds cand_list**/
	int getSuggestions(sss_pair& currentEl, sss_pair& pPrevElToBeFixed, const typo_string&  next_word, vector<ds_pair> &cand_list);
//void factorFrequencies(vector<ds_pair>& cand_list);

private:
	int doGetSuggestions(const sss_pair& currentElement, sss_pair& previousElement, 
		const typo_string& next_word, vector<ds_pair> &cand_list);
//	map<typo_string, typo_long_num>::iterator findSimilar(const typo_string &s, typo_long_num sim);
	
	//return frequency in dictionary for a given ngram
//	typo_long_num getNGramFrequency(const typo_string &s) const;
	//return frequency in dictionary for a similar ngram
//	typo_long_num getNGramFrequency(const typo_string &s, typo_long_num sim, typo_string &similar);

	//int run_NGram(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim);

private:
//	typo_long_num sum_of_next_n(vector <typo_long_num>::const_iterator& it);

	DictionaryIndex *_pDict;
	int _n;
	typo_string _filename;
	typo_string _GUID;
	int _size;
	map<sss_pair, typo_long_num> _SentenceNGram;
		bool _bNeedsCorrection;
	SynonymsMap* _pSynonymsMap;

	IAbstractProduct* _pComp;
	int _toFixSyntacticErrors;
	ContainersManipulators _cm;

//	typo_string name;
//	typo_string description;
//	typo_string domain;
	typo_long_num _meaningfulFrequency;
	typo_long_num _sentenceNGramFoundCoeff;
	typo_long_num _sentenceNGramNotFoundCoeff;
	typo_long_num _SynonymBonus;
	typo_long_num _matches_within_window;
	int _filterOutNonSynonyms;
	typo_long_num _minSim;
//	typo_long_num _warp;
//	typo_long_num NGramStarValue;
};

};

#endif //__SENTENCENGRAMINDEX_H__