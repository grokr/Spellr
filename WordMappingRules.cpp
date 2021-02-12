/*! \class WordsMappingRules
*  \brief WordsMappingRules - Typical misspellings to fixes map
*  \author Oxana Rybalko
*  \version 0.0.2
*  \date  Apr 2009
*  \bug 
*  \bug 
*  \warning 
*/

#include "WordsMappingRules.h"
#include "StringManipulators.h"
#include "LogConstants.h"

#include <fstream>


int WordsMappingRules::load_WordsMappingRules()
{
	ifstream is;
	typo_string wpath = _pSimplePipeBuilder->INSTALL_DIR + _wmpath;
	is.open(wpath.c_str());
	if ( false == is.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, _wmpath.c_str());	throw runtime_error(MSG_00006);	}

	typo_char buffer[LINE_SIZE];
	typo_string to_fix, sfixes, line;
	typo_long_num freq = D_ZERO;
	StringManipulators sm;

	while (!is.eof() && is.good() && ((int)_wm.size() < _MaxDictSize)){
		vector<typo_string> fixes;
		is.getline (buffer, LINE_SIZE);
		line = buffer;
		sm.getLeft(to_fix, line);  sm.trim(to_fix);
		sm.getRight(sfixes, line); sm.trim(sfixes);
		sm.parseStringAppendToVector(sfixes, C_COMA, fixes);
		_wm.insert(svs_pair(to_fix, fixes));	
	}
	is.close();
	return RET_FSUCCESS;
}

WordsMappingRules::~WordsMappingRules(){
	if (_changed){
		IOManipulators iom;
		iom.io_write<typo_string, vector<typo_string>>(_pSimplePipeBuilder->INSTALL_DIR +_wmpath, 
			_wm, S_SPACE, true/*delete the file contents before writing*/);
	}
	_wm.erase(_wm.begin(), _wm.end());
}

int WordsMappingRules::getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)
{
	map<typo_string,vector<typo_string>>::iterator it = _wm.find(to_fix);
	if (_wm.end() != it){
		vector<typo_string>::iterator vit = (*it).second.begin();
		for(; vit != (*it).second.end(); ++vit)
			res.push_back(ds_pair(D_HUNDRED, *vit));
		filter.sim(D_HUNDRED);
	}
	return RET_FSUCCESS;
}

int WordsMappingRules::add_correction(const typo_string &w, const typo_string &f)
{
	map<typo_string, vector<typo_string>>::iterator it = _wm.find(w); 
	if (it == _wm.end()){
		vector<typo_string> v;
		v.push_back(f);
		_wm.insert(svs_pair(w,v));
		_changed = true;
	}
	return RET_FSUCCESS;
}

/*
file format:
misspelled,fixed;
absorpsion,absorption;
*/

int WordsMappingRules::add_correction(const typo_string &filename)
{
	typo_string misspelled;
	typo_string fixed, in;
	ifstream is;
	is.open(filename.c_str());
	typo_string word=S_EMPTY;
	while (!is.eof()){
		is >> in;
		_sm.parseMappings(in, C_COMA, misspelled, fixed);
		add_correction(misspelled, fixed);
	}
	is.close();

	return RET_FSUCCESS;
}

int WordsMappingRules::remove_correction(const typo_string &w, const typo_string &f)
{
	map<typo_string, vector<typo_string>>::iterator it = _wm.find(w); 
	if (it != _wm.end()){
		_wm.erase(it);
		_changed = true;
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

