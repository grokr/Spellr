#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "data_types.h"

namespace SpellChecker 
{
	const int MINIMAL_WORD_SIZE=2;

	const int RET_FIND_EQUAL_CS = 0;
	const int RET_FIND_EQUAL_CIS = 1;
	const int RET_FIND_NOT_FOUND = 2;
	


#ifdef _DOUBLE
	const typo_long_num D_HUNDRED=100.0;
	const typo_long_num RET_DZERO=0;
	const typo_long_num D_ZERO=0.0;
	const typo_long_num D_ONE=1.0;
	const typo_long_num D_TWO=2.0;
	const typo_long_num D_THREE=3.0;
	const typo_long_num D_HALF=50.0;
#else if
	const typo_long_num D_HUNDRED=100;
	const typo_long_num RET_DZERO=0;
	const typo_long_num D_ZERO=0;
	const typo_long_num D_ONE=1;
	const typo_long_num D_TWO=2;
	const typo_long_num D_THREE=3;
	const typo_long_num D_HALF=50;
#endif

	const int ONE_HUNDRED_PERCENT=100;
	const int LINE_SIZE=1024;
	const int MAX_PATH_SIZE = 512;

	const int RET_FFAILURE=-1;
	const int RET_FFAILURE2=2;
	const int RET_FSUCCESS=1;

	const typo_string TESTING=_T("TESTING");
	const typo_string BUILD_DICTIONARY=_T("BUILD_DICTIONARY");
	const typo_string PRODUCTION=_T("PRODUCTION");

//////////////////FROM HERE
	//GET OUT TO ini files
	const typo_string UC_ALPHABET_EN_UPPER =_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	const typo_string UC_VOWELS_EN =_T("aeiouy");
	const typo_string UC_NON_VOWELS_EN =_T("bcdfghjklmnpqrstvwxz");
	const typo_string UC_FREQ_DOUBLES_EN= _T("csfdpbeolmnr");
	const typo_string UC_ALPHABET_EN_LOWER =_T("abcdefghijklmnopqrstuvwxyz-");
	const typo_string UC_NON_ALPHABET_EN =_T("\"-\\0987654321`~!+_)(*&^%$#@,./?><—;:}][{");
	const typo_string SENTENCE_ENDINGS =_T("!.?;:");
	const typo_string SENTENCE_PUNCTUATION =_T(",—...");
	const typo_string UC_ALPHABET_DE =_T("");//"ÄÖÜABCDEFGHIJKLMNOPQRSTUVWXYZ");
	const typo_string LC_ALPHABET_HE =_T("");//"קראטוןםפשדגכעיחלךףזסבהנמצתץ");
	const typo_string UC_ALPHABET_RU =_T("");//"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
	const typo_string IN=_T("IN");
	const typo_string OUT=_T("OUT");

	const typo_string output_dir = _T("\\output");

	const typo_string APP_INI_FILENAME=_T("\\ini\\app.ini");
	const typo_string ErrorDefinitionsFileName = _T("\\ini\\messages_");
	//
	const typo_string INI_EXT = _T(".ini");
	const typo_string TXT_EXT = _T(".txt");

	const typo_string input_file_name=_T("\\misspelled.txt");
	const typo_string user_fixed_file_name=_T("\\user_fixed.txt");


	const typo_string output_txt_filename = _T("\\output.txt");
	const typo_string output_xml_filename = _T("\\output.xml");
	const typo_string output_aspell_filename = _T("\\aspell_output.txt");

	const typo_string output_filename_nofreq = "dict_no_freq.txt";

	const typo_string wordsMappingFileName = _T("\\config\\wordsMapping_"); 
	const typo_string keyboardRulesFileName = _T("\\config\\KeysMapping_"); //Dvorak by default (preinstalled)
	const typo_string lettersMappingFileName = _T("\\config\\LettersMapping_"); 
	const typo_string curentDictionaryFileName = _T("\\config\\dict_"); 
	const typo_string nGramFileName = _T("\\config\\3gram_freq2_"); 
	const typo_string synonymsFileName = _T("\\config\\synonyms_"); 
	const typo_string sentencenGramFileName = _T("\\config\\sentence_ngram_"); 
	const typo_string LetterSetFileName = _T("\\config\\lettersSetIndex_"); 
	const typo_string outputFileName = _T("\\output.txt");

	const typo_string prefixes_file_name=_T("\\config\\prefixes_en.txt");
	const typo_string suffixes_file_name=_T("\\config\\suffixes_en.txt");
		 
//////////////////UNTIL HERE
	const typo_string USERNAME=_T("USERNAME");
	const typo_string DICT=_T("DICT");
	const typo_string PARAM_SET=_T("PARAM_SET");
	
