#include "IStringComparison.h"
#include "SpellerImp.h"
#include "TestSpellerImp.h"
#include "TestSpellerBenchmark.h"
#include "Timer.h"
#include "TestConfig.h"
#include "StringManipulators.h"


#include <fstream>
#include <sstream>

/*
void TestSpellerImp::test_getFixesList_manual(int size){

	string to_fix = "";
	vector<ds_pair> similar_strings;

	while (to_fix != "exit"){
		cin >> to_fix;

		if (_dictionary.find(to_fix) < 2)
			continue;
		
		similar_strings.clear();
		getFixesList(to_fix, similar_strings, 0.0, ContainersInterface::computePMillerSimilarityRatio); 
		cout << "Suggestions of computePMillerSimilarityRatio search :\n";
		if (similar_strings.empty())
			cout << "None. "<< endl;
		StlContainersHelpers::printList(similar_strings);
		
		similar_strings.clear();
		getFixesList(to_fix, similar_strings,  0.0, ContainersInterface::computeLinearSimilarityRatio); 
		cout << "Suggestions of computeLinearSimilarityRatio search :\n";
		if (similar_strings.empty())
			cout << "None. "<< endl;
		StlContainersHelpers::printList(similar_strings);

	}
}
*/
/* mode: 
	ContainersInterface::computeMetaphoneSimilarityRatio 
	ContainersInterface::computeLinearSimilarityRatio
	ContainersInterface::computePMillerSimilarityRatio
	*/

void TestSpellerImp::test_getFixesList(const string &input_path, const string &report_html_path, 
											   const string &expected_path)
{
	int total = 0;
	int total_found = 0;
	//SpellerImp sis;
	MetaphoneSimilarity cmsr;
	Timer t;
	t.timer_start();

	ifstream infile(input_path.c_str(), ifstream::in);
	ifstream expfile(expected_path.c_str(), ifstream::in);

	string to_fix = "", expected = "";
	vector <ds_pair> res;

	while (infile.good()){

		total++;
		infile >> to_fix;
		expfile >> expected;

		//if a word is found(case-insensitive), don't try to fix it, return
		//TBD temporary commented to make comopile unstatic change
//		if (ContainersInterface::_dictionary->find(to_fix) < 2){
//			if (expected == to_fix){
//				total_found++;
//			}
//			continue;
//		}

		
		//DoubleMetaPhoneAlg::getFixesList_by_metaphone(to_fix, res, cmsr.computeSimilarityRatio);
		//_pSpellerImp.getFixesList_by_metaphone(to_fix, res, 	cmsr.computeSimilarityRatio);
	}

	double run_time = t.get_time_lapsed();

/* TBD: remove?
#ifdef _TESTING
	TestSpellerBenchmark _tsb;
	if (BenchmarkTestsSection::_createProductReport){
		//	string report_html_path = BenchmarkTestsSection::_reportPath +  date + ".html";
		_tsb.createProductReport(run_time, in_path, BenchmarkTestsSection::_reportPath);
	}
	if (BenchmarkTestsSection::_createProductsComparisonReport){
		string date="";
		t.get_date(date);
		ContainersInterface::trim(date);
		string reportComparisonPath = BenchmarkTestsSection::_reportComparisonPath +  date + ".html";
		_tsb.createProductsComparisonReport(run_time, in_path, reportComparisonPath);
	}
#endif
*/
	infile.close();
	expfile.close();
}



//#define dict_path		"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\dict_a.txt"
//#define in_path			"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\misspelled_a.txt"
//#define out_path		"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\output_a.txt"
//#define metaphone_dir	"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\metaphone_a.txt"
//#define expected_path	"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\expected_a.txt"
//#define comres_path		"C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\Full_tests\\a\\comres_a.txt"

int mainTestSpellerImp()
//int TestSpellerImp::run_test_getFixesList()
{
	Config *pConf = Config::create();
	pConf->init();
	//	Config c(atoi(argv[1]));

	//en_common_1075.txt
	//	string dict_path=  "C:\\_spellChecker\\code\\model\\spelling\\data\\wordslists\\en\\index_single_word\\a1.txt";
	//	string in_path= "C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\tests\\misspelled_a.txt";
	//	string out_path="C:\\_spellChecker\\code\\model\\spelling\\benchmark_tests\\HP_2135\\Typo\\tests\\output_a.txt";

	//	TestSpeller ts(dict_path, metaphone_dir);
	//	ts.test_Speller();

	string dict_path = BenchmarkTestsSection::_dictPath;
	string metaphone_dir = Paths::_metaphonePath;
	string in_path	= BenchmarkTestsSection::_inPath;

	string date="";
	Timer t;
	StringManipulators sm;
	t.get_date(date);
	sm.trim(date);
	string perf_report_html_path = BenchmarkTestsSection::_performanceReportPath;
//	string reportComparisonPath = BenchmarkTestsSection::_reportComparisonPath +  date + ".html";
	string exp_path = BenchmarkTestsSection::_expPath;
	
	//TestSpellerImp _testSpellerImp(dict_path, metaphone_dir);
	TestSpellerImp _testSpellerImp;
	_testSpellerImp.test_getFixesList(in_path, 
		perf_report_html_path, 
		exp_path);


	//	td.test_getFixesList(2);
	//	string out;
	//	MetaphoneSimilarity cmsr;
	//	cmsr.phoneticize("should", out);


	//	CString m1("");
	//	CString m2("");
	//	MString metaphone("should");
	//	metaphone.DoubleMetaphone(m1, m2);
	//	cout << m1.c_str() << " " << m2.c_str() << endl;

	return 	1; //CppUnitTestNGram();

}


/*
void TestSpellerImp::readExpected(vector <string> &expected)
{
ifstream datafile(correct_path);
string word;
while (!datafile.eof()){
datafile >> word;
expected.push_back(word);
}
datafile.close();
}*/