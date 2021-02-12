#ifndef __SYNONYMSMAP_H__
#define __SYNONYMSMAP_H__

#include "IDictionary.h"
#include "Application.h"
#include "Filter.h"
#include "IOManipulators.h"
#include "DictionaryIndex.h"
#include "ContainersManipulators.h"

namespace SpellChecker
{
	class SynonymsMap:  public IRulesList{

		Application *_pSimplePipeBuilder; 

	public:

		SynonymsMap(Application *mf, const typo_string &filename);
		~SynonymsMap();

		virtual int add_correction(const typo_string &w, const typo_string &f){ return RET_FFAILURE;}
		virtual int remove_correction(const typo_string &w, const typo_string &f){ return RET_FFAILURE;}
		virtual typo_string getID(){return _GUID;}
		virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter);//{ return RET_FFAILURE;}
		
		bool isSynonym(const typo_string& to_find, const typo_string &);
	private:

		void init();

		ContainersManipulators cm;

		/** synonymsDict structure: 
		(big, 0), (huge, 0), (blue, 1), (sad, 1), (zap, 2)
		**/
		synonymsDict _synMap; //map<typo_string, int>> //TBD extend to: <typo_string, int, typo_long_num>
		typo_string _filename;
		typo_string _GUID;
	};
};

#endif //__SYNONYMSMAP_H__