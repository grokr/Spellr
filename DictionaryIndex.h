#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "IDictionary.h" //for IIndexedDictionary
#include "data_types.h"
#include "Filter.h"
#include "IOManipulators.h"
#include "Application.h"
#include "ContainersManipulators.h" 
#include "StringManipulators.h"
#include "WordNetApiHandler.h"
#include <map>
namespace SpellChecker
{

	typedef TYPODLL_API pair<typo_char, ii_pair> cii_pair;
	typedef TYPODLL_API map<typo_char, ii_pair> IndexedDictionary;

class Word;
//typedef TYPODLL_API pair<typo_long_num, Word> dw_pair; ///for sim, Word

struct DictionaryIndexParameters
{
	typo_long_num 	minSim;
	typo_long_num 	toAdjustWeight;// 0 or 1
	typo_long_num 	countProbabilitiesFromCandidatesList;
	typo_long_num	dictFreqWeight;// 0.5
	typo_long_num	toCalculateSyntacticSimilarity;// 0 or 1
	typo_long_num	syntacticSimilarityWeight;//200
	typo_long_num	toCalculateFreqWeight;//0 or 1
	typo_long_num 	prevCalcWeight; //0.5
	typo_long_num 	opinionMatters ; //0.8
	typo_long_num 	linearWeight; //0.8
	typo_long_num 	linearSimilarityMinimalPercentage;//50%
	typo_long_num 	syntSimilarityMinimalPercentage;//50%
	typo_long_num 	syntWeight;//2.0
	typo_long_num 	lettersetWeight ; //0.8
	typo_long_num 	sameSizeWeight ; //0.03
	typo_long_num 	diffSizeWeight ; //0.02
	typo_long_num 	sameLastLetter ; //0.03
	typo_long_num	sameFirstLetter;
	typo_long_num 	diffLastLetter; //0.05
	typo_long_num 	samePreLastLetter ; //0.01
	typo_long_num 	diffPreLastLetter; //0.01
	typo_long_num	compFuncForWeight;
	typo_long_num	compFuncAgainstWeight;
	typo_long_num	zeroCounterWeight;
	typo_long_num	filter_out_length_factor;
	int				first_letter_optimization;
	int				spellingsSuggestionsListSize;
	int				minimal_meaningful_substring_length;
	int				most_likable_candidates_number;
	size_t			minimal_compound_word_size;

	int				maxDictSize;
	typo_string		control_word;
	int				use_wordnet;
};

class DictionaryIndex : public IIndexedDictionary 
{
//	friend class SpellerTrainer; //needed?
	Application *_pSimplePipeBuilder; 
public:

	DictionaryIndex(Application *mf, const typo_string &p, const typo_string &compFuncName=S_EMPTY);
	virtual ~DictionaryIndex();
	virtual int add_word(const Word &w);
	virtual int remove_word(const Word &w);

	virtual typo_string getID(){return _GUID;}

	virtual int getSize(){return _dict.size();
	}
	virtual int find(typo_string &s);

	virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter); 

	virtual int add_correction(const typo_string &w, const typo_string &f);
	virtual int add_correction(const typo_string &file);

	int filterOutSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter); 
	void getLetterIntersectionIndexMapping(int index, typo_string &s, typo_long_num &freq);
	typo_long_num factorFrequency(const typo_long_num& f);

private:

	//int Handle_plural_missing_from_dictionary();

	DictionaryIndex &operator=(const DictionaryIndex& d){}
	int load_dictionary();
	int save_dictionary(const typo_string &delim);
	
	void adjustWeight(const typo_string& to_fix, ds_pair& cand);

	void init_cii_dictIndexMap();
	vector<Word>::const_iterator getStartIndex(const typo_char& t);
	vector<Word>::const_iterator getEndIndex(const typo_char& t);

	void readParameters();

	void countProbabilitiesFromCandidatesList(vector <ds_pair> &res);
	int stop_if_found(vector <ds_pair>& res);

private:

	WordNetApiHandler _wn;

	bool _changed;
	DictionaryIndexParameters _params;
	ContainersManipulators _cm;
	
	typo_string _compFuncName;

	vector <Word> _dict;
	IndexedDictionary _cii_dictIndexMap;
	
	IOManipulators iom;
	StringManipulators _sm;
	IAbstractProduct* _pComp;

	IAbstractProduct *_nGram; 

	
	typo_string _dpath;
	typo_string _GUID; //not in use?
	int _maxDictSize;
//	typo_string name;
//	typo_string description;
//	typo_string domain;	
};

class Word
{
public:
	
	Word(const typo_string &w, const typo_long_num& f):_data(w), _freq(f){}
//	void setFreq(typo_long_num d)  { _freq=d;}
	void incrementFreq() { _freq++;}
	bool operator==(const Word& c);
//	typo_string getSyn() const { return _synonym;}

//private:
	typo_string _data;
	typo_long_num _freq;
//	typo_string _synonym;
//	typo_string _antonym;
};



};

#endif //__DICTIONARY_H__