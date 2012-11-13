


void MainWindow::addFiles()
{
    QString path = QDir::homePath();
    if (settings->contains("General/Last Opened")) {
        path = settings->value("General/Last Opened").toString();
    }

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Images"),
                            path, readableFiltersString);
    if (!fileNmes.isEmpty()) {
        loadFiles(filenames);
    }
}

void MainWindow::addDirs()
{

    QString path = QDir::homePath();
    if (settings->contains("General/Last Opened")) {
        path = settings->value("General/Last Opened").toString();
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    path,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
}
