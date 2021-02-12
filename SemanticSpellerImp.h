#ifndef __SEMANTICSPELLER_H__
#define __SEMANTICSPELLER_H__

#include "ISpellCheckerImp.h"
#include "IOManipulators.h"
#include "ContainersManipulators.h" 
#include "StringManipulators.h"
#include "SentenceNGramIndex.h"

#ifdef BENCHMARK_TESTING_MODE
#include "TestSpellerBenchmark.h"
#include "Timer.h"
#endif

#include <fstream>
#include <sstream>

namespace SpellChecker
{
	class SemanticSpellerImp : public ISpellCheckerImp{ 

	public:

		SemanticSpellerImp(Application *mp);
		virtual ~SemanticSpellerImp();

		virtual int do_spell_word(typo_string &to_fix, vector<ds_pair> &vector);
		virtual int do_spell_file(const typo_string &input_file, const typo_string &output_file);

	private:

		int do_do_spell_file(const typo_string &input_file, const typo_string &output_file);
		int spell_sentences();
		int do_spell_sentence_semantically(stringstream &, stringstream &,const typo_string&  ending);
		int spell_words_stream_semantically(const typo_string &);

		void closeFiles();
		int openFiles(const typo_string &input_file, const typo_string &output_file);

	private:			

		int fetch_sentence(stringstream& sentence, typo_string& ending);
		int init_the_first_triple( stringstream&  sentence, stringstream&  fixed_sentence,sss_pair& currentElement, 
			const typo_string& sent_ending);
		int add_the_first_word_without_test(sss_pair& currentElement,  stringstream& fixed_sentence, 
			const typo_string& sent_ending);
		int process_the_first_triple( sss_pair& currentElement, sss_pair& previousElement, stringstream& fixed_sentence, 
			typo_string&  next_word, const typo_string& sent_ending, int &break_condition);
		void add_the_last_word_without_test(sss_pair& previousElement, stringstream& fixed_sentence, const typo_string& sent_ending);
		void do_do_spell_sentence_semantically( sss_pair& currentElement, sss_pair& previousElement, 
			stringstream& sentence, stringstream& fixed_sentence,typo_string&  next_word, int & break_condition);
		int set_previous_element(sss_pair& currentElement, sss_pair& previousElement, stringstream& sentence, 
			stringstream& fixed_sentence, typo_string& next_word, int &break_condition, const typo_string& sent_ending );
		int make_plural_or_single(typo_string &to_fix);

	private:

		fstream infile;
		ofstream not_found;
		ofstream autofix_succeeded;
		ofstream autofix_failed;
		ofstream first5Matches;
		ofstream total_found_file;
		ofstream outfile;
		ofstream copy_of_infile;

		ContainersManipulators _cm;
		IOManipulators iom;
		StringManipulators _sm;

		int _toSpellBySentences;
		int _toSpellWordsStream;
		int _filterOutNonSynonyms;
		bool _finalStage;
		int _toFixSemanticErrors;
		int _toFixSyntacticErrors;
		int _SafeFall;
		int _runSemanticTwice;
		int _runSemanticTwiceCounter;

		Application *_pSimplePipeBuilder;
		DictionaryIndex *_pDict;
		SentenceNGramIndex *_pSentNGram;
		Output *pOutput;

#ifdef BENCHMARK_TESTING_MODE
	private:
		TestSpellerBenchmark _tsb;
		int do_spell_file_benchmark(const typo_string &input_file, const typo_string &output_file);
#endif 

	};
};
#endif //__SEMANTICSPELLER_H__