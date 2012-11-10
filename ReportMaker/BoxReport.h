/**************************************************************************
 *  ReportMaker (SCADA)                                                   *
 *  Copyright (C)  2012   by  Oleg Shirokov       olgshir@gmail.com       *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _BoxReport_H_
#define _BoxReport_H_

#include <QWidget>
#include "trend.h"
#include "parameter.h"
#include "wxGrid.h"
//  ----------------------------------------------------------------
class BoxReport : public QWidget{
//    Q_OBJECT
public:
    BoxReport(QString nam, QWidget *parent = 0);
    bool initBox(void);
//  --
    QString name;	// название и дата отчёта
//  шкала времени
    int lentime;	// длинна тренда в секундах (от нуля)
    int minTime;	// минимальное значение на шкале времени
    int incTime;	// приращение значений шкалы времени
//  --
    int size;
    Trend **trend;
    Parametr **param;
//  --
private:
    QVBoxLayout *vl;
//  --
    void initWxGrid(WxGrid **grid, int i, int j, int g);
};
//  ----------------------------------------------------------------
#endif
