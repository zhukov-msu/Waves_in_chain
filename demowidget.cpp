#include "demowidget.h"
#include "ui_demowidget.h"
#include "qcustomplot.h"
#include "windows.h"
#include <QDebug>
#include <QTimer>
#include <QGraphicsView>
#include "monochain.h"
#include "doublechain.h"
#include <math.h>
#include <QKeyEvent>
#define absF(x) (x<0? -x : x)

DemoWidget::DemoWidget(QWidget *parent, unsigned int _plotCount) :
    QWidget(parent),
    ui(new Ui::DemoWidget)
{
    plotCount = _plotCount;
    currRate = 5.0;
    dotPos = currRate * 2;
    working = true;
    ui->setupUi(this);
    this->showFullScreen();
    omegaDot.resize(1);
    omegaDot[0] = 0;
    kDot.resize(1);
    kDot[0] = 0;
    omega1.resize(101);
    k1.resize(101);
    omega2.resize(101);
    k2.resize(101);
    kDot[0] = 0;
    omegaDot[0] = 0;


    ui->labelValues->hide();
    ui->labelW->hide();
    ui->labelK->hide();
    ui->resetButton->hide();
    ui->verticalSlider->setStyleSheet("background-color: white; color: black; border-style: outset;border-width: 10px;border-color: blue;");
    ui->horizontalSlider->setStyleSheet("background-color: white; color: black; border-style: outset;border-width: 10px;border-color: blue;");

    ui->labelNameWindow->move(this->width()/2-ui->labelNameWindow->width()/2,10);
    ui->plot->setGeometry(10,ui->labelNameWindow->y()+ui->labelNameWindow->height()+10, this->width()/2-10,this->height()*0.7);
    ui->back->move(this->width()-100,this->height()-80);
    ui->dispCurve->setGeometry(ui->plot->width()+ui->plot->x(),ui->plot->y(),
                               this->width()-(ui->plot->width()+ui->plot->x())-20, this->height()*0.7);

    ui->graphicsView->move(ui->graphicsView->x(),ui->plot->y()+ui->plot->height());
    ui->pushButtonPause->move(ui->plot->x(),ui->graphicsView->y()+ui->graphicsView->height()+15);
    ui->pushButtonSlower->move(ui->pushButtonPause->x()+ui->pushButtonPause->width()+20,ui->pushButtonPause->y());
    ui->pushButtonFaster->move(ui->pushButtonSlower->x()+ui->pushButtonSlower->width()+20,ui->pushButtonPause->y());
    ui->pushButtonMax->move(ui->pushButtonFaster->x()+ui->pushButtonFaster->width()+20,ui->pushButtonPause->y());
    ui->pushButtonMaxOptical->move(ui->pushButtonFaster->x()+ui->pushButtonFaster->width()+30,ui->pushButtonPause->y());
    ui->pushButtonMaxAcoustic->move(ui->pushButtonMaxOptical-> x()+ui->pushButtonMaxOptical->width()+30,ui->pushButtonPause->y());
    ui->pushButtonMinAcoustic->move(ui->pushButtonMaxAcoustic->x()+ui->pushButtonMaxAcoustic->width()+30,ui->pushButtonPause->y());
    ui->pushButtonMaxOptical->hide();
    ui->pushButtonMaxAcoustic->hide();
    ui->pushButtonMinAcoustic->hide();

    ui->labelOmega->move(ui->dispCurve->x()+60,ui->dispCurve->y()+ui->verticalSlider->height());
    ui->labelOmega->raise();
    ui->labelKValue->move(ui->dispCurve->x()+ui->dispCurve->width()/2+160,ui->dispCurve->y()+ui->dispCurve->height()-26);
    ui->labelKValue->raise();
    ui->verticalSlider->setGeometry(ui->dispCurve->x()-ui->verticalSlider->width()+ui->verticalSlider->width()+100,
                                    ui->dispCurve->y()+12,ui->verticalSlider->width(),ui->dispCurve->height()-62);
    ui->labelW0->move(ui->dispCurve->x()+110,ui->dispCurve->y()+ui->dispCurve->height()+ui->graphicsView->height());
    QObject::connect(ui->back, SIGNAL(clicked()), this, SLOT(back()));
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->dispCurve->addGraph();
    //--------------
    //=====PEN======
    //--------------
    blueDotPen.setColor(Qt::blue);
    blueDotPen.setWidthF(7);
    redDotPen.setColor(Qt::red);
    redDotPen.setWidthF(7);
    axisPen.setWidth(2);
    gridPen.setStyle(Qt::DashLine);
    gridPen.setWidthF(0.8);
    graphFont.setStyleHint(QFont::Times);
    graphFont.setPointSize(12);
    graphFont.setBold(true);

    // часть Новикова
    scene = new QGraphicsScene(ui->graphicsView);
    scene->setBackgroundBrush(QColor(43,46,87));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    backgroundSpringGradient = new QLinearGradient(0, 0, 5, 10);
    backgroundSpringGradient->setColorAt(0.3, QColor(53,56,97));

    springGradient = new QLinearGradient(0, 0, 5, 10);
    //springGradient->setColorAt(0, QColor(112,128,144));
    springGradient->setColorAt(0.3, QColor(240,255,255));
    //springGradient->setColorAt(0.7, QColor(112,128,144));
    //springGradient->setColorAt(1, QColor(105,105,105));
    backgroundMassGradient = new QRadialGradient(0,0, 43);
    backgroundMassGradient->setColorAt(0.6, Qt::gray);
    massGradient = new QRadialGradient(30, 15, 43);
    //massGradient->setColorAt(0.1, QColor(240,255,255) );
    massGradient->setColorAt(0.6, Qt::red);
    //massGradient->setColorAt(0.7, QColor(30,00,00) );

    massGradient2 = new QRadialGradient(30, 15, 40);
    //massGradient2->setColorAt(0.1, QColor(240,255,255) );
    massGradient2->setColorAt(0.6, Qt::blue);
    //massGradient2->setColorAt(0.7, QColor(00,00,30) );


    // конец части Новикова

    ui->dispCurve->graph(0)->setPen(QPen(Qt::red));
    ui->dispCurve->xAxis->grid()->setPen(gridPen);
    ui->dispCurve->yAxis->grid()->setPen(gridPen);
    ui->dispCurve->yAxis->setLabel(" \n\n\n");
    ui->dispCurve->xAxis->setLabel("Phase shift of neighboring masses: kh       ");
    ui->dispCurve->xAxis->setRange(0,M_PI);
    ui->dispCurve->yAxis->setRange(0,100);
    QVector<double> piTicks;
    QVector<QString> piLabels;
    piTicks << 0  << 0.5*M_PI << M_PI;
    piLabels << "0" << QString::fromUtf8("0.5π") << QString::fromUtf8("π");
    ui->dispCurve->xAxis->setAutoTicks(false);
    ui->dispCurve->xAxis->setAutoTickLabels(false);
    ui->dispCurve->xAxis->setTickVector(piTicks);
    ui->dispCurve->xAxis->setTickVectorLabels(piLabels);
    graphFont.setBold(false);
    graphFont.setPointSize(18);
    ui->dispCurve->yAxis->setLabelFont(graphFont);
    graphFont.setPointSize(12);
    ui->dispCurve->xAxis->setLabelFont(graphFont);

    QFont font("Times", 14);
    ui->dispCurve->xAxis->setTickLabelFont(font);
    ui->dispCurve->yAxis->setTickLabelFont(font);

    graphFont.setBold(true);
    ui->dispCurve->xAxis->setBasePen(axisPen);
    ui->dispCurve->yAxis->setBasePen(axisPen);
    ui->dispCurve->xAxis->setTickPen(axisPen);
    ui->dispCurve->yAxis->setTickPen(axisPen);
    ui->dispCurve->replot();

    // График смещений
    ui->plot->xAxis->grid()->setPen(gridPen);
    ui->plot->yAxis->setLabelFont(graphFont);
    ui->plot->xAxis->setLabelFont(graphFont);
    qDebug() << plotCount;
    ui->plot->xAxis->setRange(0, plotCount+=1);
    ui->plot->yAxis->setRange(-5000,5000);
    ui->plot->yAxis->setLabel("Coordinate");
    ui->plot->xAxis->setLabel("Mass number");
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
    ui->plot->graph(0)->setPen(redDotPen);
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
    ui->plot->graph(1)->setPen(blueDotPen);
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->xAxis->setAutoSubTicks(false);
    ui->plot->xAxis->setSubTickCount(0);
    ui->plot->yAxis->setTickLabels(false);
    ui->plot->yAxis->setTicks(false);

    ui->plot->xAxis->setTickLabelFont(graphFont);
    ui->plot->yAxis->setTickLabelFont(graphFont);
    ui->plot->xAxis->setBasePen(axisPen);
    ui->plot->yAxis->setBasePen(axisPen);
    ui->plot->xAxis->setTickPen(axisPen);
    ui->plot->yAxis->setTickPen(axisPen);
    ui->dispCurve->xAxis->setSubTickPen(axisPen);
    ui->dispCurve->yAxis->setSubTickPen(axisPen);
    QVector<double> myTicks(plotCount);
    for (unsigned int i = 0; i < plotCount; i++)
    {
        myTicks[i] = i + 1;
    }
    ui->plot->xAxis->setTickVector(myTicks);
    ui->plot->replot();
}

