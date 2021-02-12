/*! \class IIgnore
 *  \brief Interface for building classes supporting dismissing of certain kinds of inputs 
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug Not tested.
 *  \bug 
 *  \warning 
 */
#ifndef __IIGNORE_H__
#define __IIGNORE_H__

#include "Constants.h"
#include "IAbstractProduct.h"

namespace SpellChecker
{
	class TYPODLL_API IIgnore: public IAbstractProduct{
	///use it by decorator pattern
	public:

		virtual ~IIgnore(){}
		virtual void ignoreUppercased()=0;
		virtual void ignoreMixedCase()=0;
		virtual void ignoreContainingNumbers()=0;
		virtual void ignoreSize(const int sz)=0;
		virtual void ignoreFromTo(const typo_string &from, const typo_string &to)=0;
		virtual void ignoreFileNames()=0;
		virtual void ignoreCompanyProductNames()=0;
		virtual void ignoreAbbreviations()=0;
		virtual void addToIgnoreList(const typo_string &in)=0;
		virtual void addToIgnoreList(const typo_char &in)=0;
		virtual void ignoreLanguage(const typo_string &in)=0;
		virtual void ignoreDomain(const typo_string &domain)=0;
		virtual void ignoreHtml()=0;
		
	};	 
};

#endif //__IIGNORE_H__