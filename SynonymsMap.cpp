/*! \class SynonymsMap
*  \brief SynonymsMap - A map containing synonyms and optionally their frequencies
*  \author Oxana Rybalko
*  \version 0.0.3
*  \date  8 Nov 2009
*  \bug Frequencies are missing, the structure is not well disigned.
*  \bug 
*  \warning 
*/

#include "SynonymsMap.h"

SynonymsMap::SynonymsMap(Application *mf, const typo_string &filename): _pSimplePipeBuilder(mf),
_filename(filename), _GUID(SYNONYMS_MAP)
{
	init();
}

void SynonymsMap::init() //expected format:  big large 5 huge 3 enormous 12;
{
	typo_string s = _pSimplePipeBuilder->INSTALL_DIR + _filename;
	_filename = s;
	IOManipulators iom;

	try{
		iom.io_read_synonymsMap(_filename, _synMap);
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}

}
SynonymsMap::~SynonymsMap()
{ 
	_synMap.erase(_synMap.begin(), _synMap.end());
}

int SynonymsMap::getSuggestions(const typo_string& to_find, vector<ds_pair>& cand_list, Filter &filter)
{
	int index = (*(_synMap.find(to_find))).second;  
	synonymsDict::iterator it = _synMap.begin();  
	for (it = _synMap.begin(); it != _synMap.end(); it++){
		if ((*it).second == index)
			cand_list.push_back(ds_pair(D_ZERO, (*it).first));
	}
return RET_FSUCCESS;
}

bool SynonymsMap::isSynonym(const typo_string& to_find, const typo_string& candidate)
{
	synonymsDict::iterator it1 = _synMap.find(to_find);
	if (it1 == _synMap.end()) return false;

	synonymsDict::iterator it = _synMap.begin();  
	for (it = _synMap.begin(); it != _synMap.end(); it++){
		if ((*it).second == (*(it1)).second){
			if ((*it).first == candidate) 
				return true;
		}
	}
	return false;
}