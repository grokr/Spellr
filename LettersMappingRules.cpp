#include "LettersMappingRules.h"
#include "SpellerTrainer.h"
#include "LogConstants.h"
#include "ContainersManipulators.h"
#include "NGramIndex.h"
#include "Utilities.h"

#include <fstream>
#include <algorithm>

bool LettersMappingRules_compare(const ds_pair& a, const ds_pair& b) {
	return a.first > b.first;
}

LettersMappingRules::LettersMappingRules(Application *mf, const typo_string &d) : _pSimplePipeBuilder(mf), 
	_changed(false), _lmpath(d), _GUID(LETTERS_MAPPING_RULES_LIST), _pDict(NULL), _pNGram(NULL), _pComp(NULL)
{
	try{
		_iom.io_read((_pSimplePipeBuilder->INSTALL_DIR + _lmpath).c_str(), _lm);
	}
	catch(...){
		throw runtime_error(MSG_00006);
	}
	
	readParameters();

	_pDict = static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(DICTIONARY_INDEX, S_EMPTY));
	_pNGram = static_cast<DictionaryIndex *>(_pSimplePipeBuilder->getContainer(NGRAM_INDEX, S_EMPTY));
	_pComp = _pSimplePipeBuilder->makeComparisonFunction((*_pSimplePipeBuilder->paramSet_inifile)[CONTAINER_TO_COMPARISON_MAP_SECTION][LETTERS_MAPPING_RULES_LIST]);
	_recursionDepth = _params.recursionDepth;
}

int LettersMappingRules::getSuggestions(const typo_string &in, vector<ds_pair> &res, Filter &filter)
{
	_timer.timer_start();
	_recursionDepth = _params.recursionDepth;

	int iii=0;
	if (in == _params.control_word)
		iii=0;

	_ngram_sum_of_prob = D_ZERO;
	in_ngram_transformed = in;
	(static_cast<NGramIndex* >(_pNGram))->encode(in, in_ngram_transformed, _ngram_sum_of_prob);

	vector<ds_pair> existing_words;	
	if (RET_FSUCCESS == createLettersMappings(in, res, existing_words))
		return RET_FSUCCESS; 

	if (_params.encounter_only_existing_words){
		if (!existing_words.empty()){
			res = existing_words;
			return RET_FSUCCESS;
		}
	}

	//existing_words_limit
	if (existing_words.size() > 3){
		res = existing_words;
		return RET_FSUCCESS;
	}
	Utilities ut;
	ut.factorFrequencies(res);

	getWordsSimilarToLettersMappings(in, in_ngram_transformed, res);

	if (!existing_words.empty())
		mergeExistingCandidates(in, res, existing_words);

	return RET_FSUCCESS;
}

//self-learning phase 
int LettersMappingRules::add_correction(const typo_string &w, const typo_string &f)
{
	if (w == f) return RET_FSUCCESS;

	vector<si_pair> v;
	map<typo_string, vector<si_pair>>::iterator it = _lm.find(w); 
	if (it == _lm.end())
	{
		v.push_back(si_pair(f, 1));
		_lm.insert(svpsi_pair(w, v));
		_changed = true;
		return RET_FSUCCESS;
	}
	v = (*it).second;
	vector<si_pair>::iterator vit = v.begin();
	for (; vit < v.end();++vit)
	{
		if ((*vit).first == f)
		{
			(*vit).second++;
			(*it).second = v;
			_changed = true;
			return RET_FSUCCESS;
		}
	}
	v.push_back(si_pair(f, 1));
	(*it).second = v;
	_changed = true;
	return RET_FSUCCESS;
}

