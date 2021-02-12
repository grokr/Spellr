#include "TestSpellerBenchmark.h"
#include "TestsConstants.h"
#include "DictionaryIniConstants.h"
#include "LogConstants.h"
#include "StringManipulators.h"
#include "IOManipulators.h"
#include "ContainersManipulators.h"
#include "Timer.h"
#include "SpellerTrainer.h"
#include "Application.h"
#include "IniFileHandler.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace IniFileNamespace;


bool compare_dspair_SpellerImpTestAllAlgorithms(const ds_pair& a, const ds_pair& b) {
	return a.first > b.first;
}

void BenchmarkTestsSection::init(const typo_string &install_dir){
	try{

		_install_dir = install_dir;
		typo_string tests_filepath = _install_dir + testing_ini_filename;
		IniFile inifile(tests_filepath, false, true);

		_createProductReport = (S_TRUE == inifile[BENCHMARK_TESTS_SECTION]["createProductReport"]);
		_createProductsComparisonReport = (S_TRUE == inifile[BENCHMARK_TESTS_SECTION]["createProductsComparisonReport"]);
		_updateTimePlotTxt = (S_TRUE == inifile[BENCHMARK_TESTS_SECTION]["updateTimePlotTxt"]);
		_updateTimePlotHtml= (S_TRUE == inifile[BENCHMARK_TESTS_SECTION]["updateTimePlotHtml"]);
		_algorithmToUpdate = inifile[BENCHMARK_TESTS_SECTION]["algorithmToUpdate"];

		_timePlotFileName = inifile[BENCHMARK_TESTS_SECTION]["timePlotFileName"];
		_change=inifile[BENCHMARK_TESTS_SECTION]["change"];
		_autofix_succeeded=_install_dir + inifile[BENCHMARK_TESTS_SECTION]["autofix_succeeded"];
		_timePlotHTMLPath= _install_dir + inifile[BENCHMARK_TESTS_SECTION]["timePlotHTMLPath"];

		_performanceReportPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["performanceReportPath"];
		_reportComparisonPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["reportComparisonPath"];
		_testsTemplate = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["testsTemplate"];
		_timePlotPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["timePlotPath"];
		_inPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["inPath"];
		_outPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["outPath"];
		_expPath = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["expPath"];
		_autofix_failed = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["autofix_failed"];
		_total_found_path = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["total_found_path"];
		_not_found = _install_dir + inifile[BENCHMARK_TESTS_SECTION]["not_found"];
		_first5Matches= _install_dir + inifile[BENCHMARK_TESTS_SECTION]["first5Matches"];
	}
	catch(...)
	{
		throw runtime_error(MSG_00037);
	}
}