DemoWidget::~DemoWidget()
{
    delete ui;
}
//=================================================
//========SLOTS====================================
//=================================================

void DemoWidget::back()
{

}

void DemoWidget::pause()
{
    myChain->pause(working);
    working = !working;
    if (ui->pushButtonPause->text() == "Pause")
        ui->pushButtonPause->setText("Start");
    else
        ui->pushButtonPause->setText("Pause");
}

void DemoWidget::on_pushButtonFaster_clicked(){
    myChain->speedUp(1.4);
}

void DemoWidget::on_pushButtonSlower_clicked(){
    myChain->speedDown(1.4);
}

void DemoWidget::resizeEvent(QResizeEvent* event){
   ui->graphicsView->setGeometry(QRect(0, 650, this->width(), 70));
}


void DemoWidget::on_plot_destroyed()
{

}

void DemoWidget::on_pushButtonPause_clicked()
{
    pause();
}

void DemoWidget::rateDown()
{
    if (currRate > 0)
    {
        currRate -= 0.5;
        for (int i = 0; i < 101; ++i)
        {

            if (absF(omega1[i] - currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass()))) <= 0.5*ui->dispCurve->yAxis->range().upper/101)
            {
                kDot[0] = k1[i];
                omegaDot[0] = omega1[i];
            }
        }
        myChain->reinit(currRate);
        ui->dispCurve->replot();
        num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">%1</span></p></body></html>").arg(currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())));
        ui->labelOmega->setText(num);
        if (working)
            pause();
    }
}

