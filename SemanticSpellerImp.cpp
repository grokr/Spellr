#include "SemanticSpellerImp.h"
#include "SentenceNGramIndex.h"
#include "Application.h"
#include "DictionaryIndex.h" 
#include "LogConstants.h"
#include "Utilities.h"


#include <algorithm>

using namespace IniFileNamespace;

typedef list<IAbstractProduct *> ContainerPointersList;

bool compare_dspair_TestSemanticSpeller(const ds_pair& a, const ds_pair& b) {
	return a.first > b.first;
}

SemanticSpellerImp::SemanticSpellerImp(Application *mp):
_pSimplePipeBuilder(mp), _tsb(mp), _pDict(NULL), _pSentNGram(NULL), pOutput(NULL){
	try{

		typo_string language = S_EMPTY;
		_pSimplePipeBuilder->getUILanguage(language); 
		_SafeFall = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][SAFE_FALL]).c_str());
		_pDict=static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
		if (_pDict == NULL) throw runtime_error(MSG_00006);
		_tsb.dict_size = _pDict->getSize();

		_toSpellBySentences = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][TO_SPELL_BY_SENTENCES]).c_str());
		_toSpellWordsStream = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][TO_SPELL_WORDS_STREAM]).c_str());

		_filterOutNonSynonyms= atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][FILTER_OUT_NON_SYNONYMS]).c_str());
		_finalStage = true;
		_toFixSemanticErrors = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][TO_FIX_SEMANTIC_ERRORS]).c_str());
		_toFixSyntacticErrors = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][TO_FIX_SYNTACTYC_ERRORS]).c_str());
		_runSemanticTwice = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS_SECTION][RUN_SEMANTIC_ALGORITHM_TWICE]).c_str());
		_runSemanticTwiceCounter = 0;

		_pSentNGram = static_cast<SentenceNGramIndex *>(_pSimplePipeBuilder->getContainer(SENTENCE_NGRAM_INDEX, SYNTACTIC_SIMILARITY));
	}
	catch(const typo_string& msg){
		_pSimplePipeBuilder->getLog()->report(msg);	
		throw runtime_error(MSG_00038);
	}		
}

int SemanticSpellerImp::openFiles(const typo_string &input_file, const typo_string &output_file)
{
/*
	expfile.open(_tsb._benchmarkTestsSection._expPath.c_str());	if (false == expfile.good())			{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb._benchmarkTestsSection._expPath);	throw runtime_error(MSG_00006);}
	not_found.open(_tsb.not_found.c_str());					if (false == not_found.good())				{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb.not_found);							throw runtime_error(MSG_00006);}
	total_found_file.open(_tsb.total_found_path.c_str());	if (false == total_found_file.good())		{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb.total_found_path);					throw runtime_error(MSG_00006);}			
	autofix_succeeded.open(_tsb.autofix_succeeded.c_str());	if (false == autofix_succeeded.good())		{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb.autofix_succeeded);					throw runtime_error(MSG_00006);}
	autofix_failed.open(_tsb.autofix_failed.c_str());		if (false == autofix_failed.good())			{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb.autofix_failed);					throw runtime_error(MSG_00006);}
	first5Matches.open(_tsb.first5Matches.c_str());			if (false == first5Matches.good())			{ _pSimplePipeBuilder->getLog()->report(MSG_00021, _tsb.first5Matches);						throw runtime_error(MSG_00006);}
*/
	outfile.open(_tsb.outPath.c_str()); 
	outfile.clear();

	infile.open(input_file.c_str());	
	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00009, input_file.c_str());	
		return RET_FFAILURE;
	}

	if (false == outfile.good()){ 
		_pSimplePipeBuilder->getLog()->report(MSG_00009, _tsb.outPath.c_str());	
		return RET_FFAILURE;
	}
	outfile.clear();

	return RET_FSUCCESS;
}

void SemanticSpellerImp::closeFiles()
{
	infile.seekg(0);
	infile.close();
	outfile.close();
/*	expfile.close();
	not_found.close();
	total_found_file.close();
	autofix_succeeded.close();
	autofix_failed.close();
	first5Matches.close();
	*/
}

SemanticSpellerImp::~SemanticSpellerImp()
{
	closeFiles();
}


