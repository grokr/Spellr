#ifndef __LETTERSMAPPINGRULES_H__
#define __LETTERSMAPPINGRULES_H__

#include "IDictionary.h"
#include "IOManipulators.h"
#include "IAbstractProduct.h"
#include "Application.h"
#include "DictionaryIndex.h"
#include "ContainersManipulators.h" 
#include "Timer.h"
#include "StringManipulators.h"


namespace SpellChecker
{	
	struct LettersMappingRulesParameters
	{
		typo_long_num stars_number_limit1;
		typo_long_num stars_number_limit2;
		typo_long_num stars_number_limit3;
		
		size_t output_size_limit1;
		size_t output_size_limit2;
		
		int recursionDepth;
		int encounter_only_existing_words;
		//int _two_errors_in_one_word;
		
		typo_string control_word;
		
		int learning_phase_window_size;

		typo_long_num minSim;

		int existing_word_bonus;
		int substitute_ngram_star_bonus;

		int compare_to_original;
		int shorten_list;
		int narrowSearchByNGram;
		int first_match_is_good_match;
		typo_long_num time_limit;
		int max_length_diff;

		typo_long_num rate_by_recursion_depth;
		typo_long_num ngram_star_value;
	};

class LettersMappingRules: public IRulesList{
	Application *_pSimplePipeBuilder; 
public:

	LettersMappingRules(Application *mf, const typo_string &d);

	virtual ~LettersMappingRules(){
		if (_changed)
		{
			_iom.saveLettersMappings(_pSimplePipeBuilder->INSTALL_DIR +_lmpath, 
				_lm, S_NL, true);
			//iom.io_write<typo_string, vector<typo_string>>(_pSimplePipeBuilder->INSTALL_DIR +_lmpath, 
			///	_lm, S_SPACE, true/*delete the file contents before writing*/);
		}
		_lm.erase(_lm.begin(), _lm.end());
	}

	int virtual getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);

	virtual int add_correction(const typo_string &w, const typo_string &f);
	virtual int add_correction(const typo_string &file);

	virtual int remove_correction(const typo_string &w, const typo_string &f){ return RET_FFAILURE;};

	virtual typo_string getID(){return _GUID;}

private:
	
	int doCreateMappingsList(const typo_string &w1, const typo_string &w2,
			int& leftStartIndex, int& rightStartIndex, const int& leftCurrentIndex, const int& rightCurrentIndex);

	int createMappingsList(const typo_string &w, const typo_string &f);
	int createLettersSubstituteCandidatesList(const typo_string& to_fix,
				   const typo_string &ngram_transformed, vector<typo_string>& substCandList);
	int recalculate_similarity(const typo_string &in, vector<ds_pair> &res);
	int recalculate_similarity_ngram(const typo_string &in, const typo_string &encoded, vector<ds_pair> &res);

//	void pick_n_best_suggestions(vector<ds_pair> &res, const int &size);
	void readParameters();
	void mergeVectorsConditionally(const typo_string &to_fix, vector<ds_pair> &res, vector<ds_pair> &existing_words);
	void mergeExistingCandidates(const typo_string &to_fix, vector<ds_pair> &res, vector<ds_pair> &existing_words);

	void getWordsSimilarToLettersMappings(const typo_string &in, const typo_string &encoded, vector<ds_pair> &res);
	
	int createLettersMappings(const typo_string &in, vector<ds_pair> &res, vector<ds_pair> &existing_words);
	int doCreateLettersMappings(const typo_string &in, vector<ds_pair> &res, vector<ds_pair> &existing_words);
	int doDoCreateLettersMappings(const ds_pair &to_fix, const typo_string &ngram, vector<ds_pair> &res, vector<ds_pair> &existing_words);
	int createMappingsListForOneLetter(const svpsi_pair &mappings, const typo_string &reconstructed,
							const typo_string &ngram_transformed, vector<ds_pair> &res, vector<ds_pair> &existing_words,
							const typo_long_num& prob);

	int doDoCreateLettersMappingsNGram( ds_pair &to_fix,  ds_pair &ngram, vector<ds_pair> &res, vector<ds_pair> &existing_words);

	int storeCandidate(  typo_string &candidate, 
							   const typo_string &ngram_transformed, vector<ds_pair> &res, 
							   vector<ds_pair> &existing_words, const int &pos, const typo_long_num &prob);
	int shortenList(typo_string &transformed, const size_t &, const size_t &);

	typo_long_num _ngram_sum_of_prob;
	typo_string in_ngram_transformed;

	bool _changed;
	Timer _timer;
	LettersMappingRulesParameters _params;
	IOManipulators _iom;
	StringManipulators _sm;
	ContainersManipulators _cm;
	typo_string _lmpath;
	typo_string _GUID;
//	map<typo_string, vector<typo_string>> _lm;
	map<typo_string,  vector<si_pair>> _lm;

	DictionaryIndex *_pDict;
	IAbstractProduct *_pNGram;
	IAbstractProduct *_pComp;

	typo_string _name;
	typo_string _description;
	typo_string _domain;
	int _recursionDepth;
};
};

#endif //__LETTERSMAPPINGRULES_H__