//self-learning phase helper function
int LettersMappingRules::doCreateMappingsList(const typo_string &w1, const typo_string &w2,
			int& leftStartIndex, int& rightStartIndex, 
			const int& leftCurrentIndex, const int& rightCurrentIndex)
{  
	int res = RET_FFAILURE;
	typo_string left, right;

	if ((leftCurrentIndex >= w1.size()) || (rightCurrentIndex >= w2.size()))
		return res;

	if ((w1[leftCurrentIndex] == w2[rightCurrentIndex]) ||
		(leftCurrentIndex == (w1.size()-1)) && (rightCurrentIndex == (w2.size()-1)))
	{ 
		left = w1.substr(leftStartIndex, leftCurrentIndex-leftStartIndex+1);
		right = w2.substr(rightStartIndex, rightCurrentIndex-rightStartIndex+1);
		if (leftStartIndex == 0){ left = C_SHARP + left; } //for "ukddress" "address" it adds (#uk (#a,1));
		if (rightStartIndex == 0){ right = C_SHARP + right; }
		if (leftCurrentIndex == w1.size()-1){ left = left + C_SHARP ; }//for "addres"   "address" it adds (s# (ss#,1));
		if (rightCurrentIndex == w2.size()-1){ right = right + C_SHARP; }
		add_correction(left, right);
		leftStartIndex = leftCurrentIndex; rightStartIndex = rightCurrentIndex;
		res = RET_FSUCCESS;
	}
	return res;
}

//self-learning phase helper function
//for misspelled "apres" and fixed "address" adds a pair (p (dd,1)) if there is no such a mapping
int LettersMappingRules::createMappingsList(const typo_string &w1, const typo_string &w2)
{
	typo_string left=S_EMPTY, right=S_EMPTY;
	int leftStartIndex=0, rightStartIndex=0;

	for (int i = 0, j = 0; (i < w1.size()) && (j < w2.size()); i++, j++)
	{
		if ((w1[i] == w2[j]) && (leftStartIndex == i) && (rightStartIndex == j)){ 
			leftStartIndex++; rightStartIndex++; continue;
		}
		for (int window_size = 0; window_size < _params.learning_phase_window_size; window_size++)
		{
			
			if (RET_FSUCCESS == doCreateMappingsList(w1, w2, leftStartIndex, rightStartIndex, i + window_size, j)){
				i = leftStartIndex; j = rightStartIndex; break;
			}
			if (w1[i] == w2[j]) continue;
			if (RET_FSUCCESS == doCreateMappingsList(w2, w1, rightStartIndex, leftStartIndex, i, j + window_size)){
				i = leftStartIndex; j = rightStartIndex;  break;
			}

		}
	}
	return RET_FSUCCESS;
}	

//self-learning phase
int LettersMappingRules::add_correction(const typo_string &filename)
{
	typo_string misspelled;
	typo_string fixed, in;
	ifstream is;
	is.open(filename.c_str());
	typo_string word=S_EMPTY;
	while (!is.eof()){
		is >> in;
		_sm.parseMappings(in, C_COMA, misspelled, fixed);
		createMappingsList(misspelled, fixed);
	}
	is.close();

	return RET_FSUCCESS;
}
	
//recursive algorithm is too slow. Use controlled number of iterations. Can work faster x3 if first_match_is_good_match is true
int LettersMappingRules::createLettersMappings(const typo_string &in, vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	res.push_back(ds_pair(D_HUNDRED, in));
	while(_recursionDepth-- > 0)
	{


		if (RET_FSUCCESS == doCreateLettersMappings(in, res, existing_words))  
		{
			if (_params.first_match_is_good_match){
				res=existing_words;
				return RET_FSUCCESS; 
			}
		}
	}
	return RET_FFAILURE; 
}


