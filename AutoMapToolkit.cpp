#include "AutoMapToolkit.h"

AutoMapToolkit::AutoMapToolkit(Application *mf):Speller(mf), _pSimplePipeBuilder(mf), _pIgnore(NULL)
{ 
	_pSpellerImp = _pSimplePipeBuilder->makeSpellCheckerImplementation((*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION][SPELLCHECKERIMPTYPE]);
}

AutoMapToolkit::~AutoMapToolkit(){ 
	delete _pSpellerImp;
}

void AutoMapToolkit::configure(IIgnore *pIgnore){_pIgnore=pIgnore;}
void AutoMapToolkit::addLanguage(const typo_string &in){}
void AutoMapToolkit::deleteDuplicated(const typo_string &in, typo_string &out){}
void AutoMapToolkit::replaceInplace(typo_string &inout){}
void AutoMapToolkit::tokenize(const typo_string &in, const list<typo_string> &out, const typo_string &delim){}
void AutoMapToolkit::splitContractions(const typo_string &in, typo_string &out){}
void AutoMapToolkit::splitHyphenatedTokens(const typo_string &in, typo_string &out){}
void AutoMapToolkit::replaceBy(const typo_string &what, const typo_string &replaceBy, const typo_string &out){}
void AutoMapToolkit::capitalizeToken(typo_string &inout){}
void AutoMapToolkit::capitalizeAllTokens(typo_string &inout){}
void AutoMapToolkit::convertToUpperCase(typo_string &inout){}