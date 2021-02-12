/*
simple manual
http://msdn.microsoft.com/en-us/library/aa363680(VS.85).aspx

1. Create a message file (.mc)

Get the Message Compiler tool.

2. Compile the message file with the Message Compiler tool.
3. Build the resource files with the Resource Compiler tool.
4. Link the resource file to a DLL.
5. Add an event source name to the registry.

advise log4j port: C:\_spellChecker\code\model\helpers\apache-log4cxx-0.10.0
int addRegistryInfo();

*/
/*
#include "NTEventLog.h"

#include <string>

//using namespace std;
//using namespace SpellChecker;

int NTEventLogAppender::append(const typo_string &msg)
{
	SID *pCurrentUserSID;
	//log="Application";
	CCtUserSIDHelper::GetCurrentUserSID((::SID**) &pCurrentUserSID);
	addRegistryInfo();
	//wchar_t* wsource = L"Typo";
	typo_string source = "Typo";

//	HANDLE hEventLog = ::RegisterEventSourceW(NULL,wsource.c_str());

//DON'T DO IT, NEED TO CREATE MC FILE BEFORE
//	HANDLE hEventLog = ::RegisterEventSource(NULL, source.c_str());
	HANDLE hEventLog = NULL;
	//wchar_t* msgs = L"Test message";

//	WORD wType = "EVENTLOG_INFORMATION_TYPE";

	BOOL bSuccess = ::ReportEvent(
		hEventLog,
			NULL,//wType,
                NULL,//getEventCategory(event),
		0x1000,
		pCurrentUserSID,
		1,
		0,
		(LPCSTR*) &msg,
		NULL);
		return RET_FSUCCESS;
}



/*
 * Add this source with appropriate configuration keys to the registry.
 * /
int NTEventLogAppender::addRegistryInfo()
{
	typo_string source = "Typo";
			//typedef wchar_t logchar;
        DWORD disposition = 0;
        ::HKEY hkey = 0;
     //   LogString subkey(LOG4CXX_STR("SYSTEM\\CurrentControlSet\\Services\\EventLog\\"));
		typo_string subkey("SYSTEM\\CurrentControlSet\\Services\\EventLog\\");
        subkey.append("Application");
        //subkey.append(1, (logchar) 0x5C /* '\\' * /);
		subkey.append(1, '\\');
        subkey.append(source);
   //     LOG4CXX_ENCODE_WCHAR(wsubkey, subkey);

//DON'T DO IT, NEED TO CREATE MC FILE BEFORE

        long stat = RegCreateKeyEx(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, NULL,
                REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL,
                &hkey, &disposition);
        if (stat == ERROR_SUCCESS && disposition == REG_CREATED_NEW_KEY) {
            HMODULE hmodule = GetModuleHandle("Typo");
            if (hmodule == NULL) {
                hmodule = GetModuleHandle(0);
            }
            //wchar_t modpath[_MAX_PATH];
			//typo_string modpath;
			char modpath[_MAX_PATH];

//DON'T DO IT, NEED TO CREATE MC FILE BEFORE
			DWORD modlen = GetModuleFileName(hmodule, modpath, _MAX_PATH - 1);
modlen=-1;
            if (modlen > 0) {
                modpath[modlen] = 0;
                RegSetValueEx(hkey, "EventMessageFile", 0, REG_SZ, 
                        (LPBYTE) modpath, strlen(modpath) * sizeof(wchar_t));
                RegSetValueEx(hkey, "CategoryMessageFile", 0, REG_SZ, 
                        (LPBYTE) modpath, strlen(modpath) * sizeof(wchar_t));
                    DWORD typesSupported = 7;
                    DWORD categoryCount = 6;
                RegSetValueEx(hkey, "TypesSupported", 0, REG_DWORD, 
                           (LPBYTE)&typesSupported, sizeof(DWORD));
                RegSetValueEx(hkey, "CategoryCount", 0, REG_DWORD, 
                           (LPBYTE)&categoryCount, sizeof(DWORD));
            }
        }

        RegCloseKey(hkey);
        return RET_FSUCCESS;
}*/