int LettersMappingRules::doCreateLettersMappings(const typo_string &in, vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	int ret=RET_FFAILURE;
	vector<ds_pair> outputRes;
	vector<ds_pair>::iterator resIt=res.begin();
	typo_long_num cur_sim=0.0;
	for(resIt=res.begin(); resIt != res.end(); ++resIt)
	{
		if ((_timer.get_time_lapsed() >= _params.time_limit) || 
			(res.size() > _params.output_size_limit2))
			return RET_FFAILURE;

		//if the list is long and ngram produces too many ***, filter out candidates
		typo_string ngram_transformed=(*resIt).second;
		(static_cast<NGramIndex* >(_pNGram))->encode((*resIt).second, ngram_transformed, cur_sim);
		if (cur_sim < _ngram_sum_of_prob)
			continue; 

		if (_params.shorten_list){
			if (RET_FSUCCESS==shortenList(ngram_transformed, outputRes.size(), res.size()))
			{ 
				continue; 

			}
		}

		vector<ds_pair> res2;
		if (_params.narrowSearchByNGram){
			if (RET_FSUCCESS == doDoCreateLettersMappingsNGram(*resIt, ds_pair(cur_sim, ngram_transformed), res2, existing_words)){
				ret = RET_FSUCCESS;
			}
		}

		if (existing_words.empty()){
			res2.clear();
			if (RET_FSUCCESS == doDoCreateLettersMappings(*resIt, ngram_transformed, res2, existing_words))
				ret = RET_FSUCCESS;
		}
		
		mergeVectorsConditionally(in, outputRes, res2);
		if (RET_FSUCCESS == ret) 
			break;
	}

	res = outputRes;
	return ret;
}


