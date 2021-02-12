#include "NGramSimilarity.h"
#include "Constants.h"

#include <algorithm>

NGramSimilarity::NGramSimilarity(SimplePipeBuilder *mf): _pSimplePipeBuilder(mf)
{
	readParameters();
}

typo_long_num NGramSimilarity::computeSimilarityRatio(const typo_string &s1, const typo_string &s2,  
													  const typo_long_num &min_sim) 
{
	if (abs(s1.length()-s2.length()) > _params.max_length_diff)
		return RET_DZERO;
	
	typo_long_num same = 0, diff=0;
	size_t s1_size= s1.size(), s2_size=s2.size();
	size_t min = s1_size < s2_size ? s1_size: s2_size;
	
	for (size_t i = 0; i < min; i++)
	{
		if (s1[i] == s2[i]) { same++; continue; } 
		if (s1[i] == C_STAR)
			same+=_params.ngram_star_value;
//		else
//			diff++;
	}
	
	size_t max = s1_size > s2_size ? s1_size: s2_size;
	
	return  same*D_HUNDRED/(typo_long_num)max;
}

void NGramSimilarity::readParameters()
{	
	_params.ngram_star_value = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][NGRAMSTARVALUE].c_str());
	_params.max_length_diff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][MAX_LENGTH_DIFF].c_str());
}
