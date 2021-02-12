/*! \class ICandidateSelection
 *  \brief Interface for building decision making algorithms
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug Not tested.
 *  \bug 
 *  \warning 
 */
#ifndef __ICANDIDATESELECTION_H__
#define __ICANDIDATESELECTION_H__

#include "IAbstractProduct.h"
#include "Constants.h"

namespace SpellChecker
{

class TYPODLL_API ICandidateSelection: public IAbstractProduct
{
public:
	virtual ~ICandidateSelection(){}
	virtual typo_long_num select(typo_string &v_path)=0;
};

};
#endif //__ICANDIDATESELECTION_H__