	const typo_string APP=_T("APP");
	const typo_string REPORT=_T("REPORT");

	const typo_string IGNORE=_T("Ignore");
	
	const typo_string STD_OUT=_T("STD_OUT");
	const typo_string ASPELL_OUT=_T("ASPELL_OUT");
	
	const typo_string ASPELL_FIRST_LINE=_T("@(#) Typo version 0.0.3");
	const typo_string START_DIV=_T("<div>");
	const typo_string END_DIV=_T("</div>");
	
	
	const typo_string XML_HEADER_END=_T("</spellresult>");

	const typo_string XML_OUT=_T("XML_OUT");
	const typo_string XML_HEAD_START=_T("<OUTPUT>");
	const typo_string XML_HEAD_END=_T("</OUTPUT>");
	const typo_string XML_HEADER=_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	const typo_string XML_INPUT_START=_T("<input>");
	const typo_string XML_INPUT_END=_T("</input>");
	const typo_string XML_CORRECTIONS_START=_T("<corrections>");
	const typo_string XML_CORRECTIONS_END=_T("</corrections>");
	const typo_string XML_CORRECTION_START=_T("<correction>");
	const typo_string XML_CORRECTION_END=_T("</correction>");	

	const typo_string INIT=_T("INIT");
	const typo_string EXEC=_T("EXEC");
	const typo_string EXIT=_T("EXIT");

	const typo_string DEBUGGING_RUNNINGMODE =_T("DEBUGGING");
	const typo_string PRODUCTION_RUNNINGMODE =_T("PRODUCTION");
	const typo_string BUILD_DICTIONARY_RUNNINGMODE =_T("BUILD_DICTIONARY");
	const typo_string TESTING_RUNNINGMODE = _T("TESTING");

	const typo_string FAILURE="Failure";
	const typo_string SUCCESS="Success";
	const typo_string TYPO_INSTALL_DIR="TYPO_INSTALL_DIR";

	const typo_string LOGTYPE="LogType";
	const typo_string CONSOLE_REPORT="Console";
	const typo_string FILE_REPORT="File";

	enum message_type_t
	{ TYPE_BEGIN_REQUEST     =  1
	, TYPE_ABORT_REQUEST     =  2
	, TYPE_END_REQUEST       =  3
	, TYPE_PARAMS            =  4
	, TYPE_STDIN             =  5
	, TYPE_STDOUT            =  6
	, TYPE_STDERR            =  7
	, TYPE_DATA              =  8
	, TYPE_GET_VALUES        =  9
	, TYPE_GET_VALUES_RESULT = 10
	, TYPE_UNKNOWN           = 11
	};
	//END
	
	


	const typo_string RUNNING_MODE=_T("RunningMode");
	const typo_string CONTAINER_BUILDER=_T("Container_Builder");
	const typo_string SPELLING_CHECKER=_T("Spelling_Checker");
	const typo_string LEARN_BY_EXAMPLE=_T("LearnByExample");

	const typo_string NOT_IMEPLEMENTED = _T("no implementation yet");

	const typo_string S_TRUE=_T("true");
//	const typo_string S_FALSE=_T("false");
	const typo_string S_BACK_SLASH = _T("\\");

	const typo_string S_EMPTY = _T("");
	const typo_string S_DOT=_T(".");
	const typo_string S_COMA=_T(",");
	const typo_string S_STAR=_T("*");
	const typo_string S_SEM=_T(");");
	const typo_string S_SEMICOLON=_T(";");
	const typo_string S_TAB=_T("\t");
	const typo_string S_SPACE = _T(" ");
	const typo_string S_COLON = _T(":");
	const typo_string S_NL = _T("\n");
	const typo_string S_WHITESPACE = _T(" \t\n");
	const typo_string S_WORD_LIMITER=_T("#");

	const typo_char C_COMA = _T(',');
	const typo_char C_COLON = _T(':');
	const typo_char C_SEM = _T(';');
	const typo_char C_NULL=_T('\0');
	const typo_char C_STAR=_T('*');
	const typo_char C_DOT=_T('.');
	const typo_char C_SPACE=_T(' ');
	const typo_char C_AT=_T('&');
	const typo_char C_ZERO=_T('0');
	const typo_char C_SEMICOLON=_T(';');
	const typo_char C_SHARP=_T('#');
	
	
	

};

using namespace SpellChecker;

#endif //__CONSTANTS_H__