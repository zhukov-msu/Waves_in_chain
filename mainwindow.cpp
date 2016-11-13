#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include "demowidget.h"
#include "monochaindemo.h"
#include "authors.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    qDebug() << ui->buttonPresentation1->height();
    this->showFullScreen();
    ui->buttonPresentation1->move(this->width()/2-ui->buttonPresentation1->width()/2,
                                  this->height()/2-(ui->buttonPresentation1->height()*1.5+30));
    ui->pic1->move(ui->buttonPresentation1->x(),ui->buttonPresentation1->y()+ui->buttonPresentation1->height());
    ui->buttonPresentation2->move(ui->buttonPresentation1->x(),ui->pic1->y()+ui->pic1->height()+20);
    ui->pic2->move(ui->buttonPresentation2->x(),ui->buttonPresentation2->y()+ui->buttonPresentation2->height());
    ui->quit->move(this->width()-100,this->height()-80);
    ui->authors->move(100-ui->authors->width(),this->height()-80);
    ui->title->move(this->width()/2-ui->title->width()/2,10);
    ui->labelTitle->move(this->width()/2-ui->labelTitle->width()/2,ui->title->y()+ui->title->height());
    ui->labelPhys->move(this->width()-ui->labelCmc->width()-10,10);
    ui->labelCmc->move(10,10);
    ui->labelFacultyLeft->move(ui->labelCmc->x()+10,ui->labelCmc->y()+ui->labelCmc->height()+10);
    ui->labelFacultyRight->move(ui->labelPhys->x()-10,ui->labelPhys->y()+ui->labelPhys->height()+10);
    ui->year->move(this->width()/2-ui->year->width()/2,this->height()-50);
    QObject::connect(ui->quit, SIGNAL(clicked()), this, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------
//---------------SLOTS---------------------
//-----------------------------------------

void MainWindow::quit()
{
    qApp->quit();
}


void MainWindow::on_buttonPresentation1_clicked()
{
    monochain_w = new MonoChainDemo();
    monochain_w->show();
    monochain_w->raise();
}

void MainWindow::on_buttonPresentation2_clicked()
{
    doublechain_w = new DoubleChainDemo();
    doublechain_w->show();
    doublechain_w->raise();
}


void MainWindow::on_authors_clicked()
{
    authors = new Authors();
    authors->setWindowTitle("Авторы");
    authors->show();
}
