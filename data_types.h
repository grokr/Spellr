#ifndef __STD_AF_H__
#define __STD_AF_H__

#include <TCHAR.H>

#include "DllExports.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

#ifdef _DOUBLE
	typedef TYPODLL_API double typo_long_num;	
#elif defined _INT
	typedef TYPODLL_API int typo_long_num;
#else
	typedef TYPODLL_API int typo_long_num;
#endif


namespace unicode
{

//try this change:
/*
#ifdef UNICODE
typedef TYPODLL_API basic_string<wstring> typo_string; 
#else
typedef TYPODLL_API basic_string<string> typo_string; 
#endif
*/

	typedef TYPODLL_API basic_string<_TCHAR> typo_string;  //typedef basic_string<char> typo_string;
	typedef TYPODLL_API _TCHAR typo_char;
	
	typedef TYPODLL_API pair<int, int> ii_pair; ///for sim, word
	typedef TYPODLL_API pair<typo_long_num, typo_string> ds_pair; ///for sim, word
	typedef TYPODLL_API pair<typo_string, typo_string> ss_pair; ///for word-word match
	typedef TYPODLL_API pair<typo_string, vector<typo_string>> svs_pair; ///for word-word match


	typedef TYPODLL_API pair<typo_char, typo_string> cs_pair; ///for keys swapping rules
	typedef TYPODLL_API pair<typo_string, int> si_pair; ///for keys swapping rules
	typedef TYPODLL_API pair<typo_string, typo_long_num> sd_pair; ///for Viterbi
	typedef TYPODLL_API pair<typo_string,map<typo_string, typo_long_num>> sm_sd_pair; ///for Viterbi
	typedef TYPODLL_API pair<ss_pair, int> ssi_pair;

	typedef TYPODLL_API pair<ss_pair, typo_string> sss_pair;
	typedef TYPODLL_API pair<sss_pair, typo_long_num> sssd_pair;
	typedef TYPODLL_API map<sss_pair, typo_long_num> SentenceNGram;
	
	typedef TYPODLL_API map<typo_string, int> synonymsDict;
	typedef TYPODLL_API pair<typo_string, vector<si_pair>> svpsi_pair; 
	
};

using namespace unicode;
typo_long_num TYPODLL_API ato_typo_long_num(const typo_char *);


#endif //__STD_AF_H__