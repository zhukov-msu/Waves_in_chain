#include "doublechain.h"


#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

DoubleChain::DoubleChain(struct chainSettings setting,
                         QLinearGradient _springGradient,
                         QRadialGradient _firstMassGradient, QRadialGradient _secondMassGradient,
                         double _firstMass, double _secondMass):
    Chain(setting){
    massArray = new Mass*[setting.count];
    springArray = new Spring*[setting.count - 1];
    double allMassLength = 0;

    for(int i = 0; i < setting.count-1; i+=2){
        massArray[i] = new Mass(_firstMass, _firstMassGradient);
        allMassLength+=massArray[i]->getDiameter();
        massArray[i+1] = new Mass(_secondMass, _secondMassGradient);
        allMassLength+=massArray[i+1]->getDiameter();

    }

    if (setting.count % 2 == 1){
        massArray[setting.count-1] = new Mass(_firstMass, _firstMassGradient);
        allMassLength+=massArray[setting.count-1]->getDiameter();
    }

    for(int i =0; i<setting.count-1; i++)
        springArray[i] = new Spring(0.17*(setting.width-allMassLength)/setting.count,
                                    (setting.width-allMassLength)/setting.count, _springGradient);
    massArray[0]->setResults(0, setting.A*setting.w);
}

DoubleChain::~DoubleChain(){
    for(int i = 0; i < count-1; i++){
        delete massArray[i];
        delete springArray[i];
    }
}

void DoubleChain::nextState(){
#define K springsCoefficient
#define M(i) massArray[i]->getMass()
#define u1 A

 //   qDebug() << sqrt(2*K/MIN(M(0),M(1)));
   // qDebug() << sqrt(2*K*(1/M(0)+1/M(1)));
   // qDebug() << sqrt(K * (1/M(0) + 1/M(1))*2);
    double k1;
    double u2;
    complex<double> c;
    if (w <= sqrt(2*K/MAX(M(0),M(1)))){ //макс акуст

        qDebug() << "case1";
        //k1 = asin(sqrt((1-(w*w/K/(1/M(0)+1/M(1))-1)*(w*w/K/(1/M(0)+1/M(1))-1))/M(0)/M(1))*(M(0)+M(1))/2);
        k1 = asin(sqrt(2*K*(M(0)+M(1))*w*w - w*w*w*w*M(0)*M(1))/2/K);
        if ((sqrt(2*K/MAX(M(0),M(1))) - w) < 0.01)
            u2 = 0;
        else
            u2 = u1*cos(k1)*2*K/(2*K-M(1)*w*w);
        for (int i = 0; i < count; i+=2){
            massArray[i]->setResults(u1*cos(-w*chainTime+k1*i), 0);
            massArray[i+1]->setResults(u2*cos(-w*chainTime+k1*(i+1)), 0);
        }
    } else if((w > sqrt(2*K/MAX(M(0),M(1))))
              && (w < sqrt(K*(M(0)+M(1))/M(0)/M(1)))){
        qDebug() << "case2";
        double c = (-w*w*w*w + 2*K*(M(0)+M(1))/M(0)/M(1)*w*w)/4/K/K*M(0)*M(1);
        double alpha = acosh(sqrt(c));
        /*if ((w - sqrt(2*K/MAX(M(0),M(1)))) < 0.01)
            u2 = 0;
        else*/
            u2 = u1 * (2*K-M(0)*w*w)/2/K/cosh(alpha);
            qDebug() << "smthng";
        for (int i = 0; i < count; i+=2){
            massArray[i]->setResults(u1*exp(-i*alpha)*cos(-w*chainTime+M_PI/2*i), 0);
            massArray[i+1]->setResults(u2*exp(-(i+1)*alpha)*cos(-w*chainTime+M_PI/2*(i+1)), 0);
        }
    } else if((w >= sqrt(K*(M(0)+M(1))/M(0)/M(1)))
              && (w < sqrt(2*K/MIN(M(0),M(1))))){
        qDebug() << "case3";
        double c = (-w*w*w*w + 2*K*(M(0)+M(1))/M(0)/M(1)*w*w)/4/K/K*M(0)*M(1);
        double alpha = acosh(sqrt(c));

        /*if ((sqrt(2*K/MIN(M(0),M(1))) - w) < 0.01)
            u2 = 0;
        else*/
        qDebug() << alpha;
        u2 = u1 * (2*K-M(1)*w*w)/2/K/cosh(alpha);
        for (int i = 0; i < count; i+=2){
            massArray[i]->setResults(u2*exp(-i*alpha)*cos(-w*chainTime+M_PI/2*i), 0);
            massArray[i+1]->setResults(u1*exp(-(i+1)*alpha)*cos(-w*chainTime+M_PI/2*(i+1)), 0);
        }
    } else if((w >= sqrt(2*K/MIN(M(0),M(1)))) //минимум оптической
               && (w < sqrt(K * (1/M(0) + 1/M(1)) * 2))){
        qDebug() << "case4";
        //k1 = asin(sqrt((1-(w*w/K/(1/M(0)+1/M(1))-1)*(w*w/K/(1/M(0)+1/M(1))-1))/M(0)/M(1))*(M(0)+M(1))/2);
        k1 = asin(sqrt(2*K*(M(0)+M(1))*w*w - w*w*w*w*M(0)*M(1))/2/K);
        if (w - (sqrt(2*K/MIN(M(0),M(1)))) < 0.01)
            u2 = 0;
        else
            u2 = u1*cos(k1)*2*K/(2*K-M(0)*w*w);
        for (int i = 0; i < count; i+=2){
            massArray[i]->setResults(u2*cos(-w*chainTime+k1*i), 0);
            massArray[i+1]->setResults(u1*cos(-w*chainTime+k1*(i+1)), 0);
        }
    } else if(w >= sqrt(K * (1/M(0) + 1/M(1)) * 2)){ //макс оптической
        k1 = M_PI;
        qDebug() << "case5";
        float alpha = acosh(w/sqrt(2*K*(1/M(0)+1/M(1))));
        u2 = u1*cos(k1)*2*K/(2*K-M(0)*w*w);
        for (int i = 0; i < count; i+=2){
            massArray[i]->setResults(u2*exp(-i*alpha)*cos(-w*chainTime+k1*i), 0);
            massArray[i+1]->setResults(u1*exp(-(i+1)*alpha)*cos(-w*chainTime+k1*(i+1))/M(1)*M(0), 0);
        }
    }

    chainTime += timerInterval;
    for (unsigned int i = 1; i < visibleMassCount + 1; i++){
        massArray[i]->resetPosition();
        springArray[i]->setResults(massArray[i]->getOffset()/100);
        springArray[i]->setLength(massArray[i+1]->getStartX()+massArray[i+1]->getOffset()/100-massArray[i+1]->getRadius()
                -(massArray[i]->getStartX()+massArray[i]->getOffset()/100+massArray[i]->getRadius())+2);
    }
}
