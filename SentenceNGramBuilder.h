#ifndef __SENTENCENGRAMBUILDER_H__
#define __SENTENCENGRAMBUILDER_H__

#include "IDictionaryBuilder.h"
#include "Application.h"
#include <map>

#pragma warning(push)
#pragma warning(disable:4251)

namespace SpellChecker
{



class TYPODLL_API SentenceNGramBuilder : public IDictionaryBuilder{

	
private:
	Application *_pSimplePipeBuilder; 
//	size_t _n;
	//map<triple, typo_long_num> _SentenceNGram;
	SentenceNGram _sssi_NGram; 
//	vector<typo_string> _dict;
	typo_string _infile, _outfile;

public:
//
	SentenceNGramBuilder( const typo_string &infile, const typo_string &outfile, 
		AbstractApplicationInterface *pipeBuilder): _infile(infile), _outfile(outfile), 
		_pSimplePipeBuilder(static_cast<Application *>(pipeBuilder)){}
	virtual ~SentenceNGramBuilder(){ 
		_sssi_NGram.erase(_sssi_NGram.begin(), _sssi_NGram.end());
		//_dict.erase(_dict.begin(), _dict.end());
	}
	
	virtual int build();


private:	
//	int write(const typo_string &filename,  map<triple, typo_long_num> &m, const typo_string &delim);
	///compute frequency of n letters/words sequence for given DictionaryIndex
//	int makeNGram();
//	int updateNGram(const typo_string &w);
	
//	template<typename T1, typename T2> 
//	int writeMapToFile(const typo_string &filename,  map<T1, T2> &m, const typo_string &delim);
//	int printNGram() const;
//	int printDict() const;

};

#pragma warning(pop)

};
#endif //__SENTENCENGRAMBUILDER_H__