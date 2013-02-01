/**************************************************************************
 *  ReportMaker                                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <QApplication>
//  --
#include "dbreader.h"
#include "WxMain.h"
#include "defs.h"
//---------------------------------
int main( int argc, char *argv[] ){
    QApplication app( argc, argv );
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//  ----------------
    QString id = (argc > 1) ? argv[1] : QWidget::tr("КИП ПЩ");
    try{
        DBReader *dbr = new DBReader(BDHOST, BDNAME, "oleg", "", id);
//  ----------------
        WxMain gwx(dbr);
        if(gwx.Error())exit(EXIT_SUCCESS);
        gwx.setWindowTitle(QApplication::translate("ReportMaker", "Объект?", 0, QApplication::UnicodeUTF8));
        gwx.show();
//  ----------------
        return app.exec();
//  ----------------
    }catch(QString e){
        qDebug() << "FATAL_ERROR." << e;
        QMessageBox::warning(0, QWidget::tr("ERROR!"), e);
        exit(EXIT_FAILURE);
    }catch(...){
        qDebug() << "FATAL_ERROR." << "!!!";
        QMessageBox::warning(0,"!!!", QWidget::tr("!!!"));
        exit(EXIT_FAILURE);
    }
}
//----------------------------------------------------------------
