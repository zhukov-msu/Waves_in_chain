#ifndef DOUBLECHAINDEMO_H
#define DOUBLECHAINDEMO_H
#include "demowidget.h"
class DoubleChainDemo : public DemoWidget
{
    Q_OBJECT
public:
    explicit DoubleChainDemo(QWidget *parent = 0);
    virtual void time();

    void findDot();
    double w0;
    QVector<double> omegaTick;
    QVector<QString> omegaTickLabel;
signals:

public slots:
    void on_horisontalSlider_sliderMoved(int);
    void rateChanged();
    void on_verticalSlider_sliderMoved(int);
    void on_pushButtonMaxOptical_clicked();
    void on_pushButtonMaxAcoustic_clicked();
    void on_pushButtonMinOptical_clicked();
};

#endif // DOUBLECHAINDEMO_H
