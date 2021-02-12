#ifndef __SPELLERIMPSTD_H__
#define __SPELLERIMPSTD_H__

#include "ISpellCheckerImp.h"
#include "Application.h"
#include "Constants.h"
#include "data_types.h"
#include "IOManipulators.h"

#include "DictionaryIndex.h" //?

namespace SpellChecker
{

	class SpellerTrainer : public ISpellCheckerImp{ 
				
		void recursiveFetchWord(typo_string &to_fix, typo_string &punctuation, ifstream& cand_list);
		//int addBestChoice( typo_string &to_fix, vector<ds_pair> &cand_list);

	public:
		SpellerTrainer(Application *mp);
		virtual ~SpellerTrainer(){ }

		virtual int do_spell_word( typo_string &to_fix, vector<ds_pair> &cand_list);
		virtual int do_spell_file(const typo_string &input_file, const typo_string &output_file);

	protected:
	
//	#ifdef _TESTING
//		int do_do_spell_file(const typo_string &input_file, const typo_string &output_file);
//	#endif
		
		int run(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim);
	//	SpellerTrainer(const SpellerTrainer &c){	*this = c;	}	
	private:
		Application *_pSimplePipeBuilder;
		DictionaryIndex *_pDict;
		//WriteFile outputter;
		vector<string> _best_choices;
		typo_string txtOutFileName, xmlOutFileName;
		//StringManipulators _sm;
		int _SafeFall;
		fstream outfile;

	};
};
#endif //__SPELLERIMPSTD_H__

