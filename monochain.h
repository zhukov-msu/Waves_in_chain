#ifndef MONOCHAIN_H
#define MONOCHAIN_H

#include <QLinearGradient>
#include <QRadialGradient>
#include "chain.h"

class Monochain : public Chain {
public:
    Monochain(struct chainSettings setting,
              QLinearGradient _springGradient, QRadialGradient _massGradient,
              double _startMass = 1.0);
    void nextState();
};

#endif // MONOCHAIN_H
