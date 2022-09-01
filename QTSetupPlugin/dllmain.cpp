#include "stdafx.h"
#include "UI/QTInstallPage.h"
#include <QtWidgets/QApplication>
#include "ContextManager.h"

//int main(int argc, char* argv[])
//{
//	QApplication a(argc, argv);
//	QTInstallPage w;
//	//QTUninstallPage w;
//	auto iLangType = GetUserDefaultUILanguage();
//	w.SetLanguage(iLangType);
//	w.show();
//	return a.exec();
//}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		ContextManager::getInstance()->setPluginHandle(hModule);
		break;
	}
	case DLL_THREAD_ATTACH: {
		break;
	}
	case DLL_THREAD_DETACH: {
		break;
	}
	case DLL_PROCESS_DETACH: {
		ContextManager::getInstance()->setPluginHandle(NULL);
		break;
	}
	}
	return TRUE;
}