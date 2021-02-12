/*! \class ISpellChecker
 *  \brief Interface for building interfaces for Spell Checkers
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */
#ifndef __SPELLER_H__
#define __SPELLER_H__

#include "Constants.h"
#include "IAbstractProduct.h"


namespace SpellChecker
{

class TYPODLL_API ISpellChecker: public IAbstractProduct{ 

public: 
	virtual ~ISpellChecker(){}
//	virtual ISpellChecker *QueryInterface()=0;

/** input: a word to fix; 
*   output: a fixed word;
*   return value: 
*		 0 - the word was correct
*		 X - the word was incorrect and was fixed with condifidence X%(1-100)
*	(to implement optionally)
**///virtual int spell_word(typo_string &to_fix){ return 0;}

/** input: a word to fix; 
*   output: a vector of suggestions; 
*   return value: 
*		 0 - the word was correct
*		 1 - word was incorrect
**/virtual int spell_word( typo_string &to_fix, vector<ds_pair> &vector)=0;

/** input:	a file to fix; 
*   output:	a fixed file. a change is performed only if 101% confidence;
*   return value: 
*			number of mistakes not fixed
**/virtual int spell_file(const typo_string &input_file, const typo_string &output_file)=0;

	/** input:	the misspelled and the fixed words to learn from; 
**/

/** add another dictionary(to implement optionally)
*return value: 
*		 0 - the dictionary existed 
*		 1 - the dictionary was added  
**/virtual int add_dictionary(const typo_string &dpath){ return RET_FSUCCESS;}

//virtual typo_long_num compareChar(const char c1, const char c2)const =0;

/**
* The get_dictionary_encoding() function returns the character encoding , i.e. "ISO8859-1" 
**/
virtual void get_dictionary_encoding(typo_string &enc) const=0;

/**
* The get_version() returns the version typo_string of the library. 
**/
virtual void get_version(typo_string &version) const=0;

/**The add(), and remove() are helper functions of a personal dictionary implementation 
* to add and remove words from the base dictionary in run-time. 
**/
virtual int add_word(const typo_string &word)=0;
virtual int remove_word(const typo_string &word)=0; 
virtual int add_correction(const typo_string &word, const typo_string &fix){return RET_FFAILURE;};
virtual int add_correction(const typo_string &file){return RET_FFAILURE;};
virtual int remove_correction(const typo_string &word, const typo_string &fix){return RET_FFAILURE;};


protected:
	ISpellChecker(){}
};

};

#endif //__SPELLER_H__


