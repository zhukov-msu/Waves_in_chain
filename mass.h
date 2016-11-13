#ifndef MASS_H
#define MASS_H

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPainter>
#include <QRadialGradient>


class Mass{
    double _x,_dx;
    double _diameter;
    double _mass;
    QRadialGradient _gradient;
    double _offset;
    QGraphicsPixmapItem* _sprite;
    QGraphicsTextItem* _text;
public:
    Mass(double mass, QRadialGradient gradient, double dx = 0);
    QGraphicsPixmapItem* draw();
    QGraphicsTextItem* setText(QString a);
    double getDerivative(){
        return _dx;
    }
    double getDiameter(){
        return _diameter;
    }
    double getRadius(){
        return _diameter/2;
    }

    double getMass(){
        return _mass;
    }
    double getStartX(){
        return _x;
    }
    double getOffset(){
        return _offset;
    }
    //double getX(){
      //  return _sprite->x();
    //}
    void resetPosition(){
        _sprite->setX(_offset/100+_x);
        _text->setX(_offset/100+_x);
    }

    void  setResults(double offset, double dx){
        _offset = offset;
        _dx = dx;
    }

    double height(){
        return _sprite->pixmap().height();
    }
    double width(){
        return _sprite->pixmap().width();
    }

    void setPosition(double x, double y);
};

#endif // MASS_H
