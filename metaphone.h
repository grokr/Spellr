#ifndef __METAPHONE_H__
#define __METAPHONE_H__
////////////////////////////////////////////////////////////////////////////////
// Double Metaphone (c) 1998, 1999 by Lawrence Philips
//
// Slightly modified by Kevin Atkinson to fix several bugs and 
// to allow it to give back more than 4 characters.
//
//
////////////////////////////////////////////////////////////////////////////////
#include "data_types.h"
#include "metaphone_std.h"

#include <cstdarg>

#define AND &&
#define OR ||


class MString : public CString
{
private:
	CString primary, secondary;
	bool alternate;
	int length, last;
public:

	MString();
	MString(const _TCHAR* in);
	MString(const CString& in);
	bool SlavoGermanic();
	void MetaphAdd(const _TCHAR* main);
	void MetaphAdd(const _TCHAR* main, const _TCHAR* alt);
	bool IsVowel(int at);
	bool StringAt(int start, int length, ... );
	void DoubleMetaphone(CString &metaph, CString &metaph2, const typo_string& lang);
	void DoubleMetaphoneRussian(CString &metaph, CString &metaph2);
	void DoubleMetaphoneHebrew(CString &metaph, CString &metaph2);


};

#endif //__METAPHONE_H__