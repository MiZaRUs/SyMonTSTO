/**************************************************************************
 *  ReportMaker    dbreader.cpp                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include "dbreader.h"
#include "defs.h"
//  --
QSqlError sqlErr;
// ----------------------------------------------------------------------
static bool createConnection(QString dbName, QString hstName, QString usrName, QString passw){
//qDebug() << "createConnection " << hstName << ":" << dbName;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(dbName);
    db.setHostName(hstName);
    db.setPort(-1);	// !!!!!!
    db.setUserName(usrName); 
    db.setPassword(passw); 
    if (!db.open()){
        sqlErr = db.lastError();
        return false;
    }
return true;
}// End createConnection
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
DBReader::DBReader(QString hst, QString nam, QString usr, QString pas, QString id){
//qDebug() << "DBReader::DBReader()";
    msg  = "DBReader. ";
    Err = false;
//    query = NULL;
//  --
    if(QSqlDatabase::drivers().isEmpty()){
        msg += "QSqlDatabase::drivers()!";
        Err = true;
    }
//  --
    dbhost = hst;
    dbname = nam;
    dbuser = usr;
    dbpass = pas;
    identif = id;
}// End DBReader::DBReader
//  -------------------------------------------------------------------------
BoxReport* DBReader::ReadReport(BoxObject *obj, BoxCalendar *cal){
//qDebug() << "DBReader::ReadReport()-" << "Obj:" << obj->name << "Cal:" << cal->getMySqlDateTime() << cal->getUSecs();
    if(Err)throw QString(msg);
    msg  = "DBReader::ReadReport() - ";
//  --
//    static ?
    BoxReport *rez = new BoxReport(obj->name+"  "+cal->getStrDateTime()+QWidget::tr(" ч."));
    rez->hide();
//  --
    if(!createConnection(dbname, dbhost, dbuser, dbpass)){
        msg += sqlErr.text();
        Err = true;
        throw QString(msg);
    }
    QSqlQuery *query = new QSqlQuery();
    QString sql;
//  --
//получаем список архивов
    QString sarch[3];
    sarch[0] = "archiv";
    sarch[1] = "";
    sarch[2] = "";
// Расчитываем unix-время
    time_t nt = 0;
    int usecs = cal->getUSecs();
    sql = (QString("SELECT UNIX_TIMESTAMP('%1')").arg(cal->getMySqlDateTime()));
    if(query->exec(sql)){
        if(query->next()) nt = query->value(0).toInt();
    }
    if(nt < 1){
        msg += "QSqlQuery(" + sql + ");";
        throw QString(msg);
    }
// Получаем точки с 'nt' по ('nt'+'usecs').
//qDebug() << "NT & US" << nt << usecs;
//  --
    int mtrnd = obj->maxParam();
//qDebug() << "MaxP" << mtrnd;
    rez->trend = new Trend*[mtrnd];
    rez->param = new Parameter*[mtrnd];
    int i = 0;
    while(i < mtrnd){
        rez->param[i] = obj->getParam();
        rez->trend[i] = readTrend(query, sarch, rez->param[i]->id, nt, usecs);
//qDebug() << "Trnd" << i << "-" << rez->param[i]->name << rez->param[i]->comment << " -" << rez->trend[i]->size;
        i++;
    }
    rez->size = i;
    rez->lentime = usecs;
//  --
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    delete query;
    query = NULL;
return rez;
}// End
//  -------------------------------------------------------------------------
Trend* DBReader::readTrend(QSqlQuery *query, QString *tabl, int p, time_t nt, int usecs){
    time_t kt = nt + usecs;
    Trend *trn = new Trend;
    trn->point = NULL;
    trn->size = 0;
    trn->min = 100000;
    trn->max = -100000;
    trn->mas = 1;
//  --
    QString sql;
    int is = 0, w = 1;
    for(; tabl[is] != ""; is++){
        if(is > MAXTBLARC)break;
// Получаем точки от 'nt' до 'kt'.
        sql = (QString("SELECT utime, data FROM %1 WHERE \
id = %2 AND utime >= %3 AND utime <= %4 ORDER BY utime")
.arg(tabl[is]).arg(p).arg(nt).arg(kt));
//  --
//qDebug() << "Sql1 - " << sql;
        if(query->exec(sql)){
            trn->size = query->size() + 2; // начало и завершение
            trn->point = new QPointF[trn->size];
            while(query->next()){
                int utm = (query->value(0).toInt()) - nt;
                int dan = query->value(1).toInt();
                if((trn->min > dan)&&(dan > -99999))trn->min = dan;
                if((trn->max < dan)&&(dan > -99999))trn->max = dan;
                trn->point[w].rx() = utm;
                trn->point[w].ry() = dan;
//qDebug() << "Wh a "<< w <<": " << (int)utm << " : " << (int)dan;
                w++;
                if(w >= trn->size) break;
            }// while
        }else{
            msg += "QSqlQuery("+ sql +");";
            trn->size = 0;	// ERROR
            throw QString(msg);
        }
//qDebug() << "Wx min/max" << trn->min << trn->max;
//  --
//qDebug() << "Tbl" << tabl[is];

// Получаем начальную(до 'nt') точку.
        sql = (QString("SELECT data FROM %1 WHERE id = %2 AND utime = \
(SELECT MAX(utime) FROM %1 WHERE id = %2 AND utime < %3)")
.arg(tabl[is]).arg(p).arg(nt));
//  --
//qDebug() << "Sql2 - " << sql;
//  --
        trn->point[0].rx() = 0;
        if(query->exec(sql)){
            while(query->next()){
                int dan = query->value(0).toInt();
                if((trn->min > dan)&&(dan > -99999))trn->min = dan;
                if((trn->max < dan)&&(dan > -99999))trn->max = dan;
                trn->point[0].ry() = dan;
                w++;
                break;
            }
        }else{
            msg += "1QSqlQuery("+ sql +");";
            throw QString(msg);
        }
//qDebug() << "First - " << (int)trn->point[0].ry();
//  --
// Получаем заключительную(после 'kt') точку.
        sql = (QString("SELECT data FROM %1 WHERE id = %2 AND utime = \
(SELECT MIN(utime) FROM %1 WHERE id = %2 AND utime > %3)")
.arg(tabl[is]).arg(p).arg(kt));
//  --
//qDebug() << "Sql3 - " << sql;
//  --
        trn->point[w-1].rx() = usecs;
        if(query->exec(sql)){
            while(query->next()){
                int dan = query->value(0).toInt();
                if((trn->min > dan)&&(dan > -99999))trn->min = dan;
                if((trn->max < dan)&&(dan > -99999))trn->max = dan;
                trn->point[w-1].ry() = dan;
                w++;
                break;
                }
        }else{
            msg += "2QSqlQuery("+ sql +");";
            throw QString(msg);
        }
//qDebug() << "End - " << (int)trn->point[w-2].ry();
//qDebug() << "Stop" << w;

//  проверяем наличие данных
        if( w == 1 ){
            delete trn->point;
        }else{
            break;
        }
    }// for tabl[is]
//  --
//qDebug() << "Stop" << w;
    trn->size = w-1;
//  --
    return trn;
}// End
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
/*
В данной версии используется статичное меню (menu.h menu.cpp)

QMenu* DBReader::ReadMenu(QWidget *parent){
//qDebug() << "DBReader::ReadMenu();";
    if(Err)throw QString(msg);
    msg  = "DBReader::ReadMenu() - ";
    QMenu *pmnu = NULL;
    pmnu = new QMenu(parent);
//  --
// SQL-запрос по idetnif
//  --
    if(!createConnection(dbname, dbhost,dbuser, dbpass)){
        msg += sqlErr.text();
        Err = true;
        throw QString(msg);
    }
    QSqlQuery *query = new QSqlQuery();
    QString sql;
//  --
    int w = 0;
    QMenu **psub;
    sql = QString("SELECT DISTINCT area FROM %1 WHERE id > 0 AND ident = '%2' ORDER BY id").arg(TBLOBJ).arg(identif);
//qDebug() << "sql1: " << sql;
    if(query->exec(sql)){
        psub = new QMenu* [query->size()];
        while(query->next()){
            QString area = query->value(0).toString();
            psub[w] = new QMenu(area, pmnu);
            pmnu->addMenu( psub[w]);
            w++;
//qDebug() << "- " << area << " iz " << w;
        }// while
        msg += "Ok!";
    }else{
        msg += "QSqlQuery(" + sql + ");";
        throw QString(msg);
    }
//  --
//	object(id, ident, area, name, comment)
    sql = QString("SELECT area, name, id FROM %1 WHERE id > 0 AND ident = '%2' ORDER BY id").arg(TBLOBJ).arg(identif);
qDebug() << "sql2: " << sql;
    if(query->exec(sql)){
        while(query->next()){
            QString ar1 = query->value(0).toString();
            QString ar2 = query->value(1).toString();
            QString ar3 = query->value(2).toString();
qDebug() << ar1 << " - " << ar2 << " - " << ar3;
            for(int i = 0; i < w; i++){
                if(psub[i]->title() == ar1)psub[i]->addAction(ar2)->setObjectName(ar1+":"+ar3);
            }//for
        }// while
        msg += "Ok!";
    }else{
        msg += "QSqlQuery(" + sql + ");";
        throw QString(msg);
    }
//  --
//  --
    pmnu->addSeparator();
    pmnu->addAction(QWidget::tr("Параметры"));
    pmnu->addAction(QWidget::tr("Календарь"));
    pmnu->addSeparator();
//    pmnu->addAction(QWidget::tr("Справка"));
//    pmnu->addSeparator();
    pmnu->addAction(QWidget::tr("Выход"));
//  --
//qDebug() << "End.";
//  --
    msg += " Ok.";

    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    delete query;
    query = NULL;

return pmnu;
}//End ReadMenu
//  -------------------------------------------------------------------------
BoxObject* DBReader::ReadObject(QString area, QString action, QString sid){
//qDebug() << "DBReader::ReadObject("<< area <<","<< action <<","<< sid << ");";
    if(Err)throw QString(msg);
    msg = "DBReader::ReadObj("+ area + "," + sid + ");";
//  --
    BoxObject *obj;
    if(!createConnection(dbname, dbhost,dbuser, dbpass)){
        msg += sqlErr.text();
        Err = true;
        throw QString(msg);
    }
    QSqlQuery *query = new QSqlQuery();
    QString sql;
//  --
//data_ai(id,reg,...,diap,modif,param,object,flag)  param(name,comment)
    int gid = sid.toInt() / 10;
    gid = gid * 10;
qDebug() << "gid: " << gid;
    sql = QString("SELECT %1.id, %1.flag, %1.diap, %1.modif, %1.param, %2.name, %2.comment \
FROM %1, %2, %3 WHERE %1.param = %2.id AND (%1.object = %3.id OR %1.object = 1 OR %1.object = %5) \
AND %1.flag > 15 AND %3.id = %4 ORDER BY %2.id").arg(TBLDAN).arg(TBLPRM).arg(TBLOBJ).arg(sid).arg(gid);
//  --
qDebug() << "sql1: " << sql;
    int w = 0;
    if(query->exec(sql)){
        obj = new BoxObject(query->size());
        obj->hide();
        obj->name = area + " " + action;
qDebug() << "- " << area << action;
        while(query->next()){
            obj->param[w].id = query->value(0).toInt();
            obj->param[w].flag = query->value(1).toInt();
            obj->param[w].diap = query->value(2).toInt();
            obj->param[w].modif = query->value(3).toFloat();
            obj->param[w].group = query->value(4).toInt();// param
            obj->param[w].name = query->value(5).toString();
            obj->param[w].comment = query->value(6).toString();
            obj->param[w].color = Qt::black;
            obj->param[w].charact = "";		// хар-ки тренда
            bool fl1 = false;
            bool fl2 = false;
            if((obj->param[w].flag & 0x10) > 0)fl1 = true;// выбор (16)
            if((obj->param[w].flag & 0x20) > 0)fl2 = true;// вкл   (32)
            obj->check[w] = new QCheckBox();
            obj->check[w]->setEnabled(fl1);
            obj->check[w]->setChecked(fl2);
            obj->check[w]->setText(obj->param[w].name + " " + obj->param[w].comment);
qDebug() << "- " << obj->param[w].modif;
            w++;
        }// while
    }else{
        msg += "QSqlQuery(" + sql + ");";
        throw QString(msg);
    }
//  --
    msg += " Ok.";
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    delete query;
    query = NULL;
//  --
return obj;
}//End ReadObj
*/
//  -------------------------------------------------------------------------
DBReader::~DBReader(){
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}// End
//  -------------------------------------------------------------------------
