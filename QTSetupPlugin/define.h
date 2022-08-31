#ifndef QTSETUPPLUGIN_DEFINE_H_
#define QTSETUPPLUGIN_DEFINE_H_

#include <QObject>


/************************************************************************/
/* GUID保证只有一个实例运行                                                 */
/************************************************************************/

//NSIS_INSTALL PLUGIN
#define  INSTANCE_MUTEX_NSIS_INSTALL						_T("8D3BC2AE-DBDF-47F8-A0EE-E0AD4E760974")
//#define  INSTANCE_MUTEX_APP1								_T("D0D46E3E-E821-4BCF-BF49-193C16CBE539")
//#define  INSTANCE_MUTEX_APP2								_T("E3922287-EB4B-436E-B197-ADAB5A19FCE0")




#define INSTALL_EVENT_START_EXTRACT_FILES					TEXT("START_EXTRACT_FILES")
#define INSTALL_EVENT_USER_CANCEL							TEXT("USER_CANCEL")
#define INSTALL_EVENT_BEFORE_FINISHED						TEXT("BEFORE_FINISHED")
#define INSTALL_EVENT_UNSTALL_OLD_PRODUCT					TEXT("UNSTALL_OLD_PRODUCT")



#define UNINSTALL_EVENT_START_EXTRACT_FILES					TEXT("UN_START_DELETE_FILES")
#define UNINSTALL_EVENT_USER_CANCEL							TEXT("UN_USER_CANCEL")
#define UNINSTALL_EVENT_BEFORE_FINISHED						TEXT("UN_BEFORE_FINISHED")

#define STR_LANG_EN       "English"
#define STR_LANG_CN       "中文简体"
#define STR_LANG_TW       "中文繁體"

#define INT_LANG_EN			0x0409		//1033
#define INT_LANG_CN			0x0804		//2052
#define INT_LANG_TW			0x0404		//1028


inline QString tstringToQString(const std::wstring& str) {
	QString strResult;
#if (defined _UNICODE || defined UNICODE)
	strResult = QString::fromStdWString(str);
#else
	strResult = QString::fromStdString(str);
#endif
	return strResult;
}

inline std::wstring QStringTotstring(const QString& str) {
#if (defined _UNICODE || defined UNICODE)
	return str.toStdWString();
#else
	return str.toStdString();
#endif
}


#endif //QTSETUPPLUGIN_DEFINE_H_





