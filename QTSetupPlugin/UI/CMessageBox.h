
#ifndef QTSETUPPLUGIN_UI_MESSAGEBOX_H
#define QTSETUPPLUGIN_UI_MESSAGEBOX_H

#include <QDialog>
#include <QPoint>
#include <QMessageBox>

class QLabel;
class QVBoxLayout;
class QDialogButtonBox;
class QMouseEvent;

class CMessageBox : public QDialog
{
    Q_OBJECT
public:
    enum Icon {
        Messsage,
        Question,
        Warning,
        Error,
    };

    explicit CMessageBox(QWidget* parent = 0,
        Icon icon = Messsage,
        const QString& title = "Tip",
        const QString& text = "",
        QMessageBox::StandardButtons = QMessageBox::Ok,
        QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    void setDefaultButton(QPushButton* button);
    void setDefaultButton(QMessageBox::StandardButton defaultButton);

    void setBtnTextOk(const QString& strText);
    void setBtnTextNo(const QString& strText);
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    int execReturnCode(QAbstractButton* button);

signals:

public slots:
private slots:
    void onButtonClicked(QAbstractButton* button);
private:
    QLabel* m_lblIconTitle;
    QLabel* m_lblTitle;
    QLabel* m_lblIconMain;
    QLabel* m_lblText;
    QVBoxLayout* m_layout;
    QDialogButtonBox* m_pButtonBox;
    bool m_mouserPressed;
    QPoint m_point;
};

#endif // QTSETUPPLUGIN_UI_MESSAGEBOX_H