int LettersMappingRules::doDoCreateLettersMappings(const ds_pair& to_fix,
												   const typo_string &ngram_transformed, 
												   vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	int ret = RET_FFAILURE;
	typo_string reconstructed = S_WORD_LIMITER + to_fix.second + S_WORD_LIMITER;

	for(map<typo_string,vector<si_pair>>::iterator lm_it = _lm.begin(); lm_it != _lm.end(); lm_it++){
		ret = createMappingsListForOneLetter(*lm_it, reconstructed, ngram_transformed, res, existing_words, to_fix.first);
		if (RET_FSUCCESS == ret) return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

int LettersMappingRules::doDoCreateLettersMappingsNGram( ds_pair& to_fix,
														ds_pair &ngram_transformed, 
														vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	int ret = RET_FFAILURE;
	int pos=0;
	typo_string reconstructed = S_WORD_LIMITER + to_fix.second + S_WORD_LIMITER;

	vector<typo_string> substCandList;
	createLettersSubstituteCandidatesList(to_fix.second, ngram_transformed.second, substCandList);

	vector<typo_string>::iterator it = substCandList.begin();
	map<typo_string,vector<si_pair>>::iterator lm_it = _lm.begin();
	for(it = substCandList.begin(); it != substCandList.end(); it++){
		if ((lm_it = _lm.find(*it)) == _lm.end()) continue;

		svpsi_pair mappings = *lm_it;
		vector<si_pair>::const_iterator mappingsIt = mappings.second.begin();
		for(mappingsIt = mappings.second.begin(); mappingsIt != mappings.second.end(); mappingsIt++)
		{
			typo_string candidate=reconstructed;
			pos = candidate.find(*it);
			if (pos == 0) continue;
			candidate.replace(pos, mappings.first.size(), (*mappingsIt).first);
			candidate=candidate.substr(1, candidate.size()-2);//remove #...#

			if (RET_FSUCCESS == storeCandidate(candidate, ngram_transformed.second, res, existing_words, pos-1, (*mappingsIt).second+to_fix.first)) 
				return RET_FSUCCESS;
		}

	}

	return RET_FFAILURE;
}

int LettersMappingRules::createMappingsListForOneLetter(const svpsi_pair &mappings, const typo_string &reconstructed, 
						const typo_string &ngram_transformed, vector<ds_pair> &res, vector<ds_pair> &existing_words, const typo_long_num& prob)
{
	int ret=RET_FFAILURE;
	int pos=0, previousPos=0;

	while ((pos=reconstructed.find(mappings.first, previousPos)) != typo_string::npos)
	{
		if ((pos == 1) && ((mappings.first)[0]!='#')){
			previousPos=pos+1; continue;
		}
		if ((pos + mappings.first.size() == reconstructed.size()-1) && ((mappings.first)[mappings.first.size()-1]!='#')){
			previousPos=pos+1; continue;
		}

	

		vector<si_pair>::const_iterator mappingsIt = mappings.second.begin();
		for(mappingsIt = mappings.second.begin(); mappingsIt != mappings.second.end(); mappingsIt++)
		{
			typo_string candidate=reconstructed;
			candidate.replace(pos, mappings.first.size(), (*mappingsIt).first);
			candidate=candidate.substr(1, candidate.size()-2);//remove #...#

			if (RET_FSUCCESS == storeCandidate(candidate, ngram_transformed, res, existing_words, pos, (*mappingsIt).second+prob)) //pos-1: for # in the beginning
				return RET_FSUCCESS;
		}
		previousPos=pos+1;
	}
	return ret;
}

int LettersMappingRules::storeCandidate(  typo_string &candidate, 
		const typo_string &ngram_transformed, vector<ds_pair> &res, vector<ds_pair> &existing_words, const int &pos, const typo_long_num &prob)
{
	if (_cm.findIndex(res, candidate) != RET_FFAILURE) { return RET_FFAILURE;}
	if (_pDict->find(candidate) != RET_FIND_NOT_FOUND){
		if (_cm.findIndex(existing_words, candidate) == RET_FFAILURE)
			existing_words.push_back(ds_pair(_params.existing_word_bonus + prob, candidate));
		if (_params.first_match_is_good_match)
			return RET_FSUCCESS;
	}
	else{

		typo_string new_ngram_transformed=candidate;
		typo_long_num prob_sum = D_ZERO;
		(static_cast<NGramIndex* >(_pNGram))->encode(candidate, new_ngram_transformed, prob_sum);
		if (prob_sum < _ngram_sum_of_prob)
			return RET_FFAILURE;

		if (new_ngram_transformed.find(C_STAR) != -1) //doDoCreateLettersMappingsNGram() case
			res.push_back(ds_pair(_params.substitute_ngram_star_bonus + _recursionDepth*_params.rate_by_recursion_depth + prob, candidate));
		else //doDoCreateLettersMappings() case
			res.push_back(ds_pair(_recursionDepth*_params.rate_by_recursion_depth + prob, candidate));
	}
	return RET_FFAILURE;
}

int LettersMappingRules::recalculate_similarity(const typo_string &in, vector<ds_pair> &res)
{
	vector<ds_pair> out_res;
	vector<ds_pair>::iterator cand_list1It=res.begin();
	typo_long_num cur_sim=0.0;
	for(cand_list1It=res.begin(); cand_list1It != res.end(); ++cand_list1It)
	{
		if ((cur_sim = (static_cast<IStringComparison* >(_pComp))->computeSimilarityRatio(in, (*cand_list1It).second, cur_sim)) >= _params.minSim)
		{
			out_res.push_back(ds_pair((cur_sim+(*cand_list1It).first/2), (*cand_list1It).second));
		}
	}
	res=out_res;
	return RET_FSUCCESS;
}

int LettersMappingRules::recalculate_similarity_ngram(const typo_string &in, const typo_string &encoded, vector<ds_pair> &res)
{
	vector<ds_pair> out_res;
	vector<ds_pair>::iterator cand_list1It=res.begin();
	typo_long_num cur_sim=0.0;



	for(cand_list1It=res.begin(); cand_list1It != res.end(); ++cand_list1It)
	{
		typo_long_num min =  (*cand_list1It).second.length() < encoded.length() ?  (*cand_list1It).second.length(): encoded.length();
		typo_long_num max =  (*cand_list1It).second.length() > encoded.length() ?  (*cand_list1It).second.length(): encoded.length();
		if (max - min > _params.max_length_diff)
			continue;

		typo_long_num cur_sim = 0.0;
		for (int i=0; i < min; i++){
			if (encoded[i]==(*cand_list1It).second[i]) {
				cur_sim++; continue;
			}
			if (encoded[i] == C_STAR)
				cur_sim+=_params.ngram_star_value;
		}
		out_res.push_back(ds_pair((cur_sim+(*cand_list1It).first/2), (*cand_list1It).second));

	}
	res=out_res;
	return RET_FSUCCESS;
}


void LettersMappingRules::getWordsSimilarToLettersMappings(const typo_string &in, const typo_string &encoded, vector<ds_pair> &res)
{
	const int ngram_filter = 1;

	typo_long_num cur_sim = 0.0;
	vector<ds_pair> out_res;
	Filter f;
	vector<ds_pair>::iterator resIt=res.begin();
	for(resIt=res.begin(); resIt != res.end(); ++resIt){
		f.sim((*resIt).first);
		_pDict->getSuggestions((*resIt).second, out_res, f);
		if (out_res.size() > 100)
			break;
	}
	if (_params.compare_to_original){ //see if candidates still ressemble an original misspelled word
		recalculate_similarity(in, out_res);
	}

	
	if (ngram_filter){ //see if candidates still ressemble an original misspelled word
		recalculate_similarity_ngram(in, encoded, out_res);
	}

	res = out_res;
}
void LettersMappingRules::readParameters()
{
	_params.stars_number_limit1 = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][STARS_NUMBER_LIMIT1].c_str()); 
	_params.stars_number_limit2 = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][STARS_NUMBER_LIMIT2].c_str()); 
	_params.stars_number_limit3 = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][STARS_NUMBER_LIMIT3].c_str()); 

	_params.output_size_limit1  =	atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][OUTPUT_SIZE_LIMIT1].c_str()); 
	_params.output_size_limit2  =	atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][OUTPUT_SIZE_LIMIT2].c_str());
	_params.recursionDepth =		atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][RECURSIONDEPTH].c_str());
	_params.minSim =				atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][MINSIM].c_str());
	_params.existing_word_bonus  =	atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][EXISTING_WORD_BONUS].c_str());
	_params.substitute_ngram_star_bonus  = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][SUBSTITUTE_NGRAM_STAR_BONUS].c_str());
	_params.compare_to_original  =	atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][COMPARE_TO_ORIGINAL].c_str());
	_params.shorten_list =			atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][SHORTEN_LIST].c_str());
	_params.first_match_is_good_match = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][FIRST_MATCH_IS_GOOD_MATCH].c_str());	
	_params.time_limit =			ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][TIME_LIMIT].c_str());	
	_params.rate_by_recursion_depth = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][RATE_BY_RECURSION_DEPTH].c_str());	

	_params.narrowSearchByNGram = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][NARROWSEARCHBYNGRAM].c_str());	

	_params.control_word = (*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][CONTROL_WORD].c_str();
	_params.max_length_diff = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][MAX_LENGTH_DIFF].c_str());	
	_params.encounter_only_existing_words = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][ENCOUNTER_ONLY_EXISTING_WORDS].c_str());	

	_params.ngram_star_value = ato_typo_long_num((*_pSimplePipeBuilder->paramSet_inifile)[NGRAMCONSTANTS_SECTION][NGRAMSTARVALUE].c_str());
	_params.learning_phase_window_size = atoi((*_pSimplePipeBuilder->paramSet_inifile)[LETTERSMAPPINGRULESCONSTANTS_SECTION][LEARNING_PHASE_WINDOW_SIZE].c_str());

}

