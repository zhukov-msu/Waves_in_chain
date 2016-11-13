#include "chain.h"

double Chain::timerInterval = 0.01;

Chain::Chain(struct chainSettings setting):
    A(setting.A),
    w(setting.w),
    chainTime(0),
    //chainSprite(new QGraphicsItemGroup()),
    count(setting.count),
    visibleMassCount(setting.visible_count),
    springsCoefficient(setting.k),
    width(setting.width){
    chainSprite = new QGraphicsItemGroup();
    //не забываем проиницилиализировать буфер коэффициентов
    coefficientsV = new double[setting.count][4];
    coefficientsX = new double[setting.count][4];
}

Chain::~Chain(){
    delete chainSprite;
}

QGraphicsItemGroup* Chain::drawAt(double x, double y){

    chainSprite->addToGroup(massArray[1]->draw());
    chainSprite->addToGroup(massArray[1]->setText(QString("1")));
    massArray[1]->setPosition(0, 0);
    //отрисуем только видимые
    for (int i = 1; i < visibleMassCount+1; i++){
        chainSprite->addToGroup(springArray[i]->drawSpring());
        springArray[i]->setPosition(1+massArray[i]->getStartX() + massArray[i]->getRadius(), 0);
        chainSprite->addToGroup(massArray[i+1]->draw());
        chainSprite->addToGroup(massArray[i+1]->setText(QString().setNum(i+1)));
        massArray[i+1]->setPosition(massArray[1]->getDiameter()*((float)i/2) + massArray[2]->getDiameter()*((float)i/2) + springArray[0]->width()*(i), 0);
    }

    chainSprite->setX(x);
    chainSprite->setY(y);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextState()));
    timer->start(40);

    return chainSprite;

}



void Chain:: reinit(/*double _A,*/ double _w){
    timer->stop();
    //A = _A;
    w = _w;
    for (int i = 1; i < visibleMassCount+1; i++){
        springArray[i]->setPosition( massArray[i]->getStartX() + massArray[i]->getRadius(), 0);
        springArray[i]->setLength(springArray[i]->getStartLength());
    }
    for(int i = 0; i < count-1; i++){
        massArray[i]->setResults(0,0);
    }
    massArray[0]->setResults(0, A*w);
    chainTime = 0;
    \
    for (int i = 0; i < 1; i++)
        nextState();
}

void Chain::pause(bool working){
    if(working)
        timer->stop();
    else
        timer->start(40);
}

void Chain::speedDown(float k){
    timerInterval /= k;
}

void Chain::speedUp(float k){
    timerInterval *= k;
}


