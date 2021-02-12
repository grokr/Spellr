#ifndef __CCTUSERSIDHELPER_H__
#define __CCTUSERSIDHELPER_H__

#if (defined(WIN32) || defined(_WIN32)) && !defined(_WIN32_WCE)
#include <windows.h>

/*#include "Log.h"
//using namespace std;

namespace SpellChecker{

class CCtUserSIDHelper
{
public:
        static bool FreeSid(SID * pSid)
        {
                return ::HeapFree(GetProcessHeap(), 0, (LPVOID)pSid) != 0;
        }

        static bool CopySid(SID * * ppDstSid, SID * pSrcSid)
        {
                bool bSuccess = false;

                DWORD dwLength = ::GetLengthSid(pSrcSid);
                *ppDstSid = (SID *) ::HeapAlloc(GetProcessHeap(),
                 HEAP_ZERO_MEMORY, dwLength);

                if (::CopySid(dwLength, *ppDstSid, pSrcSid))
                {
                        bSuccess = true;
                }
                else
                {
                        FreeSid(*ppDstSid);
                }

                return bSuccess;
        }

        static bool GetCurrentUserSID(SID * * ppSid)
        {
                bool bSuccess = false;

                // Pseudohandle so don't need to cl ose it
                HANDLE hProcess = ::GetCurrentProcess();
                HANDLE hToken = NULL;
                if (::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
                {
                        // Get the required size
                        DWORD tusize = 0;
                        GetTokenInformation(hToken, TokenUser, NULL, 0, &tusize);
                        TOKEN_USER* ptu = (TOKEN_USER*)new BYTE[tusize];

                        if (GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, tusize, &tusize))
                        {
                                bSuccess = CopySid(ppSid, (SID *)ptu->User.Sid);
                        }

                        CloseHandle(hToken);
                        delete [] ptu;
                }

                return bSuccess;
        }
};

class NTEventLogAppender: public ErrorReport{
public:
	virtual int report(const typo_string &id, const typo_string &err_message=S_EMPTY){
		append(err_message);
	}
private:
	int append(const typo_string &err_message);
	int addRegistryInfo();
};

};
*/
#endif //WIN32
#endif //__CCTUSERSIDHELPER_H__