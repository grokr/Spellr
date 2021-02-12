#ifndef __LETTERSETINDEX_H__
#define __LETTERSETINDEX_H__

#include "IDictionaryBuilder.h"
#include "IOManipulators.h"
#include "Constants.h"
#include "data_types.h"
#include "SimplePipeBuilder.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace SpellChecker
{

	class TYPODLL_API LetterSetIndexBuilder : public IDictionaryBuilder
	{
		
	public:

		LetterSetIndexBuilder(const typo_string &infile, const typo_string &outfile, 
			IAbstractBuilder *pipeBuilder): _infile(infile), _outfile(outfile), 
			_pSimplePipeBuilder(static_cast<SimplePipeBuilder *>(pipeBuilder))
		{
			_maxDictSize =  1000000;//atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
		}
		virtual ~LetterSetIndexBuilder(){ _msi.erase(_msi.begin(), _msi.end());}
		virtual int build();

	private:
		SimplePipeBuilder *_pSimplePipeBuilder; 
		int io_write(const typo_string &filename,  map<typo_string, vector<si_pair>> &m, const typo_string &delim);

		///sort lexicographically
	//	bool lex_sort(si_pair i, si_pair j) { return (i.first < j.first); }
		///sort by length
		//bool length_sort(si_pair i, si_pair j) { return (i.first.size() < j.first.size()); }
		map<typo_string, vector<si_pair>> _msi;


		int indexContainer();
		int loadDictionary();
		int updateContainer(const si_pair &sip);

		typo_string _infile, _outfile;
		int _maxDictSize;
		IOManipulators _iom;

	};

};

#pragma warning(pop)

#endif //__LETTERSETINDEX_H__