typo_string TestSpellerBenchmark::getNumber()
{
	typo_char buffer [33];
	_itoa_s (path_ending,buffer, 33, 10);
	path_ending++;
	return typo_string(buffer);
}
TestSpellerBenchmark::TestSpellerBenchmark(Application *mf):_pSimplePipeBuilder(mf)
{
	try
	{
		path_ending = 100;
		total_found = 0;
		in_total = 0;
		exp_total = 0;
		total = 0;
		dict_size = _pSimplePipeBuilder->getSize(DICTIONARY_INDEX);
		total_found_1st = 0;
		total_found_1st_5th = 0;
		total_found_1st_10th = 0;
		total_found_1st_25th = 0;
		total_found_1st_50th = 0;
		total_found_1st_100th = 0;
		score = 0;

		Timer timer;
		typo_string date=S_EMPTY;
		timer.get_date(date);
		StringManipulators sm;
		sm.trim(date);

		_pSimplePipeBuilder->getInstallDir(_benchmarkTestsSection._install_dir);
		_benchmarkTestsSection.init(_benchmarkTestsSection._install_dir);

		inPath  = _benchmarkTestsSection._install_dir + output_dir + copy_of_input + date + S_DOT + getNumber() + TXT_EXT;
		outPath = _benchmarkTestsSection._outPath + date + S_DOT + getNumber() + TXT_EXT;
		expPath = _benchmarkTestsSection._install_dir + output_dir + copy_of_exp + date + S_DOT + getNumber() + TXT_EXT;
		total_found_path = _benchmarkTestsSection._total_found_path +  date + S_DOT + getNumber() + TXT_EXT;
		autofix_failed_path = _benchmarkTestsSection._autofix_failed +  date + S_DOT + getNumber() + TXT_EXT;
		not_found_path = _benchmarkTestsSection._not_found +  date + S_DOT + getNumber() + TXT_EXT;
		autofix_succeeded_path = _benchmarkTestsSection._autofix_succeeded +  date + S_DOT + getNumber() + TXT_EXT;
		first5Matches_path = _benchmarkTestsSection._first5Matches +  date + S_DOT + getNumber() + TXT_EXT;
		alg_list=S_EMPTY;

		if_expfile.open(_benchmarkTestsSection._expPath.c_str());	if (false == if_expfile.good())				{ _pSimplePipeBuilder->getLog()->report(MSG_00021, expPath);					throw runtime_error(MSG_00006);}
		of_not_found.open(not_found_path.c_str());						if (false == of_not_found.good())		{ _pSimplePipeBuilder->getLog()->report(MSG_00021, not_found_path);					throw runtime_error(MSG_00006);}
		of_total_found_file.open(total_found_path.c_str());			if (false == of_total_found_file.good())	{ _pSimplePipeBuilder->getLog()->report(MSG_00021, total_found_path);			throw runtime_error(MSG_00006);}			
		of_autofix_succeeded.open(autofix_succeeded_path.c_str());		if (false == of_autofix_succeeded.good())	{ _pSimplePipeBuilder->getLog()->report(MSG_00021, autofix_succeeded_path);		throw runtime_error(MSG_00006);}
		of_autofix_failed.open(autofix_failed_path.c_str());				if (false == of_autofix_failed.good())		{ _pSimplePipeBuilder->getLog()->report(MSG_00021, autofix_failed_path);		throw runtime_error(MSG_00006);}
		of_first5Matches.open(first5Matches_path.c_str());				if (false == of_first5Matches.good())		{ _pSimplePipeBuilder->getLog()->report(MSG_00021, first5Matches_path);			throw runtime_error(MSG_00006);}


		//make a copy of algorithms.ini 
		dictIniFilePathCopy = S_SPACE + _benchmarkTestsSection._install_dir + output_dir + "\\dict_" + date + S_DOT + getNumber() + TXT_EXT;
		typo_string dict_ini_file_path = S_EMPTY;
		_pSimplePipeBuilder->getParamSetIniFilePath(dict_ini_file_path);
		
		_iom.fileCopy(dict_ini_file_path, dictIniFilePathCopy);
//#ifdef _WIN32
//		typo_string command = "copy "; command += dict_ini_file_path; command += dictIniFilePathCopy;
//		system(win_command.c_str());
//#else
//		typo_string command = "cp "; command += dict_ini_file_path; command += dictIniFilePathCopy;
//		system(unix_command.c_str());
//#endif
}
	catch(const typo_string& msg){
		_pSimplePipeBuilder->getLog()->report(msg);	
		throw runtime_error(msg);
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00008);	
		throw runtime_error(MSG_00008);
	}
}

TestSpellerBenchmark::~TestSpellerBenchmark()
{
	if_expfile.close();
	of_not_found.close();
	of_total_found_file.close();
	of_autofix_succeeded.close();
	of_autofix_failed.close();
	of_first5Matches.close();
}

void TestSpellerBenchmark::copyFiles()
{
	IOManipulators _iom;
	_iom.fileCopy(_benchmarkTestsSection._inPath, inPath); 
	_iom.fileCopy(outPath, _benchmarkTestsSection._install_dir + outputFileName);
	_iom.fileCopy(_benchmarkTestsSection._expPath, expPath);
}


void TestSpellerBenchmark::createOtherReports(Timer &timer )
{
	typo_long_num run_time = timer.get_time_lapsed();
	typo_string date=S_EMPTY;
	timer.get_date(date);
	StringManipulators sm;
	sm.format(date);

	//	int res = createReport(); 
	createProductReport(run_time, date);
	if (_benchmarkTestsSection._createProductsComparisonReport){
		typo_string reportComparisonPath = _benchmarkTestsSection._reportComparisonPath;
		createProductsComparisonReport(run_time, date, _benchmarkTestsSection._inPath, reportComparisonPath);
	}
	if (_benchmarkTestsSection._updateTimePlotTxt){
		typo_string line=S_EMPTY;
		formatLine(line, timer);
		line += "\n";
		updateTimePlotTxt(line);
	}

	if (_benchmarkTestsSection._updateTimePlotHtml){
		updateTimePlotHtml();
	}

	copyFiles();
}

