#include <QtGui>
#include "BoxCalendar.h"
//  -------------------------------------------------------------------------
BoxCalendar::BoxCalendar(QWidget *parent) : QWidget(parent){
    setupUi(this);
//  --
    calendar->setMinimumDate(QDate(2011, 2, 4));
    calendar->setMaximumDate(QDate::currentDate());
//  --
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(QCalendarWidget::NoVerticalHeader));
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(Qt::Monday));
//  --
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::blue);
    calendar->setHeaderTextFormat(format);
    calendar->setGridVisible(true);
//  --

    dateEdit->setDisplayFormat("d MM yyyy");
    dateEdit->setDateRange(calendar->minimumDate(),calendar->maximumDate());
    dateEdit->setDate(QDate::currentDate());
//  --
    QStringList lstr1;
    QStringList lstr2;
    lstr1 <<"00"<<"01"<<"02"<<"03"<<"04"<<"05"<<"06"<<"07"<<"08"<<"09"<<"10"<<"11"<<"12"\
    <<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"<<"21"<<"22"<<"23";
    lstr2 <<"1"<<"2"<<"4"<<"8"<<"12"<<"24"<<"48"<<"168";
    boxTimeN->addItems(lstr1);
    boxTimeK->addItems(lstr2);
//  --
}
//  -------------------------------------------------------------------------
QString BoxCalendar::getStrDateTime(void){
    QString str = calendar->selectedDate().toString("dd/MM/yy");
    str += "  " + boxTimeN->currentText() + ":00 + " + boxTimeK->currentText();
return str;
}
//  -------------------------------------------------------------------------
QString BoxCalendar::getMySqlDateTime(void){
    QString str = calendar->selectedDate().toString("yyyy-MM-dd");
    str += "T" + boxTimeN->currentText() + ":00:00";
return str;
}
//  -------------------------------------------------------------------------
