
#ifndef __WORDNETAPIHANDLER_H__
#define __WORDNETAPIHANDLER_H__

//#include "wn.h"
//#include <stdio.h>
//#include <ctype.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
extern "C" {
#include "wn.h"
}

extern "C" int wninit();

class WordNetApiHandler
{
public : 
	WordNetApiHandler(const char* filename="C:\\Program Files\\WordNet\\2.1\\dict\\data.verb")://data.noun") : 
	   _filename(filename),_fp(NULL)
	   {	
	/*	   if (wninit()) {		/* open database * /
			   printf("wn: Fatal error - cannot open WordNet database\n");
			   exit (-1);
		   }

		   errno_t err;
		   // Open for read (will fail if file "crt_fopen_s.c" does not exist)
		   if( (err  = fopen_s( &_fp, filename, "r" )) !=0 )
			   printf( "The file %s was not opened\n", filename );
		   else
			   printf( "The file %s was opened\n", filename );
*/
		   int ret = wninit();
		   if (ret) {
			   cout << "WordNet could not be started" << endl;
			   exit(ret);
		   }


//		   if (in_wn(token, NOUN) || in_wn(token, VERB) || in_wn(token, ADJECTIVE)|| in_wn(token, ADVERB))

//		   char* token = 0;
//		   token = "Kansas";
//		   token = "Andreaea";
//		   token = "sepal";
//		   token = "leaf";
		   // unsigned int is_defined(canonicalForm, pos); // ALL_POS);
		   // unsigned int = in_wn(canonicalForm, pos);
		  // SynsetPtr s_w = findtheinfo_ds(token, VERB, HYPERPTR, ALLSENSES);
		   
//		   unsigned int res = is_defined(token, NOUN);
		   /* TO CONTINUE HERE */
		  // if (in_wn(token, NOUN) || in_wn(token, VERB) || in_wn(token, ADJECTIVE)|| in_wn(token, ADVERB))
//		   is_defined();
//		   SynsetPtr s_w = findtheinfo(token, NOUN, HYPERPTR, ALLSENSES);
	   }	

	   ~WordNetApiHandler()
	   {
		   if (NULL != _fp)
			   fclose(_fp);
	   }

	   int findWord(char *word);
	   string _filename;
	   FILE *_fp;
};


#endif //__WORDNETAPIHANDLER_H__