int TestSpellerBenchmark::createReport()
{
	ifstream res; res.open(outPath.c_str());
	ifstream exp; exp.open((_benchmarkTestsSection._expPath).c_str());
	ofstream rep; rep.open(autofix_failed_path.c_str());

	typo_string resw;  typo_string expw;
	int ires = 0;
	while (!res.eof() && !exp.eof()){
		res >> resw; exp >> expw;
		if (resw != expw){
			rep << resw << S_SEM  << expw  <<  endl;
			ires++;
		}
	}

	res.close(); 
	exp.close(); 
	rep.close();

	return ires == 0;
}

int TestSpellerBenchmark::evaluateResult(const typo_string &expected, const vector <ds_pair> &res)
{
	vector <ds_pair>::const_iterator it=res.begin();
	for(int index = 0; it != res.end(); ++it, index++){ 
		if (expected == (*it).second)
			return index;
	}
	return RET_FFAILURE;
}


void TestSpellerBenchmark::updateTimePlotTxt(const typo_string &line){
	typo_string filename = _benchmarkTestsSection._timePlotPath + _benchmarkTestsSection._timePlotFileName;
	ifstream is; is.open(filename.c_str());
	typo_string s((istreambuf_iterator<typo_char>(is)), istreambuf_iterator<typo_char>());
	is.close();
	s += line;	 
	ofstream os; os.open(filename.c_str());
	os.write(s.c_str(), s.size()); 
	os.close();
}

void TestSpellerBenchmark::updateTimePlotHtml(){

	list<ssi_pair>::iterator it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	for(; it != _pSimplePipeBuilder->getContainerNamesList()->end(); ++it){ 
		updateTimePlotHtmlEntry((*it).first);
	}
}

void TestSpellerBenchmark::updateTimePlotHtmlEntry(const ss_pair &alg){

	stringstream score;
	int value=0;
	_pSimplePipeBuilder->getContainerNamesListWrapper()->getRating(alg, value);
	typo_long_num d = value;
	typo_long_num curAlgRating = (typo_long_num )d/(typo_long_num )total ; 
	score <<  (int)((typo_long_num )total_found*curAlgRating/(typo_long_num )total*ONE_HUNDRED_PERCENT);
	typo_string filename = _benchmarkTestsSection._timePlotHTMLPath;
	ifstream is; is.open(filename.c_str());
	typo_string s((istreambuf_iterator<typo_char>(is)), istreambuf_iterator<typo_char>());
	is.close();

	typo_string ss_alg = alg.first + S_COLON + alg.second;
	int ind_alg_list = s.find(ss_alg);
	if (ind_alg_list == -1) 
		return;
	typo_string after = s.substr(ind_alg_list, s.size()-ind_alg_list);
	int ind = after.find("data");
	int data_end = after.find("]]") + 1;
	after = after.substr(ind, data_end - ind);

	int after_test_number = after.rfind(",");
	int before_test_number = after.rfind("[");
	typo_string test_number = after.substr(before_test_number+1, after_test_number-before_test_number-1);
	stringstream to_insert;
	to_insert << "  ,[" << atoi(test_number.c_str()) + 1 << ", " << score.str() << "]";

	typo_string left = s.substr(0, ind_alg_list + data_end);
	typo_string right = s.substr(ind_alg_list + data_end, s.size() - ind_alg_list - data_end);

	stringstream output;
	output << left << to_insert.str() << right; 


	s = output.str();	
	typo_string outfilename = filename;
	ofstream os; os.open(outfilename.c_str());
	os.write(s.c_str(), s.size()); 
	os.close();
}



void TestSpellerBenchmark::updateResultsHtml(const typo_string &expected, const vector <ds_pair> &res, int ind){


	if (!res.empty() || (ind == 0)){
		//		int ind = res.size() - evaluateResult(expected, res) - 1;
		if (ind >= 0) total_found++;
		if (ind == 0) total_found_1st++;
		if (ind >= 0 && ind <  5) total_found_1st_5th++;
		if (ind >= 0 && ind < 10) total_found_1st_10th++;
		if (ind >= 0 && ind < 25) total_found_1st_25th++; 
		if (ind >= 0 && ind < 50) total_found_1st_50th++;
		if (ind >= 0 && ind < ONE_HUNDRED_PERCENT) total_found_1st_100th++;
	}
	total++;
}

