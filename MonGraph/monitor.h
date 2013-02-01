#ifndef _monitor_h_
#define _monitor_h_

#include <QtGui>
#include <QString>
//  --
const int MAXPOINT = 255;
class QLCDNumber;
// ===================================================================
class WxGrid: public QWidget{

public:
    WxGrid(void); // (0,100)
//    init(int mx=0, int bx=0, int my=0, int by=0); // (10,80)
    void pushPoint(float f);	// вывод на экран новой точки
    void resetPoint(void);
    int max, min;
    int xsm;	// разделы "сантиметры"

private:
    int poz;		// прокрутка диаграммы
    int ymm;		// делений на шкале "милиметры"
    int ysm, xsn;	// разделы "сантиметры"
    int yZero, xZero;	// нижний и правый край i*cd_y и i*cd_x
//  --
    int iPin, maxPin;
    QPointF *pin;
    QColor color;
//  --
    void drawGrid(QPainter *painter);
    void drawGraph(QPainter *painter);
    void drawShkala(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
};
// ===================================================================
class Monitor: public QWidget{
    QVBoxLayout *plout;
    QLCDNumber *plcd;
    WxGrid *pgrd;
//  --
    float mng;
    float ing;
//  --
public:
    Monitor(void);

    void setData(float f = 0);
    void setScale( int sc );
};
// ===================================================================
#endif //_monitor_h_
