#include "CMessageBox.h"
#include <windows.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMouseEvent>
#include "QTranslator"
#include "../define.h"

CMessageBox::CMessageBox(QWidget* parent,
    Icon icon,
    const QString& title,
    const QString& text,
    QMessageBox::StandardButtons standButtons,
    QMessageBox::StandardButton defaultButton)
    :QDialog(parent)
    , m_lblIconTitle(new QLabel(this))
    , m_lblTitle(new QLabel(this))
    , m_lblIconMain(new QLabel(this))
    , m_lblText(new QLabel(this))
    , m_btnSysClose(new QPushButton(this))
    , m_layout(new QVBoxLayout())
    , m_pButtonBox(new QDialogButtonBox(this))
    , m_mouserPressed(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(QSize(400, 166));

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    //Title
    QWidget* titleWidget = new QWidget();
    titleWidget->setObjectName("msgBox_widget");
    titleWidget->setFixedHeight(26);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(8, 0, 8, 0);
    titleLayout->setSpacing(10);
    titleWidget->setLayout(titleLayout);
    //titleWidget->setStyleSheet("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #343434, stop:1 #1e1e1e);");
	QPixmap pixmap(":/Resource/icon/ico_wnd.png");
	m_lblIconTitle->setPixmap(pixmap);
	m_lblIconTitle->setFixedSize(12, 12);
	m_lblIconTitle->setScaledContents(true);
    m_lblIconTitle->setAlignment(Qt::AlignVCenter);
    titleLayout->addWidget(m_lblIconTitle);

    titleLayout->addWidget(m_lblTitle);
    titleLayout->addStretch(1);

    m_btnSysClose->setFixedSize(18, 18);
	m_btnSysClose->setStyleSheet("QPushButton{\
	                                   border:none;\
	                                   background:transparent;\
	                                   border-image: url(:/Resource/image/sysBtnClose_normal.png);\
                                  }\
                                  QPushButton:focus{border-image: url(:/Resource/image/sysBtnClose_hover.png);}\
                                  QPushButton:hover{border-image: url(:/Resource/image/sysBtnClose_hover.png);}\
                                  QPushButton:pressed{border-image: url(:/Resource/image/sysBtnClose_pressed.png);}");
    titleLayout->addWidget(m_btnSysClose);
    m_layout->addWidget(titleWidget);

    QGroupBox* groupBox = new QGroupBox(this);
    groupBox->setObjectName("msgBox_box");
    groupBox->setFixedHeight(140);
    //groupBox->setStyleSheet("QGroupBox { \
    //                            background-color: #343434; \
    //                            border:1px solid #1b1b1b; \
    //                            margin-top: 0ex; \
    //                        }");
    m_layout->addWidget(groupBox);

    //Context + Buttons
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    groupBox->setLayout(vBoxLayout);
    vBoxLayout->setContentsMargins(20, 20, 20, 20);
    vBoxLayout->setSpacing(10);

    //Context
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    m_lblIconMain->setFixedSize(QSize(34, 34));
    m_lblText->setFixedHeight(60);
    m_lblText->setTextFormat(Qt::TextFormat::AutoText);
    hLayout->addWidget(m_lblIconMain);
    hLayout->addWidget(m_lblText);
    hLayout->addStretch(1);
    hLayout->setSpacing(16);

    vBoxLayout->addLayout(hLayout);

    //Buttons
    QHBoxLayout* hLayoutButtons = new QHBoxLayout();
    hLayoutButtons->setContentsMargins(0, 0, 0, 0);
    hLayoutButtons->addStretch(1);
    hLayoutButtons->addWidget(m_pButtonBox);
    vBoxLayout->addLayout(hLayoutButtons);

    m_pButtonBox->setFixedHeight(28);
    setLayout(m_layout);

    m_pButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons((int)standButtons));
    setDefaultButton(defaultButton);

    QList<QAbstractButton*> buttons = m_pButtonBox->buttons();
    for (int i = 0; i < buttons.size(); i++) {
        QDialogButtonBox::StandardButton button = m_pButtonBox->standardButton(buttons.at(i));
        QPushButton* pushButton = m_pButtonBox->button(button);
        if (pushButton)
        {
            pushButton->setFixedSize(QSize(90, 28));
            if (button == QDialogButtonBox::Ok || button == QDialogButtonBox::Yes) {
                pushButton->setText(tr("00050001"));
            }
            else {
                pushButton->setText(tr("00050002"));
            }
            if (i == 0)
                pushButton->setFocus();
        }
    }

    m_lblTitle->setObjectName("msgBox_lblTitle");
    m_lblTitle->setText(title);
    m_lblTitle->setStyleSheet("font-family:Microsoft YaHei;font-size:14px;");
    m_lblText->setObjectName("msgbox_lblText");
    m_lblText->setText(text);
    m_lblText->setStyleSheet("font-family:Microsoft YaHei;font-size:12px;");

    if (icon == Messsage) {
        m_lblIconMain->setStyleSheet("border-image:url(::/Resource/icon/ico_remind.png)");
    }
    else if (icon == Question) {
        m_lblIconMain->setStyleSheet("border-image:url(:/Resource/icon/ico_question.png)");
    }
    else if (icon == Warning) {
        m_lblIconMain->setStyleSheet("border-image:url(:/Resource/icon/ico_warn.png)");
    }
    else if (icon == Error) {
        m_lblIconMain->setStyleSheet("border-image:url(:/Resource/icon/ico_error.png)");
    }

    //setStyleSheet("QDialog{border:1px solid black;border-color: rgb(29, 29, 29);}");

    connect(m_pButtonBox, SIGNAL(clicked(QAbstractButton*)), this,
        SLOT(onButtonClicked(QAbstractButton*)));

    connect(m_btnSysClose, &QPushButton::clicked, [this]() {
        done(QMessageBox::StandardButton::NoButton);
        });
