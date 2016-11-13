#include "spring.h"
#include <QMessageBox>
#include <math.h>
#include <QDebug>
Spring::Spring(double _diameter, double _length, QLinearGradient _gradient):
    diameter(_diameter),
    length(_length),
    gradient(_gradient){

    //РАСЧЕТЫ ПЕРЕМЕННЫХ
    wireRadius = diameter/15;
    count = round(length/(wireRadius*5)*0.7);
    radianAngle = asin(((length-2*wireRadius)/(2*count-1))/(diameter-2*wireRadius));
    angle = radianAngle*180/M_PI;
    dy = (diameter-2*wireRadius)*(1-cos(radianAngle))/2;
    startLength = length;
}



//РИСУЕМ ПЕРЕДНИЙ/ЗАДНИЙ ВИТОК
QGraphicsPixmapItem* Spring::drawCoil(const QLinearGradient &gradient){
    QPixmap* coil = new QPixmap(diameter, diameter);
    coil->fill(Qt::transparent);
    QPainter* painter = new QPainter(coil);


    painter->setPen(Qt::transparent);
    painter->setBrush(gradient);
    painter->drawRoundedRect(0, 0, 2*wireRadius,  diameter, wireRadius, diameter/2);


    QGraphicsPixmapItem* coilSprite = new QGraphicsPixmapItem(*coil);
    coilSprite->setTransformationMode(Qt::SmoothTransformation);

    delete painter;
    delete coil;

    return coilSprite;
}

QGraphicsItemGroup* Spring:: drawSpring(){
    coil = new QGraphicsPixmapItem*[2*count];

    coil[0] = drawCoil(gradient);
    springSprite.addToGroup(coil[0]);

    for (int i = 1; i < 2*count-1; i++){
        coil[i] = drawCoil(gradient);
        springSprite.addToGroup(coil[i]);
    }

    coil[2*count-1] = drawCoil(gradient);
    springSprite.addToGroup(coil[2*count-1]);
    refreshSpring();

    return &springSprite;
}


void Spring::refreshSpring(){
    //вычисляем угол наклона витка (он меняется в зависимости от длины пружины)
    radianAngle = asin(((length-2*wireRadius)/(2*count-1))/(diameter-2*wireRadius));
    //его же в градусах
    angle = radianAngle*180/M_PI;
    //корректировка по диаметру
    dy = (diameter-2*wireRadius)*(1-cos(radianAngle))/2;
    //dx – расстояние между центрами двух соседних витков (передних)
    double dx = (diameter-2*wireRadius)*sin(radianAngle);
    //поворачиваем и смещаем все витки кроме первого и последнего
    //первый корректируем только по диаметру и не поворачиваем
    coil[0]->setX(0);
    coil[0]->setY(-diameter/2+wireRadius+dy);
    for(int i = 1; i < 2*count - 1; i++){
      //передние (четные) против часовой стрелки, задние по
        if (i % 2 == 0){
          coil[i]->setRotation(-angle);
          //координата по X
          coil[i]->setX(coil[i-1]->x());
        } else {
            coil[i]->setRotation(angle);
            coil[i]->setX(wireRadius + i*dx);
        };
        coil[i]->setY(-diameter/2+wireRadius+dy);
     }
    //последний тоже не поворачиваем
    coil[2*count-1]->setX(length-2*wireRadius);
    coil[2*count-1]->setY(-diameter/2+wireRadius+dy);
}

void Spring::setLength(double _length){
    length = _length;
    refreshSpring();
}

void Spring::setPosition(double _x, double _y){
    x = _x;
    springSprite.setX(_x);
    springSprite.setY(_y);
}



