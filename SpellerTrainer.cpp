#pragma warning (disable:4018)

#ifdef _TESTING
#include "TestSpellerBenchmark.h"
#endif

#include "SpellerTrainer.h"
#include "IDictionary.h"
#include "SimplePipeBuilder.h"
#include "StringManipulators.h"

#include "LogConstants.h"

#include <istream>
#include <fstream>
#include <list>
#include <iterator> //?
#include <algorithm>  //?


SpellerTrainer::SpellerTrainer(SimplePipeBuilder *mp):_pSimplePipeBuilder(mp)
{
	_SafeFall = atoi(((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS][SAFE_FALL]).c_str());
	_pDict=static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
	if (_pDict == NULL) throw runtime_error(MSG_00006);	
	txtOutFileName = output_txt_filename;
	xmlOutFileName = output_xml_filename;
}

int SpellerTrainer::do_spell_word(typo_string &to_fix, vector<ds_pair> &cand_list)
{
	list<ssi_pair>::iterator alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	IDictionary *pit = NULL;
	Filter f0(D_HALF);
	if (_pDict->find(to_fix) != RET_FIND_NOT_FOUND) 
	{
//		cand_list.push_back(ds_pair(D_HUNDRED, to_fix));
		return RET_FSUCCESS;
	} 
	for (alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin(); 
		alg_it != _pSimplePipeBuilder->getContainerNamesList()->end(); ++alg_it)
	{	//added second argument, test it; maybe better S_EMPTY
		pit = static_cast<IDictionary *>(_pSimplePipeBuilder->getContainer((*alg_it).first.first, (*alg_it).first.second));
		if (pit->getSuggestions(to_fix, cand_list, f0) == RET_FFAILURE){ 	
			//TODO return this(better error handeling)	 
			//cm.remove(_pSimplePipeBuilder->getContainerPointersList(), pit);	//continue;		
		} 	
//		typo_string current_id = pit->getID();
//		if ((current_id == LETTER_SET_INTERSECTION_LIST) || (current_id == KEYBOARD_RULES_LIST) || 
//			(current_id == NGRAM_INDEX) || (current_id == LETTERS_MAPPING_RULES_LIST))
//		{	
			//commented to test the difference on 06.10.09
			_pDict->filterOutSuggestions(to_fix, cand_list, f0);
//		}
	}
	if (_SafeFall && cand_list.empty()){ //safe fall: none algorithm succeeded or no algorithm defined or valid
		_pDict->getSuggestions(to_fix, cand_list, f0);
	}

	return RET_FSUCCESS;
}
int SpellerTrainer::do_spell_file(const typo_string &input_file, const typo_string &outFilePath)
{
	ifstream infile;
	infile.open(input_file.c_str());
	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00013, input_file);
		return RET_FFAILURE;
	}

	Output txtOutput(outFilePath + txtOutFileName);
//	XmlOutput xmlOutput(outFilePath + xmlOutFileName);
	if (false == txtOutput._bIsGood){
		_pSimplePipeBuilder->getLog()->report(MSG_00014, outFilePath + txtOutFileName);
		return RET_FFAILURE;
	}
//	if (false == xmlOutput._bIsGood){
//		_pSimplePipeBuilder->getLog()->report(MSG_00014, outFilePath + xmlOutFileName);
//		return RET_FFAILURE;
//	}

	while (! infile.eof())
	{
		typo_string to_fix = S_EMPTY;
		typo_string punctuation = S_EMPTY;
		recursiveFetchWord(to_fix, punctuation, infile);
		if (to_fix.size() < 2)
		{
			txtOutput.update(to_fix, punctuation);
			to_fix = S_EMPTY;
			continue;
//			recursiveFetchWord(to_fix, punctuation, infile);
		}
		vector<ds_pair> cand_list;
		do_spell_word(to_fix, cand_list);
		if (punctuation == S_EMPTY)	
			punctuation = S_SPACE;
		txtOutput.update(to_fix, punctuation, cand_list);
//		xmlOutput.update(to_fix, cand_list);
	}
	return RET_FSUCCESS;	
}

