#include "DictionaryBuilder.h"
#include "IniFileHandler.h"
#include "StringManipulators.h"

using namespace IniFileNamespace;

#include <fstream>
#include <algorithm>

DictionaryBuilder::DictionaryBuilder(const typo_string &infile, const typo_string &outfile, 
	AbstractApplicationInterface *pipeBuilder):_infile(infile), _outfile(outfile), _pSimplePipeBuilder(static_cast<Application *>(pipeBuilder))
{
	try{ 
		_MaxDictSize = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][MAXDICTSIZE].c_str());
	}
	catch(...){
		_pSimplePipeBuilder->getLog()->report(MSG_00019);
		throw runtime_error(MSG_00006);
	}
}
bool compare(const Word& a, const Word& b) {
    return a._data < b._data;
}
int DictionaryBuilder::build()
{ 
	loadDict();
//	const typo_long_num D_HUND=100.0;

	sort(_vw.begin(), _vw.end(), compare);

	for (vector<Word>::iterator it = _vw.begin(); it != _vw.end(); it++)
		(*it)._freq=(*it)._freq*ONE_HUNDRED_PERCENT*ONE_HUNDRED_PERCENT/_vw.size();

	saveDict(S_SEMICOLON);
//	IOManipulators iom;
//	iom.io_write<Word>(_outfile, _vw, S_SEMICOLON);
	return RET_FSUCCESS;
}


int DictionaryBuilder::saveDict(const typo_string &delim)
{
	typo_string	filename = _outfile; //_pSimplePipeBuilder->INSTALL_DIR + S_BACK_SLASH + 
	typo_string	filenameNoFreq = output_filename_nofreq; //_pSimplePipeBuilder->INSTALL_DIR + S_BACK_SLASH + 
	ofstream os, osNoFreq;
	os.open(filename.c_str()); 
	if (!os.good()){		
		_pSimplePipeBuilder->getLog()->report(MSG_00009, filename.c_str());
		return RET_FFAILURE;	
	}
	osNoFreq.open(filenameNoFreq.c_str());
	if (!osNoFreq.good()){		
		_pSimplePipeBuilder->getLog()->report(MSG_00009, filenameNoFreq.c_str());
		os.close();
		return RET_FFAILURE;	
	}

	vector<Word>::const_iterator it = _vw.begin();  
	for (it = _vw.begin(); it != _vw.end(); it++){
		os << (*it)._data; 
		os << delim; 
		os << (*it)._freq;
		os << S_NL;	

		osNoFreq << (*it)._data;
		osNoFreq <<S_NL;
	}
	os.close();
	osNoFreq.close();
	return RET_FSUCCESS;
}

int DictionaryBuilder::loadDict()
{ 
	typo_string	filename = _infile; //_pSimplePipeBuilder->INSTALL_DIR + S_BACK_SLASH + 
	ifstream is;
	is.open(filename.c_str());
	if ( false == is.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, filename.c_str());	is.close(); throw runtime_error(MSG_00006);	}
	typo_string word=S_EMPTY;
	while (!is.eof() && ((int)_vw.size() < _MaxDictSize)){
		is >> word;
		updateContainer(word);
	}
	is.close();
	return RET_FSUCCESS;
}

int DictionaryBuilder::updateContainer(typo_string &w)
{
	StringManipulators sm;
//	transform(w.begin(), w.end(), w.begin(), tolower);
	sm.removeSpecialCharacters(w, UC_NON_ALPHABET_EN);
	if (w.size() < 3) return RET_FSUCCESS;
	Word word(w, D_ONE);
	vector<Word>::iterator it = find(_vw.begin(), _vw.end(), word);
	if (it == _vw.end())	
		_vw.push_back(word);
	else 
		(*it).incrementFreq();
	return RET_FSUCCESS;
}