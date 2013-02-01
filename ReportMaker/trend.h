/**************************************************************************
 *  ReportMaker    trend.h                                                *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _Trend_H_
#define _Trend_H_
//  ----------------------------------------------------------------
class Trend{
public:
    QPointF *point;		// массив точек (.rx - time; .ry - view)
    int	size;			// размер массива
    float min, max, mas;	// пределы тренда и масштаб (для отображения)
};// End Trend
//  ----------------------------------------------------------------
#endif
