/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include "mainwindow.hpp"
#include <QApplication>
#include <QIcon>
#include <ctime>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(app.translate("main", "Page Designer"));
    app.setOrganizationName("Qtrac Ltd.");
    app.setOrganizationDomain("qtrac.eu");
    app.setWindowIcon(QIcon(":/icon.png"));
#ifdef Q_WS_MAC
    app.setCursorFlashTime(0);
#endif

    qsrand(static_cast<uint>(time(0)));
    MainWindow window;
    window.show();
    return app.exec();
}
