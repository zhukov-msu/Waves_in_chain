#ifndef DOUBLECHAIN_H
#define DOUBLECHAIN_H

#include "chain.h"
#include <QLinearGradient>
#include <QRadialGradient>
#include <complex>

using namespace std;
class DoubleChain : public Chain{
public:
    DoubleChain (struct chainSettings setting,
                 QLinearGradient _springGradient,
                 QRadialGradient _firstMassGradient, QRadialGradient _secondMassGradient,
                 double _firstMass, double _secondMass);
    ~DoubleChain();
    void nextState();
};

#endif // DOUBLECHAIN_H
