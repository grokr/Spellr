// EditDist.h: interface for the CEditDist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITDIST_H__34A34743_2727_11D3_9F4A_0C8A06C10627__INCLUDED_)
#define AFX_EDITDIST_H__34A34743_2727_11D3_9F4A_0C8A06C10627__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Abstract base class for performing edit-distance calculations
// Requirements from the template class Type:
// 1. Must have default constructor
// 2. Must have X::operator=(const X&);
template <class Type>
class CEditDist //: public CObject  
{
public:
	// the following three functions are the only things that the
	// derived class needs to define.
	virtual int DeleteCost(const Type& deleted, int x, int y) = 0; // cost of deletion
	virtual int InsertCost(const Type& inserted, int x, int y) = 0; // cost of insertion
	virtual int ChangeCost(const Type& from, const Type& to, int x, int y) = 0; // cost of changing

	// call this function to calculate the edit distance.
	// ar1, ar2 = the two arrays to compare
	// n = size of ar1; m = size of ar2.
	int EditDistance(const Type *ar1, int n, const Type *ar2, int m);
protected:
	int m_xmax,m_ymax;
};

// example of an implementation:
class CIntEditDist : public CEditDist<int>
{
public:
	int DeleteCost(const int& deleted, int x, int y);
	int InsertCost(const int& inserted, int x, int y);
	int ChangeCost(const int& from, const int& to, int x, int y);
};

#endif // !defined(AFX_EDITDIST_H__34A34743_2727_11D3_9F4A_0C8A06C10627__INCLUDED_)
