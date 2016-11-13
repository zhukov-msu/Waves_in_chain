#include "wavetraindemo.h"
#include "demowidget.h"
#include "wavetrainchain.h"
#include "ui_demowidget.h"

WaveTrainDemo::WaveTrainDemo(QWidget *parent, unsigned int pc) :
    DemoWidget(parent,pc)
{
    ui->labelNameWindow->setText("Неоднородная цепочка");
    ui->labelNameWindow->setGeometry(ui->labelNameWindow->x(),ui->labelNameWindow->y(),330,ui->labelNameWindow->height());
    ui->labelNameWindow->move(this->width()/2-ui->labelNameWindow->width()/2,10);

    ui->plot->setGeometry(10,ui->plot->y(), this->width()*0.98,this->height()*0.7);

    ui->graphicsView->move(ui->graphicsView->x(),ui->plot->y()+ui->plot->height()+30);
    ui->pushButtonPause->move(ui->plot->x(),ui->graphicsView->y()+ui->graphicsView->height()+30);
    ui->pushButtonSlower->move(ui->pushButtonPause->x()+ui->pushButtonPause->width()+30,ui->pushButtonPause->y());
    ui->pushButtonFaster->move(ui->pushButtonSlower->x()+ui->pushButtonSlower->width()+30,ui->pushButtonPause->y());
    ui->pushButtonMax->move(ui->pushButtonFaster->x()+ui->pushButtonFaster->width()+30,ui->pushButtonPause->y());
    ui->resetButton->move(ui->pushButtonFaster->x()+ui->pushButtonFaster->width()+30,ui->pushButtonPause->y());
    ui->horizontalSlider->move(ui->resetButton->x()+ui->resetButton->width()+20,ui->resetButton->y());
    ui->labelMass->move(ui->horizontalSlider->x(),ui->horizontalSlider->y()+ui->horizontalSlider->height()+10);
    ui->labelMassValue->move(ui->labelMass->x()+ui->labelMass->width()+10,ui->labelMass->y()+3);
    ui->horizontalSlider->setMaximum(8);
    ui->horizontalSlider->setMinimum(0);
    ui->pushButtonMax->hide();
    ui->verticalSlider->hide();
    ui->labelW0->hide();
    ui->labelKValue->hide();
    ui->labelOmega->hide();
    ui->resetButton->show();
    setting = new chainSettings(90, 3000, 30, 8000/150*90, 30);
    qDebug() << ui->horizontalSlider->maximum();
    qDebug() << ui->horizontalSlider->minimum();
    ui->horizontalSlider->setSliderPosition(ui->horizontalSlider->minimum()+(ui->horizontalSlider->maximum()-ui->horizontalSlider->minimum())/2);
    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> 1</span></p></body></html>");
    ui->labelMassValue->setText(num);
    myChain = new WaveTrainChain(*setting, *springGradient, *massGradient, *massGradient2, 0.05, 0.05);

    backgroundChain = new WaveTrainChain(*setting, *backgroundSpringGradient, *backgroundMassGradient,*backgroundMassGradient,m1,m2);

    scene->addItem(backgroundChain->drawAt(0,0));
    backgroundChain->pause(true);

    scene->addItem(myChain -> drawAt(0,0));
    pause();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)), this, SLOT(on_horisontalSlider_sliderMoved(int)));

    ui->plot->xAxis->setRange(0,plotCount);
    ui->plot->xAxis2->setRange(0,plotCount);

    ui->dispCurve->hide();

    QObject::connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(on_resetButton_clicked()));
}


void WaveTrainDemo::on_resetButton_clicked(){
    myChain->pause(true);
    working = false;
    myChain->reinit(currRate);

    ui->pushButtonPause->setText("Старт");
    time();
}

void WaveTrainDemo::time(){
    QVector<double> offset1(plotCount/2), x1(plotCount/2);
    QVector<double> offset2(plotCount/2), x2(plotCount/2);


    double* offsets = myChain->getMassOffsets();
    int j = 0;
    for (int i = 0; i < plotCount/2; i++, j++)
    {
        offset1[j] = offsets[i+
                myChain->getCount()/2-plotCount/2];
        x1[j] = i + 1;
    }
    ui->plot->graph(0)->setData(x1,offset1);
    j = 0;
    for (int i = 0; i < plotCount/2; i++, j++)
    {
        offset2[j] = offsets[i+myChain->getCount()/2];
        x2[j] = i + plotCount/2 + 1;
    }
    ui->plot->graph(1)->setData(x2,offset2);
    delete[] offsets;

    ui->plot->replot();
}

void WaveTrainDemo::on_horisontalSlider_sliderMoved(int value)
{
    m1 = 0.05;
    float s = (ui->horizontalSlider->maximum()-2*value);
    m2 = m1*pow(2,s/8);
    delete myChain;
    delete backgroundChain;

    myChain = new WaveTrainChain(*setting,*springGradient, *massGradient, *massGradient2, m1, m2);
    backgroundChain = new WaveTrainChain(*setting, *backgroundSpringGradient, *backgroundMassGradient, *backgroundMassGradient, m1, m2);
    scene->addItem(backgroundChain->drawAt(0,0));
    scene->addItem(myChain->drawAt(0,0));
    myChain->pause(true);
    backgroundChain->pause(true);
    working = false;
    ui->pushButtonPause->setText("Старт");
    time();
    connect(myChain->getTimer(), SIGNAL(timeout()), this, SLOT(time_slot()));


    num = QString("<html><head/><body><p><span style=\" font-size:12pt;\"> %1</span></p></body></html>").arg(m1/m2,0,'g',2);
    ui->labelMassValue->setText(num);
}
