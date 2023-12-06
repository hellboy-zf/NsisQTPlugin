#include "QTSetupPluginHelper.h"
#include "ContextManager.h"
#include <sstream>
#pragma comment(lib,"User32.lib")


#define NSMETHOD_INIT() do {\
        ContextManager::getInstance()->setExtraParameters(extra); \
        ContextManager::getInstance()->setParentHwnd(hwndParent); \
        if(extra) { \
        extra->RegisterPluginCallback(ContextManager::getInstance()->getPluginHandle(), PluginCallback); } \
        EXDLL_INIT(); } while(false);

static UINT_PTR PluginCallback(enum NSPIM msg) {
    return 0;
}

void BindEventToNsisFunc(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
		TCHAR szEventName[MAX_PATH] = { 0 };
	popstring(szEventName);
	INT_PTR callbackFuncAddress = popintptr();
	ContextManager::getInstance()->BindNsisFuncEvent(szEventName, callbackFuncAddress);
}

void NewThreadRun(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
		long nsisFuncAddress = popint();

	std::thread t = std::thread([nsisFuncAddress]() {
		ContextManager::getInstance()->ExecuteNsisFunction(nsisFuncAddress - 1);
		});
	t.detach();
}

//void CheckMutexProgramRunning(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
//{
//
//	int iRet = ContextManager::getInstance()->CheckMutexProgramRunning();
//	pushint(iRet);
//}

void FinalCall(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
}

void InitSetupPluginHelper(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()

	TCHAR szAppName[MAX_PATH] = { 0 };
	popstring(szAppName);

	TCHAR szAppVersion[MAX_PATH] = { 0 };
	popstring(szAppVersion);

	int iAppFilesSize = popint();

	TCHAR szExeName[MAX_PATH] = { 0 };
	popstring(szExeName);



	ContextManager::getInstance()->setAppName(szAppName);
	ContextManager::getInstance()->setAppVersion(szAppVersion);
	ContextManager::getInstance()->setRequireSize(iAppFilesSize);
	ContextManager::getInstance()->setExeName(szExeName);
}

void ShowInstallWindow(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop, extra_parameters *extra)
{
	NSMETHOD_INIT()
	TCHAR szNsisPluginDir[MAX_PATH] = { 0 };
	popstring(szNsisPluginDir);

	TCHAR szLanguage[20] = { 0 };
	popstring(szLanguage);

	int iLangType(-1);
	iLangType = _tstoi(szLanguage);

    ContextManager::getInstance()->showInstallWindow(szNsisPluginDir, iLangType);
}


void SetInstallStepDescription(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
	TCHAR szDescription[MAX_PATH] = { 0 };
	long percent = -1;

	popstring(szDescription);
	percent = popint();

	if (ContextManager::getInstance()->getInstallPagePtr()) {
		ContextManager::getInstance()->getInstallPagePtr()->SetInstallStepDescription(szDescription, percent);
	}
}

void NsisExtractFilesFinished(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()

	if (ContextManager::getInstance()->getInstallPagePtr()) {
		ContextManager::getInstance()->getInstallPagePtr()->NsisExtractFilesFinished();
	}
}



void GetInstallDirectory(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
	std::wstring strInstallDir;
	if (ContextManager::getInstance()->getInstallPagePtr())
	{
		strInstallDir = ContextManager::getInstance()->getInstallPagePtr()->GetInstallDirectory();
	}
	pushstring(strInstallDir.c_str());
}

void IsRunExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
	
	int iRet(0);
	if (ContextManager::getInstance()->getInstallPagePtr())
	{
		iRet = ContextManager::getInstance()->getInstallPagePtr()->IsRunExe();
	}
	pushint(iRet);
}

void IsCreateDesktopShortcutExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
	
	int iRet(0);
	if (ContextManager::getInstance()->getInstallPagePtr())
	{
		iRet = ContextManager::getInstance()->getInstallPagePtr()->IsCreateDesktopShortcutExe();
	}
	pushint(iRet);
}

void IsBootExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
	
	int iRet(0);
	if (ContextManager::getInstance()->getInstallPagePtr())
	{
		iRet = ContextManager::getInstance()->getInstallPagePtr()->IsBootExe();
	}
	pushint(iRet);
}


void GetLanguageType(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	int iRet(0);
	if (ContextManager::getInstance()->getInstallPagePtr())
	{
		iRet = ContextManager::getInstance()->getInstallPagePtr()->GetLanguageType();
	}

	TCHAR str[20] = { 0 };
	swprintf_s(str, 20, _T("%d"), iRet); 
	pushstring(str);
}


void UnInitUninstallPluginHelper(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()

	TCHAR szAppName[MAX_PATH] = { 0 };
	popstring(szAppName);

	TCHAR szAppVersion[MAX_PATH] = { 0 };
	popstring(szAppVersion);

	TCHAR szExeName[MAX_PATH] = { 0 };
	popstring(szExeName);

	TCHAR szAppAliasName[MAX_PATH] = { 0 };
	popstring(szAppAliasName);

	std::wstring str(szAppAliasName);
	std::replace(str.begin(), str.end(), _T('&'), _T(' '));

	ContextManager::getInstance()->setAppName(szAppName);
	ContextManager::getInstance()->setAppVersion(szAppVersion);

	ContextManager::getInstance()->setExeName(szExeName);
	ContextManager::getInstance()->setAppAliasName(str.c_str());
}

void UnShowUninstallWindow(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
		
	TCHAR szNsisPluginDir[MAX_PATH] = { 0 };
	popstring(szNsisPluginDir);

	TCHAR szLanguage[20] = { 0 };
	popstring(szLanguage);

	int iLangType(0);
	iLangType = _tstoi(szLanguage);

	ContextManager::getInstance()->showUninstallWindow(szNsisPluginDir, iLangType);

}

void UnSetUnInstallStepDescription(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()
		
	TCHAR szDescription[MAX_PATH] = { 0 };
	long percent = -1;

	popstring(szDescription);
	percent = popint();

	if (ContextManager::getInstance()->getUninstallPagePtr()) {
		ContextManager::getInstance()->getUninstallPagePtr()->SetUninstallStepDescription(szDescription, percent);
	}
}

void UnNsisDeleteFilesFinished(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra)
{
	NSMETHOD_INIT()

		if (ContextManager::getInstance()->getUninstallPagePtr()) {
			ContextManager::getInstance()->getUninstallPagePtr()->NsisDeleteFilesFinished();
		}
}
