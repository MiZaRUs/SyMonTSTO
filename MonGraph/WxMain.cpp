#include <string.h>
#include "WxMain.h"
// ----------------------------------------------------------------------
void WxMain::msgError(QString snm, QString ser){
        pLayout = new QGridLayout;
        QLabel *labx1 = new QLabel("<h3><font color=red>"+ snm +"</font></h3>");
//        QLabel *labx2 = new QLabel("<font color=blue>"+ ser +"</font>");
        QLabel *labx2 = new QLabel(QString("<font color=blue>%1</font>").arg(ser));
        pLayout->addWidget(labx1, 0, 0);    
        pLayout->addWidget(labx2, 1, 0);    

	QPushButton* pcmd = new QPushButton(tr("Выход"));
        pcmd->setMinimumSize(80, 20);
	connect(pcmd, SIGNAL(clicked()), SLOT(slotActivExit()));
        pLayout->addWidget(pcmd, 3, 0);    
        setLayout(pLayout);
}//End msgError
// ----------------------------------------------------------------------
WxMain::WxMain(QSqlQuery &qs, QWidget* pwgt/*= 0*/) : QWidget(pwgt){
    setCursor(Qt::PointingHandCursor);
    setMinimumSize(140, 200);
    resize(180,273);
//    resize(280,740);

    pSql = &qs;
    pId = 0;
//  --
    if( (pMenu = WxMenu())){
        connect(pMenu, SIGNAL(triggered(QAction*)),SLOT(slotMenuClicked(QAction*)));
//  --
	pId = 1001;
        pmon = new Monitor();
        pLayout = new QGridLayout;
        pLayout->addWidget(pmon);

        setLayout(pLayout);
	slotTimerRefresh();
	startRefresh(900000);// 2000 - 2sek
        pmon->show();
//  ----------------
    }// if else Error
}//End WxMain
// ----------------------------------------------------------------------
QMenu* WxMain::WxMenu(void){
    QMenu *pmnu = NULL;

    QMenu *mvds1;
    QMenu *mvds5;

    QMenu *mtp1;
    QMenu *mtp2;
    QMenu *mtp3;
    QMenu *mtp4;
    QMenu *mtp5;

    QMenu *mps16;
    QMenu *mps21;
    QMenu *mps22;
    QMenu *mps23;

    QMenu *mpsAll;

    QMenu *mspeed;
    QMenu *mscale;

    pmnu = new QMenu(this);
//  --
    mvds1 = new QMenu(tr("ВДС-1"), pmnu);
    pmnu->addMenu( mvds1);

    mtp1 = new QMenu(tr("1 ВДМ"), mvds1);
    mvds1->addMenu( mtp1);
    mtp1->addAction(tr("1 вдм t 1"))->setObjectName(tr("2001"));//2 0
    mtp1->addAction(tr("1 вдм t 2"))->setObjectName(tr("2002"));//2 1
//    mtp1->addAction(tr("1 вдм t 3"))->setObjectName(tr("2003"));//2 2
//    mtp1->addAction(tr("1 вдм t 4"))->setObjectName(tr("2004"));//2 3
    mtp1->addSeparator();
    mtp1->addAction(tr("1 вдм Зз 1"))->setObjectName(tr("12001"));//12 0
    mtp1->addAction(tr("1 вдм Зз 2"))->setObjectName(tr("12002"));//12 1


    mtp2 = new QMenu(tr("2 ВДМ"), mvds1);
    mvds1->addMenu( mtp2);
    mtp2->addAction(tr("2 вдм t 1"))->setObjectName(tr("3001"));//3
    mtp2->addAction(tr("2 вдм t 2"))->setObjectName(tr("3002"));//3
    mtp2->addAction(tr("2 вдм t 3"))->setObjectName(tr("3003"));//3
    mtp2->addAction(tr("2 вдм t 4"))->setObjectName(tr("3004"));//3

    mtp3 = new QMenu(tr("3 ВДМ"), mvds1);
    mvds1->addMenu( mtp3);
    mtp3->addAction(tr("3 вдм t 1"))->setObjectName(tr("4001"));//4
    mtp3->addAction(tr("3 вдм t 2"))->setObjectName(tr("4002"));//4
    mtp3->addAction(tr("3 вдм t 3"))->setObjectName(tr("4003"));//4
    mtp3->addAction(tr("3 вдм t 4"))->setObjectName(tr("4004 "));//4

    mtp4 = new QMenu(tr("4 ВДМ"), mvds1);
    mvds1->addMenu( mtp4);
    mtp4->addAction(tr("4 вдм t 1"))->setObjectName(tr("5001"));//5
    mtp4->addAction(tr("4 вдм t 2"))->setObjectName(tr("5002"));//5
//    mtp4->addAction(tr("4 вдм t 3"))->setObjectName(tr("5003"));//5
//    mtp4->addAction(tr("4 вдм t 4"))->setObjectName(tr("5004"));//5
    mtp4->addSeparator();
    mtp4->addAction(tr("4 вдм Зз 1"))->setObjectName(tr("11001"));//11
    mtp4->addAction(tr("4 вдм Зз 2"))->setObjectName(tr("11002"));//11

    mtp5 = new QMenu(tr("5 ВДМ"), mvds1);
    mvds1->addMenu( mtp5);
    mtp5->addAction(tr("5 вдм t 1"))->setObjectName(tr("6001"));//6
    mtp5->addAction(tr("5 вдм t 2"))->setObjectName(tr("6002"));//6
    mtp5->addAction(tr("5 вдм t 3"))->setObjectName(tr("6003"));//6
    mtp5->addAction(tr("5 вдм t 4"))->setObjectName(tr("6004"));//6
//  --

    mvds5 = new QMenu(tr("ВДС-5"), pmnu);
    pmnu->addMenu( mvds5);

    mps16 = new QMenu(tr("16 П/С"), mvds5);
    mvds5->addMenu( mps16);
    mps16->addAction(tr("16 п/с t 1"))->setObjectName(tr("7001"));//7
    mps16->addAction(tr("16 п/с t 2"))->setObjectName(tr("7002"));//7
//    mps16->addAction(tr("16 п/с t 3"))->setObjectName(tr("7003"));//7
//    mps16->addAction(tr("16 п/с t 4"))->setObjectName(tr("7004"));//7

    mps21 = new QMenu(tr("21 П/С"), mvds5);
    mvds5->addMenu( mps21);
    mps21->addAction(tr("21 п/с t 1"))->setObjectName(tr("8001"));//8
    mps21->addAction(tr("21 п/с t 2"))->setObjectName(tr("8002"));
    mps21->addAction(tr("21 п/с t 3"))->setObjectName(tr("8003"));
    mps21->addAction(tr("21 п/с t 4"))->setObjectName(tr("8004"));

    mps22 = new QMenu(tr("22 П/С"), mvds5);
    mvds5->addMenu( mps22);
    mps22->addAction(tr("22 п/с t 1"))->setObjectName(tr("9001"));//9
    mps22->addAction(tr("22 п/с t 2"))->setObjectName(tr("9002"));
//    mps22->addAction(tr("22 п/с t 3"))->setObjectName(tr("9003"));
//    mps22->addAction(tr("22 п/с t 4"))->setObjectName(tr("9004"));

    mps23 = new QMenu(tr("23 П/С"), mvds5);
    mvds5->addMenu( mps23);
    mps23->addAction(tr("23 п/с t 1"))->setObjectName(tr("10001"));//10
    mps23->addAction(tr("23 п/с t 2"))->setObjectName(tr("10002"));
//    mps23->addAction(tr("23 п/с t 3"))->setObjectName(tr("10003"));
//    mps23->addAction(tr("23 п/с t 4"))->setObjectName(tr("10004"));
//  --


    pmnu->addSeparator();

    mpsAll = new QMenu(tr("Давление"), mvds5);
    mvds5->addMenu( mpsAll);
    mpsAll->addAction(tr("16 п/с"))->setObjectName(tr("15001"));
    mpsAll->addAction(tr("21 п/с"))->setObjectName(tr("15002"));
    mpsAll->addAction(tr("22 п/с"))->setObjectName(tr("15003"));
    mpsAll->addAction(tr("23 п/с"))->setObjectName(tr("15004"));
    mpsAll->addAction(tr("26 п/с"))->setObjectName(tr("15005"));

//    mmuti = new QMenu(tr("ДПЦ"), pmnu);
//    pmnu->addMenu(mmuti);
//    mmuti->addAction(tr("Муть"))->setObjectName(tr("14.1"));

    pmnu->addSeparator();
    mscale = new QMenu(tr("Шкала"), pmnu);
    pmnu->addMenu( mscale);
    mscale->addAction(tr("от 0 до 800"))->setObjectName(tr("d1"));
    mscale->addAction(tr("от 0 до 80"))->setObjectName(tr("d2"));
    mscale->addAction(tr("от 0 до 3"))->setObjectName(tr("d3"));
    mscale->addAction(tr("от -40 до 0"))->setObjectName(tr("d4"));

    pmnu->addSeparator();
    mspeed = new QMenu(tr("Скорость"), pmnu);
    pmnu->addMenu( mspeed);
    mspeed->addAction(tr("6 делений в минуту"))->setObjectName(tr("s1"));
    mspeed->addAction(tr("1 деление в мнуту")  )->setObjectName(tr("s2"));
    mspeed->addAction(tr("12 делений в час")   )->setObjectName(tr("s3"));
    mspeed->addAction(tr("6 делений в час")   )->setObjectName(tr("s4"));

    pmnu->addSeparator();
    pmnu->addAction(tr("&Справка"));
    pmnu->addSeparator();
    pmnu->addAction(tr("Вы&xод"));
//  --
return pmnu;
}//End WxMenu
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void WxMain::slotMenuClicked(QAction* pAction){
    QString strmn = pAction->text().remove("&");
    QString st = pAction->objectName();
//qDebug() << "SlotMenu: "<< st << ">> "<< " Id: " << pId << " - " << strmn;
    int tmp = st.toInt();
    if(tmp > 0){
        pId = tmp;
/////	pmon->init();//перестройка
        setWindowTitle(strmn);
        slotTimerRefresh();
        pmon->setScale(0);	 //
    }else{
//qDebug() << "2";
        if(st == tr("s1")){
            timer->setInterval(1000);	 // 1 pixel v 1 sek (1 del = 10 pixel)
        }
        if(st == tr("s2")){
            timer->setInterval(6000);	 //
        }
        if(st == tr("s3")){
            timer->setInterval(20000);	 // 1 pixel v min
        }
        if(st == tr("s4")){
            timer->setInterval(60000);	 // 20 sek = 20000
        }
//  --
        if(st == tr("d1")){
            pmon->setScale(1);	 //
        }
        if(st == tr("d2")){
            pmon->setScale(2);	 //
        }
        if(st == tr("d3")){
            pmon->setScale(3);	 //
        }
        if(st == tr("d4")){
            pmon->setScale(4);	 //
        }
//  --
        if(strmn == tr("Справка")){
	    slotActivHelp();
        }
	if(strmn == tr("Выxод"))slotActivExit();
    }
//qDebug() << "Speed: "<< st << ">> "<< " Id: " << pId;
return;
}// End slot
//-----------------------------------------------------------------------
int WxMain::startRefresh(int interval){
    if(!pId)return 0;
    timer = new QTimer( this );
    timer->setInterval(interval);	 // 20 sek = 20000
    connect( timer, SIGNAL(timeout()), this, SLOT(slotTimerRefresh()) );
    timer->start();
return 1;
}// End startRefresh
// ----------------------------------------------------------------------
void WxMain::slotTimerRefresh(){
//qDebug() << "TimerRfresh\n";
    if(pId > 0){
        if(!pSql->exec(QString("SELECT data FROM archiv WHERE id = %1 AND utime = (SELECT MAX(utime) FROM archiv WHERE id = %1)").arg(pId))){
	}else{
	    while (pSql->next()){
		int d = pSql->value(0).toInt();
		if(d < -100000 ) d = d + 2000000000;
//qDebug() << "D=" << d;

		float f;
		if(d > -9999){
		    d = d / 10;
	    	    f = (float)d / 10;
		}else{
		    f = (float)d;
		}
//qDebug() << "F=" << f;
		pmon->setData( f);
//qDebug() << "LCD" << iK << " " << f;
	    }//while
	}// if else
    }// if pid
};
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
                    "-9000 : Прибор не отвечает.<BR>"
                    "-9900 : Прибор неисправен.<BR>"
                    "-9901 : К.З. датчика.<BR>"
                    "-9903 : Нет канала.<BR>"
                    "-9906 : Канал не доступен.<BR>"
                    "-9908 : Обрыв датчика.<BR>"
                    "-9998 : Ошибка обмена.<BR>"
                    "-9999 : Ошибка КС.<BR>"
                    "<BR>"
                    "<BR>"
                    "<BR>"
        	"</P>"
                "<H3><CENTER> Версия 0.7.7 </CENTER></H3>"
                "<H4><CENTER> Февраль 2010 </CENTER></H4>"
                "<H4><CENTER> Июнь 2011 </CENTER></H4>"
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
