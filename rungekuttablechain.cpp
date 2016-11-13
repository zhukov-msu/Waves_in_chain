#include "rungekuttablechain.h"

RungeKuttableChain::RungeKuttableChain(struct chainSettings setting):
    Chain(setting){
}

void RungeKuttableChain::rungeKutta(double h){
    int i;

#define K springsCoefficient
#define M(i) massArray[i]->getMass()
#define X(i) massArray[i]->getOffset()
#define DX(i) massArray[i]->getDerivative()
#define CX(i) coefficientsX[i]
#define CV(i) coefficientsV[i]

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
    coefficientsV[count-1][1] = 1;//h*(-K*(X(count-1)+CX(count-1)[0]/2-X(count-2)-CX(count-2)[0]/2)-K*(X(count-1)+CX(count-1)[0]/2-X(count-2)-CX(count-2)[0]/2))/M(count-1);

    //Третий шаг.
    for (i = 1; i < count; i++)
        CX(i)[2] = h*(DX(i) + CV(i)[1]/2);

    for (i = 1; i < count - 1; i++)
        CV(i)[2] = K*h*(X(i+1)- 2*X(i) + X(i-1) + CX(i+1)[1]/2 - CX(i)[1] + CX(i-1)[1]/2)/M(i);
    coefficientsV[count-1][2] = 1;//h*(-K*(X(count-1)+CX(count-1)[1]/2-X(count-2)-CX(count-2)[1]/2)-K*(X(count-1)+CX(count-1)[1]/2-X(count-2)-CX(count-2)[1]/2))/M(count-1);
    //Четвертый шаг.
    for (i = 1; i < count; i++)
        CX(i)[3] = h*(DX(i) + CV(i)[2]);

    for (i = 1; i < count - 1; i++)
        CV(i)[3] = K*h*(X(i+1)- 2*X(i) + X(i-1) + CX(i+1)[2]/2 - CX(i)[2] + CX(i-1)[2]/2)/M(i);
    coefficientsV[count-1][3] = 1;//h*(-K*(X(count-1)+CX(count-1)[2]-X(count-2)-CX(count-2)[2])-K*(X(count-1)+CX(count-1)[2]-X(count-2)-CX(count-2)[2]))/M(count-1);

    //Суммируем.

    massArray[0]->setResults(A*sin(w*chainTime), A*w*cos(w*chainTime));
    for (i = 1; i < count; i++){
        float x = X(i) + (CX(i)[0]+2*CX(i)[1]+2*CX(i)[2]+CX(i)[3])/6;
        float dx = DX(i) + (CV(i)[0]+2*CV(i)[1]+2*CV(i)[2]+CV(i)[3])/6;
        massArray[i]->setResults(x, dx);
    }
    chainTime += h;

#undef K
#undef M
#undef X
#undef DX
#undef CX
#undef CV
}