void TestSpellerBenchmark::set_algorithms_column(typo_string &algList){

	//	algList = _pSimplePipeBuilder->_contCompFuncName;
	//	if (algList != S_EMPTY) return;
	//IniFile (*username_inifile)((*_pSimplePipeBuilder->app_inifile)[PATH_SECTION][LASTUSERCONFIGFILEPATH], false, true);  
	//_pSimplePipeBuilder->QueryInterface((*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["StatisticsAnalizerDefinedAlgorithmsSequence"]);

	//	if ((*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["StatisticsAnalizerDefinedAlgorithmsSequence"] != S_EMPTY)
	//		algList +=(*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["StatisticsAnalizerDefinedAlgorithmsSequence"];
	//	else if ((*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["InstallationWizardDefinedAlgorithms"] != S_EMPTY)
	//		algList +=(*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["InstallationWizardDefinedAlgorithms"];
	//	else
	//		algList=(*_pSimplePipeBuilder->username_inifile)[USER_PROFILE_APP_DATA_SECTION]["UserDefinedAlgorithmsSequence"];

	//s +="SAAS: " + upads._StatisticsAnalizerDefinedAlgorithmsSequence;
	//TBD 
	//typo_string algList=S_EMPTY;
	list<ssi_pair>::iterator alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	//	map<ss_pair, int>::iterator alg_it = _pSimplePipeBuilder->_mmssiContCompRating.begin();
	for(; alg_it != _pSimplePipeBuilder->getContainerNamesList()->end(); ++alg_it){ 
		ss_pair ss = (*alg_it).first;
		algList += ss.first + S_COLON + ss.second;
		if ((ss.first == NGRAM_INDEX) || (ss.first == LETTER_SET_INTERSECTION_LIST) ||
			(ss.first == LETTERS_MAPPING_RULES_LIST) || (ss.first == KEYBOARD_RULES_LIST))
			//algList += "(DictionaryIndex:" + (*_pSimplePipeBuilder->paramSet_inifile)[CONTAINER_TO_COMPARISON_MAP_SECTION][DICTIONARY_INDEX] + ")";			
			algList += "</br>";
	}
	//	}
	//	s +="<br />SAAS: " + upads._StatisticsAnalizerDefinedAlgorithmsSequence;
	//	s +="<br />IWAS: " + upads._InstallationWizardDefinedAlgorithms;
}

void TestSpellerBenchmark::populate_scores(stringstream &scores, typo_long_num drun_time){

	const typo_string PERCENT= "%";

	typo_string col_marker = "<td align=center>";

	///VERSION
	//	typo_string version = main_inifile[APP_DATA_SECTION]["product_version"];
	//	scores << "<tr><td><b>" << _ads._product_version << "\t";

	///SCORE
	//	scores << "<td align=center style=\"color: OrangeRed\">" << "<b>" << (int)((typo_long_num )total_found/(typo_long_num )total*100) << "%</b>";
	///FIRST MATCH
	int first=(int)(total_found_1st*ONE_HUNDRED_PERCENT/total);
	scores << col_marker << "<span id=\"FIRST\"><a href=\"" << autofix_succeeded_path << "\">" << first << "</a>" << PERCENT << "</span>"; 

	///FIRST 5 MATCHES
	scores << col_marker << "<span id=\"FIRST_FIVE\"><a href=\"" << first5Matches_path << "\">" << (int)(total_found_1st_5th*ONE_HUNDRED_PERCENT/total) << "</a>" << PERCENT << "</span>"; 

	///FIRST 10 MATCHES
	scores << col_marker << (int)(total_found_1st_10th*ONE_HUNDRED_PERCENT/total) << PERCENT;

	///FIRST 100 MATCHES
	scores << col_marker << (int)(total_found_1st_100th*ONE_HUNDRED_PERCENT/total)<< PERCENT;

	//RUN TIME
	scores << "<td align=center>" << (double)(drun_time)/(double)in_total; //	<th width=5%>Run time

	///INPUT 
	scores << col_marker << "<a href=\"" << inPath << "\">" << in_total << "</a>"; 

	///OUTPUT 
	scores << col_marker << "<a href=\"" << outPath << "\">" << total << "</a>"; 

	///EXPECTED 
	scores << col_marker << "<a href=\"" << expPath << "\">" << exp_total << "</a>"; 

	///DICT SIZE 
	//	scores << "<td align=center>" << "<a href=\"" <<  dictIniFilePathCopy <<  "\">" << dict_size  << "</a>"; 
	if (dict_size == -1) scores << col_marker << "N/A" << "</a>"; 
	else scores << col_marker << dict_size << "</a>"; 

	///NOT FOUND
	scores << col_marker << "<a href=\"" << not_found_path << "\">" << total-total_found << "</a>";

	///FOUND: ANY ITEMS FROM OUTPUT, MATCHING EXPECTED
	scores << col_marker << "<a href=\"" << total_found_path << "\">" << total_found << "</a>"; 

	//ANY MATCH BUT FIRST
	scores << col_marker << "<a href=\"" << autofix_failed_path << "\">" << (total_found - total_found_1st)*ONE_HUNDRED_PERCENT/total << "</a>" << PERCENT; 

	///FIRST 25 MATCHES
	scores << col_marker << total_found_1st_25th*ONE_HUNDRED_PERCENT/total << PERCENT;

	///FIRST 50 MATCHES
	scores << col_marker << total_found_1st_50th*ONE_HUNDRED_PERCENT/total<< PERCENT;


}

