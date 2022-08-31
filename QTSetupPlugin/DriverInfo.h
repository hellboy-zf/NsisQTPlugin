
#ifndef QTSETUPPLUGIN_DRIVER_INFO_H_
#define QTSETUPPLUGIN_DRIVER_INFO_H_
#pragma once

#include <string>

enum { DRIVE_HOST = 7 };

class DriveInfo {
  public:
    static float GetFreeMB(int nDrive); // in MB
    static float GetTotalMB(int nDrive); // in MB

    static std::wstring GetVolume(int nDrive);
    static std::wstring GetRoot(int nDrive);
    static std::wstring GetFullName(int nDrive);
    static wchar_t GetLetter(int nDrive);
    static int GetType(int nDrive);
    static int GetPathType(const wchar_t *szPathName);
    static bool IsDriveAvailable(int nDrive);
    static void FormatName(std::wstring &sFileName);
    static int GetDrive(const wchar_t *szPathName);
    static bool IsMappedPath(const wchar_t *szPathName);
    static int IsRemotePath(const wchar_t *szPathName, bool bAllowFileCheck = true);
    static bool IsRemovablePath(const wchar_t *szPathName);
    static bool IsFixedPath(const wchar_t *szPathName);
    static int IsReadonlyPath(const wchar_t *szPathName);  // -1 = no such path, else TRUE/FALSE
    static unsigned long GetSerialNumber(int nDrive);
    static bool IsUNCPath(const wchar_t *szPathName);
};

#endif  //QTSETUPPLUGIN_DRIVER_INFO_H_