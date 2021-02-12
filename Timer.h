#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef UNIX
#include <sys/time.h>
#else
#include <time.h>
#endif 

#include <algorithm>
#include <functional>

const int BUFF_MAX_SIZE=128;

class Timer
{
public:
	Timer(){}
	void timer_start() { start=clock();}
	typo_long_num get_time_lapsed() { return (typo_long_num) (clock() - start)/CLOCKS_PER_SEC;} 
	bool equal (typo_char c1, typo_char c2){return c1== c2;}

	void get_date(typo_string &s){
		time_t rawtime;
		typo_char timebuf[BUFF_MAX_SIZE];
		time( &rawtime );
		ctime_s(timebuf, BUFF_MAX_SIZE, &rawtime);
		s = timebuf;
		replace_if(s.begin(), s.end(), bind2nd(equal_to<typo_char>(), C_COLON), C_DOT ) ;
		replace_if(s.begin(), s.end(), bind2nd(equal_to<typo_char>(), C_SPACE), C_DOT ) ;
	}

	/// gets format 2009-24-02
	void get_date_time_plot(typo_string &s){
		struct tm timeinfo;
		time_t rawtime;
		typo_char timebuf[BUFF_MAX_SIZE];
		time(&rawtime);

		localtime_s(&timeinfo, &rawtime);
		strftime(timebuf, BUFF_MAX_SIZE, "%Y-%m-%d", &timeinfo);
		s=timebuf;
	}
private:
	clock_t start;

};



  

#endif // __TIMER_H__
