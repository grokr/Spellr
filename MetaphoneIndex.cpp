/*! \class MetaphoneIndex
 *  \brief MetaphoneIndex: A container, implemented as vector <ss_pair>
 *  \brief Words are sorted lexicographically
 *  \brief Performance is gained by 1. sorting words by length(20%)
 *  \author Oxana Rybalko
 *  \version 0.0.3
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */

#include "MetaphoneIndex.h"
#include "metaphone.h"
#include "Constants.h"
#include "data_types.h"
#include "SpellerImp.h"
#include "StringComparisonFacade.h"
#include "StringManipulators.h"

#include <fstream>
#include <list>

MetaphoneIndex::MetaphoneIndex(SimplePipeBuilder *mf, const typo_string &d,   
	const typo_string &m):_pSimplePipeBuilder(mf), dpath(d), mpath(m), _GUID(METAPHONE_INDEX)
{
	_MaxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS][MAXDICTSIZE].c_str());
	try{
		load_metaphone_map();
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}
	typo_string f = S_EMPTY;
	_pSimplePipeBuilder->getComparisonFunction(METAPHONE_INDEX, f);
	if (f != S_EMPTY)
		_pComp = _pSimplePipeBuilder->makeComparisonFunction(f);
	else
		_pComp = _pSimplePipeBuilder->makeComparisonFunction((*_pSimplePipeBuilder->username_inifile)[CONTAINER_TO_COMPARISON_MAP_SECTION][METAPHONE_INDEX]);

/** 
* try to fix error and keep program runnning instead of throwing an exception 
**/
	if (NULL == _pComp){  /** instead of reporting error log it and keep going**/
		_pSimplePipeBuilder->getLog()->report(MSG_00010);
		_pComp = _pSimplePipeBuilder->makeComparisonFunction(LINEAR_SIMILARITY);
	}
	if (NULL == _pComp){ /** instead of reporting error log it and keep going**/
		_pSimplePipeBuilder->getLog()->report(MSG_00011);
		_pComp = _pSimplePipeBuilder->makeComparisonFunction(PMILLER_SIMILARITY);
	}
	_TopSim = _pSimplePipeBuilder->_TopSim;
}

int MetaphoneIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &vds, Filter &filter)
{
	if (NULL == _pComp){  /** instead of throwing and exception log it and keep going**/
		_pSimplePipeBuilder->getLog()->report(MSG_00012);
		return RET_FFAILURE;  
	} 
	
	list<ds_pair> lds;
	int vsz = vds.size();
	lds.resize(vsz);
	copy(vds.begin(), vds.end(), lds.begin());

	typo_string s_phon;
	typo_string lang = ISO639_1_EN;
	_pSimplePipeBuilder->getCurrentInputLanguage(lang);
	(static_cast<MetaphoneSimilarity* >(_pComp))->phoneticize(to_fix, s_phon, lang);

	typo_long_num cur_sim = D_ZERO;
	vector<ss_pair>::const_iterator it=_vss.begin();
	typo_long_num min_sim = filter.sim();
	
	int SpellingsSuggestionsListSize = _pSimplePipeBuilder->getSpellingsSuggestionsListSize();

	for(; it != _vss.end(); ++it){

		if ((cur_sim = (static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(s_phon, (*it).second, min_sim)) > min_sim){
		
			if (cur_sim >= _TopSim){
				vds.insert(vds.begin(),ds_pair(cur_sim, (*it).first));
				return RET_FSUCCESS;
			}

			lds.insert(lds.end(), ds_pair(cur_sim, (*it).first));
			lds.sort();
			min_sim = lds.front().first;
			if(lds.size() > (size_t)SpellingsSuggestionsListSize) 
				lds.erase(lds.begin());
		}
	}
	/* populate output vector<ds_pair> vds with (sim, fixed) */
	int sz = lds.size();
	vds.resize(lds.size());
	copy(lds.begin(), lds.end(), vds.begin());

//	list<ds_pair>::iterator it_cand=lds.begin();
//	for(; it_cand != lds.end(); ++it_cand){
//		vds.push_back(*it_cand); 
//	}
//	top_sim = vds.empty() ? 0.0 : res.back().first;
//	return 0;

	filter.sim(vds.empty() ? D_ZERO : vds.back().first);
	return RET_FSUCCESS;
}

int MetaphoneIndex::load_metaphone_map()
{
	ifstream dfile;
	dfile.open((_pSimplePipeBuilder->INSTALL_DIR + dpath).c_str());
	if ( false == dfile.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00013, (_pSimplePipeBuilder->INSTALL_DIR + dpath).c_str());	throw runtime_error(MSG_00006);	}
	ifstream mfile;
	mfile.open((_pSimplePipeBuilder->INSTALL_DIR + mpath).c_str());
	if ( false == mfile.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00013, (_pSimplePipeBuilder->INSTALL_DIR + mpath).c_str());	throw runtime_error(MSG_00006);	}	

	typo_string word; typo_string mphone;
	
	while (!dfile.eof() && !mfile.eof() && ((int)_vss.size() < _MaxDictSize)){
		dfile >> word;
		getline(mfile, mphone);
		typo_string left = mphone;
		typo_string right = S_EMPTY;
		int lstrip = left.find(S_SPACE);
		if (lstrip != typo_string::npos){
			left = left.substr(0, lstrip);
			right = left.substr(lstrip+1, left.size());
			_vss.push_back(ss_pair(word, left));
			_vss.push_back(ss_pair(word, right));
		}
		else
			_vss.push_back(ss_pair(word, left));
	}
	dfile.close();
	mfile.close();
	return RET_FSUCCESS;
}

