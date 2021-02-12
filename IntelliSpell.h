#ifndef __INTELLISPELL_H__
#define __INTELLISPELL_H__

#include "IAbstractProduct.h"
#include "ISpellChecker.h"
#include "Application.h"

namespace SpellChecker
{
	class IntelliSpell : public IAbstractProduct{

	public:

		IntelliSpell(Application *mf);
		virtual ~IntelliSpell();

		virtual int spellFile(const typo_string &input_file, const typo_string &output_file);

		virtual int init();
		virtual int addWord(const typo_string& w);
		virtual int removeWord(const typo_string& w);
		virtual int addDictionary(const typo_string& path);
		virtual int loadDictionary(const typo_string& name);
		virtual int unloadDictionary(const typo_string& name);

		/** ***************************************
		*				CONFIGURATION
		*************************************** **/

		virtual int setListErrorsInTaskList();
		virtual int setContentType(const int& type);
		virtual int setLanguage(const typo_string& lang);
		virtual int setScope(const int scope); 
		virtual int setUserInteractionMode(const int mode);
		
	private:
		IAbstractProduct *_pSpeller;
		Application *_pSimplePipeBuilder;
		
		unsigned int _optionsBitmap;
		
		int _userInteractionMode, _contentType;
		typo_string _lang, _scope;

	private:

	};	 
};
#endif //__INTELLISPELL_H__