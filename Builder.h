/*! \class Builder
 *  \brief Creational Design Pattern, used meanwhile for Application creation
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug Not tested.
 *  \bug 
 *  \warning 
 */
#ifndef __BUILDER_H__
#define __BUILDER_H__

#include "AbstractApplicationInterface.h"


namespace SpellChecker 
{
	class TYPODLL_API Builder 
{
public:
	Builder():_ppBuilder(NULL){}
	~Builder(){ delete _ppBuilder; _ppBuilder=NULL;}

	IAbstractProduct* makeSpellChecker();
	void buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type);
	void  init(const typo_string &installDir, const typo_string &runningMode, 
				const typo_string &compFuncName=S_EMPTY, const typo_string &perfRep=S_EMPTY, 
				const typo_string &output_type=S_EMPTY);
private:

	AbstractApplicationInterface *_ppBuilder;
	typo_string _installDir; typo_string _runningMode;
	typo_string _compFuncName; typo_string _perfRep; typo_string _output_type;
	bool _self_learning_enabled;
};
};
#endif //__BUILDER_H__