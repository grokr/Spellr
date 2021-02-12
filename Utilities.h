#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#include "Constants.h"


namespace SpellChecker
{
	class Utilities
	{
	public:

		void factorFrequencies(vector<ds_pair>& cand_list);
		void pickup_n_best_suggestions(vector<ds_pair> &res, const int &size);
		void pickup_best_suggestions(vector<ds_pair> &res, const typo_long_num &similarity);
	};
};

#endif //__UTILITIES_H__
