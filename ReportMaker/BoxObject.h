/**************************************************************************
 *  ReportMaker (SCADA)                                                   *
 *  Copyright (C)  2012   by  Oleg Shirokov       olgshir@gmail.com       *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _BoxObject_H_
#define _BoxObject_H_

#include <QWidget>
#include "parameter.h"
//  ----------------------------------------------------------------
class BoxObject : public QWidget{	// контролируемый объект
//    Q_OBJECT
public:
    BoxObject(int i, QWidget *parent = 0);
    bool initBox(void);
    Parametr* getParam(void);	//поочередный запрос параметров объекта
    int maxParam(void);		// количество выбранных параметров
//    Parametr* firstParam(void);
//  --
    QString name;	// название объекта
    Parametr *param;	// массив параметров объекта (id,reg,flag)
    QCheckBox **check;	// выбор параметров объекта
//  --
private:
//  --
    QVBoxLayout *vl;
//  --
    int cpar;		// текущий параметр для getParam();
    int size;		// количество всех параметров
//  --
    QColor getColor(int i);
};
//  ----------------------------------------------------------------
#endif
