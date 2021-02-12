#ifndef __IDICTIONARYBUILDER_H__
#define __IDICTIONARYBUILDER_H__

/*! \class IDictionaryBuilder
 *  \brief Interface for building indexed words list
 *  \author Oxana Rybalko
 *  \version 0.0.1
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */

#include "data_types.h"

namespace SpellChecker
{
	class TYPODLL_API IDictionaryBuilder
	{
	public:
		virtual int build()=0;
		virtual ~IDictionaryBuilder(){}
	};
};
#endif //__IDICTIONARYBUILDER_H__

