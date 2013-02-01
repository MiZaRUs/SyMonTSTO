/**************************************************************************
 *  ReportMaker    BoxButton.cpp                                          *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <QtGui>
#include "BoxButton.h"
//  -------------------------------------------------------------------------
BoxButton::BoxButton(QWidget *parent) : QWidget(parent){
    setupUi(this);
    OkButton->setEnabled(false);
}
//  -------------------------------------------------------------------------
