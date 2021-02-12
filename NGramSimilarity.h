#ifndef __NGRAMSIMILARITY_H__
#define __NGRAMSIMILARITY_H__

#include "IStringComparison.h"
#include "LogConstants.h"
#include "SimplePipeBuilder.h"

namespace SpellChecker
{
	class NGramSimilarity : public IStringComparison
	{

	public:

		NGramSimilarity(SimplePipeBuilder *mf);
		virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 
		virtual ~NGramSimilarity(){}

	private:

		struct NGramSimilarityParameters
		{
			typo_long_num	ngram_star_value;
			typo_long_num	max_length_diff;
		};

		void readParameters();

		NGramSimilarityParameters _params;
		SimplePipeBuilder *_pSimplePipeBuilder; 
	};
	
};
#endif //__NGRAMSIMILARITY_H__