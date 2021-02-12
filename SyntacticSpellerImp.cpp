#include "SyntacticSpellerImp.h"
#include "SentenceNGramIndex.h"
#include "Application.h"
#include "DictionaryIndex.h" 
#include "LogConstants.h"
#include "Utilities.h"

#include <algorithm>

using namespace IniFileNamespace;

	typedef list<IAbstractProduct *> ContainerPointersList;
	
	SyntacticSpellerImp::SyntacticSpellerImp(Application *mp):
			_pSimplePipeBuilder(mp), _pDict(NULL), _tsb(mp) {
		try{
			outfile.open(_tsb.outPath.c_str()); 
			
			typo_string install_dir; 
			_pSimplePipeBuilder->getInstallDir(install_dir);
			typo_string xml_file_name = install_dir + output_dir + output_xml_filename;
			_xml_output.init(xml_file_name);
			
				typo_string language = S_EMPTY;
			_pSimplePipeBuilder->getUILanguage(language); 
			_SafeFall = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[SEMANTICSPELLERCONSTANTS_SECTION][SAFE_FALL]).c_str());
			_pDict=static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
			if (_pDict == NULL) throw runtime_error(MSG_00006);
			
		}
		catch(const typo_string& msg){
			_pSimplePipeBuilder->getLog()->report(msg);	
			throw runtime_error(MSG_00038);
		}		
	}

SyntacticSpellerImp::~SyntacticSpellerImp(){}

int SyntacticSpellerImp::add_correction(const typo_string &word, const typo_string &fix)
{
//	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(WORDS_MAPPING_RULES_LIST, S_EMPTY)))->add_correction(word, fix);
	
	//update WORDS_MAPPING_RULES_LIST
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(WORDS_MAPPING_RULES_LIST, S_EMPTY)))->add_correction(word, fix);

	//update letters mappings rules and their frequencies
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(LETTERS_MAPPING_RULES_LIST, S_EMPTY)))->add_correction(word, fix);

	//update dictionary frequencies
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY)))->add_correction(word, fix);

	//update parameters.ini
	_pSimplePipeBuilder->add_correction(word, fix);

	return RET_FSUCCESS;
}
int SyntacticSpellerImp::add_correction(const typo_string &file)
{
	//update WORDS_MAPPING_RULES_LIST
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(WORDS_MAPPING_RULES_LIST, S_EMPTY)))->add_correction(file);

	//update letters mappings rules and their frequencies
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(LETTERS_MAPPING_RULES_LIST, S_EMPTY)))->add_correction(file);

	//update dictionary frequencies
//	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY)))->add_correction(file); //"x","y"

	//update parameters.ini
	_pSimplePipeBuilder->add_correction(file);

	return RET_FSUCCESS;
}
int SyntacticSpellerImp::remove_correction(const typo_string &word, const typo_string &fix){
	(static_cast<IRulesList *>(_pSimplePipeBuilder->getContainer(WORDS_MAPPING_RULES_LIST, S_EMPTY)))->remove_correction(word, fix);
		return RET_FSUCCESS;
}

int SyntacticSpellerImp::add_word(const typo_string &word){
	(static_cast<IIndexedDictionary *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY)))->add_word(word);
		return RET_FSUCCESS;
}

int SyntacticSpellerImp::remove_word(const typo_string &word){
	(static_cast<IIndexedDictionary *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY)))->remove_word(word);
		return RET_FSUCCESS;
}

void SyntacticSpellerImp::get_version(typo_string &version) const{ 
	try{ 
		version = atoi((*_pSimplePipeBuilder->app_inifile)[APP_DATA_SECTION][PRODUCT_VERSION].c_str());
	}
	catch(...){
		_pSimplePipeBuilder->getLog()->report(MSG_00019, _pSimplePipeBuilder->INSTALL_DIR + APP_INI_FILENAME);
	}
}

//TBD 
void SyntacticSpellerImp::get_dictionary_encoding(typo_string &enc) const{
	//enc=_T("ISO8859-1");
}

//TBD
int SyntacticSpellerImp::add_dictionary(const typo_string &dpath){ 
	return RET_FSUCCESS; 
}

int SyntacticSpellerImp::openFiles(const typo_string &input_file, const typo_string &output_file)
{
	infile.open(input_file.c_str());	
	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00009, input_file.c_str());	
		return RET_FFAILURE;
	}

	if (false == outfile.good()){ 
		_pSimplePipeBuilder->getLog()->report(MSG_00009, output_file.c_str());	
		return RET_FFAILURE;
	}
	outfile.clear();
	return RET_FSUCCESS;
}