int SemanticSpellerImp::do_spell_file(const typo_string &input_file, const typo_string &output_file)
{
	#ifdef BENCHMARK_TESTING_MODE
		return do_spell_file_benchmark(input_file, output_file);
	#endif

	try{
		if (RET_FFAILURE == openFiles(input_file, output_file)) return RET_FFAILURE;
		
		do_do_spell_file(input_file, output_file);
		
		closeFiles();
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00028);	
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}


#ifdef BENCHMARK_TESTING_MODE
int SemanticSpellerImp::do_spell_file_benchmark(const typo_string &input_file, const typo_string &output_file)
{
	typo_string to_fix = S_EMPTY;
	typo_string expected = S_EMPTY;
	
	try{
		if (RET_FFAILURE == openFiles(input_file, output_file)) return RET_FFAILURE;
		Timer timer;
		timer.timer_start();
		do_do_spell_file(input_file, output_file);

		_tsb.createOtherReports(timer);

		closeFiles();
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00028);	
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}
#endif

int SemanticSpellerImp::do_do_spell_file(const typo_string &input_file, const typo_string &output_file)
{
	typo_string to_fix = S_EMPTY;
	typo_string expected = S_EMPTY;
	
	try{

		bool _first_line = true;

		if (_toSpellWordsStream){
			infile >> to_fix;
			_sm.trim_SpellerImpTestAllAlgorithms(to_fix, IN);
			vector<ds_pair> cand_list;
			do_spell_word(to_fix, cand_list);
			typo_string prev = to_fix;

			spell_words_stream_semantically(prev);
		}
		else if (_toSpellBySentences){
			spell_sentences();
		}


		if (_pSimplePipeBuilder->isStdOut()){

			typo_string content;
			iom.io_read(_tsb.outPath, content);
		}

		infile.seekg(0);
		infile.close();

		outfile.close();

		//TBD: commented to test error : can not open input file for second time
		iom.fileCopy(input_file, _tsb.inPath);
		iom.fileCopy(_tsb.outPath, _tsb._benchmarkTestsSection._install_dir + outputFileName);
		iom.fileCopy(_tsb._benchmarkTestsSection._expPath, _tsb.expPath);
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00028);	
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}

int SemanticSpellerImp::make_plural_or_single(typo_string &to_fix)
{
	typo_string lang;
	_pSimplePipeBuilder->getCurrentInputLanguage(lang);
	if (lang == ISO639_1_EN)
	{
		if (*(to_fix.end()) == 's')
			to_fix=to_fix.substr(0, to_fix.size()-1);
		else
			to_fix+='s';
	}
	return RET_FSUCCESS;
}

