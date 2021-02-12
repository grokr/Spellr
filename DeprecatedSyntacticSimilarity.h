#ifndef __SYNTACTICSIMILARITY_H__
#define __SYNTACTICSIMILARITY_H__

#include "IStringComparison.h"
#include "SimplePipeBuilder.h"

namespace SpellChecker
{
	struct SyntacticSimilarityParameters{
			typo_long_num nonvowels_weight;
			typo_long_num misplaced_letter_weight;
			typo_long_num	max_length_diff;
	};

	class SyntacticSimilarity : public IStringComparison
	{
		SimplePipeBuilder *_pSimplePipeBuilder; 

	public:

		SyntacticSimilarity(SimplePipeBuilder *mf);
		virtual ~SyntacticSimilarity(){}
		virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 
	
	private:

//		int skipNonMatchingLetters( unsigned int& i,  unsigned int& j, const typo_string& s1, const typo_string& s2, int& rec_depth );
//		int getMissingOrSpareDoublesBonus(const typo_string& ss1, const typo_string& ss2);
//		typo_long_num getCommonSubstringBonus(const int& s1_index, const int& s2_index, const int& s1_size, 
//			const int& s2_size, const int&);
//		int defineRecursionDepth(const typo_string &s1, const typo_string &s2, typo_string& intersection);		
		void readParameters();

		SyntacticSimilarityParameters _params;

//		typo_long_num _first_part_score;
//		typo_long_num _middle_part_score;
//		typo_long_num _ending_part_score;

		vector<typo_string> m_vPrefixes;
		vector<typo_string> m_vSuffixes;
		
		

	};

};
#endif //__SYNTACTICSIMILARITY_H__