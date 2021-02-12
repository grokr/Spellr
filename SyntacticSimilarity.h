#ifndef __SYNTACTICSIMILARITY_H__
#define __SYNTACTICSIMILARITY_H__

#include "IStringComparison.h"
#include "Application.h"

namespace SpellChecker
{
	struct SyntacticSimilarityParameters
	{
		typo_long_num full_size_bonus;
		typo_long_num first_part_bonus;
		typo_long_num middle_part_bonus;
		typo_long_num ending_bonus;
		typo_long_num non_comparable_bonus;
		typo_long_num letterset_difference_coeff;
		typo_long_num same_letterset_bonus;
		typo_long_num middle_part_score_zero_coeff;
		typo_long_num middle_and_ending_too_little_coeff;
		typo_long_num middle_and_ending_too_little;
		typo_long_num doubles_full_match_bonus;

		int shift_max;
		int substr_length_max;
		int length_difference_coeff;

		typo_long_num _first_part_score;
		typo_long_num _middle_part_score;
		typo_long_num _ending_part_score;
	};

	class SyntacticSimilarity : public IStringComparison
	{
		Application *_pSimplePipeBuilder; 

	public:

		SyntacticSimilarity(Application *mf);
		virtual ~SyntacticSimilarity(){}
		virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 
		typo_long_num TempSyntComputeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 

	private:

		int skipNonMatchingLetters( unsigned int& i,  unsigned int& j, const typo_string& s1, const typo_string& s2, 
			const typo_long_num& rec_depth );
		int getMissingOrSpareDoublesBonus(const typo_string& ss1, const typo_string& ss2);
		typo_long_num getCommonSubstringBonus(const int& s1_index, const int& s2_index, const int& s1_size, 
			const int& s2_size, const int&);
		int getNumberOfDifferentLetters(const typo_string &s1, const typo_string &s2, typo_string& intersection);
		int countMissingOrSpareDoubleLetters(const typo_string &s1, const typo_string &s2, const typo_string& intersection,
			typo_long_num& sum);
		typo_long_num computeLinearSimilarity(const typo_string &s1, const typo_string &s2,  const typo_long_num &min_sim);
		void readParameters();
		typo_long_num hasSingleSpareOrMissingLetter(const typo_string &s1, const typo_string &s2);
		int compareLinearly(const typo_string &to_fix, const typo_string &candidate, 
			const  typo_long_num &rec_depth, typo_long_num &same);

		SyntacticSimilarityParameters _params;

	};

};
#endif //__SYNTACTICSIMILARITY_H__