void LettersMappingRules::mergeExistingCandidates(const typo_string &to_fix, vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	vector<ds_pair>::iterator resIt=existing_words.begin();
	int pos;
	for(resIt=existing_words.begin(); resIt != existing_words.end(); ++resIt){
		if ((pos = _cm.findIndex(res, (*resIt).second)) == RET_FFAILURE){
			if (abs(to_fix.length()-(*resIt).second.length()) > _params.max_length_diff)
				continue;
			res.push_back(*resIt);
		}else{
			(*(res.begin()+pos)).first=(*resIt).first;
		}
	}
}

void LettersMappingRules::mergeVectorsConditionally(const typo_string &to_fix, vector<ds_pair> &res, vector<ds_pair> &existing_words)
{
	vector<ds_pair>::iterator resIt=existing_words.begin();
	int pos;
	for(resIt=existing_words.begin(); resIt != existing_words.end(); ++resIt){
		if ((pos = _cm.findIndex(res, (*resIt).second)) == RET_FFAILURE){
			if (abs(to_fix.length() - (*resIt).second.length()) > _params.max_length_diff)
				continue;
			res.push_back(*resIt);
		}
	}
}

int LettersMappingRules::shortenList(typo_string &transformed, const size_t& outputResSize, const size_t& initialResSize)
{
	typo_long_num stars_rel_number = ((typo_long_num) count (transformed.begin(), transformed.end(), C_STAR))/(typo_long_num)transformed.size();

	if ((outputResSize > _params.output_size_limit1) && (stars_rel_number > _params.stars_number_limit1))
		return RET_FSUCCESS;
	if ((outputResSize > _params.output_size_limit2) && (stars_rel_number  > _params.stars_number_limit2)) 
		return RET_FSUCCESS;
	if ((outputResSize < _params.output_size_limit2) && (initialResSize < _params.output_size_limit2) && 
		(stars_rel_number > _params.stars_number_limit3))
		return RET_FSUCCESS;
	return RET_FFAILURE;
}