void SyntacticSpellerImp::do_do_spell_file()
{
	typo_string to_fix = S_EMPTY;
	infile >> to_fix;
	

	

	_sm.trim_SpellerImpTestAllAlgorithms(to_fix, IN);
	vector<ds_pair> cand_list;
	do_spell_word(to_fix, cand_list);
	while (!infile.eof())
	{	
		vector<ds_pair> cand_list;
		_pSimplePipeBuilder->resetContainerPtr(); 
		infile >> to_fix;	
		if (to_fix == S_EMPTY) break;
		do_spell_word(to_fix, cand_list);
		to_fix=S_EMPTY;
	}
}

void SyntacticSpellerImp::closeFiles()
{
	infile.seekg(0);
	infile.close();
	outfile.close();
}

#ifdef BENCHMARK_TESTING_MODE
int SyntacticSpellerImp::do_spell_file_benchmark(const typo_string &input_file, const typo_string &output_file)
{
	try{
		if (RET_FFAILURE == openFiles(input_file, output_file)) return RET_FFAILURE;
		Timer timer;
		timer.timer_start();

		do_do_spell_file();
		
		closeFiles();

		_tsb.createOtherReports(timer);
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00028);	
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}
#endif

int SyntacticSpellerImp::do_spell_file(const typo_string &input_file, const typo_string &output_file)
{
	#ifdef BENCHMARK_TESTING_MODE
		return do_spell_file_benchmark(input_file, output_file);
	#endif

	try{
		if (RET_FFAILURE == openFiles(input_file, output_file)) return RET_FFAILURE;
		
		do_do_spell_file();
		
		closeFiles();
	}
	catch(...)
	{
		_pSimplePipeBuilder->getLog()->report(MSG_00028);	
		return RET_FFAILURE;
	}

	return RET_FSUCCESS;
}

int SyntacticSpellerImp::do_spell_word(typo_string &to_fix, vector<ds_pair> &cand_list)
{
	list<ssi_pair>::iterator alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	IDictionary *pit = NULL;
	Filter f0(D_HALF);
	typo_string install_dir; 
	_pSimplePipeBuilder->getInstallDir(install_dir);
	Output output( install_dir + output_dir + output_txt_filename);
	
	
	//CORRECT SPELLING OR REAL WORLD ERROR
	if ((to_fix.size() < MINIMAL_WORD_SIZE) || (_pDict->find(to_fix) != RET_FIND_NOT_FOUND)) 
	{
//		if (_pDict->find(to_fix) != RET_FIND_NOT_FOUND)
//		{
			cand_list.push_back(ds_pair(D_HUNDRED, to_fix));
#ifdef BENCHMARK_TESTING_MODE
			_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);
#endif
			return RET_FFAILURE2;
//		}
	} 
	//MISSPELLING
	for (alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin(); alg_it != _pSimplePipeBuilder->getContainerNamesList()->end(); ++alg_it)
	{	
		pit = static_cast<IDictionary *>(_pSimplePipeBuilder->getContainer((*alg_it).first.first, (*alg_it).first.second));
		
		//SAVE PREVIOUS ALGORITHMS' RESULTS 
		vector<ds_pair> previous_cand_list; 
		if (!cand_list.empty()){
			previous_cand_list=cand_list;
			cand_list.erase(cand_list.begin(), cand_list.end());
		}

		if (pit->getSuggestions(to_fix, cand_list, f0) == RET_FFAILURE){ 	
			//TODO return this(better error handeling)	 //_cm.remove(_pSimplePipeBuilder->getContainerPointersList(), pit);	//continue;		
		} 	

		//APPEND PREVIOUS ALGORITHMS RESULTS 
		_cm.append<ds_pair>(cand_list, previous_cand_list); 
		typo_string current_id = pit->getID();
		_pDict->filterOutSuggestions(to_fix, cand_list, f0);
	}

	//SAFE FALL: NONE ALGORITHM SUCCEEDED OR NO ALGORITHM DEFINED OR VALID
	if (_SafeFall && cand_list.empty()){ 
		_pDict->getSuggestions(to_fix, cand_list, f0);
	}

	Utilities ut;
	ut.factorFrequencies(cand_list);
	if (_pSimplePipeBuilder->isStdOut()){
		output.update(to_fix);
		_xml_output.update(to_fix, cand_list);
#ifdef BENCHMARK_TESTING_MODE
		_tsb.updateStdBenchmarkResults(to_fix, cand_list, outfile);
#endif
	}

	return RET_FSUCCESS;
}

