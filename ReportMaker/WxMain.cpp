/**************************************************************************
 *  ReportMaker                                                           *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include "WxMain.h"
//#include "defs.h"
// ----------------------------------------------------------------------
WxMain::WxMain(DBReader *db, QWidget *pwgt) : QWidget(pwgt){
    dbr = db;
    parent = pwgt;
    Err = 0;
    Fl = 0;
    report = NULL;
//    countreport = 0;
    boxob = NULL;
    resize(220,310);
    lout = new QVBoxLayout;
    lout->setMargin(8);
    lout->setSpacing(15);
//  --
    button = new BoxButton();
    connect(button->OkButton, SIGNAL(clicked()), SLOT(slotOkButton()));
//  --
    mnu = new Menu(this);
    menu = mnu->ReadMenu();
    connect(menu, SIGNAL(triggered(QAction*)),SLOT(slotMenuClicked(QAction*)));
    calend = new BoxCalendar();
//  --
    lout->addWidget(calend);
    lout->addWidget(button);
    setLayout(lout);
    calend->show();
}//End WxMain
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void WxMain::slotMenuClicked(QAction* pAction){
    QString stact = pAction->text().remove("&");
    QString stid = pAction->objectName();
//qDebug() << tr("Делай ЭТО! >>") << stact;
    if(stid != ""){
        QStringList sttit = stid.split(":");
        QString area = sttit[0];
        QString id = sttit[1];
        setWindowTitle(area +" : " + stact);
//qDebug() << tr("Вот так! >> ") << area << " : " << id;
        calend->hide();
        if(boxob != NULL){
            lout->removeWidget(boxob);
            delete boxob;
            boxob = NULL;
        }
        button->OkButton->setEnabled(true);	// !!!!!!!
        boxob = mnu->ReadObject(area, stact, id);
        boxob->initBox();
        boxob->show();
        lout->insertWidget(0,boxob);
        setLayout(lout);
    }else{
        if(stact == tr("Параметры")){
            if(boxob == NULL)return;
            calend->hide();
            boxob->show();
//            resize( calend->size() );
        }
        if(stact == tr("Календарь")){
            if(boxob != NULL)boxob->hide();
            calend->show();
            resize( calend->size() );
        }
        if(stact == tr("Справка"))slotActivHelp();
        if(stact == tr("Выход"))slotActivExit();
    }
return;
}// End slot
//-----------------------------------------------------------------------
void WxMain::slotOkButton(void){
    if(boxob == NULL)return;
//  --
    int mp = boxob->maxParam();
    if((mp < 1)||(mp > 32)){	//слишком мало/много параметров !!!
//        button->OkButton->setEnabled(false);
//        QMessageBox::warning(0, tr("Ошибка!"),tr("Не верное количество параметров."));
        return;
    }
//  --
    if(report != NULL){
        report->close();
        delete report;
        report = NULL;
    }
//  --
// предусматреть возможность отображения нескольких отчетов
//  --
    report = dbr->ReadReport(boxob, calend);
    report->setWindowFlags(Qt::Tool);
////    report->setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    report->initBox();
    report->resize(600, 700);
    report->show();
}// End slot
// ----------------------------------------------------------------------
//->setWindowFlags(Qt::Window | Qt::WindowTitleHint);
//->setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);
// ----------------------------------------------------------------------
void WxMain::slotActivHelp(){
//    QString str = ((QPushButton*)sender())->text();
//    if (str == tr("Справка")){
        QTextEdit *txt = new QTextEdit;
        txt->setReadOnly(true);
	txt->setHtml( tr("<HTML>"
                "<BODY>"
                "<H2><CENTER> Справка </CENTER></H2>"
                "<P ALIGN=\"left\">"
                    "<BR>"
                    "<BR>"
                    "<BR>"
        	"</P>"
                "<H3><CENTER> Версия 1.2 </CENTER></H3>"
                "<H4><CENTER> Октябрьь 2013 </CENTER></H4>"
                "<H4><CENTER> Широков О.Ю. </CENTER></H4>"
                "<BR>"
                "</BODY>"
                "</HTML>"
               ));
        txt->resize(250, 200);
        txt->show();
//                "<BODY BGCOLOR=MAGENTA>"
//                "<FONT COLOR=BLUE>"
//                "</FONT>"
//    }
//    qDebug() << tr("Справка");  
return;
}// End slot
// ----------------------------------------------------------------------
void WxMain::slotActivExit(){
    close();
return;
}// End slot
// ----------------------------------------------------------------------
//    setHtml(QString("<BODY BGCOLOR=%1></BODY>").arg(strColor));
// ----------------------------------------------------------------------
