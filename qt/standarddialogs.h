
#include <QLayout>
#include <QApplication>
#include <QDialog>
#include <QtGui>
#include <QFont>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QObject>
class StandardDialogs: public QDialog
{
    Q_OBJECT
 public:
    StandardDialogs(QWidget *parent = 0, Qt::WindowFlags f = 0);
    QGridLayout *layout;
    QPushButton *filePushButton;
    QPushButton *colorPushButton;
    QPushButton *fontPushButton;
    QLineEdit *fileLineEdit;
    QLineEdit *fontLineEdit;
    QFrame *colorFrame;
 private slots:
    void slotOpenFileDlg();
    void slotOpenColorDlg();
    void slotOpenFontDlg();
};
