#include "doublechaindemo.h"
#include "doublechain.h"
#include "chain.h"
#include "ui_demowidget.h"
DoubleChainDemo::DoubleChainDemo(QWidget *parent) :
    DemoWidget(parent)
{
    ui->labelNameWindow->setText("Chain of Two Different Masses");
    ui->labelNameWindow->setGeometry(ui->labelNameWindow->x(),ui->labelNameWindow->y(),370,ui->labelNameWindow->height());
    ui->labelNameWindow->move(this->width()/2-ui->labelNameWindow->width()/2,10);
    ui->labelOmega->move(ui->labelOmega->x()-3,ui->labelOmega->y()-1);
    ui->pushButtonMax->move(ui->dispCurve->x()+10,ui->dispCurve->y()+50);
    //минимумы максимумы
    ui->pushButtonMax->hide();
    ui->pushButtonMaxOptical->show();
    ui->pushButtonMaxAcoustic->show();
    ui->pushButtonMinAcoustic->show();
    ui->labelW0->show();
    ui->pushButtonMaxOptical->raise();
    ui->pushButtonMaxAcoustic->raise();
    ui->pushButtonMinAcoustic->raise();
    ui->labelW0->raise();
    //слайдер масс
    ui->horizontalSlider->move(ui->pushButtonFaster->x()+ui->pushButtonFaster->width()+20,ui->pushButtonFaster->y());
    ui->labelMass->move(ui->horizontalSlider->x(),ui->horizontalSlider->y()+ui->horizontalSlider->height()+10);
    ui->labelMassValue->move(ui->labelMass->x()+ui->labelMass->width(),ui->labelMass->y()+20);


    //изменим шаг слайдера
    ui->verticalSlider->setSingleStep(10);
    //поехали
    m1 = 1.0;
    m2 = 1.5;

    setting = new chainSettings(1000, 750, currRate, 150000);
    myChain = new DoubleChain(*setting, *springGradient, *massGradient, *massGradient2, m1, m2);

    backgroundChain = new DoubleChain(*setting, *backgroundSpringGradient, *backgroundMassGradient,*backgroundMassGradient,m1,m2);

    scene->addItem(backgroundChain->drawAt(0,0));
    backgroundChain->pause(true);

    scene->addItem(myChain->drawAt(0,0));

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define K myChain->getCoefficient()
#define M(i) myChain->getMasses()[i]
    ui->horizontalSlider->setMinimum(20);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> %1</span></p></body></html>").arg(m2,0,'g',2);
    ui->labelMassValue->setText(num);
    ui->pushButtonMaxOptical->move(ui->dispCurve->x()-10,ui->dispCurve->y()+70);
    ui->labelOptMaxFormula->move(ui->pushButtonMaxOptical->x(),ui->pushButtonMaxOptical->y()+ui->pushButtonMaxOptical->height());
    ui->pushButtonMinAcoustic->move(ui->labelOptMaxFormula->x(),ui->labelOptMaxFormula->y()+ui->labelOptMaxFormula->height());
    ui->labelOptMinFormula->move(ui->pushButtonMaxOptical->x(),ui->pushButtonMinAcoustic->y()+ui->pushButtonMinAcoustic->height());
    ui->pushButtonMaxAcoustic->move(ui->labelOptMinFormula->x(),ui->labelOptMinFormula->y()+ui->labelOptMinFormula->height());
    ui->labelAcMaxFormula->move(ui->pushButtonMaxOptical->x(),ui->pushButtonMaxAcoustic->y()+ui->pushButtonMaxAcoustic->height());


    ui->labelKValue->move(ui->labelKValue->x()+30,ui->labelKValue->y());
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));
    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(on_verticalSlider_sliderMoved(int)));
    QObject::connect(ui->pushButtonMaxAcoustic, SIGNAL(clicked()), this, SLOT(on_pushButtonMaxAcoustic_clicked()));
    QObject::connect(ui->pushButtonMaxOptical,  SIGNAL(clicked()), this, SLOT(on_pushButtonMaxOptical_clicked()));
    QObject::connect(ui->pushButtonMinAcoustic, SIGNAL(clicked()), this, SLOT(on_pushButtonMinOptical_clicked()));

    ui->dispCurve->xAxis->setRange(0,M_PI);
    ui->dispCurve->yAxis->setRange(0,19);


    b  = myChain->getCoefficient();

    //optical
    for (int i = 0; i < 101; ++i)
    {
        k1[i] = M_PI/100*i;
        omega1[i] = sqrt(b * (1/m1 + 1/m2) * (1 + sqrt(1 - 4 * m1 * m2 * sin(k1[i]) * sin(k1[i]) / (m1+ m2) / (m1 + m2))));
    }
    //acoustic
    for (int i = 0; i < 101; ++i)
    {
        k2[i] = M_PI/100*i;
        omega2[i] = sqrt(b * (1/m1 + 1/m2) * (1 - sqrt(1 - 4 * m1 * m2 * sin(k2[i]) * sin(k2[i]) / (m1+ m2) / (m1 + m2))));
    }

    ui->dispCurve->graph(0)->setData(k1,omega1);
    ui->dispCurve->addGraph();
    ui->dispCurve->graph(1)->setData(k2,omega2);
    ui->dispCurve->replot();

    this->findDot();

    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    qDebug() << "rate" << currRate;
    qDebug() << "omega:" << omegaDot[0];
    qDebug() << "k:" << kDot[0];

    ui->dispCurve->addGraph();
    QPen graphPen(blueDotPen);
    ui->dispCurve->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->dispCurve->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
    graphPen.setWidthF(2);
    ui->dispCurve->graph(1)->setPen(graphPen);
    graphPen.setColor(QColor(Qt::red));
    ui->dispCurve->graph(0)->setPen(graphPen);
    ui->dispCurve->graph(2)->setPen(blueDotPen);
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->yAxis->setAutoTicks(false);
    w0 = sqrt(K*(M(0)+M(1))/M(1)/M(0));//собственная частота
    omegaTick.resize(1);
    omegaTick << w0;
    ui->dispCurve->yAxis->setTickVector(omegaTick);
    ui->dispCurve->yAxis->setAutoTickLabels(false);


