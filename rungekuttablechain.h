#ifndef RUNGEKUTTABLECHAIN_H
#define RUNGEKUTTABLECHAIN_H

#include "chain.h"
#include "math.h"

class RungeKuttableChain:public Chain{
    Q_OBJECT
protected:
    void rungeKutta(double timeInterval);
public:
    RungeKuttableChain(struct chainSettings setting);
};

#endif // RUNGEKUTTABLECHAIN_H
