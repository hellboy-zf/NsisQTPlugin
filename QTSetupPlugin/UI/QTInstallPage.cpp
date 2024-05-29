#include "QTInstallPage.h"
#include <QtXml>
#include <ShlObj_core.h>
#include <QTextCodec>
#include "../DriverInfo.h"
#include "../ContextManager.h"
#include "../define.h"
#include "CMessageBox.h"


void SetAppStartLanguage(const QString& filePath, int32_t nIndex, int32_t nType)
{
	return; //设置启动语言

	//if (filePath.isEmpty())
		//return;
	//QString strFileName("AmasterClient.xml");
	//QString strName("LANGUAGE");
	//QString strValue("en");

	//if (nType == INT_LANG_CN)
	//	strValue = "chs";
	//else if (nType == INT_LANG_TW)
	//	strValue = "cht";

	//QString strFile = QString("%1\\AmasterClient\\%2").arg(filePath, strFileName);

	//if (nIndex == 2)
	//{
	//	strFileName = "ServerConfig.xml";
	//	if (nType == INT_LANG_CN)
	//		strValue = "zh-CN";
	//	else if (nType == INT_LANG_TW)
	//		strValue = "zh-Hant";
	//	strFile = QString("%1\\AmasterServer\\%2").arg(filePath, strFileName);
	//}

	//QFile file(strFile);
	//if (!file.open(QFile::ReadOnly | QFile::Text))
	//	return;

	//// Create a QXmlStreamReader object and set it to read from the file
	//QXmlStreamReader xmlReader(&file);

	//// Create a temporary file for writing the modified XML
	//QTemporaryFile tempFile;
	//if (!tempFile.open()) {
	//	return;
	//}

	//// Create a QXmlStreamWriter object and set it to write to the temporary file
	//QXmlStreamWriter xmlWriter(&tempFile);
	//xmlWriter.setAutoFormatting(true);
	//xmlWriter.writeStartDocument();

	//// Traverse the XML file using QXmlStreamReader and write each node to the temporary file using QXmlStreamWriter
	//while (!xmlReader.atEnd() && !xmlReader.hasError()) {
	//	QXmlStreamReader::TokenType token = xmlReader.readNext();
	//	if (token == QXmlStreamReader::StartElement) {
	//		xmlWriter.writeStartElement(xmlReader.name().toString());
	//		if (xmlReader.name() == "item" && xmlReader.attributes().value("name") == strName) {
	//			for (auto& attribute : xmlReader.attributes())
	//			{
	//				if (attribute.name() != "value")
	//					xmlWriter.writeAttribute(attribute.name().toString(), attribute.value().toString());
	//			}
	//			xmlWriter.writeAttribute("value", strValue);
	//		}
	//		else
	//			xmlWriter.writeAttributes(xmlReader.attributes());
	//	}
	//	else if (token == QXmlStreamReader::EndElement) {
	//		xmlWriter.writeEndElement();
	//	}
	//	else if (token == QXmlStreamReader::Comment) {
	//		xmlWriter.writeComment(xmlReader.text().toString());
	//	}
	//	else if (token == QXmlStreamReader::Characters) {
	//		xmlWriter.writeCharacters(xmlReader.text().toString());
	//	}
	//}
	//xmlWriter.writeEndDocument();

	//file.close();

	//if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
	//	return;

	//tempFile.reset();
	//QByteArray data = tempFile.readAll();
	//file.write(data);

	//tempFile.close();
	//file.close();
}

