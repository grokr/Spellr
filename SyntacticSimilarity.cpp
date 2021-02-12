#include "SyntacticSimilarity.h"
#include <algorithm>

SyntacticSimilarity::SyntacticSimilarity(Application *mf): _pSimplePipeBuilder(mf)
{
	readParameters();
}

typo_long_num SyntacticSimilarity::computeSimilarityRatio(const typo_string &to_fix, const typo_string &candidate, 
														  const  typo_long_num &minimum)
{
	if ((to_fix.size() == 0) || (candidate.size() == 0))
		return RET_DZERO;

	int imin = to_fix.size() < candidate.size() ?  to_fix.size() : candidate.size();
	int imax = to_fix.size() > candidate.size() ?  to_fix.size() : candidate.size();
	if (imax - imin > imax/_params.length_difference_coeff)
		return RET_DZERO;

	typo_long_num same = 0;
	typo_string intersection=S_EMPTY;

	int rec_depth = getNumberOfDifferentLetters(to_fix, candidate, intersection);
	if (rec_depth > imin*_params.letterset_difference_coeff) 
		return RET_DZERO;

	countMissingOrSpareDoubleLetters(to_fix, candidate, intersection, same);

	compareLinearly(to_fix, candidate, rec_depth, same);

	typo_long_num res = same*D_HUNDRED/imax;

	typo_long_num common_letterset = hasSingleSpareOrMissingLetter(to_fix, candidate);
	if ((common_letterset > 40) && (common_letterset < 75))
		res += common_letterset*_params.same_letterset_bonus/3;
	if (common_letterset >= 75)
		res += common_letterset*_params.same_letterset_bonus;

	//encounter the position of different letter within a word
	if (_params._middle_part_score == 0.0) //_middle_part_score is computed in linearComparison()->getCommonSubstringBonus()
		return  _params.middle_part_score_zero_coeff*res;
	if (_params._first_part_score/(_params._middle_part_score + _params._ending_part_score + 0.1) > _params.middle_and_ending_too_little)
		return  _params.middle_and_ending_too_little_coeff*res;

	return  res;
}

int SyntacticSimilarity::compareLinearly(const typo_string &to_fix, const typo_string &candidate, 
		const typo_long_num &rec_depth, typo_long_num &same)
{
	unsigned int s1_index=0, s2_index=0;
	int res1 = 0, res2 = 0, equal_susbtring_length = 0;

	while ((s1_index < to_fix.size()) && (s2_index < candidate.size()))
	{	
		equal_susbtring_length = 0;
		while ((to_fix[s1_index]==candidate[s2_index]) && (s1_index < to_fix.size()) && (s2_index < candidate.size())){	
			s1_index++; s2_index++; equal_susbtring_length++;
		}
		if (equal_susbtring_length)
			same+=getCommonSubstringBonus(s1_index, s2_index, to_fix.size(), candidate.size(), equal_susbtring_length);
		skipNonMatchingLetters(s1_index, s2_index, to_fix, candidate, rec_depth);
	}

	return RET_FSUCCESS;
}

//encounter easily made mistake of missing or spare double letter, i.e  abc and abbc or aabc and abc
int SyntacticSimilarity::countMissingOrSpareDoubleLetters(const typo_string &to_fix, const typo_string &candidate, 
							const typo_string& intersection,  typo_long_num &same)
{
	if ( min(candidate.size()-intersection.size(), to_fix.size()-intersection.size()) < 2 )
	{
		if ((candidate.end() != std::adjacent_find(candidate.begin(), candidate.end())) ||
			(to_fix.end() != std::adjacent_find(to_fix.begin(), to_fix.end())))
		{
			int ret= getMissingOrSpareDoublesBonus(to_fix, candidate);
			if (2 == ret) {
				same = to_fix.size()*to_fix.size()*_params.doubles_full_match_bonus;
			}
			else {
				if (1 == ret) {
					same = to_fix.size()*_params.doubles_full_match_bonus;
				}
				else{ 
					if (0 == ret) {
						ret = getMissingOrSpareDoublesBonus(candidate, to_fix);
						if (2 == ret) 
							same = to_fix.size()*to_fix.size()*_params.doubles_full_match_bonus;
						else if (1 == ret) 
							same = to_fix.size()*_params.doubles_full_match_bonus;
						else 
							same = 0;
					}
				}
			}
		}
		//else
		if (0 == same){
			if ( (candidate.size()==intersection.size()) || (to_fix.size()==intersection.size()))
				same = to_fix.size()*_params.same_letterset_bonus;
			else
				same = _params.same_letterset_bonus;
		}
	}

	return RET_FSUCCESS;

}

