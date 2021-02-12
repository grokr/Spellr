#ifndef __NGRAMBUILDER_H__
#define __NGRAMBUILDER_H__

#include "IDictionaryBuilder.h"
#include "Application.h"

#pragma warning(push)
#pragma warning(disable:4251)

class TYPODLL_API NGramBuilder : public IDictionaryBuilder{

	
private:

	size_t _n;
	map<typo_string, typo_long_num> _NGram;
	vector<typo_string> _dict;
	typo_string _infile, _outfile;
	Application *_pSimplePipeBuilder; 

public:

	NGramBuilder( size_t size, const typo_string &infile, const typo_string &outfile, 
		AbstractApplicationInterface *pipeBuilder):_n(size), _infile(infile), _outfile(outfile), 
		_pSimplePipeBuilder(static_cast<Application *>(pipeBuilder)){}
	virtual ~NGramBuilder(){ 
		_NGram.erase(_NGram.begin(), _NGram.end());
		_dict.erase(_dict.begin(), _dict.end());
	}
	
	virtual int build();

private:	
	///compute frequency of n letters/words sequence for given DictionaryIndex
	int makeNGram();
	int updateNGram(const typo_string &w);
	
//	template<typename T1, typename T2> 
//	int writeMapToFile(const typo_string &filename,  map<T1, T2> &m, const typo_string &delim);
//	int printNGram() const;
//	int printDict() const;

};

#pragma warning(pop)


#endif //__NGRAMBUILDER_H__