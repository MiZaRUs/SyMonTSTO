/**************************************************************************
 *  ReportMaker                                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <QtGui>
#include "BoxReport.h"
//#include "defs.h"
//  -------------------------------------------------------------------------
BoxReport::BoxReport(QString nam,QWidget *parent) : QWidget(parent){
    name = nam;
    minTime = 0;
    incTime = 1;
    setWindowTitle(name);
    resize(600,50);
    vl = new QVBoxLayout;
    vl->setMargin(8);
    vl->setSpacing(15);
}
//  -------------------------------------------------------------------------
bool BoxReport::initBox(void){
//qDebug() << tr("Start BoxReport.") << size;

//qDebug() << "Trend 1: len -" << trend[0]->size;
//for(int i = 0; i < trend[0]->size; i++){
//qDebug() << i << ")" << (int)trend[0]->point[i].rx() << (int)trend[0]->point[i].ry();
//}

// определение групп
    int grp = 1;	// минимум одна
    int tmp = param[0]->group;
    for(int j = 0; j < size; j++){
//qDebug() << "G" << grp << "" << tmp;
        if(tmp < param[j]->group){
            tmp = param[j]->group;
            grp++;
        }
    }
//qDebug() << "Grp" << grp;
    WxGrid *grid[grp];
//  --
// разделение на группы
    int i = 0, j = 0, g = 0; 
    tmp = param[0]->group;
    while(j < size){
        if(tmp < param[j]->group){
            tmp = param[j]->group;
            initWxGrid(grid, i, j, g);
            i = j;
            g++;
        }
        j++;
    }
    initWxGrid(grid, i, j, g);
    setLayout(vl);
//qDebug() << "End BoxReport.";
return true;
}
//  -------------------------------------------------------------------------
void BoxReport::initWxGrid(WxGrid **graf, int st, int co, int gr){
//qDebug() << "new wxGrid"<< gr << "-" << st << co;
//  --
    graf[gr] = new WxGrid("Test", lentime, minTime);
//  --
    int min = trend[st]->min;
    int max = trend[st]->max;
    for(int i = st; i < co; i++){
        if(min > trend[i]->min) min = trend[i]->min;
        if(max < trend[i]->max) max = trend[i]->max;
//qDebug() << "AddTrend" <<  
        graf[gr]->addTrend(trend[i], param[i]);
    }
//qDebug() << gr <<"Min/Max" << min << max << "=" << max-min;

    int datascale = max - min;
//    if(datascale < -100000) return;
    int mindata = 0;
    float incdata = 1;
// --
    if(param[st]->group == 1){	//давление воздуха 0 : 1000
        datascale = 700;
        incdata = 100;
    }
    if(param[st]->group == 2){	//зазоры 0 : 2,5
        datascale = 3;
        incdata = 1;
    }
    if(param[st]->group == 3){	//давление масла в редукторе 0 : 16
        datascale = 16;
        incdata = 1;
    }
//    if(param[st]->group == 4){	//
//        datascale = 10;
//        incdata = 1;
//        mindata = 0;
//    }
    if(param[st]->group == 5){	//температура окружающего воздуха
        datascale = 90;
        incdata = 10;
        mindata = -40;
    }
    if(param[st]->group == 6){	//температура 10 : 90
        datascale = 80;
        incdata = 10;
        mindata = 10;
    }
    if(param[st]->group >= 10){	//дискретный
        datascale = 0;
//qDebug() << "Tr"  << co - st;
    }
//  --
    graf[gr]->setDataScale(datascale, mindata, incdata);
    vl->addWidget(graf[gr]);
}
//  -------------------------------------------------------------------------
