// EditDist.cpp: implementation of the CEditDist class.
// CEditDist is a class for performing calculations of edit distance
// on generic data types.
// (c) Zvika Ben-Haim, 1999 <zvikabh@aluf.technion.ac.il>
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "Muse.h"
#include <stdlib.h>
#include <algorithm>
#include "EditDist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

template <class Type>
int CEditDist<Type>::EditDistance(const Type* ar1, int n, const Type* ar2, int m)
{
	int *graph = new int[(n+1)*(m+1)];
	int a,b,p1,p2,p3;
	int nPrice;
	int maxnm = (n>m ? n : m);

	m_xmax = n;
	m_ymax = m;

	// Fill first row and column of graph
	graph[0] = 0;
	for(a=1; a<=n; a++)
		graph[a] = graph[a-1] + InsertCost(ar1[a-1],a,0);
	for(a=1; a<=m; a++)
		graph[a*(n+1)] = graph[(a-1)*(n+1)] + DeleteCost(ar2[a-1],0,a);

	// Fill the rest of the graph
	// Order of filling is in an L-shape: One row, then one column, then one row etc.
	for(a=1; a<=maxnm; a++) {
		// fill all (remaining) entries in row 'a'.
		for(b=a; b<=n; b++) {
			if(a>m) continue;
			p1 = graph[(a-1)*(n+1)+b-1] + ChangeCost(ar1[b-1],ar2[a-1],b,a);
			p2 = graph[(a-1)*(n+1)+b]   + InsertCost(ar2[a-1],b,a);
			p3 = graph[a*(n+1)+b-1]     + DeleteCost(ar1[b-1],b,a);
			graph[a*(n+1)+b] = min(min(p1,p2),p3);
		}

		// fill all (remaining) entries in column 'a'.
		for(b=a; b<=m; b++) {
			if(a>n) continue;
			p1 = graph[(b-1)*(n+1)+a-1] + ChangeCost(ar1[a-1],ar2[b-1],a,b);
			p2 = graph[(b-1)*(n+1)+a]   + InsertCost(ar2[b-1],a,b);
			p3 = graph[b*(n+1)+a-1]     + DeleteCost(ar1[a-1],a,b);
			graph[b*(n+1)+a] = min(min(p1,p2),p3);
		}
	}

	nPrice = graph[(n+1)*(m+1)-1];
	delete[] graph;
	return nPrice;
}

// Implementation of CIntEditDist class
// (This is an example of a CEditDist implementation)
//////////////////////////////////////////////////////////////////////

int CIntEditDist::DeleteCost(const int& deleted, int x, int y)
{
	if(x==m_xmax || y==m_ymax)
		return 0;
	return 15000/(m_xmax+m_ymax);
}

int CIntEditDist::InsertCost(const int& inserted, int x, int y)
{
	if(x==m_xmax || y==m_ymax)
		return 0;
	return 15000/(m_xmax+m_ymax);
}

int CIntEditDist::ChangeCost(const int& from, const int& to, int x, int y)
{
	return abs(from-to);
}

#pragma warning(disable: 4661)
#pragma warning(disable: 4660)

// use lines like this to explicitly instantiate all member functions of a class
// (these lines must appear in this file)
// This line causes warning messages during compilation (C4660 and C4661); these
// can be safely ignored.
template class CEditDist<int>; // see "Explicit Instantiation" in the online help

#pragma warning(default: 4660)
#pragma warning(default: 4661)
