#ifndef __TEST_DICTIONARY_H__
#define __TEST_DICTIONARY_H__

#include "SpellerImp.h"
#include "Paths.h"
#include "Constants.h"

#include "TypeDefs.h"
#include <iostream>
#include <vector>
#include <string>

using namespace SpellChecker;
using namespace std;

class TestSpellerImp
{
	friend class SpellerImp;
//	SpellerImp _pSpellerImp;
//	void readExpected(vector <string> &expected);
public:
//	TestSpellerImp(const string &dirpath, const string &metaphone_path) : _pSpellerImp(dirpath, metaphone_path){
	TestSpellerImp(){
	}
	
//	int run_test_getFixesList();

	void test_getFixesList(const string &input_path, const string &report_html_path, 
											   const string &expected_path);
//	void test_getFixesList_manual(int size);
};

#endif //__TEST_DICTIONARY_H__