//    ui->dispCurve->legend->setVisible(true);
//    ui->dispCurve->legend->setFont(QFont("Times", 14));
//    ui->dispCurve->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
//    ui->dispCurve->graph(0)->setName("Оптическая ветвь");
//    ui->dispCurve->graph(1)->setName("Акустическая ветвь");
//    ui->dispCurve->legend->removeItem(2);
    ui->dispCurve->replot();
    ui->labelChastota->move(ui->pushButtonMax->x()-10,ui->pushButtonMax->y()-ui->labelChastota->height());
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
    ui->horizontalSlider->setSliderPosition(m2/5*100);
    if (this->width()<1200)
        ui->labelHuyeybl->move(ui->verticalSlider->x()+130,ui->dispCurve->y()+50);
    else
        ui->labelHuyeybl->move(ui->verticalSlider->x()+200,ui->dispCurve->y()+50);
    ui->labelHuyeybl_2->move(ui->labelHuyeybl->x(),ui->labelHuyeybl->y()+400);

    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/w0,0,'gb',2);
    qDebug() << num;
    ui->labelOmega->setText(num);
}

void DoubleChainDemo::time()
{
    QVector<double> offset(plotCount/2/*+plotCount%2*/), x(plotCount/2/*+plotCount%2*/);

    double* offsets = myChain->getMassOffsets();
    int j = 0;
    for (int i = 1; i < plotCount+1; i+=2)
    {
        offset[j] = offsets[i];
        x[j] = i;
        j += 1;
    }
    ui->plot->graph(0)->setData(x,offset);
    j = 0;
    offset.resize(plotCount/2+1);
    x.resize(plotCount/2+1);
    for (int i = 0; i < plotCount+1; i+=2)
    {
        offset[j] = offsets[i];
        x[j] = i;
        j += 1;
    }
    ui->plot->graph(1)->setData(x,offset);
    delete[] offsets;

    ui->plot->replot();
}

