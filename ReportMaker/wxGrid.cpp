/**************************************************************************
 *  ReportMaker    wxGrid.cpp                                             *
 *  Copyright (C)  2010-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include "wxGrid.h"
//#include <math.h>
// ----------------------------------------------------------------------
WxGrid::WxGrid(QString gname, int t, int tn) : QWidget(){
//qDebug() << "WxGrid::WxGrid" << gname << t << tn;
    view = false;
    groupName = gname;		//название группы
//  --
    trend = new Trend*[MAXTREND];	// указатели на массив точек
    param = new Parameter*[MAXTREND];	// указатели на параметры trenda
    maxtrend = 0;			// количество указателей на массивы
//  --
//  шкала времени
    timesecs = t;		// длинна тренда в секундах (от нуля)
    minTimeScale = tn;		// начальное значение шкалы
    incTimeScale = 1;		// приращевание значений на шкале
//  --
    int tm = timesecs / 3600;	// длинна тренда в часах
    xsm = tm;			// количество крупных делений на шкале времени
    xmm = 6;			// мелкие деления шкалы
//  --
    if(tm <= 1){		// один час и меньше
        xmm = 10;
        xsm = 6;
    }
    if((tm > 24)&&(tm <= 168)){	//от суток до недели
        xmm = 24;
        xsm = tm / 24;
    }
    if(tm > 168){		//более недели
        xmm = 4;
        xsm = tm / 24;
    }
//  --
//  шкала данных (по умолчанию)
    ymm = 10;			// количество делений в ysm
    ysm = 10;			// количество разделов на графике  1...15
    datascal = 0;		//диапазон данных
    minDataScale = 0;		// начальное (минимальное) значение шкалы
    incDataScale = 1;		// приращение значений шкалы
//  --
//    setMouseTracking(true);
}// End WxGrid
// ----------------------------------------------------------------------
void WxGrid::setDataScale(int d, float dn, float di){
//  шкала данных
    datascal = d;	//диапазон данных
    minDataScale = dn;	// начальное (минимальное) значение шкалы
    incDataScale = di;	// приращение значений шкалы
//  --
//  шкала данных при ноле - дискретная
    if(datascal == 0){	// график дискретного сигнала
//qDebug() << tr("график дискретного сигнала");
        ymm = 4;		// количество делений в ysm
        ysm = maxtrend;		// количество разделов на графике  1...15
    }else{		// график аналогового сигнала
//qDebug() << tr("график аналогового сигнала");
        ymm = 10;		// количество делений в ysm
        ysm = (int)datascal / di;	// делений окна (2...15) по 10 клеток(минимальных значений)
        if(ysm > 7) ymm = 5;		// количество делений в ysm
        if(ysm > 10) ymm = 2;		// количество делений в ysm
    }
    view = true;
}
// ----------------------------------------------------------------------
int WxGrid::addTrend(Trend *trn, Parameter *prm){
    if(MAXTREND <= maxtrend)return 0;
//  --
    trend[maxtrend] = new Trend;	// указатели на массив точек
    param[maxtrend] = new Parameter;	// указатели на параметры
//  --
    trend[maxtrend] = trn;		// указатели на массив точек
    param[maxtrend] = prm;
//  --
// масштаб тренда = 80*(DATASCALE/100) = 80%
    trend[maxtrend]->mas = param[maxtrend]->modif;
//  --
    maxtrend++;		// количество указателей на массивы & ФЛАГ
//  --
return MAXTREND - maxtrend;
}// End
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void WxGrid::drawGraph(QPainter *painter){
    if((maxtrend < 1)||(trend[curtrend]->size < 1)) return;
//    if((curtrend < 0)||(curtrend > MAXTREND)) return;
//  --
//    QPointF tp[(trend[curtrend]->size +1)];
    QPointF tp[((trend[curtrend]->size +1) * 2)];
    int irx = 0;
//  --
    if(datascal == 0){	// Discret
        painter->setPen(QPen(param[curtrend]->color, 2));
        float ft = yZero / maxtrend;
        float mas =  ft / 2;
        float fsm = (float)(ft * curtrend) + ((ft - mas) / 2);
        tp[0].rx() = (float)xP * trend[curtrend]->point[0].rx(); //time
        int data = ((trend[curtrend]->point[0].ry() < 1) ? 1 : 0 );
        tp[0].ry() = (float)(data * mas) + fsm; //data modif
        for(int i = 1; i < trend[curtrend]->size; i++){
            tp[1].rx() = (float)xP * trend[curtrend]->point[i].rx(); //time
            tp[1].ry() = tp[0].ry();
            painter->drawLine( tp[0], tp[1]);
            if(trend[curtrend]->point[i-1].ry() != trend[curtrend]->point[i].ry()){// переключение
                tp[0].rx() = tp[1].rx();
                data = ((trend[curtrend]->point[i].ry() < 1) ? 1 : 0 );
                tp[1].ry() = (float)(data * mas) + fsm; //data
                painter->drawLine( tp[0], tp[1]);
            }// if
        tp[0] = tp[1];
        }// for
        tp[1].rx() = (float)xZero; //time
        tp[1].ry() = tp[0].ry();
        painter->drawLine( tp[0], tp[1]);
//  --
    }else{	// Analog
        float mas = (float)yZero / (datascal * 100);
        float fsm = (float)mas * (minDataScale * incDataScale * 10);

        for(int i = 0; i < trend[curtrend]->size; i++, irx++){
            tp[irx].rx() = (float)xP * trend[curtrend]->point[i].rx(); //time
            float tmprx = 0, tmpry = 0;
            if(irx > 0){
                tmprx = tp[irx].rx() - tp[irx-1].rx();
                tmpry = abs(tp[irx].ry() - tp[irx-1].ry());
            }
            if( (tmprx > 4) && (tmpry > 7)){
                tp[irx].ry() = tp[irx-1].ry(); //data
                irx++;
                tp[irx].rx() = tp[irx-1].rx();
            }
            tp[irx].ry() = (float)yZero - ((trend[curtrend]->point[i].ry() * mas) - fsm); //data
//qDebug() << "Tp  " << (int)tp[i].rx() << "  " << (float)tp[i].ry() << "*" << mas;
        }
    painter->setPen(QPen(param[curtrend]->color, 0.7));
    painter->drawPolyline(tp, irx);
    }
}// End drawGraph
// ----------------------------------------------------------------------
void WxGrid::drawGrid(QPainter *painter){
//  шкала времени (xP = 1 sec) вся шкала = timesecs
    int j = timesecs / (xmm * xsm);
    float t = xP * j;
    for(int i = 0; i < j; i++){ // временная линейка
        if( i % xmm ){
            painter->setPen(QPen(Qt::black, 0.1));
        }else{
            painter->setPen(QPen(Qt::black, 0.2));
        }
        painter->drawLine( i*t, 0, i*t, yZero );
    }
    painter->setPen(QPen(Qt::black, 0.9));
    painter->drawLine( xZero, 0, xZero, yZero );
    painter->setPen(QPen(Qt::black, 0.5));
    painter->drawLine( 0, 0, 0, yZero );
//  --
//  шкала данных
    j = ysm * ymm;
    t = (float)yZero / j;
    for(int i = 0; i < j; i++){ // линейка данных
        if( i % ymm ){
            painter->setPen(QPen(Qt::black, 0.1));
        }else{
            if(datascal == 0){	// Discret
                painter->setPen(QPen(Qt::black, 0.5));
            }else{		// Analog
                painter->setPen(QPen(Qt::black, 0.2));
            }
        }
        painter->drawLine( 0, i*t, xZero, i*t );
    }
    painter->setPen(QPen(Qt::black, 0.5));
    painter->drawLine( 0, 0, xZero, 0 );
    painter->drawLine(0, yZero, xZero, yZero);
}// End drawGrid
// ----------------------------------------------------------------------
void WxGrid::drawScale(QPainter *painter){
    painter->setPen(QPen(Qt::darkBlue, 0.7));
    for(int i = 1; i < xsm; i++){
        float num = (i + minTimeScale) * incTimeScale;
        painter->drawText((xZero / xsm) * i + 2, 10, QString::number( num ));
    }
//  --
    if(datascal == 0){
        for(int i = 0; i < ysm; i++){
            int iter = ysm-i-1;
            painter->setPen(QPen(param[iter]->color, 0.7));
            QString n = param[iter]->name;
            painter->drawText(2, (yZero - (i * (yZero / ysm)) - 2), QString(tr("%1) %2.").arg(ysm-i).arg(n)));
        }
    }else{
        for(int i = 0, ii = yZero; i < ysm; i++){
            float num = (i + (minDataScale / 10)) * incDataScale;
            if( num > 0 ) painter->setPen(QPen(Qt::darkRed, 0.7));
            if( num == 0 ) painter->setPen(QPen(Qt::black, 0.7));
            if( num < 0 ) painter->setPen(QPen(Qt::darkBlue, 0.7));
            painter->drawText(2, ii - 2, QString::number( num ));
//            painter->drawText(xZero - 22, ii - 2, QString::number( num ));
            ii = ii - (yZero / ysm);
        }
        for(int i = 0; i < maxtrend; i++){
//  установить соответстующий цвет из тренда
            painter->setPen(QPen(param[i]->color, 0.7));
            painter->drawText(30, 30 + (16 * i), QString(tr("%1.").arg(param[i]->name)));
        }
    }
}// End 
// ----------------------------------------------------------------------
void WxGrid::paintEvent(QPaintEvent * /*event*/){
    QPainter painter( this );
    painter.setRenderHint(QPainter::Antialiasing, true);
//  --
    yZero = height();
    xZero = width();
//    yP = (float)yZero / datascal; // деление шкалы (условный пиксель)
    xP = (float)xZero / timesecs; // цена деление шкалы / секунда
//  --
    if(!view)return;
    drawGrid(&painter);
//  --
    if(maxtrend == 0)return;
//  --
    for(curtrend = 0; curtrend < maxtrend; curtrend++){
        drawGraph(&painter);
    }
    drawScale(&painter);
}// End paintEvent
// ----------------------------------------------------------------------
