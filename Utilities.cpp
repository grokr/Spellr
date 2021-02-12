#include "Utilities.h"
#include <algorithm>

using namespace SpellChecker;

bool compare_factorFrequencies(const ds_pair& a, const ds_pair& b) {
    return a.first > b.first;
}


void Utilities::factorFrequencies(vector<ds_pair>& cand_list)
{
	if (cand_list.empty()) return;
	sort(cand_list.begin(), cand_list.end(), compare_factorFrequencies);
	typo_long_num max_freq = (cand_list.front()).first; 
	typo_long_num factor = max_freq/D_HUNDRED;
	
	for(vector<ds_pair>::iterator it=cand_list.begin(); it != cand_list.end(); ++it)
	{
		(*it).first = (*it).first/factor;
	}
}

void Utilities::pickup_n_best_suggestions(vector<ds_pair> &res, const int &size)
{
	sort(res.begin(), res.end(), compare_factorFrequencies);
	if (res.size() > size)
		res.erase(res.begin() + size, res.end());
}

void Utilities::pickup_best_suggestions(vector<ds_pair> &res, const typo_long_num &similarity)
{
	sort(res.begin(), res.end(), compare_factorFrequencies);
	vector<ds_pair>::iterator it=res.begin();
	for(; it != res.end(); it++)
	{
		if ((*it).first < similarity) break;
			
	}
	res.erase(it, res.end());
}