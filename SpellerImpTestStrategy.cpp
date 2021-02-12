/*
#include "SpellerImpTestStrategy.h"
#include "StringManipulators.h"
#include "TestConfig.h"
#include "ContainersManipulators.h"
#include "SimplePipeBuilder.h"
#include "DictionaryIndex.h" //DictionaryIndex di(_pSimplePipeBuilder, path._dictPath.c_str());
////#include <vector>

//using namespace std;
//using namespace SpellChecker;

SpellerImpTestStrategy::SpellerImpTestStrategy(SimplePipeBuilder *mp):
		SpellerTrainer(mp), _pSimplePipeBuilder(mp), _tsb(mp){
	output_file_with_date.op en(_tsb.outPath.c_str());	//this filepath contains date
	expfile.op en(BenchmarkTestsSection::_expPath.c_str());	
	not_found.op en(_tsb.not_found.c_str());	
	total_found_file.op en(_tsb.total_found_path.c_str());			
	autofix_succeeded.op en(_tsb.autofix_succeeded.c_str());	
	autofix_failed.op en(_tsb.autofix_failed.c_str());			
	first5Matches.op en(_tsb.first5Matches.c_str());	

}
SpellerImpTestStrategy::~SpellerImpTestStrategy(){
	expfile.cl ose();
	output_file_with_date.clo se();

	expfile.cl ose();
	not_found.cl ose();
	total_found_file.cl ose();
	autofix_succeeded.cl ose();
	autofix_failed.cl ose();
	first5Matches.cl ose();
}

int SpellerImpTestStrategy::do_spell_file(const typo_string &input_file, const typo_string &output_file){

	return do_do_spell_file(input_file, output_file);
}

int SpellerImpTestStrategy::do_do_spell_file(IAbstractProduct *pit, const typo_string &input_file, const typo_string &output_file){

	ifs tream infile;
	ofs tream outfile;

	infile.op en(input_file.c_str());
	outfile.op en(_tsb.outPath.c_str());

	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report("00013", input_file);
		return RET_FFAILURE;
	}
	if (false == outfile.good()){
		_pSimplePipeBuilder->getLog()->report("00014", output_file);
		return RET_FFAILURE;
	}
	t.timer_start();

	int not_fixed_mistakes = 0;
	typo_string to_fix = S_EMPTY;
	typo_string expected = S_EMPTY;
	typo_string first = S_EMPTY;

	ContainersManipulators cm;

	while (!infile.eof()){
		infile >> to_fix; expfile >> expected;
		vector<ds_pair> cand_list;	Filter f0(D_HALF);
		 
		list<IAbstractProduct *>::iterator pit;
		for ( pit = _pSimplePipeBuilder->getContainerPointersList()->begin(); pit!= _pSimplePipeBuilder->getContainerPointersList()->end(); pit++)
		{
			if (*pit == NULL) return RET_FFAILURE;
			if ((static_cast<IDictionary *>(pit))->getSuggestions(to_fix, cand_list, f0) == -1){
				cm.remove(_pSimplePipeBuilder->getContainerPointersList(), pit);
				if (_pSimplePipeBuilder->getContainerPointersList()->empty()){
					infile.cl ose();
					outfile.cl ose();
					return RET_FFAILURE;
				}
			}
			if (cand_list.empty())
				continue;
			first = ((cand_list.front()).first > (cand_list.back()).first) ? (cand_list.front()).second : (cand_list.back()).second;
			int ind = cm.find(cand_list, expected);
		}
		analizeResult();
	}

	_tsb.bm3(t);

	infile.cl ose();
	outfile.cl ose();

	return not_fixed_mistakes;	
}

void SpellerImpTestStrategy::analizeResult(const vector<ds_pair> &cand_list, const typo_string &to_fix, 
										   const typo_string &expected)
{
	int not_fixed_mistakes = 0;
	typo_string to_fix = S_EMPTY;
	typo_string expected = S_EMPTY;
	typo_string first = S_EMPTY;
	DictionaryIndex di(_pSimplePipeBuilder, (*_pSimplePipeBuilder->dict_inifile)[PATH_SECTION]["DictPath"]);
	int ind = -1;

	if (cand_list.empty()){
		not_found << "to fix: " << to_fix << ",\t\t expected: " << expected << endl; 
		outfile << to_fix << endl; 
		++not_fixed_mistakes;
	}else{ 
		first = ((cand_list.front()).first > (cand_list.back()).first) ? (cand_list.front()).second : (cand_list.back()).second;
		ind = cm.find(cand_list, expected);
		if (ind == -1){
			if (di.getSuggestions(to_fix, cand_list, f0) != -1)
				ind = cm.find(cand_list, expected);
		}

		outfile << first << endl; 
		output_file_with_date<< first << endl; 

		if (ind == -1){
			not_found << "to fix: " << to_fix << ", expected: " << expected << endl; 
		}
		else{
			if (ind == 0){ 
				autofix_succeeded << expected << endl; 
			}
			if (ind >= 0 && ind < 5) {
				first5Matches << expected << endl; 
			}
			if (ind > 0) {
				autofix_failed<< expected << endl; 
			}
			total_found_file<< expected << endl; 
		}
	}
	_tsb.updateResultsHtml(expected, cand_list, ind);
}

/*
int SpellerTrainer::do_do_spell_word(IAbstractProduct ** pit, const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim)
{
//	list<IAbstractProduct *>::iterator pit;
//	int TopSim = _pSimplePipeBuilder->TopSim;
//	vector<typo_string>::iterator alg_it = _pSimplePipeBuilder->_vsAlgPipe.begin();
	
//	typo_string WinnerAlgName=S_EMPTY;
//	typo_long_num WinnerSimilarity=0.0;
	
	
//	for ( pit = _pSimplePipeBuilder->getContainerPointersList()->begin(), alg_it = _pSimplePipeBuilder->_vsAlgPipe.begin();
//		pit!= _pSimplePipeBuilder->getContainerPointersList()->end(), alg_it != _pSimplePipeBuilder->_vsAlgPipe.end(); 
//		pit++,  ++alg_it
//		){
		if (*pit == NULL)
			return RET_FFAILURE;
		
		//to be used by TestSpellerBenchmark::updateTimePlotHtml()
	//	typo_string algName = *alg_it + S_COLON + (*_pSimplePipeBuilder->paramSet_inifile)[CONTAINER_TO_COMPARISON_MAP_SECTION][*alg_it];		

		Filter f0(top_sim);
		if ((static_cast<IDictionary *>(*pit))->getSuggestions(to_fix, cand_list, f0) == -1){
			_pSimplePipeBuilder->getContainerPointersList()->erase(pit); ///cannot ruun with this algorithm, try to continue with other
			//continue;
		}
//		top_sim = f0.sim();
//		if (top_sim >= TopSim) {
//			_pSimplePipeBuilder->increaseRating(algName);
///			return RET_FSUCCESS;
//		}
//		if (top_sim > WinnerSimilarity)
//			WinnerAlgName = algName;
//	}
//	if (WinnerAlgName != S_EMPTY)
//		_pSimplePipeBuilder->increaseRating(WinnerAlgName);
	return 0;
}* /

*/