QTInstallPage::QTInstallPage(QWidget *parent)
    : FramelessMainWindow(true,parent)
	, m_extractFilesFinished(false)
	, m_qTranslator(nullptr)
	, m_iLangType(INT_LANG_EN)
	, m_driverFreeGb(0.0)
	, m_isRunExe(false)
{
    ui.setupUi(this);
	setWindowIcon(QIcon(":/Resource/image/logo.ico"));
	FramelessMainWindow::loadStyleSheetFile(":/Resource/css/main.css", this);

	FramelessMainWindow::setAllWidgetMouseTracking(this);
	

	setResizeable(false);
	setTitlebar({ ui.boxTitle });
	ui.tabWidget->tabBar()->hide();

	m_style1 = m_style2 = m_style3 = styleSheet();
	m_style2.replace("back1.png", "back2.png");
	m_style3.replace("back1.png", "back3.png");

	SetCurrentPage(0);

	QString qStrText;
	std::wstring strText;
	/**********************************Install Main Page****************************************/


	connect(ui.btnMin, &QPushButton::clicked, [this]() {
		showMinimized();
		});

	connect(ui.btnClose, &QPushButton::clicked, [this]() {
		if (ui.tabWidget->currentIndex() == 4)
			InstallFinished();
		else
			InstallExit();
		});

	/**********************************Welcome Page****************************************/

	connect(ui.welcome_btn_next, &QPushButton::clicked, [this]() {
		SetCurrentPage(1);
		});

	connect(ui.welcome_btn_cancel, &QPushButton::clicked, [this]() {
		InstallExit();
		});

	m_vecEnterBtns.push_back(ui.welcome_btn_next);
	m_vecEnterBtns.push_back(ui.welcome_btn_cancel);
	/**********************************License Page****************************************/

	
	ui.license_btn_next->setEnabled(false);

	connect(ui.license_radio_agree, &QRadioButton::toggled, [this](bool checked) {
		if (checked)
		{
			ui.license_btn_next->setEnabled(true);
			ui.license_btn_next->setFocus();
		}
		else
			ui.license_btn_next->setEnabled(false);
		});

	connect(ui.license_btn_back, &QPushButton::clicked, [this]() {
		SetCurrentPage(0);
		});

	connect(ui.license_btn_next, &QPushButton::clicked, [this]() {
		SetCurrentPage(2);
		});

	connect(ui.license_btn_cancel, &QPushButton::clicked, [this]() {
		InstallExit();
		});

	m_vecEnterBtns.push_back(ui.license_btn_back);
	m_vecEnterBtns.push_back(ui.license_btn_next);
	m_vecEnterBtns.push_back(ui.license_btn_cancel);
	/**********************************Directory Page****************************************/

	

	//设置默认安装路径
	TCHAR tcProgramFile[MAX_PATH];
	SHGetSpecialFolderPath(NULL, tcProgramFile, CSIDL_PROGRAM_FILES, 0);
	strText = tcProgramFile;
	strText += _T("\\") + ContextManager::getInstance()->getAppName();
	ui.directory_edit_dir->setText(tstringToQString(strText));

	connect(ui.directory_btn_dir_select, &QPushButton::clicked, [this]() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("00010001"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		ui.directory_btn_install->setFocus();
		if (dir.isEmpty())
			return;
		dir = QDir::toNativeSeparators(dir);
		//根目录自带"\\"
		if (!dir.endsWith("\\"))
			dir += "\\";
		dir += tstringToQString(ContextManager::getInstance()->getAppName());
		ui.directory_edit_dir->setText(dir);
		UpdateDriverInfo();
		});


	connect(ui.directory_btn_back, &QPushButton::clicked, [this]() {
		SetCurrentPage(1);
		});

	connect(ui.directory_btn_install, &QPushButton::clicked, [this]() {
		InstallStart();
		});

	connect(ui.directory_btn_cancel, &QPushButton::clicked, [this]() {
		InstallExit();
		});

	m_vecEnterBtns.push_back(ui.directory_btn_back);
	m_vecEnterBtns.push_back(ui.directory_btn_install);
	m_vecEnterBtns.push_back(ui.directory_btn_cancel);
	m_vecEnterBtns.push_back(ui.directory_btn_dir_select);
	/**********************************InstFiles Page****************************************/

	connect(ui.instfiles_btn_next, &QPushButton::clicked, [this]() {
		SetCurrentPage(4);
		});


	m_vecEnterBtns.push_back(ui.instfiles_btn_next);
	/**********************************Finish Page****************************************/

	connect(ui.finish_btn_run_exe, &QPushButton::clicked, [this]() {
		m_isRunExe = true;
		InstallFinished();
		});

	m_vecEnterBtns.push_back(ui.finish_btn_run_exe);


	/**********************************Multi-Language****************************************/

	m_strAppNameVersion = tstringToQString(ContextManager::getInstance()->getAppNameVersion());

	QMenu* mulLang = new QMenu(this);
	QActionGroup* acgLang = new QActionGroup(this);

	QAction* acLangCH = new QAction(QTextCodec::codecForLocale()->toUnicode(STR_LANG_CN), acgLang);
	acLangCH->setCheckable(true);
	acLangCH->setChecked(true);
	mulLang->addAction(acLangCH);

	mulLang->addSeparator();

	QAction* acLangTW = new QAction(QTextCodec::codecForLocale()->toUnicode(STR_LANG_TW), acgLang);
	acLangTW->setCheckable(true);
	mulLang->addAction(acLangTW);

	mulLang->addSeparator();

	QAction* acLangEN = new QAction(QTextCodec::codecForLocale()->toUnicode(STR_LANG_EN), acgLang);
	acLangEN->setCheckable(true);
	mulLang->addAction(acLangEN);

	acgLang->setExclusive(true);

	ui.btnLang->setMenu(mulLang);

	connect(acgLang, &QActionGroup::triggered, [this](QAction* pAc) {
		SetLanguage(pAc);
		});

	//acLangCH->trigger();
}

