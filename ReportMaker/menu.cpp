/**************************************************************************
 *  ReportMaker    menu.cpp                                               *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include "menu.h"
//  -------------------------------------------------------------------------
Menu::Menu(QWidget *parent){
//qDebug() << "Menu::Menu()";
    pmnu = NULL;
    pmnu = new QMenu(parent);
//  --
    QMenu *vds1 = new QMenu(QWidget::tr("ВДС-1"), pmnu);
    pmnu->addMenu( vds1 );
    vds1->addAction(QWidget::tr("П/С 1 т/п"))->setObjectName(QWidget::tr("ВДС-1:1"));
    vds1->addAction(QWidget::tr("П/С 2 т/п"))->setObjectName(QWidget::tr("ВДС-1:2"));
    vds1->addAction(QWidget::tr("П/С 3 т/п"))->setObjectName(QWidget::tr("ВДС-1:3"));
    vds1->addAction(QWidget::tr("П/С 4 т/п"))->setObjectName(QWidget::tr("ВДС-1:4"));
    vds1->addAction(QWidget::tr("П/С 5 т/п"))->setObjectName(QWidget::tr("ВДС-1:5"));
//  --
    QMenu *vds5 = new QMenu(QWidget::tr("ВДС-5"), pmnu);
    pmnu->addMenu( vds5 );
    vds5->addAction(QWidget::tr("П/С 16"))->setObjectName(QWidget::tr("ВДС-5:6"));
    vds5->addAction(QWidget::tr("П/С 21"))->setObjectName(QWidget::tr("ВДС-5:7"));
    vds5->addAction(QWidget::tr("П/С 22"))->setObjectName(QWidget::tr("ВДС-5:8"));
    vds5->addAction(QWidget::tr("П/С 23"))->setObjectName(QWidget::tr("ВДС-5:9"));
//  --

//  --
    pmnu->addSeparator();
    pmnu->addAction(QWidget::tr("Параметры"));
    pmnu->addAction(QWidget::tr("Календарь"));
    pmnu->addSeparator();
//    pmnu->addAction(QWidget::tr("Справка"));
//    pmnu->addSeparator();
    pmnu->addAction(QWidget::tr("Выход"));
}// End
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
BoxObject* Menu::ReadObject(QString area, QString action, QString sid){
//qDebug() << "Menu::ReadObject("<< area <<","<< action <<","<< sid << ");";
    BoxObject *obj;
//  --
    int gid = sid.toInt();
//qDebug() << "gid: " << gid;
//  --
    obj = new BoxObject(6);
    obj->hide();
    obj->name = area + " " + action;
//  -- группы обязательно по возрастанию !!!
    obj->param[0].id = 15001;
    obj->param[0].modif = 1.0;
    obj->param[0].group = 1;// группа шкалы см. BoxReport::initWxGrid
    obj->param[0].name = QWidget::tr("Давление воздуха.");
    obj->param[0].color = Qt::black;
    obj->check[0] = new QCheckBox();
    obj->check[0]->setEnabled(true);
    obj->check[0]->setChecked(true);
    obj->check[0]->setText(obj->param[0].name);
//  --
    obj->param[1].id = 8004;
    obj->param[1].modif = 1.0;
    obj->param[1].group = 5;// группа шкалы см. BoxReport::initWxGrid
    obj->param[1].name = QWidget::tr("Температура наружного воздуха.");
    obj->param[1].color = Qt::black;
    obj->check[1] = new QCheckBox();
    obj->check[1]->setEnabled(true);
    obj->check[1]->setChecked(true);
    obj->check[1]->setText(obj->param[1].name);
//  --
    obj->param[2].id = 7001;
    obj->param[2].modif = 1.0;
    obj->param[2].group = 6;// группа шкалы см. BoxReport::initWxGrid
    obj->param[2].name = QWidget::tr("Температура подшип. узла А.");
    obj->param[2].color = Qt::black;
    obj->check[2] = new QCheckBox();
    obj->check[2]->setEnabled(true);
    obj->check[2]->setChecked(true);
    obj->check[2]->setText(obj->param[2].name);
//  --
    obj->param[3].id = 7002;
    obj->param[3].modif = 1.0;
    obj->param[3].group = 6;// группа шкалы см. BoxReport::initWxGrid
    obj->param[3].name = QWidget::tr("Температура подшип. узла Б.");
    obj->param[3].color = Qt::black;
    obj->check[3] = new QCheckBox();
    obj->check[3]->setEnabled(true);
    obj->check[3]->setChecked(true);
    obj->check[3]->setText(obj->param[3].name);
//  --
    obj->param[4].id = 13001;
    obj->param[4].modif = 1.0;
    obj->param[4].group = 10;// группа шкалы см. BoxReport::initWxGrid
    obj->param[4].name = QWidget::tr("Включение питателя.");
    obj->param[4].color = Qt::black;
    obj->check[4] = new QCheckBox();
    obj->check[4]->setEnabled(true);
    obj->check[4]->setChecked(true);
    obj->check[4]->setText(obj->param[4].name);
//  --
    obj->param[5].id = 13002;
    obj->param[5].modif = 1.0;
    obj->param[5].group = 10;// группа шкалы см. BoxReport::initWxGrid
    obj->param[5].name = QWidget::tr("Включение вдм.");
    obj->param[5].color = Qt::black;
    obj->check[5] = new QCheckBox();
    obj->check[5]->setEnabled(true);
    obj->check[5]->setChecked(false);
    obj->check[5]->setText(obj->param[5].name);
//  --
    if( gid == 7 ){
        obj->param[0].id = 15002;
        obj->param[1].id = 8004;
        obj->param[2].id = 8001;
        obj->param[3].id = 8002;
        obj->check[2]->setEnabled(false);
        obj->check[2]->setChecked(false);
        obj->param[4].id = 13005;
        obj->param[5].id = 13006;
    }
    if( gid == 8 ){
        obj->param[0].id = 15003;
        obj->param[1].id = 8004;
        obj->param[2].id = 9001;
        obj->param[3].id = 9002;
        obj->param[4].id = 13009;
        obj->param[5].id = 13010;
    }
    if( gid == 9 ){
        obj->param[0].id = 15004;
        obj->param[1].id = 8004;
        obj->param[2].id = 10001;
        obj->param[3].id = 10002;
        obj->param[4].id = 13013;
        obj->param[5].id = 13014;
    }

    if( gid < 6 ){
        obj->check[0]->setChecked(false);
        obj->check[0]->setEnabled(false);
        obj->param[1].id = 8004;
        obj->param[2].id = 2001;
        obj->param[3].id = 2002;
        obj->check[4]->setChecked(false);
        obj->check[4]->setEnabled(false);
        obj->check[5]->setChecked(false);
        obj->check[5]->setEnabled(false);
    }
//  --
    if( gid == 1 ){
        obj->param[1].id = 8004;
        obj->param[2].id = 2001;
        obj->param[3].id = 2002;
    }
//  --
    if( gid == 2 ){
        obj->param[1].id = 8004;
        obj->param[2].id = 3001;
        obj->param[3].id = 3002;
    }
//  --
    if( gid == 3 ){
        obj->param[1].id = 8004;
        obj->param[2].id = 4001;
        obj->param[3].id = 4002;
    }
//  --
    if( gid == 4 ){
        obj->param[1].id = 8004;
        obj->param[2].id = 5001;
        obj->param[3].id = 5002;
    }
//  --
    if( gid == 5 ){
        obj->param[1].id = 8004;
        obj->param[2].id = 6001;
        obj->param[3].id = 6002;
    }
//  --
return obj;
}//End ReadObj
//  -------------------------------------------------------------------------
