/**************************************************************************
 *  MonGraph                                                              *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <QApplication>
#include <QtSql>
#include "WxMain.h"
//---------------------------------
static bool createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("server_sd");
    db.setUserName("oleg"); 
    db.setHostName("10.0.10.45"); 
    db.setPassword(""); 
    if (!db.open()) return false;
    return true;
}
//---------------------------------
int main( int argc, char *argv[] ){
    QApplication app( argc, argv );
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    if (!createConnection()){
//        qDebug() << "Cannot open database:" << db.lastError();
        QMessageBox::warning(0, "Error", "Cannot open database");
        return -1;
    }
    QSqlQuery query;
//  ----------------
    WxMain gwx(query);
    gwx.setWindowTitle(QApplication::translate("MonGraph", "Monitor", 0, QApplication::UnicodeUTF8));
    gwx.show();
//  ----------------
return app.exec();
}
//----------------------------------------------------------------
