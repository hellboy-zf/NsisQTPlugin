#include "QTUninstallPage.h"
#include "../define.h"
#include "../ContextManager.h"
#include <ShlObj_core.h>
#include "../DriverInfo.h"
#include "CMessageBox.h"



QTUninstallPage::QTUninstallPage(QWidget *parent)
    : FramelessMainWindow(true,parent)
	, m_deleteFilesFinished(false)
	, m_qTranslator(nullptr)
{
    ui.setupUi(this);
	setWindowIcon(QIcon(":/Resource/image/logo.ico"));
	//FramelessMainWindow::loadStyleSheetFile(":/Resource/style/main.css", this);

	FramelessMainWindow::setAllWidgetMouseTracking(this);
	setResizeable(false);
	setTitlebar({ ui.boxTitle });
	ui.tabWidget->tabBar()->hide();

	SetCurrentPage(0);

	QString qStrText;
	std::wstring strText;
	/**********************************Install Main Page****************************************/

	connect(ui.btnMin, &QPushButton::clicked, [this]() {
		showMinimized();
		});

	connect(ui.btnClose, &QPushButton::clicked, [this]() {
		UninstallExit();
		});

	/**********************************Welcome Page****************************************/

	connect(ui.welcome_btn_uninstall, &QPushButton::clicked, [this]() {
		UninstallStart();
		});

	connect(ui.welcome_btn_cancel, &QPushButton::clicked, [this]() {
		UninstallExit();
		});

	m_vecEnterBtns.push_back(ui.welcome_btn_uninstall);
	m_vecEnterBtns.push_back(ui.welcome_btn_cancel);

	/**********************************DeleteFiles Page****************************************/

	connect(ui.deletefiles_btn_next, &QPushButton::clicked, [this]() {
		SetCurrentPage(2);
		});


	m_vecEnterBtns.push_back(ui.deletefiles_btn_next);

	/**********************************Finish Page****************************************/

	connect(ui.finish_btn_finish, &QPushButton::clicked, [this]() {
		UninstallFinished();
		});

	m_vecEnterBtns.push_back(ui.finish_btn_finish);
	/**********************************Multi-Language****************************************/

	m_strAppNameVersion = tstringToQString(ContextManager::getInstance()->getAppNameVersion());

}

QTUninstallPage::~QTUninstallPage()
{}

bool QTUninstallPage::CheckMutexProgramRunning()
{
	int iRet = ContextManager::getInstance()->CheckMutexProgramRunning();
	if (iRet)
	{
		QString strText;
		strText = tr("00020005");
		switch (iRet)
		{
		case 2:
			strText = strText.arg("App1");
			break;
		case 3:
			strText = strText.arg("App2");
			break;
		default:
			strText = strText.arg("Install");
			break;
		}
		CMessageBox box(this, CMessageBox::Question, tr("00020001"), strText);
		box.setBtnTextOk(tr("00020002"));
		box.exec();
		return true;
	}
	return false;
}


void QTUninstallPage::SetLanguage(int iLangType)
{

	switch (iLangType)
	{
	case INT_LANG_CN:
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_CN));
		break;
	case INT_LANG_TW:
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_TW));
		break;
	default:
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_EN));
		break;
	}
}

void QTUninstallPage::SetCurrentPage(int iIndex)
{
	ui.btnClose->setEnabled(false);
	switch (iIndex)
	{
	case 0:
		ui.btnClose->setEnabled(true);
		ui.welcome_btn_cancel->setFocus();
		break;
	case 1:
		ui.deletefiles_btn_next->setFocus();
		break;
	case 2:
		ui.finish_btn_finish->setFocus();
		m_deleteFilesFinished = true;
		break;
	default:
		break;
	}
	ui.tabWidget->setCurrentIndex(iIndex);
}

void QTUninstallPage::SetLanguage(QString strLang)
{

	if (strLang == QTextCodec::codecForLocale()->toUnicode(STR_LANG_CN))
	{
		strLang = QLatin1String(":/translations/zh_CN.qm");
	}
	else if (strLang == QTextCodec::codecForLocale()->toUnicode(STR_LANG_TW))
	{
		strLang = QLatin1String(":/translations/zh_TW");
	}
	else
	{
		strLang = QLatin1String(":/translations/en_US");
	}
	if (!strLang.isEmpty())
	{
		if (!m_qTranslator)
			m_qTranslator = new QTranslator();
		if (m_qTranslator)
		{
			qApp->removeTranslator(m_qTranslator);
			if (m_qTranslator->load(strLang))
			{
				qApp->installTranslator(m_qTranslator);
				TranslateText();
			}
		}
	}
}



