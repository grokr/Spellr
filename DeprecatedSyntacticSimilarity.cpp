#include "Constants.h"
#include "SyntacticSimilarity.h"
#include "IOManipulators.h"


#include <algorithm>

SyntacticSimilarity::SyntacticSimilarity(SimplePipeBuilder *mf): _pSimplePipeBuilder(mf)
{

	try{
		IOManipulators iom;
		typo_string install_dir; 
		_pSimplePipeBuilder->getInstallDir(install_dir);
		iom.io_read(install_dir + prefixes_file_name, m_vPrefixes);//(_pSimplePipeBuilder->INSTALL_DIR + _prefixes_path).c_str()
		iom.io_read(install_dir + suffixes_file_name, m_vSuffixes);
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}
	readParameters();
}
void SyntacticSimilarity::readParameters()
{	
	_params.max_length_diff = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][MAX_LENGTH_DIFF].c_str());
	_params.nonvowels_weight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][NONVOWELSWEIGHT].c_str());//0.8;
	_params.misplaced_letter_weight = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MISPLACED_LETTER_WEIGHT].c_str());//0.8;
}


typo_long_num SyntacticSimilarity::computeSimilarityRatio(const typo_string &s1, const typo_string &s2,  const typo_long_num &min_sim) 
{
	int indexS1 = 0, indexS2 = 0;
	typo_string tempS1=s1, tempS2=s2;

	//SIMPLE LEVINSTEIN, LETTER SET INTERSECTION, VOWELS COUNT
	if ((s1.size() == 0) || (s2.size() == 0))
		return RET_DZERO;

	int idiff = abs(tempS1.length()-tempS2.length());
	if (idiff > _params.max_length_diff)
		return RET_DZERO;
	//linear
	typo_long_num same = 0;
//	typo_long_num imin = min(s1.size(), s2.size());
	typo_long_num imax = max(s1.size(), s2.size());


	for (unsigned int i=0, j=0; (i < s1.size()) && (j < s2.size()); i++, j++)
	{
		if (s1[i]==s2[j]) 
			same++;
		else
		{ 
			if (i != j)
			{
				i=j=min(i,j);
				if (s1[i]==s2[j]){ 
					same++;
					if (UC_VOWELS_EN.find(s1[i]) == -1)
						same+=_params.nonvowels_weight;
					continue;
				}
			}

			if (i + 1 < s1.size()){
				if (s1[i + 1] == s2[j]) {
					same++;
					if (UC_VOWELS_EN.find(s1[i + 1]) == -1)
						same+=_params.nonvowels_weight;
					i++;
					continue;
				}
			}
			if (j + 1 < s2.size()){
				if (s1[i] == s2[j+1]) {
					same++;
					if (UC_VOWELS_EN.find(s1[i]) == -1){
						same+=_params.nonvowels_weight;
					}
					j++;
				}
			}
		}
	}

/* DEPRECATED: 14/05/10
see why
LettersMappingRulesList:SyntacticSimilarity
LettersMappingRulesList:LetterSetSimilarity
gives worse score


IF NEEDED, LetterSetSimilarity should be used separetely

	typo_string ss1=s1;
	typo_string ss2=s2;
	typo_string ss3=S_EMPTY;
	sort(ss1.begin(), ss1.end());
	sort(ss2.begin(), ss2.end());
	set_intersection(ss1.begin(), ss1.end(), ss2.begin(), ss2.end(), back_inserter(ss3));

	same += ((typo_long_num)ss3.size())/imax;
	*/

	//	if (same < ss3.size())
	//		same += (ss3.size() - same)*_params.misplaced_letter_weight;

	return  same*D_HUNDRED/imax;
}


