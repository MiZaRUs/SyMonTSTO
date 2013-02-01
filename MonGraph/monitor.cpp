#include "monitor.h"

// ----------------------------------------------------------------------
Monitor::Monitor(void) : QWidget(){
// ind_element, setka
//    setCursor(Qt::PointingHandCursor);
    setMinimumSize(80, 100);
    mng = 1;
    ing = 0;
//  --
//qDebug() << "Monitor::Monitor";
//  --
    plout = new QVBoxLayout;
    plout->setMargin(2);
    plout->setSpacing(5);

    int dx = 5;
    plcd = new QLCDNumber(dx,this);
    plcd->setSegmentStyle(QLCDNumber::Flat);
    plcd->display(88888);
//  --
    pgrd = new WxGrid();
    setScale(1);
//  --
    plout->addWidget(plcd, 1);
    plout->addWidget(pgrd, 3);
    setLayout(plout);
//  --
    show();
}// End Monitor
//----------------------------------------------------------
void Monitor::setData(float f){
//qDebug() << "Monitor::setData( " << f << " );";
    plcd->display(f);
//  --
    f = f * mng + ing;
//qDebug() << "Monitor::setData( " << f << " );";
    pgrd->pushPoint(f);
}// End 
// ----------------------------------------------------------------------
void Monitor::setScale(int sc){

    if(sc == 1){//    0 : 800
        ing = 0;
        mng = 0.05;
        pgrd->max = 800;
        pgrd->min = 0;
        pgrd->xsm = 4;
    }
    if(sc == 2){//    0 : 80
        ing = 0;// - 40;
        mng = 1;
        pgrd->max = 80;
        pgrd->min = 0;
        pgrd->xsm = 8;
    }
    if(sc == 3){// 0 : 5
        ing = 0;
        mng = 10;
        pgrd->max = 3;
        pgrd->min = 0;
        pgrd->xsm = 3;
    }

    if(sc == 4){//    
        ing = 40;
        mng = 1;
        pgrd->max = 0;
        pgrd->min = -40;
        pgrd->xsm = 4;
    }
    pgrd->resetPoint();
}// End 
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//#include <math.h>
//----------------------------------------------------------
WxGrid::WxGrid(void) : QWidget(){
//    setMouseTracking(true);
//  шкала времени
    poz = 0;		// позиция диаграммы
    ymm = 10;		// мелкие деления окна
    ysm = ymm * 6;	// крупные деления

//  шкала данных
    xsm = 6;		// деления окна 2...15
    xsn = 1;		// выравнивание окна 1...xsm - 1
//  --
    max = 60;
    min = 0;

    maxPin = 2000;	//буфера данных - максималькое количество показаний (ширина экрана).
    iPin = 0; // текущее количество показаний
    pin = new QPointF[maxPin+1];	// буфер данных
//  --
    color = QColor(200, 0, 0); // цвет тренда красный
/*    QColor c1(200, 0, 0); // krasn
    QColor c2(0, 122, 0); // zelen
    QColor c3(255, 0, 255); //seren
    QColor c4(0, 0, 200); // siniy
    QColor c5(100, 100, 0); //korich
    QColor c6(0, 165, 155); //golub
    QColor c7(170, 170, 0); //jelt
*/
    show();
}// End WxGrid
//WxGrid::WxGrid(int mx, int bx, int my, int by) : QWidget(){}
// ----------------------------------------------------------------------
void WxGrid::drawGrid(QPainter *painter){
    if(poz < 1 )poz = ysm;
    int j = poz;
    for(int i = yZero; i > 0; i--, j++){ // вертикальные линии привязаны к пикселам
        if(!(j % ymm)){
            if(j % ysm){
                painter->setPen(QPen(Qt::black, 0.1));
            }else{
                painter->setPen(QPen(Qt::black, 0.2));
                j = 0;
            }
            painter->drawLine( i, 0, i, xZero );
        }
    }
    painter->setPen(QPen(Qt::black, 0.9));
    painter->drawLine( yZero, 0, yZero, xZero );
    painter->setPen(QPen(Qt::black, 0.5));
    painter->drawLine( 0, 0, 0, xZero );
//  --
    j = xsm * 10;
    float t = (float)xZero / j;
    for(int i = 0; i < j; i++){ // горизонтальные линии масштабируемые
        if(i%10){
            painter->setPen(QPen(Qt::black, 0.1));
        }else{
            painter->setPen(QPen(Qt::black, 0.2));
        }
        painter->drawLine( 0, i*t, yZero, i*t );
    }
    painter->setPen(QPen(Qt::black, 0.5));
    painter->drawLine( 0, 0, yZero, 0 );
    painter->drawLine(0, xZero, yZero, xZero);
}// End drawGrid
// ----------------------------------------------------------------------
void WxGrid::drawGraph(QPainter *painter){
    if(iPin<1) return;
//  --
    QPointF tp[iPin];
    float t = (float)xZero / (xsm * 10); // деление шкалы
    for(int i = 0; i < iPin; i++){
        tp[i].rx() = yZero - pin[i].rx(); //time
        tp[i].ry() = (float) xZero - (t * pin[i].ry()); //data
//qDebug() << "Tp  " << tp[i].rx() << "  " << tp[i].ry();
    }
    painter->setPen(QPen(color, 0.7));
    painter->drawPolyline(tp, iPin);
}// End drawGrid
// ----------------------------------------------------------------------
void WxGrid::drawShkala(QPainter *painter){
    painter->setPen(QPen(Qt::darkBlue, 0.7));
    painter->drawText(2, 10, QString::number( max ));
    painter->drawText(2, xZero - 2, QString::number( min ));
}// End 
// ----------------------------------------------------------------------
void WxGrid::pushPoint(float f){
    iPin++;
    if(iPin > maxPin) iPin = maxPin;
    for( int i = iPin; i > 0 ; i--){
        pin[i].rx() = pin[i-1].rx() + 1; // + time
        pin[i].ry() = pin[i-1].ry();
    }
    pin[0] = QPointF(0,f); // (x,y)
    poz--;
    update();
}// End pushPoint
// ----------------------------------------------------------------------
void WxGrid::resetPoint(void){
    iPin = 1;
    poz = 0;
    update();
}// End resetPoint
// ----------------------------------------------------------------------
void WxGrid::paintEvent(QPaintEvent * /*event*/){
    QPainter painter( this );
    painter.setRenderHint(QPainter::Antialiasing, true);
//  --
    xZero = height();
    yZero = width();
//  --
    drawGrid(&painter);
    drawGraph(&painter);
    drawShkala(&painter);
}// End paintEvent
// ----------------------------------------------------------------------
