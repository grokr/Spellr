#pragma warning (disable:4018)
#include "NGramBuilder.h"
#include "Log.h"
#include "StringManipulators.h"
#include "IOManipulators.h"
#include "ContainersManipulators.h"

int NGramBuilder::build()
{
	typo_string	filename = _infile; 
	IOManipulators iom;
	iom.io_read(_infile, _dict);
	if (makeNGram() == RET_FFAILURE)
		return RET_FFAILURE;
	iom.io_write<typo_long_num>(_outfile, _NGram, S_SPACE);
		return RET_FSUCCESS;
}

int NGramBuilder::updateNGram(const typo_string &w)
{
	try{
	map<typo_string, typo_long_num>::iterator it = _NGram.find(w);
	if (it != _NGram.end())	
		(*it).second++;
	else 
		_NGram.insert(pair<typo_string, typo_long_num>(w, D_HUNDRED));
	}
	catch(...)
	{
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}


int NGramBuilder::makeNGram()
{
	try{
		for(vector<typo_string>::iterator itDict = _dict.begin(); itDict != _dict.end(); ++itDict){
			if ((*itDict).size() < _n) continue;
			int i = 0;
			updateNGram(C_SHARP + (*itDict).substr(i, _n-1));
			for (i = 0; i < (*itDict).length() - _n; i++)
				updateNGram((*itDict).substr(i, _n));
			updateNGram((*itDict).substr(i+1, _n-1) + C_SHARP);
		}
	}
	catch(...)
	{
		return RET_FFAILURE;
	}
	return RET_FSUCCESS;
}

