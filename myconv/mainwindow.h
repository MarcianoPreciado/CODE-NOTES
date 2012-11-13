
#ifndef  mainwindow_INC
#define  mainwindow_INC

#include <QMainWindow>

class QAction;
class QImage;
class Qlist;
class QStringList;
class QSettings;

class ImageAttributes
{
 public:
    QString complFileName,
            fileName,
            geometry,
            format;
    qint64 size;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
 public:
    MainWindow(QWidget *parent = 0, Qt::WFlags f = 0);

 private:
    QSettings *settings;
    QString readableFiltersString;
    QStringList readableFormats;
}

#endif   /* ----- #ifndef mainwindow_INC  ----- */
