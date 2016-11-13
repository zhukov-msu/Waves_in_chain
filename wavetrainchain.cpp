#include "wavetrainchain.h"

WaveTrainChain::WaveTrainChain(struct chainSettings setting,
                               QLinearGradient _springGradient,
                               QRadialGradient _firstMassGradient, QRadialGradient _secondMassGradient,
                               double _firstMass, double _secondMass):
    RungeKuttableChain(setting),
    returnTime(0){
    massArray = new Mass*[setting.count];
    springArray = new Spring*[setting.count - 1];
    double allMassLength = 0;

    for(int i = 0; i < setting.count/2; i++){
        massArray[i] = new Mass(_firstMass, _firstMassGradient);
        allMassLength+=massArray[i]->getDiameter();
    }

    for(int i = setting.count/2; i < setting.count; i++){
        massArray[i] = new Mass(_secondMass, _secondMassGradient);
        allMassLength+=massArray[i]->getDiameter();
    }

    for(int i = 0; i<setting.count-1; i++)
        springArray[i] = new Spring(0.27*(setting.width-allMassLength)/setting.count,
                                    (setting.width-allMassLength)/setting.count, _springGradient);
    //massArray[0]->setResults(0, setting.A*setting.w);

    for(unsigned int i =(count-visibleMassCount)/2-19 ; i < (count-visibleMassCount)/2-10; i++){
        int r = i - (count-visibleMassCount)/2 + 19;
        massArray[i]->setResults(-A*sin(M_PI*2/9*r), 0/*-A*cos(M_PI*2/9*r)*/);
    }

}

void WaveTrainChain::reinit(double w){

    for(unsigned int i = 0; i < count; i++){
        massArray[i]->setResults(0, 0);
    }

    for(unsigned int i = (count-visibleMassCount)/2; i < (count + visibleMassCount)/2; i++)
        massArray[i]->resetPosition();
    chainTime = 0;
    returnTime = 0;
    disconnect(timer, SIGNAL(timeout()), this, SLOT(nextStateFree()));
    connect(timer, SIGNAL(timeout()), this, SLOT(nextState()));
}

void WaveTrainChain::nextState(){
    /*for (int i = 0; i < 100; i++)
        rungeKutta(timerInterval/100);
    for (unsigned int i = 0; i < visibleMassCount; i++){
        massArray[i]->resetPosition();
        springArray[i]->setResults(massArray[i]->getOffset()/100);
        springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
    }

    if (chainTime > 2*M_PI/w){
        rungeKutta(timerInterval);
        for (unsigned int i = 0; i < visibleMassCount; i++){
            massArray[i]->resetPosition();
            springArray[i]->setResults(massArray[i]->getOffset()/100);
            springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                    -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
        }*/
    for(int i=0; i< count;i++)
        massArray[i]->setResults(0,0);
    for(int i = ceil((w*chainTime)*5/M_PI); i < count/2, i < ceil((w*chainTime)*5/M_PI) + 10 ; i++)
        massArray[i]->setResults(A*sin(M_PI/5*i - chainTime*w), 0);
    chainTime+=timerInterval;
    if (ceil((w*chainTime)*5/M_PI) + 10 > count/2){
        disconnect(timer, SIGNAL(timeout()), this, SLOT(nextState()));
        connect(timer, SIGNAL(timeout()), this, SLOT(nextStateFree()));
    }
    for (unsigned int i = (count-visibleMassCount)/2; i < (count+visibleMassCount)/2; i++){
        massArray[i]->resetPosition();
        springArray[i]->setResults(massArray[i]->getOffset()/100);
       springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
    }
    massArray[(count+visibleMassCount)/2]->resetPosition();
    //}
}

void WaveTrainChain::nextStateFree(){
    #define M(i) massArray[i]->getMass()
    for(int i=0; i< count;i++)
        massArray[i]->setResults(0,0);

    for(int i = ceil((w*chainTime)*5/M_PI); ((i>0) && (i < count/2) && (i < ceil((w*chainTime)*5/M_PI) + 10)) ; i++)
        massArray[i]->setResults(A*sin(M_PI/5*i - chainTime*w), 0);

    for(int i = count/2-ceil((w*returnTime)*5/M_PI); ((i>0) && (i < count/2)&& (i < count/2-ceil((w*returnTime)*5/M_PI)+10))  ; i++){
        massArray[i]->setResults(massArray[i]->getOffset()-
                                 A*(M(count-1)-M(0))/(M(0)+M(count-1))*sin(M_PI/5*(count/2-i -1) - returnTime*w),0) ;
    }

    for(int i = ceil(((w*returnTime)*5/M_PI-10)/M(0)*M(count-1))+count/2; ((i<count) && (i < count) && (i < ceil(((w*returnTime)*5/M_PI)/M(0)*M(count-1))+count/2)) ; i++)
        if (i >= count/2)
            massArray[i]->setResults((A-A*(M(count-1)-M(0))/(M(0)+M(count-1)))*sin((M_PI/5*(i-count/2))*M(0)/M(count-1) - returnTime*w ),0);
    chainTime+=timerInterval;
    returnTime+=timerInterval;

    for (unsigned int i = (count-visibleMassCount)/2; i < (count+visibleMassCount)/2; i++){
        massArray[i]->resetPosition();
        springArray[i]->setResults(massArray[i]->getOffset()/100);
       springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
    }
    massArray[(count+visibleMassCount)/2]->resetPosition();
}