void DoubleChainDemo::on_horisontalSlider_sliderMoved(int value)
{
    m2 = (float)(value)*5/100;
    delete myChain;
    delete backgroundChain;

    myChain = new DoubleChain(*setting, *springGradient, *massGradient2, *massGradient, m2, m1);

    backgroundChain = new DoubleChain(*setting, *backgroundSpringGradient, *backgroundMassGradient, *backgroundMassGradient, m2, m1);
    scene->addItem(backgroundChain->drawAt(0,0));
    scene->addItem(myChain->drawAt(0,0));
    backgroundChain->pause(true);
    pause();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));
    w0 = sqrt(K*(M(0)+M(1))/M(1)/M(0));
    omegaTick.clear();
    omegaTick << w0;
    ui->dispCurve->yAxis->setTickVector(omegaTick);
    omegaTickLabel.clear();
    omegaTickLabel << "ωₒ";
    ui->dispCurve->yAxis->setTickVectorLabels(omegaTickLabel);
    if (m1 != m2)
    {
        if(!(ui->pushButtonMax->isHidden()))
        {
            ui->pushButtonMaxAcoustic->show();
            ui->pushButtonMaxOptical-> show();
            ui->pushButtonMinAcoustic->show();
            ui->pushButtonMaxAcoustic->raise();
            ui->pushButtonMaxOptical-> raise();
            ui->pushButtonMinAcoustic->raise();
            ui->pushButtonMax->hide();
        }
        for (int i = 0; i < 101; ++i)
        {
            k1[i] = M_PI/100*i;
            omega1[i] = sqrt(b * (1/m1 + 1/m2) * (1 + sqrt(1 - 4 * m1 * m2 * sin(k1[i]) * sin(k1[i]) / (m1+ m2) / (m1 + m2))));
        }
        //acoustic
        for (int i = 0; i < 101; ++i)
        {
            k2[i] = M_PI/100*i;
            omega2[i] = sqrt(b * (1/m1 + 1/m2) * (1 - sqrt(1 - 4 * m1 * m2 * sin(k2[i]) * sin(k2[i]) / (m1+ m2) / (m1 + m2))));
        }
    }
    else
    {
        ui->pushButtonMaxAcoustic->hide();
        ui->pushButtonMaxOptical->hide();
        ui->pushButtonMinAcoustic->hide();
        ui->pushButtonMax->show();
        ui->pushButtonMax->raise();
        for (int i = 0; i < 51; ++i)
        {
            k1[i] = M_PI-M_PI/100*i;
            omega1[i] = sqrt(b * (1/m1 + 1/m2) * (1 + sqrt(1 - 4 * m1 * m2 * sin(k1[i]) * sin(k1[i]) / (m1+ m2) / (m1 + m2))));
        }
        //acoustic
        for (int i = 0; i < 51; ++i)
        {
            k2[i] = M_PI/100*i;
            omega2[i] = sqrt(b * (1/m1 + 1/m2) * (1 - sqrt(1 - 4 * m1 * m2 * sin(k2[i]) * sin(k2[i]) / (m1+ m2) / (m1 + m2))));
        }
        for (int i = 50; i < 101; ++i)
        {
            k1[i] = M_PI/100*i;
            omega1[i] = sqrt(b * (1/m1 + 1/m2) * (1 + sqrt(1 - 4 * m1 * m2 * sin(k1[i]) * sin(k1[i]) / (m1+ m2) / (m1 + m2))));
        }
        //acoustic
        for (int i = 51; i < 101; ++i)
        {
            k2[i] =0; -M_PI/100*i;
            omega2[i] = 0;sqrt(b * (1/m1 + 1/m2) * (1 - sqrt(1 - 4 * m1 * m2 * sin(k2[i]) * sin(k2[i]) / (m1+ m2) / (m1 + m2))));
        }
    }

    ui->dispCurve->graph(0)->setData(k1,omega1);
    ui->dispCurve->graph(1)->setData(k2,omega2);
    this->findDot();
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> %1</span></p></body></html>").arg(m2,0,'g',2);
    ui->labelMassValue->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);

    ui->dispCurve->graph(2)->setData(kDot,omegaDot);

    ui->dispCurve->yAxis->setRange(0,omega1[0]+4);
    ui->dispCurve->replot();
    if(working)
    {
        pause();
    }
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
}

