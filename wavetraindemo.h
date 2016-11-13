#ifndef WAVETRAINDEMO_H
#define WAVETRAINDEMO_H
#include "demowidget.h"
class WaveTrainDemo : public DemoWidget {
    Q_OBJECT
public:
    explicit WaveTrainDemo(QWidget *parent = 0, unsigned int pc = 50);
    virtual void time();
signals:

public slots:
    void on_resetButton_clicked();
    void on_horisontalSlider_sliderMoved(int);
};

#endif // WAVETRAINDEMO_H
