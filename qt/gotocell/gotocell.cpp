
#include <QtGui>
#include "gotocell.h"

GoToCellDialog::GoToCellDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    linEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void GoToCellDialog::on_linEdit_textChanged()
{
    okButton->setEnabled(linEdit->hasAcceptableInput());
}
