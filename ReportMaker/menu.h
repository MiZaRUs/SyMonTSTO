/**************************************************************************
 *  ReportMaker                                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _Menu_h_
#define _Menu_h_
//  --
#include <QtGui>
#include <QString>
#include "BoxObject.h"
//  --
//----------------------------------------------------------------
//using namespace std;
//----------------------------------------------------------------
class Menu{
public:
    Menu(QWidget *parent);
    QMenu* ReadMenu(void){ return pmnu; };

    BoxObject* ReadObject(QString area="", QString action="", QString sid=""); //параметры из меню
//  --
private:
    QMenu *pmnu;
};
//----------------------------------------------------------------
#endif