void SpellerTrainer::recursiveFetchWord(typo_string &to_fix, typo_string &punctuation, ifstream& infile)
{
	if (infile.good())
	{
		char c = infile.get();
		if (isalpha(c)){
			to_fix += c;
			recursiveFetchWord(to_fix, punctuation, infile);
		}
		else{
			if (c == 13) recursiveFetchWord(to_fix, punctuation, infile);
			else if (c == 10) punctuation += "\n";
			else if (c == -1) punctuation += S_SPACE;
			else punctuation += c;
		}
	}
}

int SpellerTrainer::run(const typo_string &to_fix, vector<ds_pair> &cand_list, typo_long_num &top_sim)
{
	list<ssi_pair>::iterator alg_it = _pSimplePipeBuilder->getContainerNamesList()->begin();
	ss_pair WinnerAlgName;
	typo_long_num WinnerSimilarity=D_ZERO;
		
	for (;
		alg_it != _pSimplePipeBuilder->getContainerNamesList()->end(); 
			++alg_it
		){
		
		//to be used by TestSpellerBenchmark::updateTimePlotHtml()
		ss_pair ss = (*alg_it).first;
		typo_string algName = ss.first + S_COLON + ss.second;

		Filter f0(top_sim);
		//added second argument, test it; maybe better S_EMPTY
		IDictionary* cont = static_cast<IDictionary *>(_pSimplePipeBuilder->getContainer(ss.first, ss.second));

		if (cont->getSuggestions(to_fix, cand_list, f0) == RET_FFAILURE){
			//TODO retrive this code
			//_pSimplePipeBuilder->getContainerPointersList()->erase(pit); 
			///cannot ruun with this algorithm, try to continue with other
			continue;
		}
		top_sim = f0.sim();
		if (top_sim >= _pSimplePipeBuilder->_TopSim) {
			(*alg_it).second++;
//			_pSimplePipeBuilder->increaseRating((*alg_it).first);
			return RET_FSUCCESS;
		}
		if (top_sim > WinnerSimilarity)
			WinnerAlgName = ss;
	}
//	if (WinnerAlgName != S_EMPTY){
		//(_pSimplePipeBuilder->_mmssiContCompRating[WinnerAlgName]).second++;
//		ss_pair ss = ss_pair(getLeft(nextCont, C_COLON), getRight(nextCont, C_COLON));
	_pSimplePipeBuilder->getContainerNamesListWrapper()->increaseRating(WinnerAlgName);
//			contCompRating.insert(mss_i_pair(ss, 0));

//		_pSimplePipeBuilder->increaseRating(WinnerAlgName);
//	}
	return 0;
}

