#include "Builder.h"
#include "ApplicationInterface.h"

//BUILD_DICTIONARY_RUNNINGMODE
#include "LetterSetBuilder.h"
#include "NGramBuilder.h"
#include "SentenceNGramBuilder.h"
#include "DictionaryBuilder.h"
//ENDOF BUILD_DICTIONARY_RUNNINGMODE

void  Builder::init(const typo_string &installDir, const typo_string &runningMode, 
					const typo_string &compFuncName, const typo_string &perfRep, 
					const typo_string &output_type)
{
	_ppBuilder = new ApplicationInterface(installDir, runningMode, compFuncName, perfRep, output_type);
	_installDir=installDir; _runningMode=runningMode; _compFuncName=compFuncName; 
	_perfRep=perfRep; _output_type=output_type; 
}


IAbstractProduct* Builder::makeSpellChecker()
{
	if (NULL == _ppBuilder)  _ppBuilder = new ApplicationInterface(_installDir, _runningMode, _compFuncName, _perfRep, _output_type);
	return _ppBuilder->makeSpellChecker();
}

void Builder::buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type)
{
	if (LETTER_SET_INTERSECTION_LIST == dict_type){
		LetterSetIndexBuilder dict(input, output, _ppBuilder);
		dict.build();
	}

	if (DICTIONARY_INDEX == dict_type){
		DictionaryBuilder dict(input, output, _ppBuilder);
		dict.build();
	}

	if (NGRAM_INDEX == dict_type){
		NGramBuilder dict(3, input, output, _ppBuilder);
		dict.build();
	}

	if (SENTENCE_NGRAM_INDEX == dict_type){
		SentenceNGramBuilder dict(input, output, _ppBuilder);
		dict.build();
	}
}


/*
IAbstractBuilder *Builder::makePipeBuilder(const typo_string &installDir, 
		const typo_string &runningMode, const typo_string &compFuncName, 
		//const int &spellingsSuggestionsListSize, 
		const typo_string &perfRep, 
		const typo_string &output_type)
{
	if (NULL == _pPipeBuilder) 
		_pPipeBuilder = new ApplicationInterface(installDir, runningMode, compFuncName, perfRep, output_type);
	return _pPipeBuilder;
}

*/
