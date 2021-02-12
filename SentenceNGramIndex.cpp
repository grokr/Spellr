/*! \class SentenceNGramIndex
*  \brief SentenceNGramIndex - Semantic error matching, optionally takes into account syntactic errors
*  \author Oxana Rybalko
*  \version 0.0.3
*  \date  6 Nov 2009
*  \bug Not tested.
*  \bug 
*  \warning 
*/
#pragma warning (disable:4018)

#include "SentenceNGramIndex.h"
#include "DictionaryIndex.h"
#include "LogConstants.h"
#include "ContainersManipulators.h"
#include "StringManipulators.h"
#include "Utilities.h"

#include <algorithm>



SentenceNGramIndex::SentenceNGramIndex(Application *mf, int n, const typo_string &filename): 
_pSimplePipeBuilder(mf), _n(n), _filename(filename), _GUID(SENTENCE_NGRAM_INDEX), _pComp(NULL), _pDict(NULL), _pSynonymsMap(NULL)
{ 
	typo_string s = _pSimplePipeBuilder->INSTALL_DIR + _filename;
	_filename = s;
	IOManipulators iom;

	try{
		iom.io_read(_filename, _SentenceNGram);
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}

	_size= _SentenceNGram.size();

	typo_string compFuncName = S_EMPTY;
	_pSimplePipeBuilder->getComparisonFunctionName(SENTENCE_NGRAM_INDEX, compFuncName);
	_pComp = _pSimplePipeBuilder->makeComparisonFunction(compFuncName);

	_meaningfulFrequency = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][MEANINGFULFREQUENCY].c_str()); //0.5
	_minSim = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][SENTENCENGRAMINDEX_MIN_SIM].c_str());
	_sentenceNGramFoundCoeff = atoi((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][SENTENCENGRAM_FOUND_COEFF].c_str());
	_sentenceNGramNotFoundCoeff = atoi((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][SENTENCENGRAM_NOTFOUND_COEFF].c_str());
	_filterOutNonSynonyms= atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][FILTER_OUT_NON_SYNONYMS]).c_str());
	_toFixSyntacticErrors = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][SEMANTIC_ALG_FIXES_SYNTACTIC_ERRORS]).c_str());
	_SynonymBonus = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][SYNONYM_BONUS]).c_str());
	_matches_within_window = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MATCHES_WITHIN_WINDOW]).c_str());
	_bNeedsCorrection=true;

	_pDict=static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
	if (_pDict == NULL) throw runtime_error(MSG_00006);
	_pSynonymsMap = static_cast<SynonymsMap *>(_pSimplePipeBuilder->getContainer(SYNONYMS_MAP, S_EMPTY));
}


/** for use after another algorithms: updates cand_list similarities occording to _SentenceNGram 
triple:"I does it", 
cand_list by syntactic fix(input): candidates for "does": docs:95,dots:87,doms:70 
cand_list by semantic fix(output): candidates for "does": docs:75,dots:67,doms:50, did:90, do:82   
(_sentenceNGramNotFoundCoeff is 20)
**/
int SentenceNGramIndex::getSuggestions(const sss_pair& currentElement, vector<ds_pair> &cand_list)
{
	vector<typo_string> new_cand_list;
	vector<ds_pair>::iterator it=cand_list.begin(); 	
	map<sss_pair, typo_long_num>::iterator foundIt = _SentenceNGram.begin();
	sss_pair next=currentElement;

	for(it=cand_list.begin(); it != cand_list.end(); ++it)
	{	
		next.first.second=(*it).second;	
		foundIt = _SentenceNGram.find(next);
		if (foundIt != _SentenceNGram.end()){
			(*it).first += (*foundIt).second*_sentenceNGramFoundCoeff;
		}
		else{
			(*it).first -= _sentenceNGramNotFoundCoeff;
		}
	}

	vector<typo_string>::iterator it2=new_cand_list.begin(); 
	for(it2=new_cand_list.begin(); it2 != new_cand_list.end(); ++it2)
	{
		cand_list.push_back(ds_pair(D_ZERO, *it2));
	}

	return RET_FSUCCESS;
}

int SentenceNGramIndex::filterOutNonSynonyms(const typo_string& to_find,  vector<ds_pair> &cand_list)
{
	vector<ds_pair>::iterator it=cand_list.begin(); 	

	for(it=cand_list.begin(); it != cand_list.end(); ++it){
		if (_pSynonymsMap->isSynonym(to_find, (*it).second))
			(*it).first += _SynonymBonus;
	}
	return RET_FSUCCESS;
}

