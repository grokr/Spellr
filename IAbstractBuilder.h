/*! \class IAbstractBuilder
 *  \brief Interface for building product implementations
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning THE DEVELOPER CONTENT FURNISHED UNDER THIS AGREEMENT IS PROVIDED WITHOUT ANY EXPRESS OR 
 *  \warning IMPLIED WARRANTIES OR REPRESENTATIONS AGAINST INFRINGEMENT, AND DEVELOPER SHALL NOT INDEMNIFY CLIENT 
 *  \warning AGAINST INFRINGEMENT OF ANY PATENT, COOPYRIGHTS, TRADE SECRETS, OR OTHER PROPRIETARY RIGHTS.
 */
#ifndef __IABSTRACTBUILDER_H__
#define __IABSTRACTBUILDER_H__

#include "IAbstractProduct.h"
#include "Constants.h"
#include "data_types.h"

namespace SpellChecker 
{

	

class TYPODLL_API IAbstractBuilder 
{
public:
	virtual IAbstractProduct* makeSpellCheckerImplementation(const typo_string &id) = 0;
	virtual IAbstractProduct* makeComparisonFunction(const typo_string &id) = 0;
	virtual IAbstractProduct* makeSpellChecker() = 0;
	virtual IAbstractProduct* loadContainer(const typo_string &id, const typo_string &compFuncName) = 0;
//	virtual IAbstractProduct* getContainer(const typo_string &id, const typo_string &compFuncName) = 0;
	//virtual IAbstractProduct* buildContainer(const typo_string &id, const typo_string &compFuncName);
	virtual void buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type)=0;
	
//	virtual int add_correction(const typo_string &w, const typo_string &f){return RET_FFAILURE;};
//	virtual int add_correction(const typo_string &file){return RET_FFAILURE;};

	virtual ~IAbstractBuilder(){} // needed?
//	IAbstractBuilder(){}
};

};

#endif //__IABSTRACTBUILDER_H__