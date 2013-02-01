/**************************************************************************
 *  ReportMaker                                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _BoxButton_H_
#define _BoxButton_H_

#include <QWidget>
#include "ui_button.h"
//  -----------------------------
class BoxButton : public QWidget, public Ui::wxButton{

public:
    BoxButton(QWidget *parent = 0);
};
//  -----------------------------
#endif
