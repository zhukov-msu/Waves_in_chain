#ifndef CHAINH
#define CHAINH
#include "mass.h"
#include "spring.h"
#include <QGraphicsItemGroup>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <math.h>



struct chainSettings{
    double A, w, width, visible_count, k;
    int count;

    chainSettings(int _count, double _A, double _w, double _width, unsigned int _visible_count = 10, double _coefficient = 60):
        A(_A),
        w(_w),
        width(_width),
        visible_count(_visible_count),
        k(_coefficient),
        count(_count){};
};

class Chain: public QObject{
    Q_OBJECT
protected:
    unsigned int visibleMassCount;
    double A, w, chainTime;
    double static timerInterval;
    QTimer* timer;
    int count;
    double (*coefficientsX)[4];
    double (*coefficientsV)[4];
    Mass** massArray;
    Spring** springArray;
    double springsCoefficient;
    QGraphicsItemGroup* chainSprite;
private:
    double* massXCoordinates;
    double width;
public:
    Mass* operator[](int i){
        return massArray[i];
    }

    double getCoefficient(){
        return springsCoefficient;
    }
    int getVisibleMassCount(){
        return visibleMassCount;
    }

    double* getDerivatives(){
        double* der = new double[count];
        for(int i=0; i<count; i++)
            der[i]=massArray[i]->getDerivative();
        return der;
    }
    double* getMasses(){
        double* der = new double[count];
        for(int i=0; i<count; i++)
            der[i]=massArray[i]->getMass();
        return der;
    }

    int getCount(){
        return count;
    }
    QTimer* getTimer(){
        return timer;
    }

    double* getMassOffsets(){
        double* der = new double[count];
        for(int i=0; i<count; i++)
            der[i]=massArray[i]->getOffset();
        return der;
    }

    void speedDown(float coefficient);
    void speedUp(float coefficient);

    Chain(struct chainSettings settings);
    virtual QGraphicsItemGroup* drawAt(double x, double y);
    virtual void reinit(/*double A,*/ double w);
    void pause(bool working);

    ~Chain();
public slots:
    virtual void nextState() = 0;
};

#endif // CHAINH
