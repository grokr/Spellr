#ifndef __GENERALSC_H__
#define __GENERALSC_H__

#include "ISpellChecker.h"
#include "ISpellCheckerImp.h"
#include "Application.h"

namespace SpellChecker
{

class Speller : public ISpellChecker{ 

public:

	Speller(Application *mf);
	virtual ~Speller();

	virtual int spell_word( typo_string &to_fix, vector<ds_pair> &vector);
	virtual int spell_file(const typo_string &input_file, const typo_string &output_file);


/** add another dictionary(to implement optionally)
*return value: 
*		 0 - the dictionary existed 
*		 1 - the dictionary was added  
**/	virtual int add_dictionary(const typo_string &dpath);

/**
* The get_dictionary_encoding() function returns the character encoding , i.e. "ISO8859-1" 
**/	virtual void get_dictionary_encoding(typo_string &enc) const;

/**
* The get_version() returns the version typo_string of the library. 
**/	virtual void get_version(typo_string &version) const;

/**
* The add_word(), remove_word() allow user to fine-tune the dictionary at run-time
**/	virtual int add_word(const typo_string &word);
	virtual int remove_word(const typo_string &word);

/**
* The add_correction(), remove_correction() functions allow user to fine-tune the correction list at run-time
**/	virtual int add_correction(const typo_string &word, const typo_string &fix);
	virtual int add_correction(const typo_string &file);
	virtual int remove_correction(const typo_string &word, const typo_string &fix);

protected:

//	ISpellCheckerImp *getSpellerImp(const typo_string &id);
private:

	Speller(const Speller &c){}
	IAbstractProduct *_pSpellerImp;
	Application *_pSimplePipeBuilder;
};

};
#endif //__GENERALSC_H__