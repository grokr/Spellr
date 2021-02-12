#pragma warning (disable:4018)
#include "StringManipulators.h"
#include "ContainersManipulators.h"
#include "DictionaryIniConstants.h"

#include <list>
#include <memory>
#include <cstdlib>


// this function can throw an exception
/*
int StringManipulators::get_environment_value(const typo_string& keyName, typo_string& ret)
{
	ret.resize(MAX_PATH_SIZE);
	DWORD dwRet;
	typo_char pszVal[MAX_PATH_SIZE];
	memset(pszVal, C_NULL, MAX_PATH_SIZE);
	try
	{
		dwRet = GetEnvironmentVariable(keyName.c_str(), pszVal, MAX_PATH_SIZE);
	}
	catch(...)	{ throw RET_FFAILURE2; }		// Don't swallow exception

	if ( dwRet > MAX_PATH_SIZE-1){  //overflow
		GetEnvironmentVariable(keyName.c_str(),pszVal,dwRet);
	}
	if (dwRet ==0)
	{
		//	throw RET_FFAILURE2;
		//		dwErr = GetLastError();
		//		if (dwErr == ERROR_ENVVAR_NOT_FOUND){
		//getLog()->report(MSG_00000);//		cout << "Environment variable does not exist." << endl; 
		//			return -2;
		//		}
		//		else{
		//getLog()->report(MSG_00001); //cout << " Unknown error in reading env varialbe" << endl;
		return RET_FFAILURE;
		//		}		
	}
	//	char* buf =  RegGetLocalMachineKey<char>(registry_path, keyName);
	//	cmstring ret = buf;
	//	delete [] buf;
	//	return ret;
	ret = pszVal;
	return RET_FSUCCESS;
}
*/


