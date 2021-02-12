/*! \interface IDictionary, IIndexedDictionary, IRulesList
 *  \brief Interface for mapping facilities definition
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug 
 *  \bug 
 *  \warning 
 */

#ifndef __IDICTIONARY_H_
#define __IDICTIONARY_H_

#include "IAbstractProduct.h"
#include "IStringComparison.h"
#include "Filter.h"


namespace SpellChecker
{
	class TYPODLL_API IContainer 
	{
	public: 
		IContainer(){}
		virtual ~IContainer(){}
		virtual int find()=0;
		virtual void sort()=0;
		virtual int append()=0;
		virtual int insert()=0;
		virtual int erase()=0;
		virtual int size()=0;
		virtual int getNext()=0;

	};

	class TYPODLL_API IDictionary: public IAbstractProduct{
	public: 
		IDictionary(){}
		virtual ~IDictionary(){}
		virtual typo_string getID()=0;
		virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)=0;

/** 
*	maintain tracking of words usage frequences. suggest first more frequently used words
*	virtual int update_frequency(const typo_string &word){};
**/

	};
	
	class TYPODLL_API IIndexedDictionary: public IDictionary{
	public:
		IIndexedDictionary(){}
		virtual ~IIndexedDictionary(){}
		virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)=0;
		virtual int getSize()=0;
		virtual int find(typo_string &s)=0;

		virtual typo_string getID()=0;
		virtual int add_word(const typo_string &w){return RET_FFAILURE;}
		virtual int remove_word(const typo_string &w){return RET_FFAILURE;}
	};

	class TYPODLL_API	IRulesList: public IDictionary{
	public: 
		IRulesList(){}
		virtual ~IRulesList(){}
		virtual typo_string getID()=0;
		virtual int getSuggestions(const typo_string &to_fix, vector<ds_pair> &res, Filter &filter)=0;
		virtual int add_correction(const typo_string &w, const typo_string &f){return RET_FFAILURE;}
		virtual int add_correction(const typo_string &file){return RET_FFAILURE;}
		virtual int remove_correction(const typo_string &w, const typo_string &f){return RET_FFAILURE;}
	};

};

#endif //__IDICTIONARY_H_