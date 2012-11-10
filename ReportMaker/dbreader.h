/**************************************************************************
 *  ReportMaker (SCADA)                                                   *
 *  Copyright (C)  2012   by  Oleg Shirokov       olgshir@gmail.com       *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _DBReader_h_
#define _DBReader_h_
//  --
#include <QtGui>
#include <QtSql>
#include <QString>
#include "BoxCalendar.h"
#include "BoxObject.h"
#include "BoxReport.h"
//  --
QT_FORWARD_DECLARE_CLASS(QSqlError)
//----------------------------------------------------------------
//using namespace std;
//----------------------------------------------------------------
class DBReader{
public:
    DBReader(QString dbhst, QString dbnme, QString dbusr, QString dbps, QString id = "def");
    virtual ~DBReader();

    QMenu* ReadMenu(QWidget *parent);
    BoxObject* ReadObject(QString area="", QString action="", QString sid=""); //параметры из меню
    BoxReport* ReadReport(BoxObject *obj, BoxCalendar *cal);
//  --
    QString getMsg(void){return msg;};
//    bool isError(void){ return Err; };

private:
    bool Err;
    QString msg;
//  --
    QString dbhost;
    QString dbname;
    QString dbuser;
    QString dbpass;
    QString identif;	// определяет выбор оборудования и параметров
//  --
    bool connection(void);
    Trend* readTrend(QSqlQuery *q, QString *tabl, PidP p, time_t nt=0, int usecs=0);
};
//----------------------------------------------------------------
#endif
