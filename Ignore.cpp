#include "Ignore.h"

Ignore::Ignore():_bitmap(0), size(0),from(S_EMPTY), to(S_EMPTY){}

void Ignore::ignoreUppercased(){ _bitmap+=1;}
//PrintScreen, yearToDate, or ActiveX — variable names, computer terms, etc.
void Ignore::ignoreMixedCase(){_bitmap+=2;}

//Win98
void Ignore::ignoreContainingNumbers(){_bitmap+=4;}
void Ignore::ignoreSize(const int sz){_bitmap+=16;}
void Ignore::ignoreFromTo(const typo_string &from, const typo_string &to){_bitmap+=32;}
void Ignore::ignoreFileNames(){_bitmap+=64;}
void Ignore::ignoreCompanyProductNames(){_bitmap+=128;}
void Ignore::ignoreAbbreviations(){_bitmap+=256;}
void Ignore::addToIgnoreList(const typo_string &in){_bitmap+=512;}
void Ignore::addToIgnoreList(const typo_char &in){_bitmap+=LINE_SIZE; }
void Ignore::ignoreLanguage(const typo_string &in){_bitmap+=2048; }

//enum{Html, XML, C++, Medical, ...};
void Ignore::ignoreDomain(const typo_string &domain){_bitmap+=4096; }
void Ignore::ignoreHtml(){_bitmap+=8192; } //power 2, 12-th
