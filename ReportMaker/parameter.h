/**************************************************************************
 *  ReportMaker    parameter.h                                            *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _Parameter_H_
#define _Parameter_H_

//  ----------------------------------------------------------------
class Parameter{
public:
    int id;		// указатель параметра
//    int flag;		// флаг состояния параметра
//    int diap;		// диапозон нечувствительности
    float modif;	// модификатор параметра(множитель,приведение к настоящему значению))
    int group;		// номер группы
    QString name;	// имя параметра
    QColor color;	// цвет тренда
//    QString charact;	// характеристики
};// End Parametr
//  ----------------------------------------------------------------
#endif
