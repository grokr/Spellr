/*! \class DictionaryIndex
*  \brief DictionaryIndex simple linear container, implemented as vector <Word>
*  \author Oxana Rybalko
*  \version 0.0.3
*  \date    2009
*  \bug 
*  \bug 
*  \warning 
*/

#include "DictionaryIndex.h"
#include "LogConstants.h"
#include "ContainersManipulators.h"
#include "SpellerTrainer.h" 
#include "NGramIndex.h"

#include <list>
#include <fstream>
#include <algorithm>
#include <sstream>



bool Word::operator==(const Word& w)
{
	if (_data == w._data) 
		return true;
	return false;
}

bool compare(const ds_pair& a, const ds_pair& b) {
	return a.first > b.first;
}

DictionaryIndex::DictionaryIndex(Application *mf, const typo_string &path,
								 const typo_string &compFuncName) : _pSimplePipeBuilder(mf), _changed(false), 
								 _dpath(path), _GUID(DICTIONARY_INDEX), _compFuncName(compFuncName)
{ 
	_dpath = _pSimplePipeBuilder->INSTALL_DIR+_dpath;
	
	readParameters();

	try{
		load_dictionary();
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}

	_pSimplePipeBuilder->getComparisonFunctionName(DICTIONARY_INDEX, _compFuncName);
	_pComp = _pSimplePipeBuilder->makeComparisonFunction(_compFuncName);


	init_cii_dictIndexMap();
}

void DictionaryIndex::readParameters()
{
	_params.toAdjustWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][ADJUSTWEIGHT].c_str()); //0 OR 1
	_params.prevCalcWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][PREVCALCWEIGHT].c_str()); //0.5
	_params.opinionMatters = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][OPINIONMATTERS].c_str()); //0.8
	_params.linearWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][LINEARWEIGHT].c_str()); //0.8
	_params.linearSimilarityMinimalPercentage = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][LINEARLIMIT].c_str()); //50%
	_params.syntSimilarityMinimalPercentage = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SYNTLIMIT].c_str()); //50%
	_params.syntWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SYNTACTICWEIGHT].c_str()); //0.8
	_params.lettersetWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][LETTERSETWEIGHT].c_str()); //0.8
	_params.sameSizeWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SAMESIZEWEIGHT].c_str()); //0.03
	_params.diffSizeWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][DIFFSIZEWEIGHT].c_str()); //0.02
	_params.sameFirstLetter = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SAMEFIRSTLETTER].c_str()); //0.03
	_params.sameLastLetter = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SAMELASTLETTER].c_str()); //0.03
	_params.diffLastLetter = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][DIFFLASTLETTER].c_str()); //0.05
	_params.samePreLastLetter = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SAMEPRELASTLETTER].c_str()); //0.01
	_params.diffPreLastLetter = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][DIFFPRELASTLETTER].c_str()); //0.01
	_params.compFuncForWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][COMPFUNCFORWEIGHT].c_str()); //0.01
	_params.compFuncAgainstWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][COMPFUNCAGAINSTWEIGHT].c_str()); //0.01
	_params.zeroCounterWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][ZEROCOUNTERWEIGHT].c_str()); //0.01
	_params.syntacticSimilarityWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][SYNTACTICSIMILARITYWEIGHT].c_str()); 
	_params.toCalculateSyntacticSimilarity = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][TOCALCULATESYNTACTICSIMILARITY].c_str()); 

	 
	_params.filter_out_length_factor = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][FILTER_OUT_LENGTH_FACTOR].c_str()); 
	_params.dictFreqWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][DICTFREQWEIGHT].c_str()); 
	_params.toCalculateFreqWeight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][TOCALCULATEFREQWEIGHT].c_str()); 
	_params.first_letter_optimization = atoi((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][FIRST_LETTER_OPTIMIZATION].c_str()); 
	_params.most_likable_candidates_number = atoi((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][MOST_LIKABLE_CANDIDATES_NUMBER].c_str());
	_params.minimal_meaningful_substring_length = atoi((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][MINIMAL_MEANINGFUL_SUBSTRING_LENGTH].c_str());
	_params.minimal_compound_word_size = atoi((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][MINIMAL_COMPOUND_WORD_SIZE].c_str());
	_params.spellingsSuggestionsListSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][SPELLINGSSUGGESTIONSLISTSIZE].c_str());
	_params.maxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
	_params.minSim = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MINSIM].c_str());

	_params.countProbabilitiesFromCandidatesList = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSPELLERCONSTANT_SECTIONS][COUNTPROBABILITIESFROMCANDIDATESLIST].c_str()); //0 OR 1
	_params.control_word = (*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][CONTROL_WORD].c_str();
	_params.use_wordnet =  atoi((*_pSimplePipeBuilder->paramSet_inifile)[DICTIONARYINDEXCONSTANTS_SECTION][USE_WORDNET].c_str());

}

