#include <QtGui>
#include "BoxButton.h"
//  -------------------------------------------------------------------------
BoxButton::BoxButton(QWidget *parent) : QWidget(parent){
    setupUi(this);
    OkButton->setEnabled(false);
}
//  -------------------------------------------------------------------------
