
#ifndef  sortdialog_h_INC
#define  sortdialog_h_INC

#include <QDialog>

#include "ui_sortdialog.h"

class SortDialog : public QDialog, public Ui::SortDialog
{
    Q_OBJECT
 public:
    SortDialog(QWidget *parent = 0);
    void setColumnRange(QChar first, QChar last);
};

#endif   /* ----- #ifndef sortdialog_h_INC  ----- */