//* HAVE TO REWRITE!!!!!!!!!!!
int SemanticSpellerImp::do_spell_word(typo_string &to_fix, vector<ds_pair> &cand_list)
{
	//TAKE IT OUT!!!
	const int SPELL_SENTENCES = 0;
	const int MINIMAL_WORD_SIZE=2;


	list<ssi_pair>::iterator alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	IDictionary *pit = NULL;
	Filter f0(D_HALF);

	//TxtOutput txtOutput(outFilePath + txtOutFileName);
	//XmlOutput xmlOutput(outFilePath + xmlOutFileName);
	typo_string install_dir; 
	_pSimplePipeBuilder->getInstallDir(install_dir);
	//AspellOutput outputter;
	//	if (_pSimplePipeBuilder->_output_type == XML_OUT)
	//		pOutput = new XmlOutput( install_dir + output_dir + output_xml_filename);
	//	else 
	if (_pSimplePipeBuilder->isStdOut())
		pOutput = new Output( install_dir + output_dir + output_txt_filename);
	//	else if (_pSimplePipeBuilder->_output_type == ASPELL_OUT)
	//		pOutput = new AspellOutput( install_dir + output_dir + output_aspell_filename);

	//	TxtOutput txtOutput( install_dir + output_dir + output_txt_filename);
	//	XmlOutput xmlOutput( install_dir + output_dir + output_xml_filename);

	if ((to_fix.size() < MINIMAL_WORD_SIZE) || (_pDict->find(to_fix) != RET_FIND_NOT_FOUND)) {

		//make_plural_or_single(to_fix);
		if (_pDict->find(to_fix) != RET_FIND_NOT_FOUND)
		{
			cand_list.push_back(ds_pair(D_HUNDRED, to_fix));

			if (false == _finalStage) return  RET_FFAILURE2;

			//			pOutput->update(to_fix, cand_list);
			//			if (_pSimplePipeBuilder->_output_type == ASPELL_OUT)
			//				pOutput->update(to_fix, cand_list);
			//			else if (_pSimplePipeBuilder->_output_type == XML_OUT)
			//				pOutput->update(to_fix, cand_list);
			//			else {
#ifdef BENCHMARK_TESTING_MODE
			if (SPELL_SENTENCES)	
				_tsb.updateStdBenchmarkResultsSentences(to_fix, cand_list, outfile);
			else
				_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);
#endif

//#ifdef BENCHMARK_TESTING_MODE
//			_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);
//#endif

			//			}
			return RET_FFAILURE2;
		}
	} 

	//add: if _pSimplePipeBuilder->getContainerNamesList() has LETTERS_MAPPING_RULES_LIST make this init
	//	IDictionary *pNGram = static_cast<IDictionary *>(_pSimplePipeBuilder->getContainer(NGRAM_INDEX, NGRAM_SIMILARITY));

	for (alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin(); alg_it != _pSimplePipeBuilder->getContainerNamesList()->end(); ++alg_it)
	{	//added second argument, test it; maybe better S_EMPTY
		pit = static_cast<IDictionary *>(_pSimplePipeBuilder->getContainer((*alg_it).first.first, (*alg_it).first.second));

		vector<ds_pair> previous_cand_list; //save results from previous algorithms
		if (!cand_list.empty()){
			previous_cand_list=cand_list;
			cand_list.erase(cand_list.begin(), cand_list.end());
		}

		if (pit->getSuggestions(to_fix, cand_list, f0) == RET_FFAILURE){ 	
			//TODO return this(better error handeling)	 //_cm.remove(_pSimplePipeBuilder->getContainerPointersList(), pit);	//continue;		
		} 	

		_cm.append<ds_pair>(cand_list, previous_cand_list); //append results from previous algorithms

		typo_string current_id = pit->getID();
		_pDict->filterOutSuggestions(to_fix, cand_list, f0);
	}
	if (_SafeFall && cand_list.empty()){ //safe fall: none algorithm succeeded or no algorithm defined or valid
		_pDict->getSuggestions(to_fix, cand_list, f0);
	}

	Utilities ut;
	ut.factorFrequencies(cand_list);
	if (false == _finalStage) return  RET_FSUCCESS;
	//ut.pickup_best_suggestions(cand_list, 50);

	//	if (_pSimplePipeBuilder->_output_type == ASPELL_OUT)
	//		pOutput->update(to_fix, cand_list);
	//	else if (_pSimplePipeBuilder->_output_type == XML_OUT)
	//		outputter.addXmlSection(to_fix, cand_list, outfile);
	//	else 
	//		outputter.updateStdResults(to_fix, cand_list, outfile);
	_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);

	return RET_FSUCCESS;
}



int SemanticSpellerImp::fetch_sentence(stringstream& sentence, typo_string& ending)
{
	typo_string next;

	while (!infile.eof())
	{	
		infile >> next;
		if (next == S_EMPTY) 
			return RET_FSUCCESS;
		int end_of_sentence = next.find_first_of(SENTENCE_ENDINGS);
		if (end_of_sentence != typo_string::npos){
			_sm.extract_sentence_ending(next, ending);
			if (end_of_sentence == 0){ // case: "...nothing."  "nothing." returns to stream
				for (unsigned int i=0; i< next.size();i++) infile.unget();// << next;
				return RET_FSUCCESS;
			}
			sentence << next << C_SPACE;

			typo_string return_to_stream = S_EMPTY;
			int lstrip = ending.find_first_of(UC_ALPHABET_EN_LOWER);
			if (lstrip != typo_string::npos){
				return_to_stream = ending.substr(lstrip, ending.size()-1);
				ending = ending.substr(0, lstrip);
				for (unsigned int i=0; i< return_to_stream.size()-1;i++) infile.unget();
				//infile << return_to_stream;
			}
			return RET_FSUCCESS;
		}
		sentence << next << C_SPACE;
	}
	sentence >> next;
	if (next.empty())
		return RET_FFAILURE;
	else
		for (unsigned int i=0; i< next.size();i++) sentence.unget();
	return RET_FSUCCESS;
}

