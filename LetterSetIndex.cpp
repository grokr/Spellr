/*! \class LetterSetIndex
 *  \brief LetterSetIndex: A container, implemented as map<typo_string, typo_string>
 *  \brief Words are sorted lexicographically
 *  \brief Performance is gained by 1. sorting words by length(20%)
 *  \author Oxana Rybalko
 *  \version 0.0.3
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */

#include "LetterSetIndex.h"
#include "StringManipulators.h"

#include <fstream>
#include <algorithm>

LetterSetIndex::LetterSetIndex(Application *mf=NULL, const typo_string &d=S_EMPTY, const typo_string &ls=S_EMPTY):
	_pSimplePipeBuilder(mf), _dpath(d), _lspath(ls), _GUID(LETTER_SET_INTERSECTION_LIST), _pDict(NULL)
{
	_MaxDictSize =  atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
	_min_sim = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSETINTERSECTIONCONSTANTS_SECTION][MINSIM].c_str());
	_only_exact_match_counts = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSETINTERSECTIONCONSTANTS_SECTION][ONLY_EXACT_MATCH_COUNTS].c_str());

	load_letterSetIndex();
	
	typo_string compFuncName = S_EMPTY;
	_pSimplePipeBuilder->getComparisonFunctionName(LETTER_SET_INTERSECTION_LIST, compFuncName);
	_pComp = _pSimplePipeBuilder->makeComparisonFunction(compFuncName);
	_pDict = static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
}

bool compare_str(const ds_pair& a, const ds_pair& b) {
    return a.first > b.first;
}

int LetterSetIndex::getSectionBeginning(int size)
{
	map<int, pair<int, int>>::iterator it = length_map.find(size);
	return (*it).second.first;
}

int LetterSetIndex::getSectionEnd(int size)
{
	map<int, pair<int, int>>::iterator it = length_map.find(size);
	return (*it).second.second;
}

int LetterSetIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)
{
	typo_string sorted = to_fix;
	sort(sorted.begin(), sorted.end());
	
	typo_long_num cur_sim = D_ZERO;
	StringManipulators sm;
	int SpellingsSuggestionsListSize = _pSimplePipeBuilder->getSpellingsSuggestionsListSize();

	int u = 0;
	map<typo_string, typo_string>::iterator it = _letterSet.begin();
	bool bExactMatchIsFound = false;
	for(; it != _letterSet.end(); ++it)
	{
		if (sorted == sm.getLeft((*it).first, C_COLON)){
			if (_cm.findIndex(res, (*it).second) == RET_FFAILURE){
				res.push_back(ds_pair(D_HUNDRED, (*it).second));
				if (_only_exact_match_counts)
					break;
			}
		}
//	}
		if (bExactMatchIsFound)
			break; //stop running if there are exact matches and all are found

		/* Re-REFACTORING 22.05.1010 
			Algorithm itself doesn't worth anything, too little performance

		REFACTORING 12.05.1010 
			only_exact_match_counts parameter is deprecated.
			This part is removed since the running of comparison function is not related to the LetterSetIndex algorithm itself
			The normal program usage implies using a pipe, hence instead of this code another
			DictionaryIndex:COMPARISON_FUNCTION should be added to the pipe.
			Sorting a word lexicographically and then running a COMPARISON_FUNCTION does not help anyway
		*/

		if (0 == _only_exact_match_counts){
			if ((cur_sim =(static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(sorted, sm.getLeft((*it).first, C_COLON), cur_sim)) > _min_sim)
			{
				if (_cm.findIndex(res, (*it).second) == RET_FFAILURE){
					res.push_back(ds_pair(cur_sim, (*it).second));
					sort(res.begin(), res.end(), compare_str);				
					if (res.size() >= (size_t)SpellingsSuggestionsListSize)
					{
						res.erase(res.end()-1);
					}
				}
			}
		}
		
	}

	filter.sim(_min_sim);

	vector<ds_pair> out_res;
	vector<ds_pair>::iterator resIt;
	for(resIt=res.begin(); resIt != res.end(); ++resIt){
		_pDict->getLetterIntersectionIndexMapping(atoi((*resIt).second.c_str()), (*resIt).second, (*resIt).first);
		if (_cm.findIndex(out_res, (*resIt).second) == RET_FFAILURE){
			out_res.push_back(ds_pair((*resIt).first, (*resIt).second));
		}
	}

	res=out_res;
	sort(res.begin(), res.end(), compare_str);		

	return RET_FSUCCESS;
}
int LetterSetIndex::load_letterSetIndex()
{
	ifstream datafile;
	datafile.open((_pSimplePipeBuilder->INSTALL_DIR + _lspath).c_str());
	if ( false == datafile.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, (_pSimplePipeBuilder->INSTALL_DIR + _lspath).c_str());	throw runtime_error(MSG_00006);	}

	typo_string word1, word2;
	while (!datafile.eof() && ((int)_letterSet.size() < _MaxDictSize)){
		datafile >> word1; datafile >> word2;
		_letterSet.insert(ss_pair(word1, word2));
	}
	datafile.close();


	return RET_FSUCCESS;
}
int LetterSetIndex::add_word(const typo_string &w, const typo_string &f)
{
	map<typo_string, typo_string>::iterator it=_letterSet.find(w);
	if (it == _letterSet.end()){
		_letterSet.insert(ss_pair(w, f));
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

int LetterSetIndex::remove_word(const typo_string &w, const typo_string &f)
{
	map<typo_string, typo_string>::iterator it=_letterSet.find(w);
	if (it != _letterSet.end()){
		_letterSet.erase(it);
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}