/*
typo_long_num SyntacticSimilarity::computeSimilarityRatio(const typo_string &s1, const typo_string &s2,  const typo_long_num &min_sim) 
{
	int iii=0;
	int indexS1 = 0, indexS2 = 0;
	typo_string tempS1=s1, tempS2=s2;

	/*
	//remove prefix, suffix
	vector<typo_string>::iterator prefIt=m_vPrefixes.begin();
	for(; prefIt != m_vPrefixes.end(); ++prefIt){
	indexS1 = s1.find(*prefIt); indexS2 = s2.find(*prefIt);
	if ((indexS1 == 0) && (indexS2 == 0)){
	tempS1 = s1.substr((*prefIt).size(), s1.size()-(*prefIt).size());
	tempS2 = s2.substr((*prefIt).size(), s2.size()-(*prefIt).size());
	break;
	}
	}

	vector<typo_string>::iterator suffIt=m_vSuffixes.begin();
	for(; suffIt != m_vSuffixes.end(); ++suffIt){
	indexS1 = s1.find(*suffIt); indexS2 = s2.find(*suffIt);
	if ((indexS1 > 0) && (indexS2 > 0)){
	tempS1 = s1.substr(0, s1.size()-(*suffIt).size());
	tempS2 = s2.substr(0, s2.size()-(*suffIt).size());
	break;
	}
	}
	* /
	///////////////////
	// LINEAR
	//	int imax = max(tempS1.length(), tempS2.length());
	//	int imin = min(tempS1.length(), tempS2.length());
	//	typo_long_num same = 0;

	/*	for (int i=0; i < imin; i++){
	if (tempS1[i]==tempS2[i]) {
	same++; 
	if (UC_VOWELS_EN.find(tempS1[i]) == -1)
	same+=0.8;//D_NONVOWELSWEIGHT;
	}
	}

	if (same + _params.max_length_diff < imax) 
	return RET_DZERO;* /

	//	return  same*D_HUNDRED/imax;
	///////////////////


	//SIMPLE LEVINSTEIN, LETTER SET INTERSECTION, VOWELS COUNT
	if ((s1.size() == 0) || (s2.size() == 0))
		return RET_DZERO;

	int idiff = abs(tempS1.length()-tempS2.length());
	if (idiff > _params.max_length_diff)
		return RET_DZERO;
	//linear
	typo_long_num same = 0;
	int imin = min(s1.size(), s2.size());
	int imax = max(s1.size(), s2.size());

	for (unsigned int i=0, j=0; (i < s1.size()) && (j < s2.size()); i++, j++)
	{
		if (s1[i]==s2[j]) 
			same++;
		else
		{ 
			if (i != j)
			{
				i=j=min(i,j);
				if (s1[i]==s2[j]){ 
					same++;
					if (UC_VOWELS_EN.find(s1[i]) == -1)
						same+=0.8;//D_NONVOWELSWEIGHT;
					continue;
				}
			}

			if (i + 1 < s1.size()){
				if (s1[i + 1] == s2[j]) {
					same++;
					if (UC_VOWELS_EN.find(s1[i + 1]) == -1)
						same+=0.8;//D_NONVOWELSWEIGHT;
					i++;
					continue;
				}
			}
			if (j + 1 < s2.size()){
				if (s1[i] == s2[j+1]) {
					same++;
					if (UC_VOWELS_EN.find(s1[i]) == -1)
						same+=0.8;//D_NONVOWELSWEIGHT;
					j++;
				}
			}
		}
	}
	//letter set
	typo_string ss1=s1;
	typo_string ss2=s2;
	typo_string ss3=S_EMPTY;
	sort(ss1.begin(), ss1.end());
	sort(ss2.begin(), ss2.end());
	set_intersection(ss1.begin(), ss1.end(), ss2.begin(), ss2.end(), back_inserter(ss3));

	same += ss3.size();
	//	typo_long_num MISPLACED_LETTER_WIGHT = 0.8;
	//	if (same < ss3.size())
	//		same += (ss3.size() - same)*MISPLACED_LETTER_WIGHT;

	return  same*D_HUNDRED/imax;


}

*/