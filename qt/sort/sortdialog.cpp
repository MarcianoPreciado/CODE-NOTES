
#include <QtGui>
#include "sortdialog.h"

SortDialog::SortDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    groupBox_2->hide();
    groupBox_3->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    setColumnRange('A', 'Z');
}

void SortDialog::setColumnRange(QChar first, QChar last)
{
    Column_4->addItem(tr("None"));
    Column_5->addItem(tr("None"));
    Column->setMinimumSize(Column_4->sizeHint());

    QChar ch = first;
    while (ch <= last) {
        Column->addItem(QString(ch));
        Column_4->addItem(QString(ch));
        Column_5->addItem(QString(ch));
        ch = ch.unicode() + 1;
    }
}
