#ifndef __ABSTRACTAPPLICATIONINTERFACE_H__
#define __ABSTRACTAPPLICATIONINTERFACE_H__

#include "IAbstractProduct.h"
#include "Constants.h"

namespace SpellChecker 
{

class TYPODLL_API AbstractApplicationInterface 
{
public:
	virtual IAbstractProduct* makeSpellChecker() = 0;
	virtual void buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type)=0;
	
	virtual ~AbstractApplicationInterface(){} 
};

};

#endif //__ABSTRACTAPPLICATIONINTERFACE_H__