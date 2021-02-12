#ifndef __LETTERSETSIMILARITY_H__
#define __LETTERSETSIMILARITY_H__

#include "IStringComparison.h"
#include "LogConstants.h"
#include "Application.h"
#include "IOManipulators.h"

namespace SpellChecker
{

	struct LetterSetSimilarityParameters
	{
		typo_long_num	max_length_diff;
	};

	class LetterSetSimilarity : public IStringComparison
	{

	public:
		LetterSetSimilarity(Application *mf);
		virtual ~LetterSetSimilarity(){}
		virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 
	
	private:
	
		Application *_pSimplePipeBuilder; 
		void readParameters();
		typo_long_num compute(const typo_string &s1, const typo_string &s2);
		LetterSetSimilarityParameters _params;
	};
};

#endif //__LETTERSETSIMILARITY_H__

