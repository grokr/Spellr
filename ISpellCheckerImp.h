/*! \class ISpellCheckerImp
 *  \brief Interface for building implementations of Spell Checkers
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */
#ifndef __SPELLERIMP_H__
#define __SPELLERIMP_H__

#include "Constants.h"
#include "IAbstractProduct.h"


namespace SpellChecker
{

	class TYPODLL_API ISpellCheckerImp: public IAbstractProduct{ 

	public: 
		virtual ~ISpellCheckerImp(){}

		/* input: a word to fix; 
		output: a fixed word;
		return value: 
		0 - the word was correct
		X - the word was incorrect and was fixed with condifidence X%(1-100)
		(to implement optionally)
		*/
		//virtual int do_spell_word(typo_string &to_fix){ return RET_FSUCCESS;}

		/* input: a word to fix; 
		output: a vector of suggestions; 
		return value: 
		0 - the word was correct
		1 - word was incorrect
		*/virtual int do_spell_word( typo_string &to_fix, vector<ds_pair> &vector)=0;

		/* input:	a file to fix; 
		output:	a fixed file. a change is performed only if 101% confidence;
		chooses best result between all algorithms
		return value: 
		number of mistakes fixed
		*/
		virtual int do_spell_file(const typo_string &input_file, const typo_string &output_file)=0;

		virtual int add_correction(const typo_string &word, const typo_string &fix){ return RET_FSUCCESS;}
		virtual int add_correction(const typo_string &file){ return RET_FSUCCESS;}
		virtual int remove_correction(const typo_string &word, const typo_string &fix){ return RET_FSUCCESS;}

		virtual void get_version(typo_string &version) const {}

		virtual int add_word(const typo_string &word){ return RET_FSUCCESS;}
		virtual int remove_word(const typo_string &word){ return RET_FSUCCESS;}

		virtual int add_dictionary(const typo_string &dpath){ return RET_FSUCCESS;}
		virtual void get_dictionary_encoding(typo_string &enc) const {}


	protected:
		ISpellCheckerImp(){}

	private:

	};

};

#endif //__SPELLERIMP_H__


