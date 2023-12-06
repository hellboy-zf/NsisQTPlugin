#include "ContextManager.h"
#include <QGuiApplication>
#include <QLocale>
#include <QTranslator>

#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <tlhelp32.h>
#include <tchar.h>
#include <algorithm>
#include "define.h"


ContextManager::ContextManager()
    : m_installPagePtr(nullptr)
	, m_uninstallPagePtr(nullptr)
	, m_handleMutex(NULL)
{
	m_exitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_strAppName = _T("QTSetupPlugin");
	m_strAppVersion = _T("1.0.0.0");
}

ContextManager::~ContextManager()
{
	if (m_exitEvent) {
		CloseHandle(m_exitEvent);
		m_exitEvent = NULL;
	}
	if (m_handleMutex) {
		CloseHandle(m_handleMutex);
		m_handleMutex = NULL;
	}
		

}


HANDLE ContextManager::getExitEvent()
{
	return m_exitEvent;
}


QTInstallPage* ContextManager::getInstallPagePtr()
{
	return m_installPagePtr;
}

QTUninstallPage* ContextManager::getUninstallPagePtr()
{
	return m_uninstallPagePtr;
}
void ContextManager::setAppName(const TCHAR* name)
{
	m_strAppName = name;
}

std::wstring ContextManager::getAppName()
{
	return m_strAppName;
}

void ContextManager::setAppAliasName(const TCHAR* aliasName)
{
	m_strAliasName = aliasName;
}

std::wstring ContextManager::getAppAliasName()
{
	return m_strAliasName;
}

void ContextManager::setAppVersion(const TCHAR* verstion)
{
	m_strAppVersion = verstion;
}

std::wstring ContextManager::getAppVersion()
{
	return m_strAppVersion;
}

std::wstring ContextManager::getAppNameVersion()
{
	return m_strAppName + _T(" ") + m_strAppVersion;
}

void ContextManager::setRequireSize(int32_t size)
{
	m_iRequreSize = size;
}

int32_t ContextManager::getRequireSize()
{
	return m_iRequreSize;
}

void ContextManager::setExeName(const TCHAR* name)
{
	m_strExeName = name;
}

std::wstring ContextManager::getExeName()
{
	return m_strExeName;
}

int readLicenseText(const std::wstring& strLicensePath)
{
	int iRet(0);
	std::wstring strLicenseText;

	//编码转换
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;  //std::codecvt_utf16
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);

	std::wifstream inFile;
	inFile.imbue(utf8_locale);
	inFile.open(strLicensePath, std::ios_base::binary);
	if (inFile.is_open())
	{
		std::wstring text;
		std::wostringstream  wos;
		while (std::getline(inFile, text))
		{
			wos << text << std::endl;
		}
		strLicenseText = wos.str();
	}
	else
	{
		std::wcout << _T("Can't open the file!");
		iRet = -1;
	}
    inFile.close();
	return iRet;
}


HMODULE ContextManager::getPluginHandle()
{
    return m_hWindow;
}

void ContextManager::setParentHwnd(HWND hWnd)
{
    m_hParendWnd = hWnd;
}

HWND ContextManager::getParentHwnd() const
{
    return m_hParendWnd;
}

void ContextManager::setPluginHandle(HMODULE handle)
{
    m_hWindow = handle;
}

void ContextManager::setExtraParameters(extra_parameters* param)
{
	m_pluginParms = param;
}

extra_parameters* ContextManager::getExtraParameters()
{
	return m_pluginParms;
}


void ContextManager::showInstallWindow(const TCHAR* szPluginDir, int iLangType)
{
	std::wstring str(szPluginDir);
	int nLangType(iLangType);
	if (!str.empty()) {
		QApplication::addLibraryPath(tstringToQString(szPluginDir));
	}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    int argc = 1;
    char currentPath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, currentPath, MAX_PATH);
    char *argv[1] = { {currentPath} };

    
	QApplication a(argc, argv);
	m_installPagePtr = new QTInstallPage();


	if (nLangType <= 0)
		nLangType = GetUserDefaultUILanguage();//"win10>设置>语言>Windows显示语言"
	//Note: QTranslator must be initialized here, otherwise the program will crash as an NSIS plugin dll
	//m_installPagePtr->m_qTranslator = new QTranslator();
	m_installPagePtr->SetLanguage(nLangType, iLangType >= 0);

	if (m_installPagePtr->CheckMutexProgramRunning())
		return;
	m_installPagePtr->show();
	a.exec();

}

void ContextManager::showUninstallWindow(const TCHAR* szPluginDir, int iLangType)
{
	std::wstring str(szPluginDir);
	int nLangType(iLangType);
	if (!str.empty()) {
		QApplication::addLibraryPath(tstringToQString(szPluginDir));
	}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	int argc = 1;
	char currentPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, currentPath, MAX_PATH);
	char* argv[2] = { {currentPath}, {} };


	QApplication a(argc, argv);
	m_uninstallPagePtr = new QTUninstallPage();
	if (nLangType <= 0)
		nLangType = GetUserDefaultUILanguage();//"win10>设置>语言>Windows显示语言"
	//Note: QTranslator must be initialized here, otherwise the program will crash as an NSIS plugin dll
	m_uninstallPagePtr->SetLanguage(nLangType);

	if (m_uninstallPagePtr->CheckMutexProgramRunning())
		return;
	m_uninstallPagePtr->show();
	a.exec();
}


void ContextManager::BindNsisFuncEvent(const std::wstring& eventName, INT_PTR nsisFuncAddress)
{
	m_installEventBindMap[eventName] = nsisFuncAddress;
}

bool ContextManager::ExecuteNsisFunEvent(const std::wstring& installEvent)
{
	if (m_pluginParms && m_installEventBindMap.find(installEvent) != m_installEventBindMap.end()) {
		m_pluginParms->ExecuteCodeSegment(m_installEventBindMap[installEvent] - 1, NULL);
		return true;
	}
	return false;
}

bool ContextManager::ExecuteNsisFunction(long funcAddress)
{
	if (m_pluginParms) {
		m_pluginParms->ExecuteCodeSegment(funcAddress, NULL);
		return true;
	}
	return false;
}

DWORD ContextManager::IsAppRunning(const std::wstring& appName)
{
	std::wstring strName(appName);
	if (_tcsstr(strName.c_str(), _T(".exe")) == NULL)
		strName += _T(".exe");

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe32)) {
		CloseHandle(hSnapshot);
		return 0;
	}
	do {
		std::wstring str(pe32.szExeFile);
		if (_tcsicmp(str.c_str(), strName.c_str()) == 0) {
			CloseHandle(hSnapshot);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &pe32));
	CloseHandle(hSnapshot);
	return 0;
}