int SemanticSpellerImp::spell_sentences()
{
	const typo_string IN=_T("IN");
	const typo_string OUT=_T("OUT");

	typo_string ending = S_EMPTY;

	while (1)
	{
		stringstream in_sentence, out_sentence, out2_sentence; 
		if (fetch_sentence(in_sentence, ending) != RET_FSUCCESS)
			break;
		if (!_runSemanticTwice){
			_runSemanticTwiceCounter = 0;
			do_spell_sentence_semantically(in_sentence, out_sentence, ending);
		}
		else
		{
			_runSemanticTwiceCounter = 1;
			do_spell_sentence_semantically(in_sentence, out_sentence, ending);
			_runSemanticTwiceCounter = 0;
			do_spell_sentence_semantically(out_sentence, out2_sentence, ending);
		}
		ending = S_EMPTY;
	}

	return RET_FSUCCESS;
}


int SemanticSpellerImp::init_the_first_triple( stringstream&  sentence, stringstream&  fixed_sentence, 
											  sss_pair& currentElement,
											  //typo_string& next_word, 
											  const typo_string& sent_ending)
{
	vector<ds_pair> cand_list;
	sentence >> currentElement.first.first; 
	sentence >> currentElement.first.second; 
	sentence >> currentElement.second; 
	if (currentElement.first.first == S_EMPTY){
		if (sent_ending != S_EMPTY){
			if (_runSemanticTwiceCounter)
				fixed_sentence << sent_ending;
			else
				_tsb.updateStdBenchmarkResults(sent_ending, cand_list, outfile);
		}
		return RET_FSUCCESS;
	}
	return RET_FFAILURE2;
}

int SemanticSpellerImp::add_the_first_word_without_test(sss_pair& currentElement,
														stringstream& fixed_sentence, const typo_string& sent_ending)
{	
	vector<ds_pair> cand_list;
	typo_string to_fix = currentElement.first.first;
	typo_string punctuation;
	_sm.extract_punctuation(to_fix, punctuation);
	cand_list.push_back(ds_pair(D_ZERO, to_fix));
	if (currentElement.first.second == S_EMPTY) {
		if (_runSemanticTwiceCounter)
			fixed_sentence << cand_list.front().second;
		else
			_tsb.updateStdBenchmarkResults(currentElement.first.first + sent_ending, cand_list, outfile); //the first word is not tested
		return RET_FSUCCESS;
	}
	if (_runSemanticTwiceCounter)
		fixed_sentence << cand_list.front().second;
	else
		_tsb.updateStdBenchmarkResults(currentElement.first.first, cand_list, outfile); //the first word is not tested

	return RET_FFAILURE2;
}



int SemanticSpellerImp::process_the_first_triple( sss_pair& currentElement, 
												 sss_pair& previousElement, stringstream& fixed_sentence, typo_string&  next_word, 
												 const typo_string& sent_ending, int &break_condition)
{
	typo_string punctuation = S_EMPTY;
	vector<ds_pair> cand_list;
	previousElement.first.first = S_EMPTY;
	typo_string to_fix = currentElement.first.second;
	_finalStage = false;
	_sm.extract_punctuation(to_fix, punctuation);

	if (_toFixSemanticErrors)
		_pSentNGram->getSuggestions(currentElement, previousElement, next_word, cand_list);

	if (_pDict->find(to_fix) == RET_FIND_NOT_FOUND){
		Filter f0;
		if (cand_list.size()>0)
			_pDict->filterOutSuggestions(to_fix, cand_list, f0);
	}

	sort(cand_list.begin(), cand_list.end(), compare_dspair_TestSemanticSpeller);

	if(next_word == S_EMPTY)
		break_condition = 0;
	if (currentElement.second == S_EMPTY){
		if (_runSemanticTwiceCounter)
			fixed_sentence << C_SPACE << cand_list.front().second;
		else
			_tsb.updateStdBenchmarkResults(to_fix + sent_ending, cand_list, outfile);

		return RET_FSUCCESS;
	}

	if (_runSemanticTwiceCounter)
		fixed_sentence << C_SPACE << cand_list.front().second;
	else
		_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);

	return RET_FFAILURE2;
}

void SemanticSpellerImp::add_the_last_word_without_test(sss_pair& previousElement, stringstream& fixed_sentence, 
														const typo_string& sent_ending)
{
	vector<ds_pair> cand_list;
	cand_list.push_back(ds_pair(D_ZERO, previousElement.second));
	if (_runSemanticTwiceCounter)
		fixed_sentence << C_SPACE << cand_list.front().second;
	else
		_tsb.updateStdBenchmarkResults(previousElement.second + sent_ending, cand_list, outfile); //the first word is not tested
}

