/*! \class SentenceNGramBuilder
 *  \brief SentenceNGramBuilder creates a dictionary containing words' sequences frequences 
 *  \author Oxana Rybalko
 *  \version 0.0.3
 *  \date    11.2009
 *  \bug 
 *  \bug 
 *  \warning 
 */
#pragma warning (disable:4018)

#include "SentenceNGramBuilder.h"
#include "Log.h"
#include "StringManipulators.h"
#include "ContainersManipulators.h"

#include <algorithm>
#include <map>
using namespace SpellChecker;
using namespace unicode;

bool compare_sssd(const sssd_pair& t1, const sssd_pair& t2) 
{
    return  (t1.first.first==t2.first.first) && (t1.first.second==t2.first.second) && (t1.second==t2.second);
}

int SentenceNGramBuilder::build()
{
	typo_string	filename = _infile; 
	ifstream is;
	is.open(_infile.c_str());
	if (false == is.good())	{ 
		throw runtime_error(MSG_00006);
	}
	
	sss_pair firstSeqEl; 
	is >> firstSeqEl.first.first; is >> firstSeqEl.first.second; is >> firstSeqEl.second;
	_sssi_NGram.insert(sssd_pair(firstSeqEl, 1));

	sss_pair prevSeqEl; 
	prevSeqEl.first.first = firstSeqEl.first.second;
	prevSeqEl.first.second = firstSeqEl.second;
	is >> prevSeqEl.second;
	_sssi_NGram.insert(sssd_pair(prevSeqEl, 1));

	while (!is.eof()){ 
		sss_pair currentEl; 
		currentEl.first.first=prevSeqEl.first.second;
		currentEl.first.second = prevSeqEl.second;
		is >> currentEl.second;

		prevSeqEl=currentEl;

		try{
			map<sss_pair, typo_long_num>::iterator it= _sssi_NGram.find(currentEl);

			if (it != _sssi_NGram.end())	
				(*it).second++;
			else {
				_sssi_NGram.insert(sssd_pair(currentEl, 1));
			}
		}
		catch(...)
		{
			return RET_FFAILURE;
		}
	}

	is.close();

	ofstream os;
	os.open(_outfile.c_str(), ios::app);
	if (!os.good()){			
		return RET_FFAILURE;	
	}
	
	os.clear();

	map<sss_pair, typo_long_num>::iterator it = _sssi_NGram.begin();  
	for (it = _sssi_NGram.begin(); it != _sssi_NGram.end(); it++){
		sss_pair t = (*it).first;
		os << t.first.first;  os << S_TAB; os << t.first.second;  os << S_TAB; os << t.second;  os << S_TAB; 
		os << (*it).second;
		os << "\n"; 
	}
	os.close();

	return RET_FSUCCESS;
}
