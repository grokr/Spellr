/*! \class IStringComparison
 *  \brief Interface for building String Comparison classes
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */
#ifndef __ISTRINGCOMPARISON_H__
#define __ISTRINGCOMPARISON_H__

#include "IAbstractProduct.h"
#include "Constants.h"

namespace SpellChecker
{
	class TYPODLL_API IStringComparison: public IAbstractProduct
	{
		public:
			virtual typo_long_num computeSimilarityRatio(const typo_string &s1, const typo_string &s2, const typo_long_num &min)=0; 
			virtual ~IStringComparison(){}
	};
};
#endif //__ISTRINGCOMPARISON_H__