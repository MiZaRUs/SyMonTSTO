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