/*
in=ecspresif
rec=e**pres**

c,s,i,f
#ec,ecs,csp,spr
ec,cs,sp
si,if,f#
esi,sif,if#
*/
int LettersMappingRules::createLettersSubstituteCandidatesList(const typo_string& to_fix,
				const typo_string &ngram_transformed, vector<typo_string>& substCandList)
{

	typo_string::const_iterator it = ngram_transformed.begin();
	for (it = ngram_transformed.begin(); it != ngram_transformed.end(); it++)
	{
		if (*it != C_STAR) continue;
		typo_string cand = S_EMPTY;

		cand = to_fix.substr(it - ngram_transformed.begin(), 1);//e,c,s,i
		if (_cm.find<typo_string>(substCandList, cand) == false) 
			substCandList.push_back(cand);
		
		cand = to_fix.substr(it - ngram_transformed.begin(), 2);//ec,cs,si,if
		if (_cm.find<typo_string>(substCandList, cand) == false) 
			substCandList.push_back(cand);
		
		
		if (it == ngram_transformed.begin()){//#e
			cand = C_SHARP + to_fix.substr(0, 1);
			if (_cm.find<typo_string>(substCandList, cand) == false) 
				substCandList.push_back(cand);
			continue;
		}

		if (it == ngram_transformed.end()-1){
			cand = to_fix.substr(ngram_transformed.end()-ngram_transformed.begin()-1, 1) + C_SHARP;//f#
			if (_cm.find<typo_string>(substCandList, cand) == false) 
				substCandList.push_back(cand);
			cand = to_fix.substr(ngram_transformed.end()-ngram_transformed.begin()-2, 2) + C_SHARP;//if#
			if (_cm.find<typo_string>(substCandList, cand) == false) 
				substCandList.push_back(cand);
		}
		
		cand = to_fix.substr(it - ngram_transformed.begin() - 1, 2);//ec,es,si
		if (_cm.find<typo_string>(substCandList, cand) == false) 
			substCandList.push_back(cand);

		if (it - 1 == ngram_transformed.begin()){
			cand = C_SHARP + to_fix.substr(it - ngram_transformed.begin() - 1, 2);//#ec
		}
		else{
			cand = to_fix.substr(it - ngram_transformed.begin() - 1, 3);//esi,sif
		}
		if (_cm.find<typo_string>(substCandList, cand) == false) 
			substCandList.push_back(cand);

	}
	return RET_FSUCCESS;
}