void TestSpellerBenchmark::insert_scores_section(typo_string &report_html_content, stringstream &scores){

	int ipos = report_html_content.find(HTML_REPORT_PATTERN);
	if (ipos == -1){
		_pSimplePipeBuilder->getLog()->report(MSG_00002, _benchmarkTestsSection._performanceReportPath); 
		return;
	}
	ipos += HTML_REPORT_PATTERN.size();

	typo_string before = report_html_content.substr(0, ipos);//452 // 817
	typo_string after = report_html_content.substr(ipos, report_html_content.size()-ipos);//818
	report_html_content = before + scores.str() + after;
}


void TestSpellerBenchmark::insert_raw_data_section(typo_string &report_html_content, const typo_string &in_file)
{
	typo_string sRowHeader = "<h2>The Raw Data</h2>";
	int pos = report_html_content.find(sRowHeader) + sRowHeader.size()+5;
	typo_string test_data = "<li><a href=\"" + in_file + "\">Test Data</a>";
	typo_string in_dir = in_file.substr(0, in_file.rfind(S_BACK_SLASH));
	test_data += "<li><a href=\"" + in_dir + "\">Results, Scripts and Raw Data</a>";
	report_html_content.insert(pos, test_data.c_str(), test_data.size());
}
void TestSpellerBenchmark::createProductReport(typo_long_num drun_time, const typo_string &date)
{

	if (total ==0 ) return;

	stringstream scores (stringstream::in | stringstream::out);

	typo_string perfReportFilePath=S_EMPTY;
	_pSimplePipeBuilder->getPerfReportFilePath(perfReportFilePath);
	if (perfReportFilePath == S_EMPTY)
		perfReportFilePath = (_benchmarkTestsSection._performanceReportPath);

	ifstream is; is.open(perfReportFilePath.c_str());
	if ( false == is.good()){	_pSimplePipeBuilder->getLog()->report(MSG_00009, perfReportFilePath.c_str());	throw runtime_error(MSG_00006);	}
	if ( false == is.is_open() ) {_pSimplePipeBuilder->getLog()->report(MSG_00009, perfReportFilePath.c_str());	throw runtime_error(MSG_00006);	}

	typo_string report_html_content((istreambuf_iterator<typo_char>(is)), istreambuf_iterator<typo_char>());
	int SpellingsSuggestionsListSize = _pSimplePipeBuilder->getSpellingsSuggestionsListSize();

	typo_string config="RELEASE";
#ifdef _DEBUG
	config="DEBUG";
#endif

	set_algorithms_column(alg_list);


	populate_scores(scores, drun_time);

	scores << "<td align=center>" << config; //	<th width=5%>Config //debug/release

	scores << "<td align=center>" << "<a href=\"" <<  dictIniFilePathCopy <<  "\">" << SpellingsSuggestionsListSize  << "</a>"; 
	//scores << "<td align=center>" << SpellingsSuggestionsListSize; //<th width=5%>List Size

	scores << "<td align=center>" << date; //<th width=15%>Date-Time

	scores << "<td align=left>" << alg_list; // <th width=13%>Algorithm

	//	get_change_description(_benchmarkTestsSection._change);
	scores << "<td align=left>" << _benchmarkTestsSection._change; // <th width=13%>Algorithm

	//closes table
	scores << "<tr>";

	insert_scores_section(report_html_content, scores);

	//	insert_raw_data_section(report_html_content, in_file);

	report_html_content = report_html_content.substr(0, report_html_content.rfind("</html>"));

	//write output
	ofstream os; os.open(perfReportFilePath.c_str());
	os.write(report_html_content.c_str(), report_html_content.size());
	os.close();

	is.close();
}

