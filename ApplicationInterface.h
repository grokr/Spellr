#ifndef __PROXYSIMPLEPIPEBUILDER_H__
#define __PROXYSIMPLEPIPEBUILDER_H__

#include "AbstractApplicationInterface.h"


namespace SpellChecker 
{
//PipeBulder -> ApplicationInterface
class TYPODLL_API ApplicationInterface : public AbstractApplicationInterface { //IAbstractBuilder{

public:

	ApplicationInterface(const typo_string &installDir, const typo_string &runningMode,
		const typo_string &compFuncName=S_EMPTY, 
		const typo_string &perfRep=S_EMPTY, const typo_string &output_type=S_EMPTY);

	ApplicationInterface(const ApplicationInterface& c){
		delete mf;
		mf=c.mf;
	}

	virtual ~ApplicationInterface(){ 
		delete mf; mf = NULL;
	}

//	virtual IAbstractProduct *makeSpellCheckerImplementation(const typo_string &id){ 
//		return mf->makeSpellCheckerImplementation(id);
//	}

//	virtual IAbstractProduct* makeComparisonFunction(const typo_string &id){ 
//		return mf->makeComparisonFunction(id);
//		}
	virtual IAbstractProduct* makeSpellChecker(){ 
		return mf->makeSpellChecker();
		}
//	virtual IAbstractProduct* loadContainer(const typo_string &id, const typo_string &compFuncName){ 
//		return mf->loadContainer(id, compFuncName);
//		}
//	virtual IAbstractProduct* getContainer(const typo_string &id, const typo_string &compFuncName) { 
//		return mf->getContainer(id, compFuncName);
//	}

	virtual void buildContainer(const typo_string & input, const typo_string & output,  const typo_string & dict_type){
		return mf->buildContainer(input, output, dict_type);
	}
private:
	AbstractApplicationInterface *mf;
};

};
#endif //__PROXYSIMPLEPIPEBUILDER_H__