#include "IntelliSpell.h"

IntelliSpell::IntelliSpell(Application *mf):_pSimplePipeBuilder(mf), _pSpeller(NULL), _optionsBitmap(0),
_userInteractionMode(0), _contentType(0), _lang(S_EMPTY), _scope(S_EMPTY){

}

IntelliSpell::~IntelliSpell(){
	delete _pSpeller;
//	delete _pSimplePipeBuilder;
}

int  IntelliSpell::init(){ 
	_pSpeller = _pSimplePipeBuilder->makeSpellChecker();	
	return RET_FSUCCESS;
}
 
int IntelliSpell::spellFile(const typo_string &input_file, const typo_string &output_file){
	if (_pSpeller)
		return static_cast<ISpellChecker* >(_pSpeller)->spell_file(input_file, output_file);
	return RET_FFAILURE;
}
int  IntelliSpell::addWord(const typo_string& w){
	if (_pSpeller)
		return static_cast<ISpellChecker* >(_pSpeller)->add_word(w);
	return RET_FFAILURE;
}
int  IntelliSpell::removeWord(const typo_string& w){
	if (_pSpeller)
		return  static_cast<ISpellChecker* >(_pSpeller)->remove_word(w);
	return RET_FFAILURE;
}
int  IntelliSpell::addDictionary(const typo_string& path){
	if (_pSpeller)
		return static_cast<ISpellChecker* >(_pSpeller)->add_dictionary(path);
	return RET_FFAILURE;
}

int  IntelliSpell::loadDictionary(const typo_string& name){ 	return RET_FFAILURE;}
int  IntelliSpell::unloadDictionary(const typo_string& name){ 	return RET_FFAILURE;}

/** ***************************************
*				CONFIGURATION
*************************************** **/

int  IntelliSpell::setListErrorsInTaskList(){ _optionsBitmap += 1; 	return RET_FSUCCESS;};

// 0 - comments, strings; // 1 - HTML; // 2 - XML
// 3 - cpp; // 4 - java; // 5 - text;	//6 - resource 
int  IntelliSpell::setContentType(const int& type){ 
	_contentType = type; 
	return RET_FSUCCESS;
}

int  IntelliSpell::setLanguage(const typo_string& lang){ //0- file; 1- project; 2 - solution
	_lang = lang;
	return RET_FSUCCESS;
}

int  IntelliSpell::setScope(const int scope){ //0- file; 1- project; 2 - solution
	_scope = scope;
	return RET_FSUCCESS;
}

// 0 - UnderlineMisspelledWords
// 1 - ShowOptionsDialogBox
// 2 - AsYouTypeSpelling
// 3 - ContextMenuWithSuggestions
int  IntelliSpell::setUserInteractionMode(const int mode){
	_userInteractionMode=mode;
	return RET_FSUCCESS;
}
