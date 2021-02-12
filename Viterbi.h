#ifndef __VITERBI_H__
#define __VITERBI_H__

#include "ICandidateSelection.h"

#include "vector"
#include "map"
#include "iostream"


class Tracking {
public:
	typo_long_num prob;
	vector<typo_string> v_path;
	typo_long_num v_prob;

	Tracking() {
		prob = D_ZERO;
		v_prob = D_ZERO;
	}

	Tracking(typo_long_num p, vector<typo_string> & v_pth, typo_long_num v_p) {
		prob = p;
		v_path = v_pth;
		v_prob = v_p;
	}
};


class ViterbiAlg : public ICandidateSelection
{
public:
	virtual typo_long_num select(typo_string &v_path);
	ViterbiAlg(const typo_string &to_fix, const map<typo_string,typo_long_num> &start_prob,
		//const map<typo_string,map<typo_string, typo_long_num>> &transition_prob, const vector<ds_pair> &cand_list);
		const map<typo_string, typo_long_num> &transition_prob, const vector<ds_pair> &cand_list);
	virtual ~ViterbiAlg()
	{
		states.erase(states.begin(), states.end());
		observations.erase(observations.begin(), observations.end());
		start_probability.erase(start_probability.begin(), start_probability.end());
		transition_probability.erase(transition_probability.begin(), transition_probability.end());
		emission_probability.erase(emission_probability.begin(), emission_probability.end());
	}

private:
	void print(void);
	void forward(vector<typo_string> obs, vector<typo_string> states, map<typo_string, typo_long_num> start_p, 
		//map<typo_string, map<typo_string, typo_long_num>> trans_p, 
		map<typo_string, typo_long_num> trans_p, 
		map<typo_string, map<typo_string, typo_long_num>> emit_p); 

	vector<typo_string> states;
	vector<typo_string> observations;
	map<typo_string,typo_long_num> start_probability;
	//map<typo_string,map<typo_string, typo_long_num>> transition_probability;
	map<typo_string,typo_long_num> transition_probability;
	map<typo_string,map<typo_string, typo_long_num>> emission_probability;

	Tracking final_tracker;

};

#endif //__VITERBI_H__