//void TestSpellerBenchmark::get_change_description()
void TestSpellerBenchmark::createProductsComparisonReport(typo_long_num drun_time, const typo_string &date, const typo_string &in_file, 
														  const typo_string &out_filename)
{
	typo_string alg_list=S_EMPTY;
	stringstream scores (stringstream::in | stringstream::out);
	ifstream is; is.open((_benchmarkTestsSection._testsTemplate).c_str());
	typo_string report_html_content((istreambuf_iterator<typo_char>(is)), istreambuf_iterator<typo_char>());

	set_algorithms_column(alg_list);

	populate_scores(scores, drun_time);

	scores << "<td align=left>" << alg_list;

	//add date-time  : <td align=center>Feb-09
	scores << "<td align=left>" << date;

	scores << "<tr>" ;

	insert_scores_section(report_html_content, scores);

	insert_raw_data_section(report_html_content, in_file);

	report_html_content = report_html_content.substr(0, report_html_content.rfind("</html>"));

	ofstream os; os.open(out_filename.c_str());
	os.write(report_html_content.c_str(), report_html_content.size());
	os.close();
	is.close();
}

void TestSpellerBenchmark::formatLine(typo_string &line, Timer &timer){

	typo_string date=S_EMPTY;
	timer.get_date_time_plot(date);

	int iKeyboardRulesList=0, iLetterSetIntersection=0, 
		iLettersMappingRulesList=0, iNGramIndex=0,
		iWordsMappingRulesList=0;

	if ( _benchmarkTestsSection._algorithmToUpdate == LETTER_SET_INTERSECTION_LIST )
		iLetterSetIntersection = TestSpellerBenchmark::score;
	else if ( _benchmarkTestsSection._algorithmToUpdate == LETTERS_MAPPING_RULES_LIST )
		iLettersMappingRulesList = TestSpellerBenchmark::score;
	else if ( _benchmarkTestsSection._algorithmToUpdate == NGRAM_INDEX )
		iNGramIndex = TestSpellerBenchmark::score;
	else if ( _benchmarkTestsSection._algorithmToUpdate == WORDS_MAPPING_RULES_LIST )
		iWordsMappingRulesList = TestSpellerBenchmark::score;
	else if (_benchmarkTestsSection._algorithmToUpdate == KEYBOARD_RULES_LIST )
		iKeyboardRulesList = TestSpellerBenchmark::score;

	stringstream ssline (stringstream::in | stringstream::out);
	ssline << date << S_COMA << iKeyboardRulesList << S_COMA << 
		iLetterSetIntersection << S_COMA << iLettersMappingRulesList << S_COMA << iNGramIndex  << S_COMA << iWordsMappingRulesList;
	line = ssline.str();
}