void SemanticSpellerImp::do_do_spell_sentence_semantically(sss_pair& currentElement, sss_pair& previousElement,
														   stringstream& sentence, stringstream& fixed_sentence,
														   typo_string&  next_word, int & break_condition)
{
	vector<ds_pair> cand_list;

	//init the next triple
	typo_string next= S_EMPTY; 
	currentElement.first.first = previousElement.first.second; 
	currentElement.first.second = previousElement.second; 
	currentElement.second = next_word; 

	sentence >> next;
	if (next == S_EMPTY) 
		break_condition = 0; 
	next_word = next; 

	typo_string to_fix = currentElement.first.second, punctuation = S_EMPTY;
	_sm.extract_punctuation(to_fix, punctuation);

	if (_toFixSemanticErrors)
		_pSentNGram->getSuggestions(currentElement, previousElement, next_word, cand_list);
	if (_pDict->find(to_fix) == RET_FIND_NOT_FOUND){
		Filter f0;
		if (cand_list.size()>0)
			_pDict->filterOutSuggestions(to_fix, cand_list, f0);
	}

	sort(cand_list.begin(), cand_list.end(), compare_dspair_TestSemanticSpeller);

	if (_runSemanticTwiceCounter){
		fixed_sentence << C_SPACE << cand_list.front().second; 
	}
	else
		_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);

	previousElement=currentElement;
}

int SemanticSpellerImp::set_previous_element(sss_pair& currentElement, sss_pair& previousElement, 
											 stringstream& sentence, stringstream& fixed_sentence,
											 typo_string& next_word,
											 int &break_condition, const typo_string& sent_ending)
{
	if(RET_FSUCCESS == init_the_first_triple(sentence, fixed_sentence,currentElement, sent_ending)) 
		return RET_FSUCCESS;
	if(RET_FSUCCESS == add_the_first_word_without_test(currentElement, fixed_sentence, sent_ending)) 
		return RET_FSUCCESS;
	sentence >> next_word;
	if(RET_FSUCCESS == process_the_first_triple(currentElement, previousElement, fixed_sentence, next_word, sent_ending, break_condition)) 
		return RET_FSUCCESS;
	previousElement=currentElement;
	return RET_FFAILURE2;
}

int SemanticSpellerImp::do_spell_sentence_semantically( stringstream& sentence, stringstream& fixed_sentence, 
													   const typo_string&  sent_ending)
{
	typo_string next_word= S_EMPTY, to_fix= S_EMPTY;
	sss_pair previousElement, currentElement;
	int break_condition = 1;

	set_previous_element(currentElement, previousElement, sentence, fixed_sentence, next_word, break_condition, sent_ending);

	while(break_condition)
		do_do_spell_sentence_semantically(currentElement, previousElement, sentence, fixed_sentence, next_word,  break_condition);

	add_the_last_word_without_test(previousElement, fixed_sentence, sent_ending);

	return RET_FSUCCESS;
}

int SemanticSpellerImp::spell_words_stream_semantically(const typo_string& prev)
{
	typo_string punctuation = S_EMPTY;
	sss_pair firstElement; 
	sss_pair previousElement; 
	previousElement.first.first = S_EMPTY;

	firstElement.first.first = prev; infile >> firstElement.first.second; infile >> firstElement.second;
	_sm.trim_SpellerImpTestAllAlgorithms(firstElement.first.first, IN);

	vector<ds_pair> cand_list;
	typo_string to_fix = firstElement.first.second;
	int ret = _pSentNGram->getSuggestions(firstElement, previousElement, S_EMPTY, cand_list);
	_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);

	while (!infile.eof())
	{	
		vector<ds_pair> cand_list;

		sss_pair currentElement =firstElement ; 
		currentElement.first.first=firstElement.first.second;
		currentElement.first.second = firstElement.second;
		infile >> currentElement.second;
		typo_string to_fix = currentElement.first.second;

		firstElement=currentElement;

		_pSimplePipeBuilder->resetContainerPtr(); //re-init
		if ((currentElement.first.first == S_EMPTY) && (currentElement.second == S_EMPTY) && (currentElement.first.second == S_EMPTY)) 
			break;
		ret = _pSentNGram->getSuggestions(firstElement, previousElement, S_EMPTY, cand_list);

		if (_filterOutNonSynonyms)
			ret = _pSentNGram->filterOutNonSynonyms(to_fix, cand_list);

		_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);
	}

	return RET_FSUCCESS;
}