#ifdef _WIN32
	::SetWindowPos(HWND(this->winId()), HWND_TOP, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
}


void CMessageBox::setDefaultButton(QPushButton* button)
{
    if (!m_pButtonBox->buttons().contains(button)) {
        return;
    }

    button->setDefault(true);
    button->setFocus();
}

void CMessageBox::setDefaultButton(QMessageBox::StandardButton defaultButton)
{
    setDefaultButton(m_pButtonBox->button(QDialogButtonBox::StandardButton(defaultButton)));
}

void CMessageBox::setBtnTextOk(const QString& strText)
{
	QList<QAbstractButton*> buttons = m_pButtonBox->buttons();
	for (int i = 0; i < buttons.size(); i++) {
		QDialogButtonBox::StandardButton button = m_pButtonBox->standardButton(buttons.at(i));
		QPushButton* pushButton = m_pButtonBox->button(button);
		if (pushButton)
		{
			if (button == QDialogButtonBox::Ok || button == QDialogButtonBox::Yes) {
				pushButton->setText(strText);
                break;
			}
		}
	}
}

void CMessageBox::setBtnTextNo(const QString& strText)
{
	QList<QAbstractButton*> buttons = m_pButtonBox->buttons();
	for (int i = 0; i < buttons.size(); i++) {
		QDialogButtonBox::StandardButton button = m_pButtonBox->standardButton(buttons.at(i));
		QPushButton* pushButton = m_pButtonBox->button(button);
		if (pushButton)
		{
			if (button != QDialogButtonBox::Ok && button != QDialogButtonBox::Yes) {
				pushButton->setText(strText);
				break;
			}
		}
	}
}

void CMessageBox::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_mouserPressed = true;
        m_point = event->globalPos() - this->pos();
        event->accept();
    }
}

void CMessageBox::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mouserPressed && (event->buttons() & Qt::LeftButton)) {
        this->move(event->globalPos() - m_point);
        event->accept();
    }
}

void CMessageBox::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    m_mouserPressed = false;
}

int CMessageBox::execReturnCode(QAbstractButton* button)
{
    return m_pButtonBox->standardButton(button);
}

void CMessageBox::onButtonClicked(QAbstractButton* button)
{
    done(execReturnCode(button));
}
