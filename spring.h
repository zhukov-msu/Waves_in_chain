#ifndef SPRINGH
#define SPRINGH

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

//#include "myFunctions.h"

class Coil{
private:
    double x, y;
public:
    Coil();
};

class Spring {
private:
    double diameter, length, wireRadius,
    angle, radianAngle, dy, startLength, maxLength, minLength, x;
    int count;

    QGraphicsPixmapItem** coil;

    QLinearGradient gradient;
    QGraphicsPixmapItem* drawBottomCoil(const QLinearGradient &gradient);
    QGraphicsPixmapItem* drawCoil(const QLinearGradient &gradient);
    QGraphicsPixmapItem* drawTopCoil(const QLinearGradient &gradient);

    QGraphicsItemGroup springSprite;

    void refreshSpring();
public:
    Spring (double diameter, double length, QLinearGradient gradient);
    QGraphicsItemGroup* getSpringSprite(){
        return &springSprite;
    }
    QGraphicsItemGroup* drawSpring();

    double height(){
        return diameter;
    }
    double width(){
        return length;
    }
    double getStartLength(){
        return startLength;
    }

    double startX(){
        return x;
    }

    void setLength(double length);
    void setPosition(double x, double y);
    void setResults(double offset){
        springSprite.setX(x + offset);
    }
};

#endif // SPRINGH



