#include "monochain.h"

Monochain::Monochain(struct chainSettings setting,
                     QLinearGradient _springGradient, QRadialGradient _massGradient,
                     double _startMass):
    Chain(setting){
    massArray = new Mass*[setting.count];
    springArray = new Spring*[setting.count - 1];
    double allMassLength = 0;
    for(int i = 0; i<setting.count; i++){
        massArray[i] = new Mass(_startMass, _massGradient);
        allMassLength+=massArray[i]->getDiameter();
    }
    for(int i =0; i<setting.count-1; i++)
        springArray[i] = new Spring(0.17*(setting.width-allMassLength)/setting.count,
                                    (setting.width-allMassLength)/setting.count, _springGradient);
    massArray[0]->setResults(0, setting.A*setting.w);
}

void Monochain::nextState(){
#define K springsCoefficient
#define M(i) massArray[i]->getMass()

    double k;
    if (w/sqrt(K/M(0)) < 2){
        k = 2*asin(w/2/sqrt(K/M(0)));
        for (int i = 0; i < count; i++)
            massArray[i]->setResults(A*sin(w*chainTime+k*i), 0);
    }else {
        k = M_PI;
        float alpha = 2*acosh(w/2/sqrt(K/M(0)));
        for (int i = 0; i < count; i++)
            massArray[i]->setResults(A*exp(-i*alpha)*sin(w*chainTime+k*i), 0);
    }

    chainTime += timerInterval;
    for (unsigned int i = 1; i < visibleMassCount+1; i++){
        massArray[i]->resetPosition();
        springArray[i]->setResults(massArray[i]->getOffset()/100);
        springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
    }
}


