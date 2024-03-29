﻿#pragma once
#include <QtWidgets>
#include <QtGlobal>
#include <QtWidgets/QMainWindow>
#include "ui_QTInstallPage.h"
#include "FramelessMainWindow.h"
#include <concrt.h>
#include <future>




class QTInstallPage : public FramelessMainWindow
{
    Q_OBJECT

public:
    QTInstallPage(QWidget *parent = nullptr);
    ~QTInstallPage();

	bool CheckMutexProgramRunning();
	void SetLanguage(int iLangType, bool showBtnLang = true);
	int GetLanguageType();

	std::wstring GetInstallDirectory();
	void SetInstallStepDescription(const std::wstring& description, int progressValue = -1);
	void NsisExtractFilesFinished();

	bool IsRunExe();
	bool IsCreateDesktopShortcutExe();
	bool IsBootExe();


protected:
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void SetLanguage(QAction* pAc);
	void SetLanguage(QString strLang);

	void SetCurrentPage(int iIndex);

	void InstallStart();
	void InstallExit(int nType = 1);
	void InstallFinished();

	void ReadLicenseText(const QString& strFile);

	void UpdateDriverInfo();


	void TranslateText();

    Ui::QTInstallPageClass ui;
	std::vector<QPushButton*> m_vecEnterBtns;

	QStringList m_waitingAddItems;

	std::future<void> m_addListItemAsync;
	concurrency::critical_section m_listItemLock;
	bool m_extractFilesFinished;

	QString m_style1;
	QString m_style2;
	QString m_style3;

	QTranslator* m_qTranslator;
	QString m_strAppNameVersion;
	QString m_strExtract;
	int m_iLangType;
	std::vector<concurrency::task<void>> m_tasks;

	double m_driverFreeGb;
	bool m_isRunExe;
};
