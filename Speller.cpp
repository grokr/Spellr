#include "Speller.h"
#include "IDictionary.h"
#include "StringManipulators.h"
#include "LogConstants.h"

Speller::Speller(Application *mf): _pSimplePipeBuilder(mf){ 
	_pSpellerImp = _pSimplePipeBuilder->makeSpellCheckerImplementation((*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION][SPELLCHECKERIMPTYPE]);
	if (NULL == _pSpellerImp) throw runtime_error(MSG_00005);
} 

Speller::~Speller(){ 
	delete _pSpellerImp;
	_pSpellerImp=NULL;
}

int Speller::spell_word(typo_string &to_fix, vector<ds_pair> &vector){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->do_spell_word(to_fix, vector);
}

int Speller::spell_file(const typo_string &input_file, const typo_string &output_file){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->do_spell_file(input_file, output_file);
}
/*
int Speller::learn_word(const typo_string &to_fix, const typo_string &fixed){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->do_spell_word(to_fix, vector);
}

int Speller::learn_file(const typo_string &input_file, const typo_string &output_file){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->do_spell_file(input_file, output_file);
}*/

int Speller::add_correction(const typo_string &word, const typo_string &fix){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->add_correction(word, fix);
}

int Speller::add_correction(const typo_string &file){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->add_correction(file);
}

int Speller::remove_correction(const typo_string &word, const typo_string &fix){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->remove_correction(word, fix);
}

int Speller::add_word(const typo_string &word){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->add_word(word);
}

int Speller::remove_word(const typo_string &word){
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->remove_word(word);
}

void Speller::get_version(typo_string &version) const{ 
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->get_version(version);
}

void Speller::get_dictionary_encoding(typo_string &enc) const{ 
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->get_dictionary_encoding(enc);
}

int Speller::add_dictionary(const typo_string &dpath) { 
	return static_cast<ISpellCheckerImp* >(_pSpellerImp)->add_dictionary(dpath);
}