typo_long_num SyntacticSimilarity::hasSingleSpareOrMissingLetter(const typo_string &s1, const typo_string &s2) //typo_long_num hasSpareOrMissingLetter(const typo_string &s1, const typo_string &s2) 
{
	int indexS1 = 0, indexS2 = 0;
	typo_string tempS1=s1, tempS2=s2;

	if ((s1.size() == 0) || (s2.size() == 0))
		return RET_DZERO;

	int idiff = abs(tempS1.length()-tempS2.length());
	if (idiff > _params.length_difference_coeff)
		return RET_DZERO;

	typo_long_num same = 0;
	typo_long_num imin = min(s1.size(), s2.size());
	typo_long_num imax = max(s1.size(), s2.size());

	for (unsigned int i=0, j=0; (i < s1.size()) && (j < s2.size()); i++, j++)
	{
		if (s1[i]==s2[j]) {
			same++;
			continue;
		}

		if (i != j)
		{
			i=j=min(i,j);
			if (s1[i]==s2[j]){ 
				same++;
				continue;
			}
		}

		if (i + 1 < s1.size()){
			if (s1[i + 1] == s2[j]) {
				same++;
				i++;
				continue;
			}
		}
		if (j + 1 < s2.size()){
			if (s1[i] == s2[j+1]) {
				same++;
				j++;
			}
		}
	}
	return  same*D_HUNDRED/imax;
}


int SyntacticSimilarity::skipNonMatchingLetters( unsigned int& i,  unsigned int& j,const typo_string& to_fix,
												   const typo_string& candidate, const typo_long_num& rec_depth)
{	
	for (int substrLength=_params.substr_length_max; substrLength > 1; substrLength--)
	{
		for (int shift=1; (shift < _params.shift_max) && (i + shift + substrLength <= to_fix.size()); shift++)
		{
			string ss1=to_fix.substr(i+shift, substrLength);
			string ss2=candidate.substr(j, substrLength);
			if (0 == ss1.compare(ss2))
			{
				i += shift;	return substrLength;
			}
		}

		for (int shift=1; (shift < _params.shift_max) && (j + shift + substrLength  <= candidate.size()); shift++)
		{
			string ss1=to_fix.substr(i, substrLength);
			string ss2=candidate.substr(j+shift, substrLength);
			if (0 == ss1.compare(ss2))
			{
				j += shift;		return substrLength;
			}
		}
	}
	j++;i++;
	return 0;
}


int SyntacticSimilarity::getNumberOfDifferentLetters(const typo_string &to_fix, const typo_string &candidate, typo_string& intersection)
{
	typo_string sub_to_fix=to_fix, sub_candidate=candidate;
	sort(sub_to_fix.begin(), sub_to_fix.end()); sort(sub_candidate.begin(), sub_candidate.end());
	set_intersection(sub_to_fix.begin(), sub_to_fix.end(), sub_candidate.begin(), sub_candidate.end(), back_inserter(intersection));
	return max(to_fix.size(), candidate.size())-intersection.size();
}


