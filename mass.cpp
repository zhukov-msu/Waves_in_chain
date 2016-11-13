#include "mass.h"
#include <math.h>
Mass::Mass(double mass, QRadialGradient gradient, double dx):
    _dx(dx),
    _mass(mass),
    _gradient(gradient),
    _offset(0) {
    _diameter = pow(mass, 1.0/3)*45;
}

QGraphicsPixmapItem* Mass::draw(){
    QPainter* p;
    QPixmap* massPixmap;

    massPixmap = new QPixmap(_diameter + 2, _diameter + 2);
    massPixmap->fill(Qt::transparent);

    p = new QPainter(massPixmap);
    p->setRenderHint(QPainter::Antialiasing);
    p->setPen(Qt::transparent);
    p->setBrush(_gradient);
    p->drawEllipse(1, 1, _diameter, _diameter);

    _sprite = new QGraphicsPixmapItem();
    _sprite->setPixmap(*massPixmap);

    _text = new QGraphicsTextItem();
    delete p;
    delete massPixmap;

    _sprite->setOffset(-_diameter/2, -_diameter/2);

    return _sprite;
}

QGraphicsTextItem* Mass::setText(QString a){
    QFont font(QString("Times New Roman"),19, 19);
    _text->setFont(font);
    _text->setDefaultTextColor(Qt::white);
    _text->setPlainText(a);
    return _text;
}

void Mass::setPosition(double x, double y){
    _x = x;
    _sprite->setX(x);
    _sprite->setY(y);
    _text->setX(_offset/100+_x);
    _text->setY(y-25);
}
