/*
#ifndef __SPELLERIMPTESTALLALGORITHMS_H__
#define __SPELLERIMPTESTALLALGORITHMS_H__

#include "SpellerTrainer.h"
#include "data_types.h"
#include "SimplePipeBuilder.h"
#include "Timer.h"
#include "ContainersManipulators.h" 
#include "TestSpellerBenchmark.h"

#include <fstream>
//#include <iostream>
#include <string>

//using namespace std;

namespace SpellChecker
{
	class SpellerImpTestStrategy : public SpellerTrainer{ //uses SpellerTrainer::do_spell_word

	public:

		SpellerImpTestStrategy(SimplePipeBuilder *mp);
		virtual ~SpellerImpTestStrategy();
		virtual int do_spell_file(const typo_string &input_file, const typo_string &output_file);
		int do_do_spell_file(IAbstractProduct *pit, const typo_string &input_file, const typo_string &output_file);
		int run(const typo_string &input_file, const typo_string &output_file);

	private:			

		Timer t;

		ifs tream expfile;
		ofs tream not_found;
		ofs tream autofix_succeeded;
		ofs tream autofix_failed;
		ofs tream first5Matches;
		ofs tream total_found_file;
		ofs tream output_file_with_date;

		ContainersManipulators cm;
		TestSpellerBenchmark _tsb;

	private:
		SimplePipeBuilder *_pSimplePipeBuilder;
	};
};
#endif //__SPELLERIMPTESTALLALGORITHMS_H__
*/