DictionaryIndex::~DictionaryIndex()
{ 
	if (_changed)
		save_dictionary(S_NL);

	if (!_dict.empty())
		_dict.erase(_dict.begin(), _dict.end()); 

	if (!_cii_dictIndexMap.empty())
		_cii_dictIndexMap.erase(_cii_dictIndexMap.begin(), _cii_dictIndexMap.end()); 

	//last change
	delete _pComp;
	_pComp = NULL;
}


void DictionaryIndex::init_cii_dictIndexMap()
{
	vector<Word>::const_iterator it=_dict.begin();
	typo_string s = (*it)._data;
	typo_char c = s[0];
	int from = 0; int to=0;
	for(; it != _dict.end(); ++it, ++to)
	{
		if (((*it)._data)[0] == c) continue;
		_cii_dictIndexMap.insert(cii_pair(c, ii_pair(from, to)));
		from = to + 1;
		c = ((*it)._data)[0];
	}
	_cii_dictIndexMap.insert(cii_pair(c, ii_pair(from, to)));
}

vector<Word>::const_iterator DictionaryIndex::getStartIndex(const typo_char& t)
{
	if (_cii_dictIndexMap.find(t) == _cii_dictIndexMap.end())
		return _dict.end();

	if ((*(_cii_dictIndexMap.find(t))).second.first > 0)
		return _dict.begin()+(*(_cii_dictIndexMap.find(t))).second.first-1;
	return _dict.begin()+(*(_cii_dictIndexMap.find(t))).second.first;
}

vector<Word>::const_iterator DictionaryIndex::getEndIndex(const typo_char& t)
{
	if (_cii_dictIndexMap.find(t) == _cii_dictIndexMap.end())
		return _dict.end();

	if ((*(_cii_dictIndexMap.find(t))).second.second > 0)
		return _dict.begin()+(*(_cii_dictIndexMap.find(t))).second.second-1;
	return _dict.begin()+(*(_cii_dictIndexMap.find(t))).second.second;
}

