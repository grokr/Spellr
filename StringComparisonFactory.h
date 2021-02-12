#ifndef _STRINGCOMPFACTORY_H_
#define _STRINGCOMPFACTORY_H_

#include "IStringComparison.h"

#include <string>
using namespace std;

namespace SpellChecker 
{

class StringComparisonFactory{
public:
	StringComparisonFactory(){}
	~StringComparisonFactory(){}
	IStringComparison *QueryInterface(const string &id);
};

};

#endif //_STRINGCOMPFACTORY_H_