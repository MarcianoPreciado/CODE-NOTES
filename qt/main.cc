#include <QApplication>
#include <QDialog>
#include <QtGui>
#include <QFont>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include "standarddialogs.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StandardDialogs xx;
    xx.show();
    return app.exec();

}
