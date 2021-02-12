/*! \class ErrorReport
 *  \brief Error reporting and logging : Console, NTEventLog, File
*  \detailed
 * \ provide information necessary for some higher layer to automatically correct the problem
*  \ Example: 
*  \	  "Spelling check by algorithm x failed". 
*  \		More information: "Index file for algorithm x was not found". 
*  \		More information: "Click Help->Update to automatically fix the problem". 
*  \ Messages location: ErrorDefinitionsFileName
 *  \author Oxana Rybalko
 *  \version 0.0.2
 *  \date    2009
 *  \bug Not tested.
  *  \warning 
 */
#include "Log.h"
#include "Timer.h"
#include "Application.h"

#include <sstream>

using namespace IniFileNamespace;

ErrorReport::ErrorReport(const typo_string &install_dir, const IniFile *messagesIniFile): _install_dir(install_dir), _messagesIniFile(messagesIniFile)
{
//can be removed for performance sake
	DETAILED_DESCRIPTION = (*_messagesIniFile)[TEXT_CONSTANTS_SECTION][TXT_ID_00000];
//end_of_can _be removed

	_enabled = (S_TRUE == (*_messagesIniFile)[TRACE_SECTION][ENABLED]);
	_fileName = _install_dir + (*_messagesIniFile)[TRACE_SECTION][FILENAME];
	if (_fileName == S_EMPTY)
		_fileName=TYPO_LOG;
}

int ErrorReport::composeReportString(const unicode::typo_string &id, unicode::typo_string &reportStr)
{
	reportStr=id + S_SEM + (*_messagesIniFile)[MESSAGES_SECTION][id];

	Timer t;
	unicode::typo_string date=S_EMPTY;
	t.get_date(date);
	date[date.size()-1]=C_SEM;

	date += reportStr;
	reportStr = date;
	return RET_FSUCCESS;
}

int ConsoleReport::report(const unicode::typo_string &id, const unicode::typo_string &err_message)
{
	if (_enabled != true) return RET_FFAILURE;

	unicode::typo_string report=id + S_SEM + (*_messagesIniFile)[MESSAGES_SECTION][id];
	
	if (err_message != S_EMPTY) report = report + DETAILED_DESCRIPTION + err_message;
	
	cout << report << endl;
	
	return RET_FSUCCESS;
}

int LogReport::report(const unicode::typo_string &id, const unicode::typo_string &err_message){

	if (_enabled != true)
		return RET_FFAILURE;
	ofstream rs;
	rs.open(_fileName.c_str(),ios_base::app);

	if (false == rs.good())
		return RET_FFAILURE;

	unicode::typo_string report = S_EMPTY;
	if (composeReportString(id, report) == RET_FFAILURE) {
		rs.close();
		return RET_FFAILURE;
	}

	if (err_message != S_EMPTY)
		report = report + DETAILED_DESCRIPTION + err_message;

	report += S_NL;
	rs.write(report.c_str(), report.size());

	rs.close();
	return RET_FSUCCESS;
}

int LogReport::renewStream(ofstream &rs){

		return RET_FSUCCESS;
}

