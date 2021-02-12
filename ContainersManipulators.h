#ifndef __LUTILITIES_H__
#define __LUTILITIES_H__



#include "Constants.h"

#include <utility>
#include <list>
#include <algorithm>



namespace SpellChecker{


	class ContainersManipulators
	{
	public:

		template<typename T>
		bool find(const vector<T> &v, const T &s) 
		{
			vector<T>::const_iterator it=v.begin();
			for(it=v.begin(); it != v.end(); ++it){ 
				if (*it == s) return true; 
			}
			return false;
		}

		bool compare_str(const ds_pair& a, const ds_pair& b) {
			return a.first > b.first;
		}

		int findIndex(const vector<ds_pair> &v, const typo_string &s) 
		{
	
			vector<ds_pair>::const_iterator it=v.begin();
			for(it=v.begin(); it != v.end(); ++it){ 
				if ((*it).second == s) return it-v.begin(); 
			}
			return RET_FFAILURE;
		}

		int findMinimalExpectedIndex(const vector<ds_pair> &results, const vector<typo_string> &expected) 
		{
			int ret = -1;
			for(vector<typo_string>::const_iterator it = expected.begin(); it != expected.end(); ++it){ 
				int cur_index;
				if ((cur_index = findIndex(results, *it)) >= 0){
					if (ret == -1)
						ret = cur_index;
					else
						if (cur_index < ret) 
							ret = cur_index;
				}
			}
			return ret;
		}

		int findSubstrOccurrencesNumber(const vector<ds_pair> &v, const typo_string &s) 
		{
			int counter=0;
			vector<ds_pair>::const_iterator it=v.begin();
			for(it=v.begin(); it != v.end(); ++it){ 
				if ((*it).second.find(s) != string::npos) counter++; 
			}
			return counter;
		}

		int rfindIndex(const vector<ds_pair> &v, const typo_string &s) 
		{
			vector<ds_pair>::const_reverse_iterator it=v.rbegin();
			for(it=v.rbegin(); it != v.rend(); ++it){ 
				if ((*it).second == s) return it-v.rbegin(); 
			}
			return RET_FFAILURE;
		}
		
		template<typename T>
		bool find(const list<T> &v, const T &s) 
		{
			list<T>::const_iterator it=v.begin();
			for(it=v.begin(); it != v.end(); ++it){ 
				if (*it == s) return true; 
			}
			return false;
		}
		template<typename T>
		bool remove(vector<T> &v, const T &s) {
			vector <T>::iterator it=v.begin();
			for (it=v.begin(); it!= v.end(); ++it){
				if (*it == s){
					v.erase(it);
					return true;
				}
			}
			return false;
		}

		template<typename T>
		bool remove(list<T> &v, const T &s) {
			list <T>::iterator it=v.begin();
			for (it=v.begin(); it!= v.end(); ++it){
				if (*it == s){
					v.erase(it);
					return true;
				}
			}
			return false;
		}

	
		template<typename T>
		void append(vector<T> &dest, const vector<T> &source) 
		{
			if (dest.empty()){ dest = source; return;}
			if (!(source.empty())){
				for(vector<T>::const_iterator it=source.begin(); it != source.end(); ++it)
					dest.push_back(*it);
			}
		}

	};

};
#endif //__LUTILITIES_H__

