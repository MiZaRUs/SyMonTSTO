/**************************************************************************
 *  ReportMaker    WxMain.h                                               *
 *  Copyright (C)  2010-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _WxMain_h_
#define _WxMain_h_

#include <QtGui>
#include "menu.h"
#include "dbreader.h"
#include "BoxButton.h"
#include "BoxCalendar.h"
#include "BoxObject.h"
#include "BoxReport.h"
// ======================================================================
class WxMain : public QWidget {

    Q_OBJECT

public:
    WxMain(DBReader *db, QWidget *pwgt = 0);
    int Error(void){return Err;};

private:
    int Err;
    int Fl;
    QWidget *parent;
    DBReader *dbr;
//  --
    BoxButton *button;
    BoxCalendar *calend;
    BoxObject *boxob;
    BoxReport *report;

    Menu *mnu;
    QMenu *menu;
    QVBoxLayout *lout;
//  --
    void clrBox(void);
    void showBox(QString s);
//  --
protected:
    virtual void contextMenuEvent(QContextMenuEvent *pe){
        menu->exec(pe->globalPos());
    }

public slots:
    void slotMenuClicked(QAction *pAction);
    void slotOkButton();
    void slotActivHelp();
    void slotActivExit();

signals:
    void signalMainId(int a, int b, int c);
};
#endif  //_WxMain_h_
//----------------------------------------------------------