void QTUninstallPage::SetUninstallStepDescription(const std::wstring& description, int progressValue)
{
	QMetaObject::invokeMethod(this, [this, progressValue]() {
		if (progressValue >= 0 && progressValue <= 100) {
			if (progressValue < 3)
				ui.deletefiles_progress_bar->setValue(3);
			else
				ui.deletefiles_progress_bar->setValue(progressValue);
		}
		}, Qt::QueuedConnection);

	concurrency::critical_section::scoped_lock lock(m_listItemLock);
	m_waitingAddItems.push_back(m_strDelete + tstringToQString(description));
}

void QTUninstallPage::NsisDeleteFilesFinished()
{
	QMetaObject::invokeMethod(this, [this]() {
		ui.deletefiles_btn_next->setEnabled(true);
		}, Qt::BlockingQueuedConnection);

	Sleep(300);

	QMetaObject::invokeMethod(this, [this]() {
		SetCurrentPage(2);
		}, Qt::BlockingQueuedConnection);
}

void QTUninstallPage::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_Return:
	case Qt::Key_Enter:

		for (auto btn : m_vecEnterBtns)
		{
			if (btn->hasFocus())
			{
				btn->click();
				break;
			}
		}
		break;
	default:
		break;
	}
	__super::keyPressEvent(event);
}


void QTUninstallPage::UninstallStart()
{
	SetCurrentPage(1);
	ContextManager::getInstance()->ExecuteNsisFunEvent(UNINSTALL_EVENT_START_EXTRACT_FILES);

	m_addListItemAsync = std::async(std::launch::async, [this]() {
		HANDLE exitEvent = ContextManager::getInstance()->getExitEvent();
		while (!m_deleteFilesFinished && WaitForSingleObject(exitEvent, 100) != WAIT_OBJECT_0) {
			concurrency::critical_section::scoped_lock lock(m_listItemLock);
			if (m_waitingAddItems.size() > 0) {
				QMetaObject::invokeMethod(this, [this]() {
					ui.deletefiles_list_widget->addItems(m_waitingAddItems);
					ui.deletefiles_list_widget->scrollToBottom();
					}, Qt::BlockingQueuedConnection);
				m_waitingAddItems.clear();
			}
		}
		});

}

void QTUninstallPage::UninstallExit(int nType)
{
	HANDLE exitEvent = ContextManager::getInstance()->getExitEvent();
	SetEvent(exitEvent);
	if (m_addListItemAsync.valid())
		m_addListItemAsync.wait_for(std::chrono::milliseconds(10000));
	close();
	if (nType == 1)
		ContextManager::getInstance()->ExecuteNsisFunEvent(UNINSTALL_EVENT_USER_CANCEL);
}

void QTUninstallPage::UninstallFinished()
{
	UninstallExit(0);
	ContextManager::getInstance()->ExecuteNsisFunEvent(UNINSTALL_EVENT_BEFORE_FINISHED);
}


void QTUninstallPage::TranslateText()
{
	/**********************************Install Main Page****************************************/

	ui.lblTitle->setText(tr("00020100").arg(m_strAppNameVersion));

	/**********************************Welcome Page****************************************/

	ui.welcome_lbl_title->setText(tr("00020200").arg(m_strAppNameVersion));
	ui.welcome_btn_uninstall->setText(tr("00020201"));
	ui.welcome_btn_cancel->setText(tr("00020202"));
#if QT_CONFIG(shortcut)
	ui.welcome_btn_uninstall->setShortcut(QString("U"));
	ui.welcome_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)


	/**********************************InstFiles Page****************************************/

	ui.deletefiles_lbl_title->setText(tr("00020300"));
	ui.deletefiles_lbl_explain->setText(tr("00020301").arg(m_strAppNameVersion));
	ui.deletefiles_btn_next->setText(tr("00020302"));
	ui.deletefiles_btn_cancel->setText(tr("00020303"));

	m_strDelete = tr("00020304") + ": ";


#if QT_CONFIG(shortcut)
	ui.deletefiles_btn_next->setShortcut(QString("U"));
	ui.deletefiles_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)


	/**********************************Finish Page****************************************/

	ui.finish_lbl_title->setText(tr("00020400").arg(m_strAppNameVersion));
	ui.finish_lbl_explain->setText(tr("00020401").arg(m_strAppNameVersion));
	ui.finish_btn_finish->setText(tr("00020402"));
#if QT_CONFIG(shortcut)
	ui.finish_btn_finish->setShortcut(QString("F"));
#endif // QT_CONFIG(shortcut)
}
