#include "doublechaindemo.h"
#include "doublechain.h"
#include "chain.h"
#include "ui_demowidget.h"
DoubleChainDemo::DoubleChainDemo(QWidget *parent) :
    DemoWidget(parent)
{
    ui->labelNameWindow->setText("Цепочка из двух различных масс");
    ui->labelNameWindow->setGeometry(ui->labelNameWindow->x(),ui->labelNameWindow->y(),320,ui->labelNameWindow->height());
    ui->labelNameWindow->move(this->width()/2-ui->labelNameWindow->width()/2,10);
    //минимумы максимумы
    ui->pushButtonMax->hide();

    ui->pushButtonMaxOptical->show();
    ui->pushButtonMaxAcoustic->show();
    ui->pushButtonMinAcoustic->show();

    ui->pushButtonMaxOptical->raise();
    ui->pushButtonMaxAcoustic->raise();
    ui->pushButtonMinAcoustic->raise();
    //слайдер масс
    ui->horizontalSlider->move(ui->pushButtonMinAcoustic->x()+ui->pushButtonMinAcoustic->width()+30,ui->pushButtonFaster->y());
    ui->labelMass->move(ui->horizontalSlider->x(),ui->horizontalSlider->y()+ui->horizontalSlider->height()+10);
    ui->labelMassValue->move(ui->labelMass->x()+ui->labelMass->width()+10,ui->labelMass->y()+3);

    //изменим шаг слайдера
    ui->verticalSlider->setSingleStep(10);
    //поехали
    m1 = 1.0;
    m2 = 1.5;

    setting = new chainSettings(1000, 750, currRate, 150000);
    myChain = new DoubleChain(*setting, *springGradient, *massGradient, *massGradient2, m1, m2);
    scene->addItem(myChain->drawAt(0,0));

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define K myChain->getCoefficient()
#define M(i) myChain->getMasses()[i]
    ui->horizontalSlider->setMinimum(20);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(m2,0,'g',2);
    ui->labelMassValue->setText(num);
    ui->pushButtonMaxOptical->move(ui->dispCurve->x()-10,ui->dispCurve->y()+70);
    ui->pushButtonMinAcoustic->move(ui->pushButtonMaxOptical->x(),ui->pushButtonMaxOptical->y()+ui->pushButtonMaxOptical->height()+50);
    ui->pushButtonMaxAcoustic->move(ui->pushButtonMinAcoustic->x(),ui->pushButtonMinAcoustic->y()+ui->pushButtonMinAcoustic->height()+50);
    ui->labelKValue->move(ui->labelKValue->x()+10,ui->labelKValue->y());
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));
    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(rateChanged()));
    QObject::connect(ui->verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(on_verticalSlider_sliderMoved(int)));
    QObject::connect(ui->pushButtonMaxAcoustic, SIGNAL(clicked()), this, SLOT(on_pushButtonMaxAcoustic_clicked()));
    QObject::connect(ui->pushButtonMaxOptical,  SIGNAL(clicked()), this, SLOT(on_pushButtonMaxOptical_clicked()));
    QObject::connect(ui->pushButtonMinAcoustic, SIGNAL(clicked()), this, SLOT(on_pushButtonMinOptical_clicked()));

    //double a = (*myChain)[1]->getStartX()-(*myChain)[0]->getStartX();
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
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(omegaDot[0],0,'g',2);
    qDebug() << num;
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
    ui->labelKValue->setText(num);
    qDebug() << "rate" << currRate;
    qDebug() << "omega:" << omegaDot[0];
    qDebug() << "k:" << kDot[0];

    ui->dispCurve->addGraph();
    ui->dispCurve->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->dispCurve->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    ui->dispCurve->graph(2)->setPen(blueDotPen);
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();

    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0);
    ui->horizontalSlider->setSliderPosition(m2/5*100);
    pause();
}

void DoubleChainDemo::time()
{
    QVector<double> offset(5), x(5);

    double* offsets = myChain->getMassOffsets();
    int j = 0;
    for (int i = 0; i < 10; i+=2)
    {
        offset[j] = offsets[i];
        x[j] = i + 1;
        j += 1;
    }
    ui->plot->graph(0)->setData(x,offset);
    j = 0;
    for (int i = 1; i < 10; i+=2)
    {
        offset[j] = offsets[i];
        x[j] = i + 1;
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

    myChain = new DoubleChain(*setting, *springGradient, *massGradient, *massGradient2, m1, m2);
    scene->addItem(myChain->drawAt(0,0));
    pause();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));

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
    this->findDot();
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(m2,0,'g',2);
    ui->labelMassValue->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
    ui->labelKValue->setText(num);
    ui->dispCurve->graph(0)->setData(k1,omega1);
    ui->dispCurve->graph(1)->setData(k2,omega2);
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->yAxis->setRange(0,omega1[0]+4);
    ui->dispCurve->replot();
}

void DoubleChainDemo::rateChanged()
{
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
}

void DoubleChainDemo::on_verticalSlider_sliderMoved(int position){
    currRate = (float)(position+1)*ui->dispCurve->yAxis->range().upper/100;
    this->findDot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(currRate,0,'g',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}

void DoubleChainDemo::on_pushButtonMaxAcoustic_clicked(){
    currRate = sqrt(2*K/MAX(M(0),M(1)));
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(currRate,0,'g',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
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
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(currRate,0,'g',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}

void DoubleChainDemo::on_pushButtonMinOptical_clicked()
{
    currRate = sqrt(2*K/MIN(M(0),M(1)));
    this->findDot();
    ui->dispCurve->graph(2)->setData(kDot,omegaDot);
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(currRate,0,'g',2);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> = %1</span></p></body></html>").arg(kDot[0],0,'g',2);
    ui->labelKValue->setText(num);
    if (working)
        pause();
}

void DoubleChainDemo::findDot()
{   float min = ui->dispCurve->yAxis->range().upper;
    if (fabs(currRate - 1) < 0.1)
        currRate = 1;
    for (int i = 0; i < 101; ++i)
    {

        if (fabs(omega1[i] - currRate) < min)
        {
            if((M(0)!=M(1)) && (k1[i]<=M_PI/2+0.0001) || (M(0)==M(1)) && (k1[i]>=M_PI/2-0.0001))
            {
                min = fabs(omega1[i] - currRate);
                kDot[0] = k1[i];
                omegaDot[0] = omega1[i];
            }
        }
        if (fabs(omega2[i] - currRate) < min)
        {
            if(k1[i]<=M_PI/2+0.0001)
            {
                min = fabs(omega2[i] - currRate);
                kDot[0] = k2[i];
                omegaDot[0] = omega2[i];
            }
        }
    }

    ui->dispCurve->replot();
}
