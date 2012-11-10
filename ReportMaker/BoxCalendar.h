/**************************************************************************
 *  ReportMaker (SCADA)                                                   *
 *  Copyright (C)  2012   by  Oleg Shirokov       olgshir@gmail.com       *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _BoxCalendar_H_
#define _BoxCalendar_H_

#include <QWidget>
#include "ui_calendar.h"
//  -----------------------------
class BoxCalendar : public QWidget, public Ui::wxCalendar{
//    Q_OBJECT
public:
    BoxCalendar(QWidget *parent = 0);
    QString getStrDateTime(void);
    QString getMySqlDateTime(void);
//    QString getTimes(void){return boxTimeK->currentText();}; // продолжительность периода в часах str
    int getUSecs(void){return boxTimeK->currentText().toInt() * 3600;}; // продолжительность периода в сек int
private:
};
//  -----------------------------
#endif
