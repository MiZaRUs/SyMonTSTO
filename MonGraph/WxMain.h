/**************************************************************************
 *  MonGraph  WxMain.h                                                    *
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
#include <QtSql>
#include "monitor.h"

const int MAXMENU = 12;

// ======================================================================
class WxMain : public QWidget {

    Q_OBJECT

public:
    WxMain(QSqlQuery &qs, QWidget* pwgt = 0);

private:
    QSqlQuery *pSql;
    int pId,ibd;
    QMenu *pMenu;
    Monitor *pmon;

    QGridLayout *pLayout;    
    QTimer *timer;

    int startRefresh(int interval);
    QMenu* WxMenu(void);
    void msgError(QString snm, QString ser);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *pe){
	pMenu->exec(pe->globalPos());
    }

public slots:
    void slotMenuClicked(QAction *pAction);
    void slotActivHelp();
    void slotActivExit();
    void slotTimerRefresh();
};
#endif  //_WxMain_h_
//----------------------------------------------------------
