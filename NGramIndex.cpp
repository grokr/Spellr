/*! \class NGramIndex
 *  \brief NGramIndex typo_string matching algorithm
 *  \author Oxana Rybalko
 *  \version 0.0.1
 *  \date    2009
 *  \bug Not tested.
 *  \bug 
 *  \warning 
 */
#pragma warning (disable:4018)

#include "NGramIndex.h"
#include "DictionaryIndex.h"
#include "LogConstants.h"
#include <algorithm>

NGramIndex::NGramIndex(Application *mf, int n, const typo_string &filename): _pSimplePipeBuilder(mf), 
	_n(n), _filename(filename), _GUID(NGRAM_INDEX), dict(NULL)
{
	typo_string s = _pSimplePipeBuilder->INSTALL_DIR + _filename;
	_filename = s;
	IOManipulators iom;

	try{
		iom.io_read(_filename, _NGram);
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}

	_size= _NGram.size();
	_meaningfulFrequency = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][MEANINGFULFREQUENCY].c_str()); //0.5
	_warp = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][WARP].c_str()); //0.5
	_nGramStarMargin = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][NGRAMSTARMARGIN].c_str()); //0.5

	typo_string compFuncName = S_EMPTY;
	_pSimplePipeBuilder->getComparisonFunctionName(NGRAM_INDEX, compFuncName);
	_pComp = _pSimplePipeBuilder->makeComparisonFunction(compFuncName);

	dict = static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
}

int NGramIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &cand_list, Filter &filter)
{
	typo_string prepared=S_EMPTY;
	typo_long_num sim = filter.sim();
	simple_encode_imporoved(to_fix, prepared, sim);

	if (prepared.find(S_STAR) != RET_FFAILURE)
	{
		if (dict->getSuggestions(prepared, cand_list, filter) != RET_FFAILURE){
			return RET_FSUCCESS;
		}
	}
	return RET_FFAILURE;
}

typo_long_num NGramIndex::sum_of_next_n(vector <typo_long_num>::const_iterator& it)
{
	typo_long_num sum = D_ZERO;  //*it + *(it+1) + *(it+2);
	for (signed int i=0; i < _n; i++){
		sum += *(it+i);
	}
	return sum;
}

typo_long_num NGramIndex::sum_of_probabilities(const vector<typo_long_num>& v)
{
	typo_long_num sum = D_ZERO;
	vector <typo_long_num>::const_iterator it=v.begin();
	for(it = v.begin(); it != v.end(); it++)
		sum+=*it;
	return sum;
}

int NGramIndex::encode(const typo_string &to_fix, typo_string &fixed, typo_long_num &sim)
{
	//init
	Filter f1(sim, _warp, _meaningfulFrequency);
	signed int sz = (int)to_fix.length();
	typo_string to_fix_copy=to_fix;
	fixed.assign(sz, C_STAR);

	typo_long_num median = D_ZERO;

	signed int stop = (signed int)sz - _n; 
	vector<typo_long_num> v;
	int i=0;
	typo_string similar = S_EMPTY;
	v.push_back(getNGramFrequency(C_SHARP + to_fix.substr(i, 2))); 
	for (i = 0; i <= stop; i++){
		typo_long_num f = getNGramFrequency(to_fix.substr(i, _n));
		v.push_back(f); 
		median += f;
	}
	v.push_back(getNGramFrequency(to_fix.substr(i, 2)+ C_SHARP)); 

	fixed = similar = to_fix;

	sim = sum_of_probabilities(v);
	median = (sim - v.back() -  v.front())/(v.size()+1);
	

	{
		vector <typo_long_num>::const_iterator it=v.begin();
		int i = 0;
		for(it=v.begin(); it != v.end(), i < v.size(); ++it, ++i)
		{ 
			if (*it > median)
				continue;
			else
				fixed[i]=C_STAR;  
		}
	}
	return RET_FSUCCESS;
}

int NGramIndex::simple_encode(typo_string &to_fix)
{
	typo_string encoded = to_fix;
	signed int sz = (int)to_fix.length();
	int ret = 0;
	encoded.assign(sz, C_STAR);
	signed int stop = (signed int)sz - _n;
	vector<typo_long_num> freq_vector;
	typo_long_num median = D_ZERO;
	for (signed int i=0; i <= stop; i++){
		typo_long_num f = getNGramFrequency(to_fix.substr(i, _n));
		freq_vector.push_back(f); 
		median += f;
	}
	median = median/freq_vector.size();
	int i=0;
	for(vector<typo_long_num>::iterator it=freq_vector.begin(); it != freq_vector.end(); it++,i++){ 
		if (*it > median){
			encoded[i]=to_fix[i];
			ret++;
		}
	}

	to_fix = encoded;

	return ret;
}

int NGramIndex::simple_encode_imporoved(const typo_string &to_fix, typo_string &fixed, typo_long_num &sim)
{
	if (to_fix.size() < _n) return 0;

	typo_string encoded = to_fix;
	signed int sz = (int)to_fix.length();
	int ret = 0;
	signed int stop = (signed int)sz - _n;

//	litreche 3 1 2 4 1 5 candidates_indexes: 124 lit*e*he
	typo_long_num prev = getNGramFrequency(to_fix.substr(0, _n));
	typo_long_num current = prev;
	for (int i = 1; i <= stop; i++)
	{
		current = getNGramFrequency(to_fix.substr(i, _n));
		if (prev < _nGramStarMargin){
			if (current < _nGramStarMargin)
				encoded[i+1] = C_STAR;
			else
				encoded[i] = C_STAR;
		}
		prev=current;
	}

	fixed = encoded;
	return 1;
}
/**
*return frequency in dictionary for a given ngram
**/
typo_long_num NGramIndex::getNGramFrequency(const typo_string &s) const 
{
	map<typo_string, typo_long_num>::const_iterator it = _NGram.begin();
	it = _NGram.find(s);
	if (it != _NGram.end())
		return ((*it).second)*ONE_HUNDRED_PERCENT/_size;
	return RET_DZERO;
}


/**
*return frequency in dictionary for a similar ngram
**/
typo_long_num NGramIndex::getNGramFrequency(const typo_string &s, typo_long_num sim, typo_string &similar) 
{
	map<typo_string, typo_long_num>::const_iterator it = _NGram.begin();
	it = _NGram.find(s);
	if (it != _NGram.end())
		return (*it).second-ONE_HUNDRED_PERCENT;
	//no exact match, search for similar, substitute s by the similar ngram
	it = findSimilar(s, sim);
	if (it != _NGram.end()){
		similar = (*it).first;
		return (*it).second*ONE_HUNDRED_PERCENT*ONE_HUNDRED_PERCENT/_size;
	}
	return RET_DZERO;
}
 
map<typo_string, typo_long_num>::iterator NGramIndex::findSimilar(const typo_string &s, typo_long_num sim){
	map<typo_string, typo_long_num>::iterator it = _NGram.begin();
	int sz = getSize(); //instead of finalize
	
	for(; it != _NGram.end(); ++it){
		if ((static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(s, (*it).first, sim) > sim*getSize())
			return it;
	}
	return it;
}

