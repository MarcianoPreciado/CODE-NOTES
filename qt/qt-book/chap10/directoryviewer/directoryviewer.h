#ifndef DIRECTORYVIEWER_H
#define DIRECTORYVIEWER_H

#include <QDialog>
#include <QtGui>

class QDialogButtonBox;
class QFileSystemModel;
class QTableView;
class QTreeView;

class DirectoryViewer : public QDialog
{
    Q_OBJECT

public:
    DirectoryViewer(QWidget *parent = 0);

private slots:
    void createDirectory();
    void remove();

private:
    QTreeView *treeView;
    QFileSystemModel *model;
    QDialogButtonBox *buttonBox;
};

#endif