void WaveTrainChain::rungeKuttaFree(double h, double* m, double* x, double* dx){
    int i;
  //  h = 0.00005;
#define K springsCoefficient
#define M(i) massArray[i]->getMass()
#define X(i) massArray[i]->getOffset()
#define DX(i) massArray[i]->getDerivative()
#define CX(i) coefficientsX[i]
#define CV(i) coefficientsV[i]
/*
    //Первый шаг.
    for (i = 1; i < count; i++){
        CX(i)[0] = h*DX(i);
    }


    for (i = 1; i < count - 1; i++)
        CV(i)[0] = K*h*(X(i+1)- 2*X(i) + X(i-1))/M(i);
    coefficientsV[count-1][0] = h*(-K*(X(count-1)-X(count-2))-K*(X(count-1)-X(count-2)))/M(count-1);


    //Второй шаг.
    for (i = 1; i < count; i++)
        CX(i)[1] = h*(DX(i) + CV(i)[0]/2);

    for (i = 1; i < count - 1; i++)
        CV(i)[1] = K*h*(X(i+1)+CX(i+1)[0]/2-2*X(i)-CX(i)[0]+X(i-1)+CX(i-1)[0]/2)/M(i);
    coefficientsV[count-1][1] = h*(-K*(X(count-1)+CX(count-1)[0]/2-X(count-2)-CX(count-2)[0]/2)-K*(X(count-1)+CX(count-1)[0]/2-X(count-2)-CX(count-2)[0]/2))/M(count-1);

    //Третий шаг.
    for (i = 1; i < count; i++)
        CX(i)[2] = h*(DX(i) + CV(i)[1]/2);

    for (i = 1; i < count - 1; i++)
        CV(i)[2] = K*h*(X(i+1)- 2*X(i) + X(i-1) + CX(i+1)[1]/2 - CX(i)[1] + CX(i-1)[1]/2)/M(i);
    coefficientsV[count-1][2] = h*(-K*(X(count-1)+CX(count-1)[1]/2-X(count-2)-CX(count-2)[1]/2)-K*(X(count-1)+CX(count-1)[1]/2-X(count-2)-CX(count-2)[1]/2))/M(count-1);
    //Четвертый шаг.
    for (i = 1; i < count; i++)
        CX(i)[3] = h*(DX(i) + CV(i)[2]);

    for (i = 1; i < count - 1; i++)
        CV(i)[3] = K*h*(X(i+1)- 2*X(i) + X(i-1) + CX(i+1)[2]/2 - CX(i)[2] + CX(i-1)[2]/2)/M(i);
    coefficientsV[count-1][3] = h*(-K*(X(count-1)+CX(count-1)[2]-X(count-2)-CX(count-2)[2])-K*(X(count-1)+CX(count-1)[2]-X(count-2)-CX(count-2)[2]))/M(count-1);

    //Суммируем.
    massArray[0]->setResults(0, 0);

    massArray[i]->setResults(0,0);
    massArray[count-1]->setResults(0,0);
    for (i = 1; i < count-1; i++){
        float x = X(i) + (CX(i)[0]+2*CX(i)[1]+2*CX(i)[2]+CX(i)[3])/6;
        float dx = DX(i) + (CV(i)[0]+2*CV(i)[1]+2*CV(i)[2]+CV(i)[3])/6;
        massArray[i]->setResults(x, dx);
    }*/
/*
    float* newU = new float[count];
    float* newV = new float[count];

    for(int n = 1; n < count-2; n++){
        newV[n] = K*h/M(n)*(X(n+1)+X(n-1)-2*X(n)) + DX(n);
        newU[n] = h*newV[n]+X(n);
    }

    for (int n = 1; n < count-2; n++){
        massArray[n]->setResults(newU[n], newV[n]);
    }

    delete newU;
    delete newV;

    double* newU = new double[count];
    double* newV = new double[count];

    for(int n=1; n < count -1; n++){
        newU[n] = X(n) + h*DX(n);
        newV[n] = DX(n) + h*K*(X(n-1)+X(n+1)-2*X(n))/M(n);
    }

    for (int n = 1; n < count-1; n++){
        massArray[n]->setResults(newU[n], newV[n]);
    }

    delete newU;
    delete newV;
*/


    chainTime += h;

#undef K
#undef M
#undef X
#undef DX
#undef CX
#undef CV

    delete[] m;
    delete[] x;
    delete[] dx;
#undef K
}


QGraphicsItemGroup* WaveTrainChain::drawAt(double x, double y){

    chainSprite->addToGroup(massArray[(count-visibleMassCount)/2]->draw());
    massArray[(count-visibleMassCount)/2]->setPosition(-55, 0);
    double r = massArray[(count-visibleMassCount)/2]->getRadius()-55;
    //отрисуем только видимые
    for (int i = (count-visibleMassCount)/2; i < (count+visibleMassCount)/2; i++){
        chainSprite->addToGroup(springArray[i]->drawSpring());
        springArray[i]->setPosition(1+r, 0);
        r += springArray[i]->getStartLength();
        chainSprite->addToGroup(massArray[i+1]->draw());
        r+= massArray[i+1]->getRadius();
        massArray[i+1]->setPosition(r, 0);
        r+= massArray[i+1]->getRadius();
    }
    chainSprite->setX(x);
    chainSprite->setY(y);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextState()));
    timer->start(40);

    return chainSprite;
}

WaveTrainChain::~WaveTrainChain(){
    for(int i = 0; i < count-1; i++){
        delete massArray[i];
        delete springArray[i];
    }
}