int StringManipulators::getLeft(typo_string& left, const typo_string& line) {
	int lstrip = line.find(S_SPACE);
	if (lstrip != typo_string::npos){
		left = line.substr(0, lstrip);
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

typo_string StringManipulators::getLeft(const typo_string& line, const typo_char& DELIMITER) {
	typo_string left=S_EMPTY;
	int lstrip = line.find(DELIMITER);
	if (lstrip != typo_string::npos){
		left = line.substr(0, lstrip);
	}
	return left;
}
//replace this func calls by next
int StringManipulators::getRight(typo_string& right, const typo_string& line) {

	int lstrip = line.find(S_SPACE);
	if (lstrip != typo_string::npos){
		right = line.substr(lstrip + 1, line.size() - lstrip);
		return RET_FSUCCESS;
	}
	return RET_FFAILURE;
}

typo_string StringManipulators::getRight(const typo_string& line, const typo_char& DELIMITER) {
	typo_string right=S_EMPTY;
	int lstrip = line.find(DELIMITER);
	if (lstrip != typo_string::npos){
		right = line.substr(lstrip + 1, line.size() - lstrip);
	}
	return right;
}

//Sat.Mar.21.02.52.04.2009 -> 02.52.04 Mar.21.2009
int StringManipulators::format(typo_string& date) {

	int lstrip = date.find(S_DOT.c_str()) + 1;
	date = date.substr(lstrip, date.size() - lstrip); //Mar.21.02.52.04.2009 
	int rstrip = date.rfind(S_DOT) + 1;
	typo_string year = date.substr(rstrip, date.size() - rstrip); //2009 
	date = date.substr(0, rstrip-1); //Mar.21.02.52.04

	lstrip = date.find(S_DOT) + 1; // .21.03.36.16 
	typo_string num = date.substr(lstrip, date.size() - lstrip);//21.03.36.16
	lstrip = num.find(S_DOT) + 1; // .21.03.36.16 
	typo_string time = num.substr(lstrip, date.size() - lstrip - 3);//21.03.36.16
	num = num.substr(0, lstrip-1);//21.03.36.16

	date = date.substr(0, lstrip) + S_SPACE + num + S_SPACE + time + S_SPACE + year; // 02.52.04 Mar.21 2009

	return RET_FSUCCESS;
}

int StringManipulators::trim(typo_string& to_trim) 
{
	int lstrip = to_trim.find_first_not_of(S_WHITESPACE);
	int rstrip = to_trim.find_last_not_of(S_WHITESPACE);
	if (lstrip == typo_string::npos || rstrip == typo_string::npos) {
		to_trim = S_EMPTY;
	} else {
		to_trim = to_trim.substr(lstrip, rstrip + 1);
	}
	return RET_FSUCCESS;
}

int StringManipulators::removeSpecialCharacters(typo_string& to_trim, const typo_string& trimOf) 
{
	int lstrip = to_trim.find_first_not_of(trimOf);
	int rstrip = to_trim.find_last_not_of(trimOf);
	if (lstrip == typo_string::npos || rstrip == typo_string::npos) {
		to_trim = S_EMPTY;
	} else {
		to_trim = to_trim.substr(lstrip, rstrip + 1);
	}
	return RET_FSUCCESS;
}

int StringManipulators::trim(typo_string& to_trim, const typo_string& trimOf) {
	int lstrip = to_trim.find_first_not_of(trimOf);
	int rstrip = to_trim.find_last_not_of(trimOf);
	if (lstrip == typo_string::npos || rstrip == typo_string::npos) {
		to_trim = S_EMPTY;
	} else {
		to_trim = to_trim.substr(lstrip, rstrip + 1);
	}
	return RET_FSUCCESS;
}
//parse str1;str2;str3 
///if str_i can not be found in seq, append it
int StringManipulators::parseStringAppendToVector(const typo_string &algList, const typo_char& DELIMITER, vector<typo_string> &seq){
	int start = 0, end = 0;
	ContainersManipulators cm;
	while(start < algList.size()){
		end = algList.find(DELIMITER, start);
		if (end == -1){
			typo_string last=algList;
			if (start != 0)
				last = algList.substr(start, algList.size()-end);
			if (false == cm.find<typo_string>(seq, last))
				seq.push_back(last);
			return RET_FSUCCESS;
		}
		typo_string nextAlg = algList.substr(start, end-start);
		if (false == cm.find<typo_string>(seq, nextAlg))
			seq.push_back(nextAlg);
		start=end+1;
	}
	return RET_FSUCCESS;
}

//parse str1,20;str2,30;str3,40 
int StringManipulators::parseStringAppendToVector(const typo_string &in, const typo_char& delimiter, vector<si_pair> &seq)
{
	int start = 0, iEnd_of_mapping_right=0, iEnd_of_mapping_left=0;
	typo_string sEnd_of_mapping_left = S_EMPTY, sEnd_of_mapping_right = S_EMPTY;

	while(start < in.size())
	{
		iEnd_of_mapping_right = in.find(C_SEMICOLON, start);
		if (iEnd_of_mapping_right == -1) iEnd_of_mapping_right = in.size();
		iEnd_of_mapping_left = in.find(C_COMA, start);

		if ((iEnd_of_mapping_left != typo_string::npos) && (iEnd_of_mapping_right != typo_string::npos)){
			sEnd_of_mapping_left = in.substr(start, iEnd_of_mapping_left-start);
			sEnd_of_mapping_right = in.substr(iEnd_of_mapping_left+1, iEnd_of_mapping_right-iEnd_of_mapping_left-1);
			seq.push_back(si_pair(sEnd_of_mapping_left, atoi(sEnd_of_mapping_right.c_str())));
		}

		start=iEnd_of_mapping_right+1;
	}
	return RET_FSUCCESS;
}

//parse str1,20;str2,30;str3,40  C_COMA
int StringManipulators::parseMappings(const typo_string &in, const typo_char& delimiter, 
									typo_string &str1, typo_string &str2)
{
	int start = 0, iEnd_of_mapping_right=0, iEnd_of_mapping_left=0;
	typo_string sEnd_of_mapping_left = S_EMPTY, sEnd_of_mapping_right = S_EMPTY;
	iEnd_of_mapping_right = in.find(C_SEMICOLON, start);
	if (iEnd_of_mapping_right == -1) iEnd_of_mapping_right = in.size();
	iEnd_of_mapping_left = in.find(delimiter, start);

	if ((iEnd_of_mapping_left != typo_string::npos) && (iEnd_of_mapping_right != typo_string::npos)){
		sEnd_of_mapping_left = in.substr(start, iEnd_of_mapping_left-start);
		sEnd_of_mapping_right = in.substr(iEnd_of_mapping_left+1, iEnd_of_mapping_right-iEnd_of_mapping_left-1);
		str1 = sEnd_of_mapping_left; str2 = sEnd_of_mapping_right;
	}

	return RET_FSUCCESS;
}

int StringManipulators::getNext(const typo_string &algList, const typo_char& DELIMITER, int& off, typo_string& next)
{
	int prev_off = off;
	off = algList.find(DELIMITER, prev_off);
	if (off == -1){
		if (prev_off < algList.size()){
			next = algList.substr(prev_off, algList.size()-prev_off);
			return 1;
		}
		else
			return 0;
	}
	next = algList.substr(prev_off, off-prev_off);
	off++;
	return 1;
}


int StringManipulators::extract_sentence_ending(typo_string& str, typo_string& ending)
{
	if (str.find_last_of(SENTENCE_ENDINGS) == typo_string::npos)
		return 0;

	int lstrip = str.find_first_of(SENTENCE_ENDINGS);
	if (lstrip == 0){ // case: ...nothing  ?nothing
		int rstrip = str.find_first_of(UC_ALPHABET_EN_LOWER);
		if (rstrip == -1) 
			rstrip = str.size();
		ending = str.substr(lstrip, rstrip - lstrip); // ...
		str = str.substr(rstrip, str.size()-rstrip); //nothing
		return 1;
	}
	if (lstrip == -1)
		return 0;
	
	//case: nothing...
	ending = str.substr(lstrip, str.size() - lstrip); // ...
	str = str.substr(0, lstrip); //nothing
			
	return 1;
}
void StringManipulators::trim_SpellerImpTestAllAlgorithms(typo_string& str, const typo_string& in_out) 
{


	typo_string str_copy = str;
	typo_string lang =  ISO639_1_EN;
	typo_string letters_set;

	if		(lang == ISO639_1_EN) return;
	else if (lang == ISO639_1_DE){	letters_set = UC_ALPHABET_DE;	transform(str_copy.begin(), str_copy.end(), str_copy.begin(), toupper);}
	else if (lang == ISO639_1_HE){	letters_set = LC_ALPHABET_HE;}
	else if (lang == ISO639_1_RU){	letters_set = UC_ALPHABET_RU;	transform(str_copy.begin(), str_copy.end(), str_copy.begin(), toupper);}
	else return;

	int lstrip = str_copy.find_first_of(letters_set);
	int rstrip = str_copy.find_last_of(letters_set);
	if ((lstrip != 0) && (rstrip != str_copy.size()+1))
		str = str.substr(lstrip, rstrip + 1);

}

int StringManipulators::has_punctuation(typo_string& str)
{
	if (str.find_last_of(SENTENCE_PUNCTUATION) == typo_string::npos)
		return 0;
	return 1;
}
//SENTENCE_PUNCTUATION
//SENTENCE_ENDINGS
int StringManipulators::extract_punctuation(typo_string& str, typo_string& ending)
{
	if (str.find_last_of(SENTENCE_PUNCTUATION) == typo_string::npos)
		return 0;

	int lstrip = str.find_first_of(SENTENCE_PUNCTUATION);

	if (lstrip == -1)
		return 0;

	ending = str.substr(lstrip, str.size()-1);
	str = str.substr(0, lstrip);
	
	return 1;
}

