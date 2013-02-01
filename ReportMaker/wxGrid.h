/**************************************************************************
 *  ReportMaker    wxGrid.h                                               *
 *  Copyright (C)  2010-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _WxGrid_h_
#define _WxGrid_h_

#include <QtGui>
//  --
#include "trend.h"
#include "BoxObject.h"
//const int MAXGRID = 16;
//const int DATASCALE = 1000;
const int MAXTREND = 8;
// ===================================================================
class WxGrid: public QWidget{

    Q_OBJECT

public:
    WxGrid(QString gname, int t, int tn);
    void setDataScale(int d, float dn, float di);
    int addTrend(Trend *trn, Parameter *prm);	//возвращает свободное место

private:
    bool view;
    int yZero, xZero;	// нижний и правый край (условный ноль)
    float xP;//, yP;	// минимальная точка окна (условный пиксел)
//  --
    int ymm, xmm;	// количество делений в (x|y)sm. минимальная единица
    int ysm, xsm;	// делений на шкале "санти" (размер шкалы)
//  --
    Trend **trend;
    Parameter **param;
    int curtrend;
    int maxtrend;
//  --
    QString groupName;
    int timesecs;
    int datascal;
    int minTimeScale;
    int incTimeScale;
    float minDataScale;
    float incDataScale;
//  --
//    float xsms;		// смещение шкалы времени
//    float ysms;		// смещение шкалы данных
//    float thline;		// толщина линии
// ------------------------------------------------------
    void drawGrid(QPainter *painter);
    void drawGraph(QPainter *painter);
    void drawScale(QPainter *painter);
//    void mouseInfo(QMouseEvent* pe, int x);
// ------------------------------------------------------
protected:
    void paintEvent(QPaintEvent *event);
//  --
//    virtual void mousePressEvent  (QMouseEvent* pe);
//    virtual void mouseReleaseEvent(QMouseEvent* pe);
//    virtual void mouseMoveEvent   (QMouseEvent* pe);
};
// ===================================================================
#endif //_WxGrid_h_
