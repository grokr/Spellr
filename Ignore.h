#ifndef __IGNORE_H__
#define __IGNORE_H__

#include "IIgnore.h"

namespace SpellChecker
{
	class Ignore : public IIgnore{
	
	public:
Ignore();
		virtual ~Ignore(){}

		virtual void ignoreUppercased();
		virtual void ignoreMixedCase();
		virtual void ignoreContainingNumbers();
		virtual void ignoreSize(const int sz);
		virtual void ignoreFromTo(const typo_string &from, const typo_string &to);
		virtual void ignoreFileNames();
		virtual void ignoreCompanyProductNames();
		virtual void ignoreAbbreviations();
		virtual void addToIgnoreList(const typo_string &in);
		virtual void addToIgnoreList(const typo_char &in);
		virtual void ignoreLanguage(const typo_string &in);
		virtual void ignoreDomain(const typo_string &);
		virtual void ignoreHtml();
		
		
	private:

		unsigned int _bitmap;
		int size;
		typo_string from, to;
	
	protected:

		

	private:

	};	 
};
#endif //__IGNORE_H__