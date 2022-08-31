#pragma once
#include <QtWidgets>
#include <QtGlobal>
#include <QtWidgets/QMainWindow>
#include "ui_QTUninstallPage.h"
#include "FramelessMainWindow.h"
#include <concrt.h>
#include <future>





class QTUninstallPage : public FramelessMainWindow
{
    Q_OBJECT

public:
    QTUninstallPage(QWidget *parent = nullptr);
    ~QTUninstallPage();

	bool CheckMutexProgramRunning();
	void SetLanguage(int iLangType);
	
	void SetUninstallStepDescription(const std::wstring& description, int progressValue = -1);
	void NsisDeleteFilesFinished();

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void SetCurrentPage(int iIndex);
	void SetLanguage(QString strLang);
	void UninstallStart();
	void UninstallExit(int nType = 1);
	void UninstallFinished();

	void TranslateText();

    Ui::QTUninstallPageClass ui;
	std::vector<QPushButton*> m_vecEnterBtns;

	QStringList m_waitingAddItems;

	std::future<void> m_addListItemAsync;
	concurrency::critical_section m_listItemLock;
	bool m_deleteFilesFinished;

	QTranslator* m_qTranslator;
	QString m_strAppNameVersion;
	QString m_strDelete;
};
