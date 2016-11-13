#ifndef MONOCHAINDEMO_H
#define MONOCHAINDEMO_H
#include "demowidget.h"
class MonoChainDemo : public DemoWidget
{
    Q_OBJECT
public:
    explicit MonoChainDemo(QWidget *parent = 0);
    virtual void time();
signals:

public slots:
    void rateChanged();
    void on_verticalSlider_sliderMoved(int position);
};

#endif // MONOCHAINDEMO_H
