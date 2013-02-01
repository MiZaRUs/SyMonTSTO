/**************************************************************************
 *  ReportMaker    BoxObject.cpp                                          *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <QtGui>
#include "BoxObject.h"
//#include "defs.h"
//  -------------------------------------------------------------------------
BoxObject::BoxObject(int i, QWidget *parent) : QWidget(parent){
    size = i;
    name = "?!";
    cpar = 0;
//  --
    param = new Parameter[i];
    if(!param)throw QString("BoxObject: new Parametr[]");
    check = new QCheckBox* [i];
    if(!check)throw QString("BoxObject: new QCheckBox*[]");
}
//  -------------------------------------------------------------------------
bool BoxObject::initBox(void){
    if(size < 1)return false;
    vl = new QVBoxLayout;
    vl->setMargin(8);
    vl->setSpacing(6);
//  --
//  разделение групп
    int tmp = param[0].group;
    for( int j = 0, t = 1; j < size; j++, t++){
        if(tmp < param[j].group){
            tmp = param[j].group;
//            maxgroup++;
            vl->addSpacing(30);
            t = 1;
        }// if
//qDebug() << "up" << j << param[j].group << " -" << t;
        param[j].color = getColor(t-1);
        vl->addWidget(check[j]);
    }// for
    setLayout(vl);
//  -- 
// check[]->setCheckable(false); check[]->setChecked(false); check[]->setEnabled(true);
return true;
}
//  -------------------------------------------------------------------------
int BoxObject::maxParam(void){//  подсчёт всех выбранных параметров
    cpar = 0;
    int rez = 0;
    for( int j = 0; j < size; j++){
        if(check[j]->isChecked()) rez++;
    }
return rez;
}
//  -------------------------------------------------------------------------
//Parametr* BoxObject::firstParam(void){}
//  -------------------------------------------------------------------------
Parameter* BoxObject::getParam(void){
    if(cpar >= size) return NULL;
    Parameter *par = NULL;
    while(cpar < size){
        if(check[cpar]->isChecked()){
            par = &param[cpar];
            cpar++;
            break;
        }
    cpar++;
    }
return par;
}
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
QColor BoxObject::getColor(int i){
    QColor c1(200, 0, 0); // krasn
    QColor c2(0, 0, 200); // siniy
    QColor c3(0, 122, 0); // zelen
    QColor c4(255, 0, 255); //seren
    QColor c5(100, 100, 0); //korich
    QColor c6(0, 165, 155); //golub
    QColor c7(170, 170, 0); //jelt
//Qt::red,magenta,blue,cyan,green,yellow;darkMagenta,darkCyan,darkGreen,darkYellow
    if(i == 0)return c1;
    if(i == 1)return c2;
    if(i == 2)return c3;
    if(i == 3)return c4;
    if(i == 4)return c5;
    if(i == 5)return c6;
    if(i == 6)return c7;
    if(i == 7)return c1;
return Qt::black;
}// End getColor
//  -------------------------------------------------------------------------