/*
int SpellerImp::run_Metaphone(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim){
	typo_string s_phon;
	MetaphoneSimilarity cmsr;
	cmsr.phoneticize(to_fix, s_phon);
	Filter f0(top_sim);
	_metaph_map->getSuggestions(s_phon, cand_list, f0); 
	top_sim = f0.sim();
	if (top_sim >= AlgConstantsSection::_TopSim) return RET_FSUCCESS;
	return 0;
}
*/



/*
int MetaphoneIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter,
						  computeSimilarityRatioType comp_f)
{ 
//int MetaphoneIndex::getSuggestions(const typo_string &to_fix, vector<ds_pair> &vds, typo_long_num &top_sim,
//					   computeSimilarityRatioType comp_f) //ContainersInterface::computeLinearSimilarityRatio
//{
//	top_sim = filter.sim();
//	typo_long_num metaph_sim = D_ZERO;
	//	cout << MSG0001 << to_fix << "\tmetaphone:\t" << s_phon << endl;
	//create vector of candidates lds, save their indexes
	list<ds_pair> lds;
	typo_long_num aver_sim = 0.0; typo_long_num cur_sim = 0.0;
	vector<ss_pair>::const_iterator it=_metaph_map._vss.begin();
	int TopSim = _pSimplePipeBuilder->_TopSim;
	for(; it != _metaph_map._vss.end(); ++it){

		typo_long_num min_sim = MinSim;
		//if ((cur_sim = (*comp_f)(s_phon, (*it).second, min_sim)) > min_sim){
		if ((cur_sim = (*comp_f)(s_phon, (*it).second, min_sim)) > min_sim){
		

			if (cur_sim >= TopSim){
				res.insert(res.begin(),ds_pair(cur_sim, (*it).first));
				return RET_FSUCCESS;
			}

			lds.insert(lds.begin(), ds_pair(cur_sim, (*it).first));
			lds.sort();
			min_sim = lds.front().first;
			if(lds.size() > (size_t)AlgConstantsSection::_SpellingsSuggestionsListSize) 
				lds.erase(lds.begin());
			//cout << "candidate metaphone: " << c.metaphone << "\tsimilarity:\t" << c.similarity << "\tindex:\t" << c.index << endl;
		}
	}
	/* populate output vector<ds_pair> res with (sim, fixed) * /
	list<ds_pair>::iterator it_cand=lds.begin();
	for(; it_cand != lds.end(); ++it_cand){
		res.push_back(*it_cand); 
	}

	filter.sim(res.empty() ? 0.0 : res.back().first);
	return 0;
}
*/