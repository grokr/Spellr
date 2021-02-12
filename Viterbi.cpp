// ViterbiSTL.cpp : is an C++ and STL implementatiton of the Wikipedia example
// Wikipedia: http://en.wikipedia.org/wiki/Viterbi_algorithm#A_concrete_example
// It as accurate implementation as it was possible

#pragma warning (disable:4018)
#include <stdio.h>

#include "Viterbi.h"

//using namespace std;

/**
* HMM/Vitterbi
* Use case:  comparison algorithm can not select candidate from list with 100% assurance
* Dictionary:
*	states=candidates for fix, words from dictionary, output of some algorithm
*	observations=misspelling
*	start_probability=probability of candidates occurance (optionally in users's dictionary)
*	transition_probability=probability of candidate misspelling
*	emission_probability=algorithm output	
*	
*
*
**/

//Example: 
//states = ('Rainy', 'Sunny')
//states = ('appendicularia', 'antitoxic', 'abundances')  // candidates list size: 3

//observations = ('walk', 'shop', 'clean')
//observations = ('abundancies')

//start_probability = {'Rainy': 0.6, 'Sunny': 0.4}
//start_probability = {'appendicularia': 0.06, 'abundances': 0.04, 'antitoxic': 0.04}

//transition_probability = {
//   'Rainy' : {'Rainy': 0.7, 'Sunny': 0.3},
//   'Sunny' : {'Rainy': 0.4, 'Sunny': 0.6},
//   }

//transition_probability = {
//   'appendicularia' : {'appendicularia': 0.7, 'misspelling': 0.3},
//   'antitoxic' : {'antitoxic': 0.4, 'misspelling': 0.6},
//   'abundances' : {'abundances': 0.4, 'misspelling': 0.6},
//   }
//	transition_probability["appendicularia"]["appendicularia"] = 0.92; 
//	transition_probability["appendicularia"]["misspelling"] = 0.92;

