#ifndef __METAPHONEBUILDER_H__
#define __METAPHONEBUILDER_H__

#include "IDictionaryBuilder.h"
#include "SimplePipeBuilder.h"

class TYPODLL_API MetaPhoneBuilder : public IDictionaryBuilder{

	SimplePipeBuilder *_pSimplePipeBuilder; 

public:

	MetaPhoneBuilder(const typo_string &infile, const typo_string &outfile, SimplePipeBuilder *pipeBuilder):_infile(infile), 
		_outfile(outfile), _pSimplePipeBuilder(pipeBuilder){}
	virtual int build();
private:
	typo_string _infile, _outfile;
	int batch_build();
};

#endif //__METAPHONEBUILDER_H__