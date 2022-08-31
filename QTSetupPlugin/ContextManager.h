#ifndef __QTSETUPPLUGIN_CONTEXTMANAGER_H__
#define __QTSETUPPLUGIN_CONTEXTMANAGER_H__
#include <windows.h>
#include <memory>
#include <mutex>
#include <tchar.h>
#include "nsis/api.h"
#include "UI/QTInstallPage.h"
#include "UI/QTUninstallPage.h"

class ContextManager
{
public:
	static const std::unique_ptr<ContextManager>& getInstance()
	{
		static std::unique_ptr<ContextManager> pInstance = nullptr;
		static std::once_flag oc;
		std::call_once(oc, [&] {
			pInstance = std::make_unique<ContextManager>();
			});
		return pInstance;
	}
    ContextManager();
	virtual ~ContextManager();

	HANDLE getExitEvent();
	QTInstallPage* getInstallPagePtr();
	QTUninstallPage* getUninstallPagePtr();

	void setAppName(const TCHAR* name);
	std::wstring getAppName();

	void setAppVersion(const TCHAR* verstion);
	std::wstring getAppVersion();
	std::wstring getAppNameVersion();

	void setRequireSize(int32_t size);
	int32_t getRequireSize();

    void setPluginHandle(HMODULE handle);
    HMODULE getPluginHandle();

    void setParentHwnd(HWND hWnd);
    HWND getParentHwnd() const;

    void setExtraParameters(extra_parameters *param);
    extra_parameters* getExtraParameters();


    void showInstallWindow(const TCHAR* szPluginDir, int iLangType);

	void showUninstallWindow(const TCHAR* szPluginDirr, int iLangType);

	void BindNsisFuncEvent(const std::wstring& eventName, INT_PTR nsisFuncAddress);
	bool ExecuteNsisFunEvent(const std::wstring& installEvent);
	bool ExecuteNsisFunction(long funcAddress);

	//void BindButtonClickedEvent(const std::wstring& buttonName, long nsisFuncAddress);
	
	//bool ExecuteButtonClickedEventFunction(const std::wstring& buttonName);

	int CheckMutexProgramRunning();

private:

	ContextManager(const ContextManager&) = delete;
	ContextManager(ContextManager&&) = delete;
	ContextManager& operator = (const ContextManager&) = delete;
private:

	HANDLE m_exitEvent;
	HANDLE m_handleMutex;

    HMODULE m_hWindow;
    HWND m_hParendWnd;
    extra_parameters *m_pluginParms;


	std::wstring m_strAppName;
	std::wstring m_strAppVersion;

	int32_t m_iRequreSize;	//安装目录文件夹 用于获取安装时所需磁盘空间大小
	QTInstallPage* m_installPagePtr;
	QTUninstallPage* m_uninstallPagePtr;

	std::map<std::wstring, INT_PTR> m_installEventBindMap;
};

#endif //__QTSETUPPLUGIN_CONTEXTMANAGER_H__
