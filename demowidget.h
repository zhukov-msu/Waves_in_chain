#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

#include <QWidget>
#include "qcustomplot.h"
#include "chain.h"
namespace Ui {
class DemoWidget;
}

class DemoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DemoWidget(QWidget *parent = 0, unsigned int _plotCount = 15);
    void resizeEvent(QResizeEvent* event);
    ~DemoWidget();
    QVector<double> omega1, k1, omega2, k2;
    QVector<double> omegaDot, kDot;
    double m1,m2,b;
    int plotCount;
    double w0;
    QVector<double> omegaTick;
    QVector<QString> omegaTickLabel;
    chainSettings * setting;
protected:
    Chain* myChain,* backgroundChain;
    double currRate, omegaRange;//омегаРэндж - это для дисп. кривой
    bool working;
    int dotPos;
    Ui::DemoWidget *ui;
    QPen blueDotPen;
    QPen redDotPen;
    QPen axisPen;
    QPen gridPen;
    QLinearGradient* springGradient, *backgroundSpringGradient;
    QRadialGradient* backgroundMassGradient;
    QRadialGradient* massGradient;
    QRadialGradient* massGradient2;
    QGraphicsScene* scene;
    QFont graphFont;
    QString num;
public:
    virtual void time() = 0;
    void pause();
    void keyPressEvent(QKeyEvent * k);
public slots:
    void rateUp();
    void rateDown();
    void back();
    void time_slot();
    void on_pushButtonFaster_clicked();
    void on_pushButtonSlower_clicked();
    void on_plot_destroyed();
    void on_pushButtonPause_clicked();
private slots:
    void on_pushButtonMax_clicked();
};

#endif // DEMOWIDGET_H
