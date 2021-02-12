#ifndef __LINEARSIMILARITY_H__
#define __LINEARSIMILARITY_H__

#include "IStringComparison.h"
#include "LogConstants.h"
#include "Application.h"

#include "IOManipulators.h"

namespace SpellChecker
{
	struct LinearSimilarityParameters
	{
		typo_long_num	max_length_diff;
		typo_long_num	ngram_star_value;
		typo_long_num	min_sim_ratio;
	};

	class LinearSimilarity : public IStringComparison
	{
	public:
		LinearSimilarity(Application *mf);
		virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min); 
		virtual ~LinearSimilarity(){}
	private:
		Application *_pSimplePipeBuilder; 

		void readParameters();
		LinearSimilarityParameters _params;
	};


	
};
#endif //__LINEARSIMILARITY_H__