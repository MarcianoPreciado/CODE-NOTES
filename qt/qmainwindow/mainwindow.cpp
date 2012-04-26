
#include <QtGui>

#include "finddialog.h"
#include "gotocell.h"
#include "mainwindow.h"
#include "sortdialog.h"
#include "spreadsheet.h"

MainWindow::MainWindow()
{
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    createActions();
    createContextMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    FindDialog = 0;

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New):
    newAction->setStatusTip(tr("Create a new spreadsheet"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
}
