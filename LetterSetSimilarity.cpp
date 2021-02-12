#include "LetterSetSimilarity.h"
#include "LetterSetIndex.h"
#include "Constants.h"
#include <algorithm>

LetterSetSimilarity::LetterSetSimilarity(SimplePipeBuilder *mf): _pSimplePipeBuilder(mf)
{
	readParameters();
}

typo_long_num LetterSetSimilarity::computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const  typo_long_num &minimum)
{
	if (abs(s1.length()-s2.length()) > _params.max_length_diff)
		return RET_DZERO;

	typo_string ss1=s1;
	typo_string ss2=s2;
	typo_string ss3=S_EMPTY;
	sort(ss1.begin(), ss1.end());
	sort(ss2.begin(), ss2.end());
	set_intersection(ss1.begin(), ss1.end(), ss2.begin(), ss2.end(), back_inserter(ss3));
	return (D_HUNDRED*(typo_long_num)ss3.size())/max(s1.size(), s2.size());
}

void LetterSetSimilarity::readParameters()
{	
	_params.max_length_diff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][MAX_LENGTH_DIFF].c_str());
}

