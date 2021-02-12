#ifndef __TESTSPELLERBENCHMARK_H__
#define __TESTSPELLERBENCHMARK_H__

#include "Application.h"
#include "DictionaryIndex.h"
#include "Timer.h"
#include "StringManipulators.h"

const typo_string HTML_REPORT_PATTERN=	"<!-- insert_data_here -->";
const typo_string BENCHMARK_TESTS_SECTION="BenchmarkTests";

class TestSpellerBenchmark;

class TYPODLL_API BenchmarkTestsSection{

	friend TestSpellerBenchmark;
public:
	void init(const typo_string &install_dir);
	
	//public for main
	typo_string _inPath;
	typo_string _outPath;

//public for SemanticSpellerImp
public:
	typo_string _expPath;
	typo_string _install_dir;

private:
	typo_string _total_found_path;
	typo_string _autofix_failed;
	typo_string _not_found;
	typo_string _autofix_succeeded;
	typo_string _first5Matches;
	typo_string _timePlotPath;
	typo_string _timePlotFileName;
	typo_string _algorithmToUpdate;
	typo_string _reportComparisonPath;
	bool _createProductsComparisonReport;
	bool _updateTimePlotTxt;
	bool _updateTimePlotHtml;

	typo_string _change;
	typo_string _testsTemplate;
	typo_string _performanceReportPath;
	typo_string _timePlotHTMLPath;
	bool _createProductReport;

};

class TestSpellerBenchmark
{
	//friend class SemanticSpellerImp;
public:

	TestSpellerBenchmark(Application *mf);
	~TestSpellerBenchmark();

	void createOtherReports(Timer &t );
	void updateResultsHtml(const typo_string &expected, const vector <ds_pair> &res, int ind);
	void updateStdBenchmarkResults(const typo_string &to_fix, vector <ds_pair> &cand_list,  ofstream& outfile);

	void updateStdBenchmarkResultsSentences(const typo_string &to_fix, vector <ds_pair> &cand_list,
													 ofstream& outfile);

private:
		ifstream if_expfile;
		ofstream of_not_found;
		ofstream of_autofix_succeeded;
		ofstream of_autofix_failed;
		ofstream of_first5Matches;
		ofstream of_total_found_file;
		ofstream of_copy_of_infile;
		ofstream of_copy_of_expfile;
private:
	
	void insert_raw_data_section(typo_string &report_html_content, const typo_string &in_file);
	void insert_scores_section(typo_string &report_html_content, stringstream &scores);
	void populate_scores(stringstream &scores, typo_long_num drun_time);
	//	void read_template_file(typo_string &report_html_content, const typo_string &template_filename);
	void set_algorithms_column(typo_string &s);
	typo_string getNumber();
	void createProductReport(typo_long_num drun_time, const typo_string &date);
	void createProductsComparisonReport(typo_long_num drun_time, const typo_string &date, const typo_string &in_file, const typo_string &out_filename);
	void formatLine(typo_string &line, Timer &t);
	void updateTimePlotTxt(const typo_string &line);
	void updateTimePlotHtml();
	void updateTimePlotHtmlEntry(const ss_pair &alg);
	int createReport();
	int evaluateResult(const typo_string &expected, const vector <ds_pair> &res);

	void copyFiles();

public: //private:
	int total_found;
	int in_total;
	int exp_total;
	int total;
	int dict_size;
	int total_found_1st;
	int total_found_1st_5th;
	int total_found_1st_10th;
	int total_found_1st_25th;
	int total_found_1st_50th;
	int total_found_1st_100th;
	int score;

	typo_string dictIniFilePathCopy;
	
	typo_string inPath;
	typo_string outPath;
	typo_string expPath;
	typo_string total_found_path;
	typo_string autofix_failed_path;
	typo_string not_found_path;
	typo_string autofix_succeeded_path;
	typo_string first5Matches_path;

	typo_string alg_list;

	int path_ending;
	BenchmarkTestsSection _benchmarkTestsSection;
	Application *_pSimplePipeBuilder;
	StringManipulators _sm;
	IOManipulators _iom;
};
#endif //__TESTSPELLERBENCHMARK_H__