void DemoWidget::rateUp()
{
    currRate += 0.5;
    for (int i = 0; i < 101; ++i)
    {
        if (absF(omega1[i] - currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass()))) <= 0.5*ui->dispCurve->yAxis->range().upper/101)
        {
            kDot[0] = k1[i];
            omegaDot[0] = omega1[i];
        }
    }

    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">%1</span></p></body></html>").arg(currRate/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())));
    ui->labelOmega->setText(num);
    if (working)
        pause();
}

void DemoWidget::on_pushButtonMax_clicked(){
    currRate = 15.4919;
    kDot[0] = k1[100];
    omegaDot[0] = omega1[100];
    ui->dispCurve->replot();
    myChain->reinit(currRate);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">ω = ωₒ</span></p></body></html>").arg(1);
    ui->labelOmega->setText(num);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\">= 3.14</span></p></body></html>");
    ui->labelKValue->setText(num);
    ui->verticalSlider->setSliderPosition(currRate/ui->dispCurve->yAxis->range().upper*100.0/(2*sqrt(myChain->getCoefficient()/(*myChain)[0]->getMass())));
    if (working)
        pause();
}

void DemoWidget::keyPressEvent(QKeyEvent * k)
{
    switch(k->key())
    {
    case (Qt::Key_Plus):
        rateUp();
        break;
    case (Qt::Key_Minus):
        rateDown();
        break;
    case (Qt::Key_P):
        pause();
        break;
    case (Qt::Key_M):
        on_pushButtonMax_clicked();
        break;
    }
}



void DemoWidget::time_slot()
{
    time();
}

