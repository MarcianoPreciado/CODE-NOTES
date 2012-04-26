
#ifndef  gotocell_h_INC
#define  gotocell_h_INC

#include <QDialog>
#include "ui_gotocell.h"

class GoToCellDialog : public QDialog, public Ui::GoToCellDialog
{
    Q_OBJECT
 public:
    GoToCellDialog(QWidget *parent = 0);
 private slots:
     void on_linEdit_textChanged();

};

#endif   /* ----- #ifndef gotocell_h_INC  ----- */
