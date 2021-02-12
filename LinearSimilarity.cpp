#include "LinearSimilarity.h"

#include <algorithm>

LinearSimilarity::LinearSimilarity(Application *mf): _pSimplePipeBuilder(mf)
{
	readParameters();
}

typo_long_num LinearSimilarity::computeSimilarityRatio(const typo_string &s1, const typo_string &s2,  const typo_long_num &min_sim) 
{

	typo_long_num min =  s1.length() < s2.length() ?  s1.length(): s2.length();
	typo_long_num max =  s1.length() > s2.length() ?  s1.length(): s2.length();
	if (max - min > _params.max_length_diff)
		return RET_DZERO;

	typo_long_num same = 0.0;
	for (int i=0; i < min; i++){
		if (s1[i]==s2[i]) {
			same++; continue;
		}
	}
	
	if (same < max*_params.min_sim_ratio)
		return  same*D_HUNDRED/max;
	typo_string ss1=s1;
	typo_string ss2=s2;
	typo_string ss3=S_EMPTY;
	sort(ss1.begin(), ss1.end());
	sort(ss2.begin(), ss2.end());
	set_intersection(ss1.begin(), ss1.end(), ss2.begin(), ss2.end(), back_inserter(ss3));

	typo_long_num inters_sim = D_HUNDRED*(typo_long_num)ss3.size()/max;

		return inters_sim + same*D_HUNDRED/max;
}

void LinearSimilarity::readParameters()
{	
	_params.ngram_star_value = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][NGRAMSTARVALUE].c_str());
	_params.max_length_diff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAX_LENGTH_DIFF].c_str());
	_params.min_sim_ratio = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MIN_SIM_RATIO].c_str());
}