QTInstallPage::~QTInstallPage()
{}


bool QTInstallPage::CheckMutexProgramRunning()
{
	bool bRet(false);
	QString strText;
	if (ContextManager::getInstance()->IsAppRunning(ContextManager::getInstance()->getExeName()))
	{
		strText = tr("00010006");
		strText = strText.arg(tstringToQString(ContextManager::getInstance()->getExeName()));
		bRet = true;
	}
	//else
	//{
	//	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	//	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	//	TCHAR* name = _tcsrchr(szFilePath, _T('\\')) + 1;
	//	if (ContextManager::getInstance()->IsAppRunning(name))
	//	{
	//		strText = tr("00010006");
	//		strText = strText.arg("Installer");
	//		bRet = true;
	//	}
	//}
	if (bRet && !strText.isEmpty())
	{
		CMessageBox box(this, CMessageBox::Error, tr("00010002"), strText);
		box.exec();
	}
	return bRet;
}

void QTInstallPage::SetLanguage(int iLangType, bool showBtnLang)
{

	switch (iLangType)
	{
	case INT_LANG_CN:
		m_iLangType = INT_LANG_CN;
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_CN));
		break;
	case INT_LANG_TW:
		m_iLangType = INT_LANG_TW;
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_TW));
		break;
	default:
		m_iLangType = INT_LANG_EN;
		SetLanguage(QTextCodec::codecForLocale()->toUnicode(STR_LANG_EN));
		break;
	}
	ui.btnLang->setVisible(showBtnLang);
}

int QTInstallPage::GetLanguageType()
{
	return m_iLangType;
}


void QTInstallPage::keyPressEvent(QKeyEvent* event)
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

void QTInstallPage::SetLanguage(QAction* pAc)
{
	SetLanguage(pAc->text());
}

