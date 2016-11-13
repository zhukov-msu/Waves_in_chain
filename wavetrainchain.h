#ifndef WAVETRAINCHAIN_H
#define WAVETRAINCHAIN_H

#include "rungekuttablechain.h"
#include <QLinearGradient>
#include <QRadialGradient>
#include "monochain.h"
class ReturningChain: public RungeKuttableChain {
    Q_OBJECT;
    ReturningChain(struct chainSettings setting,
                   QLinearGradient springGradient,
                   QRadialGradient _firstMassGradient, QRadialGradient _secondMassGradient,
                   double _firstMass, double _secondMass);
};

class WaveTrainChain : public RungeKuttableChain  {
    Q_OBJECT
public:
    WaveTrainChain(struct chainSettings setting,
                   QLinearGradient springGradient,
                   QRadialGradient _firstMassGradient, QRadialGradient _secondMassGradient,
                   double _firstMass, double _secondMass);
    QGraphicsItemGroup* drawAt(double x, double y);
    virtual void reinit(double w);
    ~WaveTrainChain();
private:
    double returnTime;
    ReturningChain* returning;
    Monochain* mono;
    void rungeKuttaFree(double h, double* m, double* x, double* dx);
private slots:
    virtual void nextState();
    void nextStateFree();


};

#endif // WAVETRAINCHAIN_H