void DoubleChainDemo::rateChanged()
{
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
}

void DoubleChainDemo::on_verticalSlider_sliderMoved(int position){
    delete backgroundChain;
    delete myChain;
    currRate = (float)(position+1)*ui->dispCurve->yAxis->range().upper/100;


    backgroundChain = new DoubleChain(*setting, *backgroundSpringGradient, *backgroundMassGradient, *backgroundMassGradient, m2, m1);
    myChain = new DoubleChain(*setting, *springGradient, *massGradient2, *massGradient, m2, m1);
    scene->addItem(backgroundChain->drawAt(0,0));
    scene->addItem(myChain->drawAt(0,0));
    backgroundChain->pause(true);
    pause();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));
    myChain->reinit(currRate);
    this->findDot();
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/w0,0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);

    if (working)
        pause();
}

void DoubleChainDemo::on_pushButtonMaxAcoustic_clicked(){
    currRate = sqrt(2*K/MAX(M(0),M(1)));
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/w0,0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
    qDebug() << "rate" << currRate;
    qDebug() << "omega:" << omegaDot[0];
    qDebug() << "k:" << kDot[0];
}

void DoubleChainDemo::on_pushButtonMaxOptical_clicked()
{
    currRate = sqrt(K * (1/M(0) + 1/M(1)) * 2);
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/w0,0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}

void DoubleChainDemo::on_pushButtonMinOptical_clicked()
{
    currRate = sqrt(2*K/MIN(M(0),M(1)));
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"><font size = 5>ω = %1ω<sub>0</sub></font></span></p></body></html>").arg(currRate/w0,0,'gb',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'gb',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}

void DoubleChainDemo::findDot()
{   float min = ui->dispCurve->yAxis->range().upper;
    if (fabs(currRate - 1) < 0.1)
        currRate = 1;
//    if (m1 != m2)
    for (int i = 0; i < k1.size(); ++i)
        {

            if (fabs(omega1[i] - currRate) < min)
            {
                if(k1[i]>=M_PI/2-0.0001)
                {
                    min = fabs(omega1[i] - currRate);
                    kDot[0] = k1[i];
                    omegaDot[0] = omega1[i];
                }
            }
            if (fabs(omega2[i] - currRate) < min)
            {
                if((M(0)!=M(1)) && (k1[i]<=M_PI/2+0.0001) || (M(0)==M(1)) && (k1[i]>=M_PI/2-0.0001))
                {
                    min = fabs(omega2[i] - currRate);
                    kDot[0] = k2[i];
                    omegaDot[0] = omega2[i];
                }
            }
        }
//    else
//    {
//        for (int i = 0; i < 101; ++i)
//        {

//            if (fabs(omega1[i] - currRate) < 0.1)
//            {
//                kDot[0] = k1[i];
//                omegaDot[0] = omega1[i];
//            }
//        }
//    }
    if (currRate >= sqrt(K * (1/M(0) + 1/M(1)) * 2))
    {
        kDot[0] = M_PI;
        omegaDot[0] = currRate;
    }
    if((currRate >= sqrt(2*K/MAX(M(0),M(1))))
                  && (currRate <= sqrt(2*K/MIN(M(0),M(1))))){
        kDot[0] = M_PI/2;
        omegaDot[0] = currRate;
    }
    ui->dispCurve->replot();
}