void QTInstallPage::SetLanguage(QString strLang)
{
	//TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	//GetModuleFileName(ContextManager::getInstance()->getPluginHandle(), szFilePath, MAX_PATH);
	//(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串

	if (strLang == QTextCodec::codecForLocale()->toUnicode(STR_LANG_CN))
	{
		m_iLangType = INT_LANG_CN;
		ui.btnLang->setText(QTextCodec::codecForLocale()->toUnicode("中"));
		ReadLicenseText(":/Resource/License/License_chs.txt");
		strLang = QLatin1String(":/translations/zh_CN.qm");
	}
	else if (strLang == QTextCodec::codecForLocale()->toUnicode(STR_LANG_TW))
	{
		m_iLangType = INT_LANG_TW;
		ui.btnLang->setText(QTextCodec::codecForLocale()->toUnicode("繁"));
		ReadLicenseText(":/Resource/License/License_cht.txt");
		strLang = QLatin1String(":/translations/zh_TW");
	}
	else
	{
		m_iLangType = INT_LANG_EN;
		ui.btnLang->setText("En");
		ReadLicenseText(":/Resource/License/License_en.txt");
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

void QTInstallPage::SetCurrentPage(int iIndex)
{
	ui.btnLang->setEnabled(false);
	ui.btnClose->setEnabled(true);
	switch (iIndex)
	{
	case 0:
		setStyleSheet(m_style1);
		ui.welcome_btn_next->setFocus();
		ui.btnLang->setEnabled(true);
		break;
	case 1:
		setStyleSheet(m_style2);
		ui.license_btn_next->setFocus();
		break;
	case 2:
		setStyleSheet(m_style1);
		ui.directory_btn_install->setFocus();
		break;
	case 3:
		setStyleSheet(m_style2);
		ui.btnClose->setEnabled(false);
		ui.instfiles_btn_next->setFocus();
		break;
	case 4:
		setStyleSheet(m_style3);
		ui.finish_btn_run_exe->setFocus();
		m_extractFilesFinished = true;
		break;
	default:
		break;
	}
	ui.tabWidget->setCurrentIndex(iIndex);
}


std::wstring QTInstallPage::GetInstallDirectory()
{
	return ui.directory_edit_dir->text().toStdWString();
}

void QTInstallPage::SetInstallStepDescription(const std::wstring& description, int progressValue)
{
	QString qstrDesc;
	//> 100卸载旧版本
	if (progressValue > 100)
	{
		switch (progressValue)
		{
		case 101:
			//Delete directory
			qstrDesc = tr("00010010");
			break;
		case 102:
			//Delete ShortCut
			qstrDesc = tr("00010011");
			break;
		case 103:
			//Delete Regedit Info
			qstrDesc = tr("00010012");
			break;
		default:
			break;
		}
		progressValue = 3;
	}
	else
	{
		qstrDesc = m_strExtract + tstringToQString(description);
	}

	QMetaObject::invokeMethod(this, [this, progressValue]() {
		if (progressValue >= 0 && progressValue <= 100) {
			if (progressValue < 3)
				ui.instfiles_progress_bar->setValue(3);
			else
				ui.instfiles_progress_bar->setValue(progressValue);
		}
		}, Qt::QueuedConnection);

	concurrency::critical_section::scoped_lock lock(m_listItemLock);

	m_waitingAddItems.push_back(qstrDesc);
}

void QTInstallPage::NsisExtractFilesFinished()
{
	m_tasks =
	{
	concurrency::create_task([this] {
			SetAppStartLanguage(ui.directory_edit_dir->text(),1,m_iLangType);
		}),
	concurrency::create_task([this] {
			SetAppStartLanguage(ui.directory_edit_dir->text(),2,m_iLangType);
		})
	};

	QMetaObject::invokeMethod(this, [this]() {
		ui.instfiles_btn_next->setEnabled(true);
		}, Qt::BlockingQueuedConnection);

	Sleep(300);

	QMetaObject::invokeMethod(this, [this]() {
		SetCurrentPage(4);
		}, Qt::BlockingQueuedConnection);
}

bool QTInstallPage::IsRunExe()
{
	return m_isRunExe;
	//return ui.finish_ck_run1->isChecked();
}

bool QTInstallPage::IsCreateDesktopShortcutExe()
{
	return true;
	//return ui.finish_ck_desktop_shorcuts1->isChecked();
}

bool QTInstallPage::IsBootExe()
{
	return false;
	//return ui.finish_ck_boot1->isChecked();
}



void QTInstallPage::InstallStart()
{
	int dirError(0);
	//单位 Mb
	if (m_driverFreeGb * 1024 < (ContextManager::getInstance()->getRequireSize() / 1024 / 1024 + 30))
	{
		dirError = 1;
	}

	QString strDir = ui.directory_edit_dir->text();
	if (strDir.length() == 0)
		return;

	int epos = strDir.lastIndexOf('\\');
	if (epos != -1)
	{
		QString strPath = strDir.left(epos);
		if(_taccess(QStringTotstring(strDir).c_str(), 06) == -1)	//是否有读写权限
		{
			int iError = errno;
			if (iError == EINVAL)	//Invalid parameter.
				dirError = 2;
			else if (iError == EACCES)	//Access denied: the file's permission setting does not allow specified access.
				dirError = 3;
		}
	}
	else
		dirError = 4;
	
	if (dirError)
	{
		CMessageBox box(this, CMessageBox::Error, tr("00010002"), tr("00010013").arg(dirError,4,10, QChar('0')));
		box.setBtnTextOk(tr("00010007"));
		box.exec();
		return;
	}
	//QDir dir(strDir);
	//if (!dir.exists()) {
	//	if (!dir.mkdir(strDir)) {
	//		return;
	//	}
	//}

	SetCurrentPage(3);

	ContextManager::getInstance()->ExecuteNsisFunEvent(INSTALL_EVENT_START_EXTRACT_FILES);
	//

	m_addListItemAsync = std::async(std::launch::async, [this]() {
		HANDLE exitEvent = ContextManager::getInstance()->getExitEvent();
		while (!m_extractFilesFinished && WaitForSingleObject(exitEvent, 100) != WAIT_OBJECT_0) {
			concurrency::critical_section::scoped_lock lock(m_listItemLock);
			if (m_waitingAddItems.size() > 0) {
				QMetaObject::invokeMethod(this, [this]() {
					ui.instfiles_list_widget->addItems(m_waitingAddItems);
					ui.instfiles_list_widget->scrollToBottom();
					}, Qt::BlockingQueuedConnection);
				m_waitingAddItems.clear();
			}
		}
		});

}

void QTInstallPage::InstallExit(int nType)
{
	int bRet = QMessageBox::StandardButton::Yes;
	if (nType == 1)
	{
		//bRet = QMessageBox::question(this, tr("00010002"), tr("00010003"));
		CMessageBox box(this,CMessageBox::Question, tr("00010002"), tr("00010003"), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
		bRet = box.exec();
	}
	if (bRet == QMessageBox::StandardButton::Yes)
	{
		HANDLE exitEvent = ContextManager::getInstance()->getExitEvent();
		SetEvent(exitEvent);
		if (m_addListItemAsync.valid())
			m_addListItemAsync.wait_for(std::chrono::milliseconds(10000));
		close();
		if (nType == 1)
			ContextManager::getInstance()->ExecuteNsisFunEvent(INSTALL_EVENT_USER_CANCEL);
	}
}

void QTInstallPage::InstallFinished()
{
	auto jointasks = concurrency::when_all(m_tasks.begin(),m_tasks.end());
	jointasks.wait();
	InstallExit(0);
	ContextManager::getInstance()->ExecuteNsisFunEvent(INSTALL_EVENT_BEFORE_FINISHED);
}

void QTInstallPage::ReadLicenseText(const QString& strFile)
{
	QString strLicense;
	QFile file(strFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!";
	}
	QTextStream toText(&file);
	toText.setCodec("UTF-8");
	while (!toText.atEnd())
	{
		QString str = toText.readLine();
		qDebug() << str;
		str += '\n';
		strLicense.append(str);
	}

	ui.license_textEdit->setText(strLicense);
}


void QTInstallPage::UpdateDriverInfo()
{
	m_driverFreeGb = 0.0;
	int driver = DriveInfo::GetDrive(ui.directory_edit_dir->text().toStdWString().c_str());
	if (driver > 0) {
		m_driverFreeGb = (double)DriveInfo::GetFreeMB(driver) / 1024;
		QString qStrText = tr("00010005").arg(m_driverFreeGb, 0, 'f', 1);
		ui.directory_lbl_mem_available->setText(qStrText);
	}
}

void QTInstallPage::TranslateText()
{
	/**********************************Install Main Page****************************************/

	ui.lblTitle->setText(tr("00010100").arg(m_strAppNameVersion));

	/**********************************Welcome Page****************************************/

	ui.welcome_lbl_title->setText(tr("00010200").arg(m_strAppNameVersion));
	ui.welcome_lbl_context->setText(tr("00010201").arg(m_strAppNameVersion));
	ui.welcome_btn_next->setText(tr("00010202"));
	ui.welcome_btn_cancel->setText(tr("00010203"));

#if QT_CONFIG(shortcut)
	ui.welcome_btn_next->setShortcut(QString("N"));
	ui.welcome_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)

	/**********************************License Page****************************************/

	ui.license_lbl_title->setText(tr("00010300"));
	ui.license_lbl_explain->setText(tr("00010301").arg(m_strAppNameVersion));
	ui.license_radio_agree->setText(tr("00010302"));
	ui.license_radio_disagree->setText(tr("00010303"));
	ui.license_btn_back->setText(tr("00010304"));
	ui.license_btn_next->setText(tr("00010202"));
	ui.license_btn_cancel->setText(tr("00010203"));

#if QT_CONFIG(shortcut)
	ui.license_radio_agree->setShortcut(QString("A"));
	ui.license_radio_disagree->setShortcut(QString("B"));
	ui.license_btn_back->setShortcut(QString("P"));
	ui.license_btn_next->setShortcut(QString("N"));
	ui.license_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)

	/**********************************Directory Page****************************************/

	ui.directory_lbl_title->setText(tr("00010400"));
	ui.directory_lbl_explain->setText(tr("00010401").arg(m_strAppNameVersion));
	ui.directory_btn_dir_select->setText(tr("00010402"));

	ui.directory_lbl_directory->setText(tr("00010404"));

	double requiredMb = (double)ContextManager::getInstance()->getRequireSize() / 1024 / 1024;
	if (requiredMb <= 0.0)
		requiredMb = 100.0;
	//requiredMb += 30; // 数据预留内存
	ui.directory_lbl_mem_required->setText(tr("00010004").arg(requiredMb, 0, 'f', 1));


	ui.directory_btn_back->setText(tr("00010304"));
	ui.directory_btn_install->setText(tr("00010403"));
	ui.directory_btn_cancel->setText(tr("00010203"));

	UpdateDriverInfo();

#if QT_CONFIG(shortcut)
	ui.directory_btn_dir_select->setShortcut(QString("B"));
	ui.directory_btn_back->setShortcut(QString("P"));
	ui.directory_btn_install->setShortcut(QString("I"));
	ui.directory_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)

	/**********************************InstFiles Page****************************************/

	ui.instfiles_lbl_title->setText(tr("00010500"));
	ui.instfiles_lbl_explain->setText(tr("00010501").arg(m_strAppNameVersion));
	ui.instfiles_btn_next->setText(tr("00010202"));
	ui.instfiles_btn_cancel->setText(tr("00010203"));

	m_strExtract = tr("00010502") + ": ";

#if QT_CONFIG(shortcut)
	ui.instfiles_btn_next->setShortcut(QString("N"));
	ui.instfiles_btn_cancel->setShortcut(QString("C"));
#endif // QT_CONFIG(shortcut)


	/**********************************Finish Page****************************************/
	//QString strAppName;
	//strAppName = ui.finish_box->title();

	ui.finish_lbl_title->setText(tr("00010600"));
	ui.finish_lbl_explain->setText(tr("00010601").arg(m_strAppNameVersion));
	//ui.finish_ck_run->setText(tr("00010602").arg(strAppName));
	//ui.finish_ck_desktop_shorcuts->setText(tr("00010603"));
	//ui.finish_ck_boot->setText(tr("00010604"));

	ui.finish_btn_run_exe->setText(tr("00010608"));

#if QT_CONFIG(shortcut)
	ui.finish_btn_run_exe->setShortcut(QString("F"));
	//ui.finish_ck_run->setShortcut(QString("R"));
	//ui.finish_ck_desktop_shorcuts->setShortcut(QString("S"));
	//ui.finish_ck_boot->setShortcut(QString("B"));
#endif // QT_CONFIG(shortcut)

}
