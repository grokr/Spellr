#include "MetaphoneBuilder.h"
#include "metaphone.h"
#include "Constants.h"
#include "LogConstants.h"
#include "IniFileHandler.h"
#include "Log.h"
#include "SimplePipeBuilder.h"

#include <fstream>
#include <istream>
using namespace IniFileNamespace;

int MetaPhoneBuilder::build()
{
	typo_string	filename = _pSimplePipeBuilder->INSTALL_DIR + "\\" + _infile;
	ifstream infile;
	infile.open(filename.c_str());
	if (false == infile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00013, _infile);
		return RET_FFAILURE;
	}
	filename = _pSimplePipeBuilder->INSTALL_DIR +  "\\" + _outfile;
	ofstream outfile;
	outfile.open(filename.c_str());
	if (false == outfile.good()){
		_pSimplePipeBuilder->getLog()->report(MSG_00014, _outfile);
		return RET_FFAILURE;
	}
 
	typo_string to_fix = S_EMPTY;
	typo_string language = S_EMPTY;
	_pSimplePipeBuilder->getCurrentInputLanguage(language); 

	while (!infile.eof()){

		infile >> to_fix;
		if (to_fix.size() < 3) continue;
		CString m1(S_EMPTY);
		CString m2(S_EMPTY);
		MString metaphone(to_fix);
		metaphone.DoubleMetaphone(m1, m2, language);
		//outfile << m1.c_str() << "\t" << m2.c_str() << endl;
		outfile << m1.c_str() << endl;
	}

	infile.close();
	outfile.close();
	return RET_FSUCCESS;
}

int MetaPhoneBuilder::batch_build()
{
	const int ALPH_SIZE = 26;
	typo_string dir = "E:\\_spellChecker\\code\\model\\spelling\\data\\Dictionaries\\indexed\\index_single_word\\";

	typo_string out_fileslist[ALPH_SIZE]={
		 "a.txt",		 "b.txt" ,       "c.txt",        "d.txt",
		 "e.txt", 		 "f.txt", 		 "g.txt", 		 "h.txt", 
		 "i.txt", 		 "j.txt", 		 "k.txt", 		 "l.txt", 	
		 "m.txt", 		 "n.txt", 		 "o.txt", 		 "p.txt", 	
		 "q.txt", 		 "r.txt", 		 "s.txt",        "t.txt", 		 
		 "u.txt", 		 "v.txt", 		 "w.txt", 	     "x.txt", 		 
		 "y.txt",         "z.txt"};

	typo_string in_fileslist[ALPH_SIZE]={
		 "a1.txt",		 "b1.txt" ,       "c1.txt",        "d1.txt",
		 "e1.txt", 		 "f1.txt", 		 "g1.txt", 		 "h1.txt", 
		 "i1.txt", 		 "j1.txt", 		 "k1.txt", 		 "l1.txt", 	
		 "m1.txt", 		 "n1.txt", 		 "o1.txt", 		 "p1.txt", 	
		 "q1.txt", 		 "r1.txt", 		 "s1.txt",        "t1.txt", 		 
		 "u1.txt", 		 "v1.txt", 		 "w1.txt", 	     "x1.txt", 		 
		 "y1.txt",        "z1.txt"};
	
		 for (int i = 0; i < ALPH_SIZE; i++){
			 typo_string in_file = dir + in_fileslist[i];
			 typo_string out_file = dir + out_fileslist[i];
			 build();
		 }
		 	return RET_FSUCCESS;
}

