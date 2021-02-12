#include "LetterSetBuilder.h"
#include "Log.h"
#include "LogConstants.h"
#include "StringManipulators.h"

#include <fstream>
#include <algorithm>
#include <list>
#include <istream>
#include <sstream>

/*
* pre-encode words vector, create index
1. for each word sort letters, i.e. 
1. create => aeecrt
2. dictionary => acdiinorty
2. sort encoded words by words' lengths
3. each block with length N sort by A-Z

		struct myclass {
	bool operator() (typo_string i, typo_string j) { return (i.size() < j.size()); }
} length_sort;

bool lex_sort(typo_string i, typo_string j) { return (i < j); }

*/
		

int LetterSetIndexBuilder::io_write(const typo_string &filename,  map<typo_string, int> &m, const typo_string &delim)
{
	ofstream os;
	os.open(filename.c_str(), ios::app);
	if (!os.good()){	
		
		static_cast<Application* >(_pSimplePipeBuilder)->getLog()->report(MSG_00009, filename); return RET_FFAILURE;	
	}
	map<typo_string, int>::iterator it = m.begin();  
	for (it = m.begin(); it != m.end(); it++){
		os << (*it).first;  
		os << S_SPACE;
		os << (*it).second;
		os << delim; 
	}
	os.close();
	return RET_FSUCCESS;
}


int LetterSetIndexBuilder::build()
{ 
	try{
		loadDictionary();
		indexContainer();
		io_write(_outfile, _msi, S_NL);
		
	}
	catch(...){	
		static_cast<Application* >(_pSimplePipeBuilder)->getLog()->report(MSG_00042);
	}
	return RET_FSUCCESS;
}

int LetterSetIndexBuilder::loadDictionary()
{ 
	typo_string	filename = _infile; 
	ifstream is;
	is.open(filename.c_str());
	if ( false == is.good()){	static_cast<Application* >(_pSimplePipeBuilder)->getLog()->report(MSG_00009, _infile.c_str());	is.close(); throw runtime_error(MSG_00006);	}
	typo_string word=S_EMPTY;
	int index = 0;

	try{
		while (!is.eof() && ((int)_msi.size() < _maxDictSize)){
			stringstream ss;
			is >> word; 
			if (word.size() < 2) 
				continue;
			sort(word.begin(), word.end());
			ss << word << S_COLON << index;

			updateContainer(si_pair(ss.str(), index++));
		}
	}
	catch(...){	
		static_cast<Application* >(_pSimplePipeBuilder)->getLog()->report(MSG_00041);
	}
	is.close();
	return RET_FSUCCESS;
}

int LetterSetIndexBuilder::updateContainer(const si_pair &sip)
{
	map<typo_string, int>::iterator it = _msi.find(sip.first);
	if (it == _msi.end()){	
		_msi.insert(sip);
	}
	else 
		(*it).second++;
	return RET_FSUCCESS;
}

/*
typedef pair<typo_string, int> Pair;
struct lex_sort {
	bool operator() (const Pair& b1, const Pair& b2) { return  b1.first < b2.first; }
};
struct length_sort {
	bool operator() (const Pair& b1, const Pair& b2) { return  b1.first.size() < b2.first.size(); }
};
*/
bool length_sort(const si_pair& a, const si_pair& b) {
    return a.first.size() < b.first.size();
}

bool lex_sort(const si_pair& a, const si_pair& b) {
    return a.first < b.first;
}

int LetterSetIndexBuilder::indexContainer()
{
	vector<si_pair> vsi;
	map<typo_string, int>::iterator it_msi;
	for(it_msi=_msi.begin(); it_msi != _msi.end(); ++it_msi)
		vsi.insert(vsi.begin(), *it_msi);
	sort(vsi.begin(), vsi.end(), length_sort);				
	vector<si_pair>::iterator it_vsi;
	for(it_vsi=vsi.begin(); it_vsi != vsi.end(); ++it_vsi)
		_msi.insert(*it_vsi);
	return RET_FSUCCESS;
}

