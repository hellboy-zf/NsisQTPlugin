#ifndef __QTSETUPPLUGIN_QTSETUPPLUGINHELPER_H__
#define __QTSETUPPLUGIN_QTSETUPPLUGINHELPER_H__

#pragma once

#ifdef __cplusplus
#define QTPLUGINHELPER_API extern "C" __declspec(dllexport)
#else
#define QTPLUGINHELPER_API __declspec(dllexport)
#endif

#include <windows.h>
#include "nsis/pluginapi.h"

/// <summary>
/// 
/// </summary>
/// <param name="hwndParent"></param>
/// <param name="string_size"></param>
/// <param name="variables"></param>
/// <param name="stacktop"></param>
/// <param name="extra"></param>
/// <returns></returns>
/// 






QTPLUGINHELPER_API void BindEventToNsisFunc(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void NewThreadRun(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


//QTPLUGINHELPER_API void CheckMutexProgramRunning(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);

/**
* @brief Nsis calls the dll function for the last time, and releases the dll without adding /NOUNLOAD
* @return 
*/
QTPLUGINHELPER_API void FinalCall(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);

/*******************************************************************INSTALL**************************************************************************/

QTPLUGINHELPER_API void InitSetupPluginHelper(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void ShowInstallWindow(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);



QTPLUGINHELPER_API void SetInstallStepDescription(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void NsisExtractFilesFinished(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void GetInstallDirectory(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void IsRunExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void IsCreateDesktopShortcutExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void IsBootExe(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void GetLanguageType(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);

/*******************************************************************INSTALL**************************************************************************/


QTPLUGINHELPER_API void UnInitUninstallPluginHelper(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void UnShowUninstallWindow(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void UnSetUnInstallStepDescription(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


QTPLUGINHELPER_API void UnNsisDeleteFilesFinished(HWND hwndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra);


#endif	//__QTSETUPPLUGIN_QTSETUPPLUGINHELPER_H__