typo_long_num SyntacticSimilarity::getCommonSubstringBonus(const int& s1_index, const int& s2_index,
															  const int& s1_size, const int& s2_size,const int &equal_subsequence_length)
{
	int bonus = equal_subsequence_length*equal_subsequence_length;

	if (equal_subsequence_length == 0) return D_ZERO;
	if (equal_subsequence_length == 1) return D_TWO;


	if ((equal_subsequence_length == s2_size) || (equal_subsequence_length ==s1_size))
		return _params.full_size_bonus*bonus;

	if (equal_subsequence_length > 1){
		if ((s1_index - equal_subsequence_length < double(s1_size)/3.0) && 
			(s2_index - equal_subsequence_length < double(s2_size)/3.0)){
			_params._first_part_score+=_params.first_part_bonus*bonus;
			return _params.first_part_bonus*bonus;
		}
		if ((s1_index - equal_subsequence_length < double(s1_size)*2/3.0) &&
			(s2_index - equal_subsequence_length < double(s2_size)*2/3.0)){
			_params._middle_part_score+=_params.first_part_bonus*bonus;
			return _params.middle_part_bonus*bonus;
		}
		if ((s1_index - equal_subsequence_length >= double(s1_size)*2/3.0) &&
			(s2_index - equal_subsequence_length >= double(s2_size)*2/3.0)){
			_params._ending_part_score+=_params.ending_bonus*bonus;
			return _params.ending_bonus*bonus;
		}
		else 
			return _params.non_comparable_bonus*bonus;
	}
	return D_ZERO;
}

//WORKS ONLY FOR FULL MATCH : acommodate->accommodate
int SyntacticSimilarity::getMissingOrSpareDoublesBonus(const typo_string& s1, const typo_string& s2)
{
	typo_string reconstructed = s2;
	size_t doublesBonus = 0;

	for (size_t i=0; i< s1.size(); i++){
		string::const_iterator s1_position_of_double_it = std::adjacent_find( s1.begin()+i, s1.end());
		if (s1_position_of_double_it == s1.end()) 
			break; //no double letters in s1

		int s1_position_of_double=s1_position_of_double_it - s1.begin();
		string double_s1_substr = s1.substr(s1_position_of_double, 2);//extract double letter
		
		int s2_position_of_double = reconstructed.find(double_s1_substr, i);
		if (s2_position_of_double != string::npos){i+=2; continue;}//nothing to fix
		s2_position_of_double = reconstructed.find(double_s1_substr[0], i);
		if (s2_position_of_double == string::npos){i+=2; continue;}//single letter in s1 is not there
		if (s2_position_of_double != s1_position_of_double) {i+=2; continue;}//single letter in s2 does not match position of s1
		reconstructed.insert(s2_position_of_double, 1, *s1_position_of_double_it);
		doublesBonus++;
		i+=2;
	}

	if (s1 == reconstructed){
		return 2;
	}
	if (doublesBonus)
		return 1;
	return 0;
	
}

void SyntacticSimilarity::readParameters()
{	
	_params.full_size_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][FULL_SIZE_BONUS].c_str());
	_params.first_part_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][FIRST_PART_BONUS].c_str());
	_params.middle_part_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][MIDDLE_PART_BONUS].c_str());
	_params.ending_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][ENDING_BONUS].c_str());
	_params.non_comparable_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][NON_COMPARABLE_BONUS].c_str());
	_params.letterset_difference_coeff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][LETTERSET_DIFFERENCE_COEFF].c_str());
	_params.same_letterset_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][SAME_LETTERSET_BONUS].c_str());
	_params.shift_max = atoi((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][SHIFT_MAX].c_str());
	_params.substr_length_max = atoi((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][SUBSTR_LENGTH_MAX].c_str());
	_params.length_difference_coeff = atoi((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][LENGTH_DIFFERENCE_COEFF].c_str());
	_params.middle_part_score_zero_coeff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][MIDDLE_PART_SCORE_ZERO_COEFF].c_str());
	_params.middle_and_ending_too_little_coeff = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][MIDDLE_AND_ENDING_TOO_LITTLE_COEFF].c_str());
	_params.middle_and_ending_too_little = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][MIDDLE_AND_ENDING_TOO_LITTLE].c_str());
	_params.doubles_full_match_bonus = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[SYNTACTICSIMILARITYCONSTANTS_SECTION][DOUBLES_FULL_MATCH_BONUS].c_str());
	_params._first_part_score = _params._middle_part_score = _params._ending_part_score=0;
}
