#ifndef __DLLCOMMUNICATION_H__
#define __DLLCOMMUNICATION_H__

//#include "DllExports.h"
#include <iostream>
#include <TCHAR.H>

#ifdef __cplusplus
extern "C" {
#endif


// MYDATACOLLECTION is a kind of a "opaque handle"
// to a data collection instance.
// An handle uniquely identifies the collection instance.
	/*
typedef struct tagMyDataCollection
{
int dummy;
} * MYDATACOLLECTION;
*/
	//using namespace std;
class MyData
{
public:
	MyData(){}
	MyData(const MyData& data)
	{
		*this = data;
	} 
	MyData& operator=(const MyData& data)
	{
		installDir = data.installDir;
		runningMode = data.runningMode;
		compFuncName = data.compFuncName;
		perfReport = data.perfReport;
		outputType = data.outputType;
		listSize = data.listSize;
	}
	// I would prefer exposing Get/Set methods
	// instead of "raw" data members here...
	std::string installDir;
	//std::string runningMode;
	const _TCHAR *runningMode;
	std::string compFuncName;
	std::string perfReport;
	std::string outputType;
	int listSize;
};


typedef class  MyData* MYDATA;
// Creates an instance of the collection; returns its handle
 MYDATA MyData_Create( );

// Cleanups an instance of data collection, given its handle
 void MyData_Destroy(MYDATA x);

/*
// Reads string #1 of a data in a collection,
// given collection handle and item index (0-based)
TYPODLL_API const char * MyData_getInstallDir(MYDATA x, int itemIndex);


// Sets the string #1 value of a data in a collection,
// given collection handle and data index
TYPODLL_API void MyData_setInstallDir(MYDATA x, int itemIndex, const char * str);
*/
//....
//.... other "methods" (MyDataCollection_Xxxxxx) prototypes here
//....


#ifdef __cplusplus
}; // extern "C"
#endif

#endif //__DLLCOMMUNICATION_H__