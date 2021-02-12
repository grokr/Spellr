#include "WordNetApiHandler.h"

int WordNetApiHandler::findWord(char *token)
{
//	if (NULL == _fp)
//		return -1;
	//int type = getsstype(word);
	return (in_wn(token, ALL_POS));// || in_wn(token, VERB) || in_wn(token, ADJECTIVE)|| in_wn(token, ADVERB));

	//			char *buff;
	//			buff=bin_search(word, _fp);
	//			//buff=GetWNStr(word,
//	return (NULL != buff);
	//GetWNStr(word
}