//moved from TestSyntacticSpeller.cpp
void TestSpellerBenchmark::updateStdBenchmarkResults(const typo_string &to_fix, vector <ds_pair> &cand_list,
													 ofstream& outfile)
{
	in_total++;

	const typo_string IN=_T("IN");
	const typo_string OUT=_T("OUT");

	typo_string first = S_EMPTY;
	typo_string expected = S_EMPTY;
	ContainersManipulators _cm;

	if_expfile >> expected; 
	_sm.trim_SpellerImpTestAllAlgorithms(expected, OUT); 
	of_copy_of_expfile << expected << endl;

	vector<string> expected_vector;
	if (_sm.has_punctuation(expected))
		_sm.parseStringAppendToVector(expected, C_COMA, expected_vector);
	else 
		expected_vector.push_back(expected);

	typo_long_num similarity = D_ZERO;
	int ind = -1;
	if (cand_list.empty()) 	{
		first = to_fix;
		cand_list.push_back(ds_pair(D_HUNDRED,to_fix));
	}
	else{
		sort(cand_list.begin(), cand_list.end(), compare_dspair_SpellerImpTestAllAlgorithms);
		first = (cand_list.front()).second;
	}
	ind = _cm.findMinimalExpectedIndex(cand_list, expected_vector);

	outfile << first << C_SPACE; 
	exp_total++;

	if (ind == -1){
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00003] << to_fix ;
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00004] << expected;
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00006] << first << endl; 
	}
	else{
		if (ind == 0){ 
			of_autofix_succeeded << expected << endl; 
		}
		if (ind >= 0 && ind < 5) {
			of_first5Matches << expected << endl; 
		}
		if (ind > 0) 
		{
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00003] << to_fix ;
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00004] << expected;
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00006] << first; 
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00007] << ind << endl; 
		}
		of_total_found_file<< expected << endl; 
	}

	updateResultsHtml(expected, cand_list, ind);
}


void TestSpellerBenchmark::updateStdBenchmarkResultsSentences(const typo_string &to_fix, 
															vector <ds_pair> &cand_list,
													 ofstream& outfile)
{
	in_total++;

	const typo_string IN=_T("IN");
	const typo_string OUT=_T("OUT");

	typo_string first = S_EMPTY;
	typo_string expected = S_EMPTY;
	ContainersManipulators _cm;

	if_expfile >> expected; 
	_sm.trim_SpellerImpTestAllAlgorithms(expected, OUT); 
	of_copy_of_expfile << expected << endl;

	typo_string expected_no_punctuation = expected, punctuation = S_EMPTY;
	_sm.extract_punctuation(expected_no_punctuation, punctuation);

	int end_of_sentence = expected_no_punctuation.find_first_of(SENTENCE_ENDINGS);
	if (end_of_sentence != typo_string::npos){
		_sm.extract_sentence_ending(expected_no_punctuation, punctuation);
		if (end_of_sentence == 0){ // case: "...nothing."  "nothing." returns to stream
			for (unsigned int i=0; i< expected_no_punctuation.size();i++) if_expfile.unget();// << next;
		}
	}

	typo_string punctuation2 = S_EMPTY, to_fix_copy=to_fix;
	_sm.extract_punctuation(to_fix_copy, punctuation2);
	if (punctuation2 == S_EMPTY)
		_sm.extract_sentence_ending(to_fix_copy, punctuation2);

	typo_long_num similarity = D_ZERO;
	int ind = -1;
	if (cand_list.empty()) 	{
		first = to_fix;
		if (to_fix_copy == expected_no_punctuation)
			ind = 0;
	}
	else{
		sort(cand_list.begin(), cand_list.end(), compare_dspair_SpellerImpTestAllAlgorithms);

		if ((cand_list.front()).first >= (cand_list.back()).first){
			first = (cand_list.front()).second + punctuation2;
			similarity = (cand_list.front()).first; 
			ind = _cm.findIndex(cand_list, expected_no_punctuation);
		}
		else{
			first = (cand_list.back()).second + punctuation2;
			similarity = (cand_list.back()).first;
			ind = _cm.rfindIndex(cand_list, expected_no_punctuation);
		}
	}

	outfile << first << C_SPACE; 
	exp_total++;

	if (ind == -1){
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00003] << to_fix ;
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00004] << expected;
		of_not_found << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00006] << first << endl; 
	}
	else{
		if (ind == 0){ 
			of_autofix_succeeded << expected << endl; 
		}
		if (ind >= 0 && ind < 5) {
			of_first5Matches << expected << endl; 
		}
		if (ind > 0) 
		{
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00003] << to_fix ;
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00004] << expected;
			of_autofix_failed << (*(_pSimplePipeBuilder->report_inifile))[TEXT_CONSTANTS_SECTION][TXT_ID_00006] << first << endl; 
		}
		of_total_found_file<< expected << endl; 
	}

	updateResultsHtml(expected, cand_list, ind);
}