/*
void SpellerTrainer::addBestChoice(typo_string &to_fix, vector<ds_pair> &cand_list)
{
	if (cand_list.empty()) 	
		_best_choices.push_back(to_fix); 
	else if ((cand_list.front()).first > (cand_list.back()).first)
		_best_choices.push_back((cand_list.front()).second); 
	else
		_best_choices.push_back((cand_list.back()).second); 
}
*/
/*
void SpellerTrainer::trimLeft(typo_string &to_fix, typo_string &left)
{
//	ASSERT(to_fix.size());
	typo_string to_fix_out = "";

	int iLen = to_fix.size();
	int iPos = 0;

	while (iPos < iLen)
	{	
		if (!(isalpha(to_fix[iPos])))
		{
			left += to_fix[iPos];
			if ((iPos+1 < iLen) && (isalpha(to_fix[iPos+1])))
			{
				to_fix_out += to_fix.substr(iPos+1, iLen - iPos);
				to_fix = to_fix_out;
				return;
			}
		}
		else
		{
			//to_fix_out = to_fix;
			return;
		}
		iPos++;
	}
}

void SpellerTrainer::trimRight(typo_string &to_fix, typo_string &right)
{
//	ASSERT(to_fix.size());
	typo_string to_fix_out = "";

	int iLen = to_fix.size();
	int iPos = 0;

	while (iPos < iLen)
	{	
		if (!(isalpha(to_fix[iPos])))
		{
			right = to_fix.substr(iPos, iLen - iPos);
			to_fix = to_fix.substr(0, iPos);
			return;
		}
		iPos++;
	}
}

//mis-spelling: mis, -spelling
bool SpellerTrainer::isCompound(typo_string &to_fix, typo_string &to_fix2)
{
	int iPos = to_fix.find("-");
	if (iPos == -1) return false;//smth

	int iLen = to_fix.size();
	if (iLen < 2) return false;

	if (iPos == iLen-1) return false; //smth-

	to_fix2 = to_fix.substr(iPos, iLen - iPos); //a-b, extract b
	to_fix = to_fix.substr(0, iPos); //a-b, extract b

	return true;
}
*/
/*
int SpellerTrainer::do_spell_file(const typo_string &input_file, const typo_string &output_file){

	int not_fixed_mistakes = 0;
	typo_string to_fix = S_EMPTY;

	ifstream infile;
	infile.open(input_file.c_str());
	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00013, input_file);
		return RET_FFAILURE;
	}
	ofstream outfile;
	outfile.open(output_file.c_str());
	if (false == outfile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00014, output_file);
		return RET_FFAILURE;
	}
	bool bIsCompound = false;
	string second_part = "";

	//ab a-b a-b ab
	while (!infile.eof()){
		vector<ds_pair> cand_list;
		string trim_left="";
		string trim_right="";
	
		if (bIsCompound == false){
			infile >> to_fix;
			if (to_fix.size() < 3)
			{
				outfile << to_fix << endl; 
				continue;
			}
			bIsCompound = isCompound(to_fix, second_part);
		}
		else{
			to_fix = second_part;
			bIsCompound = false;
		}

		trimLeft(to_fix, trim_left);
		trimRight(to_fix, trim_right);
	
		if (0 == do_spell_word(to_fix, cand_list)){
			++not_fixed_mistakes;
			outfile << to_fix << endl; 
		}
		else{

			map<typo_string,typo_long_num> start_prob; //probability of candidates occurance (optionally in users's dictionary)
			start_prob.insert(sd_pair("appendicularia", 0.005));
			start_prob.insert(sd_pair("abundance", 0.05));
			start_prob.insert(sd_pair("abundances", 0.5));
			
			map<typo_string, typo_long_num> transition_prob;//probability of candidate misspelling
			transition_prob.insert(sd_pair("appendicularia", 0.5));		
			transition_prob.insert(sd_pair("abundance", 0.0005));
			transition_prob.insert(sd_pair("abundances", 0.005));

			typo_string fixed=S_EMPTY;
			ViterbiAlg _va(to_fix, start_prob, transition_prob, cand_list);
			typo_long_num dva = _va.select(fixed);

			typo_string fixed = trim_left;
			fixed += ((cand_list.front()).first > (cand_list.back()).first) ? (cand_list.front()).second : (cand_list.back()).second ;
			fixed += trim_right;
			outfile << fixed << endl; 


		}
	}

	infile.close();
	outfile.close();

	return not_fixed_mistakes;	
}
*/

/*
int SpellerTrainer::do_spell_word_simple(const typo_string &to_fix, vector<ds_pair> &cand_list){

	typo_string fixed = to_fix;
	int MinSim = atoi((*_pSimplePipeBuilder->paramSet_inifile)[ALG_CONSTANTS][MINSIM].c_str());
	typo_long_num top_sim = MinSim;
	int ires=0;
	if ((ires = run(to_fix, cand_list, top_sim)) == RET_FSUCCESS) return ires;
	return cand_list.empty() == false;	
}
*/
