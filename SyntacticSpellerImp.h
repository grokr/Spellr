#ifndef __SYNTACTICSPELLER_H__
#define __SYNTACTICSPELLER_H__

#include "ISpellCheckerImp.h"
#include "IOManipulators.h"
#include "ContainersManipulators.h" 
#include "StringManipulators.h"

#ifdef BENCHMARK_TESTING_MODE
#include "TestSpellerBenchmark.h"
#include "Timer.h"
#endif

#include <fstream>
#include <sstream>

namespace SpellChecker
{
	class SyntacticSpellerImp : public ISpellCheckerImp{

	public:
		SyntacticSpellerImp(Application *mp);
		virtual ~SyntacticSpellerImp();

		virtual int do_spell_word(typo_string &to_fix, vector<ds_pair> &vector);
		virtual int do_spell_file(const typo_string &input_file, const typo_string &output_file);
		
		virtual int add_correction(const typo_string &word, const typo_string &fix);
		virtual int add_correction(const typo_string &file);
		virtual int remove_correction(const typo_string &word, const typo_string &fix);

		virtual void get_version(typo_string &version) const;

		virtual int add_word(const typo_string &word);
		virtual int remove_word(const typo_string &word);
		
		virtual int add_dictionary(const typo_string &dpath);
		virtual void get_dictionary_encoding(typo_string &enc) const;

	private:			

		int openFiles(const typo_string &input_file, const typo_string &output_file);
		void do_do_spell_file();
		void closeFiles();
	
	private:	

		fstream infile;
		ofstream outfile;
		XmlOutput _xml_output;

		ContainersManipulators _cm;
		IOManipulators _iom;
		StringManipulators _sm;
		int _SafeFall;
		Application *_pSimplePipeBuilder;
		DictionaryIndex *_pDict;

#ifdef BENCHMARK_TESTING_MODE
	private:
		int do_spell_file_benchmark(const typo_string &input_file, const typo_string &output_file);
		TestSpellerBenchmark _tsb;
#endif //BENCHMARK_TESTING_MODE

	};
};
#endif //__SYNTACTICSPELLER_H__