typo_long_num getFrequencyBonus(typo_long_num num)
{
	if (0 == num) return D_ZERO;
	if (num < 3) return D_ONE;
	if (num < 10) return D_TWO;
	return D_THREE;

}
int SentenceNGramIndex::doGetSuggestions(const sss_pair& currentElement, sss_pair& previousElement, const typo_string& next_word, 
									  vector<ds_pair> &cand_list)
{
	
	if (_SentenceNGram.find(currentElement) != _SentenceNGram.end() )
		return RET_FIND_EQUAL_CIS;

	
	ContainersManipulators cm;
	map<sss_pair, typo_long_num>::iterator it = _SentenceNGram.begin();
	typo_long_num local_sim = D_ZERO;
	for (; it != _SentenceNGram.end(); it++)
	{
		if ( (currentElement.first.first == (*it).first.first.first) && (currentElement.second == (*it).first.second))
		{
			typo_long_num occurrency_bonus = 0;
			typo_long_num frequency_bonus = getFrequencyBonus((*it).second);
	

			if (previousElement.first.first != S_EMPTY)
			{
				sss_pair cand = previousElement;
				cand.second = (*it).first.first.second;
				if (_SentenceNGram.find(cand) != _SentenceNGram.end() )
					occurrency_bonus += _matches_within_window*(frequency_bonus);
			}

			if (!next_word.empty())
			{
				sss_pair cand;
				cand.first.first = (*it).first.first.second;
				cand.first.second = (*it).first.second;
				cand.second = next_word;
				if (_SentenceNGram.find(cand) != _SentenceNGram.end() ){
					if (occurrency_bonus)
						occurrency_bonus *= _matches_within_window*(frequency_bonus);
					else
						occurrency_bonus += _matches_within_window*(frequency_bonus);
				}
			}

			int ind = cm.findIndex(cand_list, (*it).first.first.second);
			if (ind == -1)
				cand_list.push_back(ds_pair((frequency_bonus+occurrency_bonus)*D_HUNDRED, (*it).first.first.second));
			else {
				cand_list[ind].first++;
				cand_list[ind].first*=D_HUNDRED;
				cand_list[ind].first+=occurrency_bonus;
			}

		}
	}
	return RET_FFAILURE;
}

//getSynonyms
int SentenceNGramIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)
{
	return RET_FSUCCESS;
}




bool compare_dspair_SentenceNGramIndex(const ds_pair& a, const ds_pair& b) {
    return a.first > b.first;
}

//do_spell_words_stream_semantically
int SentenceNGramIndex::getSuggestions(sss_pair& currentEl, sss_pair& previousElement, 
												const typo_string&  next_word, vector<ds_pair>& cand_list)
{
	StringManipulators sm;
	
	typo_string next=next_word;
	typo_string punctuation1= S_EMPTY, punctuation2= S_EMPTY, punctuation3= S_EMPTY, punctuation4=S_EMPTY;
	sm.extract_punctuation(currentEl.first.first, punctuation1);
	sm.extract_punctuation(currentEl.first.second, punctuation2);
	sm.extract_punctuation(currentEl.second, punctuation3);
	sm.extract_punctuation(next, punctuation4);

	typo_string to_fix = currentEl.first.second;

	int ret = RET_FSUCCESS;

	//_toFixSyntacticErrors defines weather the algorithm fixes syntactic errors
	if ((false == _toFixSyntacticErrors) && (_pDict->find(to_fix) == RET_FIND_NOT_FOUND)){
		to_fix+=punctuation2;
		cand_list.push_back(ds_pair(D_ZERO, to_fix)); 
		return ret;
	}
	else
	{
		to_fix+=punctuation2;

		//the sequence (w1,w2,w3) is found in NGram dictionary
		if (doGetSuggestions(currentEl, previousElement, next, cand_list) == RET_FIND_EQUAL_CIS) 
		{
			_bNeedsCorrection = false;
			cand_list.erase(cand_list.begin(), cand_list.end());
			cand_list.push_back(ds_pair(D_ZERO, currentEl.first.second));
		}
		//for w2 in (w1,w2,w3) was found a substitution(s) 
		else if (!cand_list.empty())
		{
			if (_filterOutNonSynonyms)
				filterOutNonSynonyms(currentEl.first.second, cand_list);
			sort(cand_list.begin(), cand_list.end(), compare_dspair_SentenceNGramIndex);
			currentEl.first.second = cand_list.front().second;
			
		}
		//not found nor (w1,w2,w3) neither any substitution for w2 
		else
		{ 
			_bNeedsCorrection = true;
			cand_list.push_back(ds_pair(D_ZERO, currentEl.first.second));
			ret = RET_FFAILURE;
		}
	}	

	currentEl.first.first+=punctuation1;
	currentEl.first.second+=punctuation2;
	currentEl.second+=punctuation3;

	Utilities ut;
	ut.factorFrequencies(cand_list);

	return ret;
}