typo_long_num DictionaryIndex::factorFrequency(const typo_long_num& f)
{
	if (f > 10) return 10.0;
	return f;
}
int DictionaryIndex::getSuggestions(const typo_string &to_fix, vector <ds_pair> &res, Filter &filter) 
{
	typo_long_num cur_sim = D_ZERO;
	vector<Word>::const_iterator dictIt=_dict.begin();
	vector<ds_pair>::iterator resIt=res.begin();
	vector<Word>::const_iterator startIt, endIt;
	if (_params.first_letter_optimization){
		startIt = getStartIndex(to_fix[0]);
		endIt =getEndIndex(to_fix[0]);
	}
	else{
		startIt = _dict.begin();
		endIt = _dict.end();
	}
	
#ifdef _DEBUG
	int iii=0;
	if (to_fix == _params.control_word)
		iii=0;
#endif //_DEBUG

	typo_string encoded = to_fix;
	for(vector<Word>::const_iterator dictIt = startIt; dictIt != endIt; ++dictIt)
	{
		if ((cur_sim = (static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(encoded, (*dictIt)._data, cur_sim)) >= (_params.minSim)) //
		{	
			int pos = 0;
			if ((pos = _cm.findIndex(res, (*dictIt)._data)) != RET_FFAILURE){
				res[pos].first=max(res[pos].first, cur_sim);
			}else{
				if (_params.toCalculateFreqWeight) //adjust according to frequency
					cur_sim += (factorFrequency((*dictIt)._freq))*_params.dictFreqWeight;
				
				res.push_back(ds_pair(cur_sim, (*dictIt)._data));

				if (res.size() > (size_t)_params.spellingsSuggestionsListSize)
				{
					sort(res.begin(), res.end(), compare);
					if (res.size() >= (size_t)_params.spellingsSuggestionsListSize*_params.filter_out_length_factor){
						res.erase(res.end() - 1);
					}
				}
			}
		}
	}

	typo_long_num max_list_sim1 = D_ZERO;
	if (false == res.empty())
		max_list_sim1 = ((res.front()).first > (res.back()).first) ? (res.front()).first : (res.back()).first;
	else 
		return RET_FFAILURE;
	filter.sim(max_list_sim1);
	return RET_FSUCCESS;

	//Handle_plural_missing_from_dictionary();


}
/*
int DictionaryIndex::Handle_plural_missing_from_dictionary()
{
	typo_string to_fix_plural = S_EMPTY;
	vector <ds_pair> res2;
	typo_long_num min_list_sim2 = filter.sim();

	if (max_list_sim1 != 1.0){
		if ((to_fix[to_fix.size() - 1] == 's'))
		{
			to_fix_plural = to_fix.substr(0, to_fix.size() - 1);
			for(dictIt=_dict.begin(); dictIt != _dict.end(); ++dictIt){
				if ((cur_sim = (static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(to_fix_plural, (*dictIt)._data, cur_sim)) > min_list_sim2){
					if (res2.size() >= (size_t)SpellingsSuggestionsListSize){
						res2.erase(res2.end()-1);
					}
					for(resIt=res2.begin(); resIt != res2.end(); ++resIt)
						if (cur_sim > (*resIt).first) break;
					res2.insert(resIt, ds_pair(cur_sim, (*dictIt)._data));
					min_list_sim2 = res2.back().first;
				}
			}
		}
	}
	if (to_fix_plural == S_EMPTY){
		filter.sim(min_list_sim1);
	}
	else{
		typo_long_num max_list_sim2 = ((res2.front()).first > (res2.back()).first) ? (res2.front()).first : (res2.back()).first;
		if (max_list_sim1 > max_list_sim2)
			filter.sim(min_list_sim1);
		else{
			filter.sim(min_list_sim2);
			res = res2;
		}
		//res.erase(res.begin(), res.end());
		//;
	}
	return RET_FSUCCESS;

}*/

int DictionaryIndex::add_word(const Word &w)
{
	vector<Word>::iterator dictIt=_dict.begin();
	for(dictIt=_dict.begin(); dictIt != _dict.end(); ++dictIt) {
		if ((*dictIt)._data == w._data) 
			return RET_FSUCCESS; 
	}
	_dict.push_back(w);
	_changed = true;
	return RET_FSUCCESS;
}

int DictionaryIndex::remove_word(const Word &word)
{
	if (_cm.remove<Word>(_dict, word)){
		_changed=true;
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

int DictionaryIndex::add_correction(const typo_string &w, const typo_string &f)
{
	return RET_FFAILURE;
}

int DictionaryIndex::add_correction(const typo_string &filename)
{
	typo_string misspelled;
	typo_string fixed, in;
	ifstream is;
	is.open(filename.c_str());
	typo_string word=S_EMPTY;
	while (!is.eof()){
		is >> in;
		_sm.parseMappings(in, C_COMA, misspelled, fixed);
		add_correction(misspelled, fixed);
	}
	is.close();

	return RET_FSUCCESS;
}

int DictionaryIndex::stop_if_found(vector <ds_pair>& res)
{
	int found_sim=atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][EXISTING_WORD_BONUS].c_str());
	if ((*(res.begin())).first >= found_sim)
	{
		vector <ds_pair> new_res;
		vector<ds_pair>::iterator resIt=res.begin();
		for(; (resIt != res.end()); ++resIt){
			if ((*(resIt)).first < found_sim){
				res=new_res;
				return RET_FSUCCESS;
			}
			new_res.push_back(*resIt);
		}
		res=new_res;
		return RET_FSUCCESS;
	}
	return RET_FSUCCESS;
}

int DictionaryIndex::filterOutSuggestions(const typo_string &to_fix, vector <ds_pair> &res, Filter &filter) 
{
	vector<ds_pair>::iterator resIt=res.begin();
	vector<ds_pair> out_cand_list;

	//two first identical case(improves numbers, don't know why), to be refactored
	int TopSim = 100;//it is zero for some reason: _pSimplePipeBuilder->_TopSim;

	sort(res.begin(), res.end(), compare);

	if (_params.countProbabilitiesFromCandidatesList){
		if (to_fix.size() > _params.minimal_compound_word_size) 
			countProbabilitiesFromCandidatesList(res);
	}

	int i=0;
	if (_params.toAdjustWeight){
		for(; (resIt != res.end()) && (i++ < _params.spellingsSuggestionsListSize); ++resIt){
			adjustWeight(to_fix, *resIt);
			if ((*resIt).first < _params.minSim)
				continue;
			int pos=0;
			if (RET_FFAILURE == (pos=(_cm.findIndex(out_cand_list, (*resIt).second))))
				out_cand_list.push_back(ds_pair((*resIt).first, (*resIt).second));
			else
				(*(out_cand_list.begin()+pos)).first += (*resIt).first;
		}
		res = out_cand_list;
	}

	typo_long_num max_list_sim1 = D_ZERO;
	if (false == res.empty())
		max_list_sim1 = ((res.front()).first > (res.back()).first) ? (res.front()).first : (res.back()).first;
	filter.sim(max_list_sim1);
	return RET_FSUCCESS;
}

void DictionaryIndex::countProbabilitiesFromCandidatesList(vector <ds_pair> &res)
{
	if (res.empty()) return;
	const typo_long_num PROBABLE_CANDIDATE_GRADE = 0.7;

	vector <ds_pair> new_res;
	vector<ds_pair>::iterator resIt=res.begin();
	int i=0;
	int same_semantics = 0;
	typo_string word_beginning = ((*resIt).second).substr(0, _params.minimal_meaningful_substring_length);
	for(; (resIt != res.end()) && (i++ < _params.most_likable_candidates_number); ++resIt){
		typo_string next_word_beginning = ((*resIt).second).substr(0, _params.minimal_meaningful_substring_length);
		if (word_beginning == next_word_beginning) 
			same_semantics++;
	}
	if (same_semantics > PROBABLE_CANDIDATE_GRADE*_params.most_likable_candidates_number){
		for(resIt=res.begin(); (resIt != res.end()); ++resIt){
			typo_string next_word_beginning = ((*resIt).second).substr(0, _params.minimal_meaningful_substring_length);
			if (word_beginning == next_word_beginning)
				new_res.push_back(*resIt);
		}
		res=new_res;
	}
	
}
void DictionaryIndex::adjustWeight(const typo_string& to_fix, ds_pair& cand)
{
	if (0 == cand.second.size()) return; 

	//TODO: MOVE TO Filter?
	typo_long_num cur_sim = cand.first*_params.prevCalcWeight; 
	typo_long_num pm_opinion = D_ZERO, ls_opinion = D_ZERO, mf_opinion = D_ZERO, 
		lts_opinion = D_ZERO, ss_opinion = D_ZERO, led_opinion = D_ZERO;
	int zero_counter = 0;

	if (_params.toCalculateSyntacticSimilarity) //levinstein distance
		cur_sim += _params.syntacticSimilarityWeight*(static_cast<IStringComparison* >(_pSimplePipeBuilder->makeComparisonFunction(SYNTACTIC_SIMILARITY))->computeSimilarityRatio(to_fix, cand.second, cur_sim));


	ss_opinion = static_cast<IStringComparison* >(_pSimplePipeBuilder->makeComparisonFunction(SYNTACTIC_SIMILARITY))->computeSimilarityRatio(to_fix, cand.second, cur_sim);
	if ((ls_opinion = (static_cast<IStringComparison* >(_pSimplePipeBuilder->makeComparisonFunction(LINEAR_SIMILARITY))->computeSimilarityRatio(to_fix, cand.second, cur_sim))) >= _params.opinionMatters)
	{
		if (ls_opinion < _params.linearSimilarityMinimalPercentage)
		{
			if (ss_opinion < _params.syntSimilarityMinimalPercentage)
			{
				cur_sim -= (_params.syntSimilarityMinimalPercentage - ss_opinion)*_params.syntWeight;
			}
			if (ss_opinion < ls_opinion)
				cur_sim -= ls_opinion*_params.linearWeight;
			else 
			{
				if(ls_opinion > _params.linearSimilarityMinimalPercentage/2)
					cur_sim += ss_opinion*_params.syntWeight;
			}
		}
		else
			cur_sim += ls_opinion*_params.linearWeight;	
	}

	cur_sim += _params.syntWeight*ss_opinion;

	int cand_size = cand.second.size();
	int to_fix_size = to_fix.size();

	if (cand_size ==  to_fix_size)	
		cur_sim += cur_sim*_params.sameSizeWeight; 
	else {
		int diff = cand_size - to_fix_size;
		if (diff > 50) diff = to_fix_size - cand_size;
		cur_sim -= cur_sim*diff*_params.diffSizeWeight;
	}

	if ( cand.second[0] ==  to_fix[0])	
		cur_sim += cur_sim*_params.sameFirstLetter; 
	else 
		cur_sim -= cur_sim*_params.sameFirstLetter;

	if ( cand.second[cand_size - 1] ==  to_fix[to_fix_size - 1])	
		cur_sim += cur_sim*_params.sameLastLetter; 
	else {
		cur_sim -= cur_sim*_params.diffLastLetter;
		if (cand.second[cand_size - 1] == 's')
			cur_sim -= cur_sim*_params.diffLastLetter*_params.compFuncForWeight;
	}
	if ((cand_size > 2) && (to_fix_size >= 2)){
		if ( cand.second[cand_size - 2] ==  to_fix[to_fix_size - 2]) cur_sim += cur_sim*_params.samePreLastLetter; 
		else cur_sim -= cur_sim*_params.diffPreLastLetter;
	}

	cand.first = cur_sim/D_HUNDRED;
}


void DictionaryIndex::getLetterIntersectionIndexMapping(int index, typo_string &s, typo_long_num &freq)
{
	s = (*(_dict.begin()+index))._data;
	freq = (freq*(*(_dict.begin()+index))._freq);
}

int DictionaryIndex::find(typo_string &word) //quick_find
{
	typo_char c = word[0];
	vector<Word>::const_iterator startIt = getStartIndex(c);
	vector<Word>::const_iterator endIt =getEndIndex(c);

	if (startIt == _dict.end())
		startIt = _dict.begin();

	typo_string x = word;
	typo_string y = S_EMPTY;
	for(vector<Word>::const_iterator it = startIt; it != endIt; ++it){
		y = (*it)._data;
		if (x == y){ 
			if (word == (*it)._data) return RET_FIND_EQUAL_CIS; 
			else { 
				word = (*it)._data; //FIX CASE ERROR
				return RET_FIND_EQUAL_CIS; 
			}
		}
	}

	if (_params.use_wordnet)
	{
		int res = _wn.findWord((char *)(word.c_str()));
		if (0 == res)
			return RET_FIND_NOT_FOUND; 

		return RET_FIND_EQUAL_CIS;
	}
	return RET_FIND_NOT_FOUND; 
}

int DictionaryIndex::load_dictionary()
{
	ifstream datafile;
	datafile.open(_dpath.c_str());
	if ( false == datafile.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, _dpath.c_str());	throw runtime_error(MSG_00006);	}

	typo_string in, word, sfreq;
	typo_long_num freq = D_ZERO;

	while (!datafile.eof() && ((int)_dict.size() < _params.maxDictSize)){
		datafile >> in;
		word = _sm.getLeft(in, C_SEMICOLON);
		sfreq = _sm.getRight(in, C_SEMICOLON);
		stringstream ssfreq(sfreq);
		ssfreq>>freq;
		Word w(word, freq);	
		_dict.push_back(w);
	}
	datafile.close();
	return RET_FSUCCESS;
}

int DictionaryIndex::save_dictionary(const typo_string &delim)
{
	typo_string	filename = _pSimplePipeBuilder->INSTALL_DIR + S_BACK_SLASH + _dpath;
	ofstream os;
	os.open(filename.c_str());
	if (!os.good()){		
		_pSimplePipeBuilder->getLog()->report(MSG_00009, filename.c_str());
		return RET_FFAILURE;	
	}
	vector<Word>::const_iterator it = _dict.begin();  
	for (it = _dict.begin(); it != _dict.end(); it++){
		os << (*it)._data; 
		os << delim; 
		os << (*it)._freq;
		os << S_NL;
	}
	os.close();
	return RET_FSUCCESS;
}
