#ifndef __DICTIONARYBUILDER_H__
#define __DICTIONARYBUILDER_H__

#include "IDictionaryBuilder.h"
#include "ApplicationInterface.h"
#include "DictionaryIndex.h"
#include "Application.h"
#include "LogConstants.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace SpellChecker
{

	class TYPODLL_API DictionaryBuilder : public IDictionaryBuilder
	{
		
	public:

		DictionaryBuilder(const typo_string &infile, const typo_string &outfile, AbstractApplicationInterface *pipeBuilder);

		virtual ~DictionaryBuilder(){ _vw.erase(_vw.begin(), _vw.end());}
		virtual int build();

	private:

		int loadDict();
		int saveDict(const typo_string &delim);
		//void read();
		int updateContainer( typo_string &w);
		typo_string _infile, _outfile;
		vector <Word> _vw;
		int _MaxDictSize; 
		Application *_pSimplePipeBuilder; 
	};

};

#pragma warning(pop)



#endif //__DICTIONARYBUILDER_H__