//emission_probability = {
//   'Rainy' : {'walk': 0.1, 'shop': 0.4, 'clean': 0.5},
//   'Sunny' : {'walk': 0.6, 'shop': 0.3, 'clean': 0.1},
//   }
//emission_probability = {
//   'appendicularia' : {'abundancies': 0.1},
//   'abundances' : {'abundancies': 0.6},
//   'antitoxic' : {'abundancies': 0.6},
//   }
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ViterbiAlg::ViterbiAlg(const typo_string &to_fix, const map<typo_string,typo_long_num> &start_prob,
					   // const map<typo_string,map<typo_string, typo_long_num>> &transition_prob, const vector<ds_pair> &cand_list)
					   const map<typo_string, typo_long_num> &transition_prob, const vector<ds_pair> &cand_list)
{
	observations.push_back(to_fix);
	start_probability = start_prob;
	transition_probability = transition_prob;

	vector<ds_pair>::const_iterator it=cand_list.begin(); 	//	emission_probability["appendicularia"]["abundancies"] = 0.92; 
	for(it=cand_list.begin(); it != cand_list.end(); ++it){
		states.push_back((*it).second);
		map<typo_string, typo_long_num> m;
		m.insert(sd_pair(to_fix, (*it).first));
		emission_probability.insert(sm_sd_pair((*it).second, m));	
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ViterbiAlg::print() {
	// print states
	cout << "States:" << endl;
	for(vector<typo_string>::iterator i=states.begin();i!=states.end();i++) {
		cout << "S: " << (*i) << endl;
	}
	// print observations
	cout << "Observations:" << endl;
	for(vector<typo_string>::iterator i=observations.begin();i!=observations.end();i++) {
		cout << "O: " << (*i) << endl;
	}

	// print start probabilities
	cout << "Start probabilities:" << endl;
	for(map<typo_string, typo_long_num>::iterator i=start_probability.begin();i!=start_probability.end();i++) {
		cout << "S: " << (*i).first << " P: " << (*i).second << endl;
	}

	// print transition_probability
	cout << "Transition probabilities:" << endl;
	//for(map<typo_string,map<typo_string, typo_long_num>>::iterator i=transition_probability.begin();i!=transition_probability.end();i++) {
	for(map<typo_string,typo_long_num>::iterator i=transition_probability.begin();i!=transition_probability.end();i++) {
		//for(map<typo_string, typo_long_num>::iterator j=(*i).second.begin();j!=(*i).second.end();j++) {
		//	cout << "FS: " << (*i).first << " TS: " << (*j).first << " P: " << (*j).second << endl;
		cout << "FS: " << (*i).first << " P: " << (*i).second << " P: " << endl;
		//	}
	}

	// print emission probabilities
	cout << "Emission probabilities:" << endl;
	for(int i=0; i<states.size(); i++) {
		for(int j=0; j<observations.size(); j++) {
			cout << "FS: " << states[i] << " TO: " << observations[j] <<
				" P: " << emission_probability[states[i]][observations[j]] << endl;
		}
	}
}

//this method compute total probability for observation, most likely viterbi path 
//and probability of such path
void ViterbiAlg::forward(vector<typo_string> obs, vector<typo_string> states, map<typo_string, typo_long_num> start_p, 
						 //map<typo_string, map<typo_string, typo_long_num>> trans_p, map<typo_string, map<typo_string, typo_long_num>> emit_p) 
						 map<typo_string, typo_long_num> trans_p, map<typo_string, map<typo_string, typo_long_num>> emit_p) 
{
	map<typo_string, Tracking> T;

	for(vector<typo_string>::iterator state=states.begin(); state!=states.end();state++) {
		vector<typo_string> v_pth;
		v_pth.push_back(*state);

		T[*state] = Tracking(start_p[*state], v_pth, start_p[*state]);
	}

	for(vector<typo_string>::iterator output=obs.begin(); output!=obs.end();output++) {
		map<typo_string, Tracking> U;

		for(vector<typo_string>::iterator next_state=states.begin(); next_state!=states.end(); next_state++) {
			Tracking next_tracker;

			for(vector<typo_string>::iterator source_state=states.begin(); source_state!=states.end(); source_state++) {
				Tracking source_tracker = T[*source_state];

				//typo_long_num next_state = trans_p[*source_state][*next_state];
				typo_long_num dnext_state = trans_p[*source_state];
				typo_long_num p = emit_p[*source_state][*output]*dnext_state;
				source_tracker.prob *= p;
				source_tracker.v_prob *= p;

				next_tracker.prob += source_tracker.prob;

				if(source_tracker.v_prob > next_tracker.v_prob) {
					next_tracker.v_path = source_tracker.v_path;
					next_tracker.v_path.push_back(*next_state);
					next_tracker.v_prob = source_tracker.v_prob;
				}
			}

			U[*next_state] = next_tracker;
		}

		T = U;
	}

	// apply sum/max to the final states


	for(vector<typo_string>::iterator state=states.begin(); state!=states.end(); state++) {
		Tracking tracker = T[*state];

		final_tracker.prob += tracker.prob;

		if(tracker.v_prob > final_tracker.v_prob) {
			final_tracker.v_path = tracker.v_path;
			final_tracker.v_prob = tracker.v_prob;
		}
	}

	cout << "Total probability of the observation sequence: " << final_tracker.prob << endl;
	cout << "Probability of the Viterbi path: " << final_tracker.v_prob << endl;
	cout << "The Viterbi path: " << endl;
	for(vector<typo_string>::iterator state=final_tracker.v_path.begin(); state!=final_tracker.v_path.end(); state++) {
		cout << "VState: " << *state << endl;
	}
}

typo_long_num ViterbiAlg::select(typo_string &fixed)
{
	print();
	forward(observations, states, start_probability, transition_probability, emission_probability);
	fixed= *final_tracker.v_path.begin();
	return final_tracker.v_prob;
}


/*
void ViterbiAlg::init(void) {
states.push_back("Rainy");
states.push_back("Sunny");

observations.push_back("walk");
observations.push_back("shop");
observations.push_back("clean");

start_probability["Rainy"] = 0.6;
start_probability["Sunny"] = 0.4;

transition_probability["Rainy"]["Rainy"] = 0.7;
transition_probability["Rainy"]["Sunny"] = 0.3;
transition_probability["Sunny"]["Rainy"] = 0.4;
transition_probability["Sunny"]["Sunny"] = 0.6;

emission_probability["Rainy"]["walk"] = 0.1;
emission_probability["Rainy"]["shop"] = 0.4;
emission_probability["Rainy"]["clean"] = 0.5;
emission_probability["Sunny"]["walk"] = 0.6;
emission_probability["Sunny"]["shop"] = 0.3;
emission_probability["Sunny"]["clean"] = 0.1;
}
*/