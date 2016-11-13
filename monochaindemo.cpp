#include "monochaindemo.h"
#include "demowidget.h"
#include "ui_demowidget.h"
#include "qcustomplot.h"
#include "chain.h"
#include "monochain.h"

#define absF(x) (x<0? -x : x)

MonoChainDemo::MonoChainDemo(QWidget *parent) :
    DemoWidget(parent)
{
    ui->labelMass->hide();
    ui->labelMassValue->hide();
    ui->horizontalSlider->hide();
    ui->pushButtonMax->move(ui->dispCurve->x()+10,ui->dispCurve->y()+50);
    ui->labelOmega->move(ui->labelOmega->x()-3,ui->labelOmega->y()-1);
    ui->labelKValue->move(ui->dispCurve->x()+ui->dispCurve->width()/2+195,ui->dispCurve->y()+ui->dispCurve->height()-26);
    ui->labelW0->setPixmap(QPixmap(":/images/omega1.png"));
    ui->labelHuyeybl->hide();
    ui->labelHuyeybl_2->hide();
    ui->labelAcMaxFormula->hide();
    ui->labelOptMaxFormula->hide();
    ui->labelOptMinFormula->hide();
    QObject::connect(ui->pushButtonMax,  SIGNAL(clicked()), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(on_verticalSlider_sliderMoved(int)));
    //create chain
    chainSettings setting(18, 1500, currRate, 1650/10*17, 16);
    myChain = new Monochain(setting, *springGradient, *massGradient);
    backgroundChain = new Monochain(setting, *backgroundSpringGradient, *backgroundMassGradient);

    scene->addItem(backgroundChain->drawAt(0,0));
    backgroundChain->pause(true);
    scene->addItem(myChain->drawAt(0,0));
    pause();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));

    ui->dispCurve->xAxis->setRange(0,M_PI);
    ui->dispCurve->yAxis->setRange(0,1.1);
    ui->dispCurve->yAxis->setAutoTickStep(0);
    ui->dispCurve->yAxis->setTickStep(1.0);
    for (int i = 0; i < 101; ++i)
    {
        k1[i] = i*M_PI/100;
        omega1[i] = sin(k1[i]/2);
        if (absF(omega1[i] - currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass()))) <= 0.5*ui->dispCurve->yAxis->range().upper/100)
        {
            kDot[0] = k1[i];
            omegaDot[0] = omega1[i];
        }
    }
    qDebug() << k1[100] << "k1";
    //draw plot
    ui->dispCurve->graph(0)->setData(k1,omega1);
    QPen graphPen(blueDotPen);
    graphPen.setWidthF(2.0);
    graphPen.setColor(QColor(Qt::red));
    ui->dispCurve->graph(0)->setPen(graphPen);
    ui->dispCurve->addGraph();
    ui->dispCurve->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->dispCurve->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
    ui->dispCurve->graph(1)->setPen(blueDotPen);
    qDebug() << kDot[0] << omegaDot[0];
    ui->dispCurve->graph(1)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())),0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">   = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    w0 = 1;
    ui->dispCurve->yAxis->setAutoTickLabels(false);
    omegaTickLabel.clear();
    omegaTickLabel << "" << "ωₒ";
    ui->dispCurve->yAxis->setTickVectorLabels(omegaTickLabel);
    ui->labelChastota->move(ui->pushButtonMax->x(),ui->pushButtonMax->y()-ui->labelChastota->height()-20);
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())));
}


void MonoChainDemo::time()
{
    QVector<double> offset(plotCount), x(plotCount);
    double* offsets = myChain->getMassOffsets();
    for (int i = 0; i < plotCount; ++i) {
        offset[i] = offsets[i];
        x[i] = i;
    }
    delete[] offsets;
    ui->plot->graph(0)->setData(x,offset);
    ui->plot->replot();
}

void MonoChainDemo::rateChanged()
{
    ui->dispCurve->graph(1)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
}

void MonoChainDemo::on_verticalSlider_sliderMoved(int position){
    currRate = (float)(position+1)*17/100;
    if (fabs(currRate - 1) < 0.1)
        currRate = 1;
    for (int i = 0; i < 101; ++i)
    {
        if (fabs(omega1[i] - currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())) <= ui->dispCurve->yAxis->range().upper/100))
        {
            kDot[0] = k1[i];
            omegaDot[0] = omega1[i];
        }
    }

    if (currRate >= 2* sqrt(myChain->getCoefficient() / myChain->getMasses()[0]))
    {
        kDot[0] = M_PI;
        omegaDot[0] = currRate / (2* sqrt(myChain->getCoefficient() / myChain->getMasses()[0]));
    }
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    backgroundChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())),0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">= %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}
