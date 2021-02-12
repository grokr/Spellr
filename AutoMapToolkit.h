#ifndef __AUTOMAPTOOLKIT_H__
#define __AUTOMAPTOOLKIT_H__

#include "IAbstractProduct.h"
#include "IIgnore.h"
#include "ISpellCheckerImp.h"
#include "Speller.h"
#include "Application.h"

#include <list>

namespace SpellChecker
{

class AutoMapToolkit : public Speller{ 
///use it by decorator pattern

public:
	AutoMapToolkit(Application *mf);
	virtual ~AutoMapToolkit();

	void configure(IIgnore *pIgnore);
	void addLanguage(const typo_string &in);
	void deleteDuplicated(const typo_string &in, typo_string &out);
	void replaceInplace(typo_string &inout);
	void tokenize(const typo_string &in, const list<typo_string> &out, const typo_string &delim);
	void splitContractions(const typo_string &in, typo_string &out);
	void splitHyphenatedTokens(const typo_string &in, typo_string &out);
	void replaceBy(const typo_string &what, const typo_string &replaceBy, const typo_string &out);
	void capitalizeToken(typo_string &inout);
	void capitalizeAllTokens(typo_string &inout);
	void convertToUpperCase(typo_string &inout);

	virtual int add_dictionary(const typo_string &dpath){return RET_FSUCCESS;}
	virtual void get_dictionary_encoding(typo_string &enc) const{}

protected:
	
private:


	IAbstractProduct *_pSpellerImp;
	IIgnore *_pIgnore;//use decorator pattern
	Application *_pSimplePipeBuilder;
};

};
#endif //__AUTOMAPTOOLKIT_H__