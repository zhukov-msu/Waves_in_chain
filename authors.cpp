#include "authors.h"
#include "ui_authors.h"

Authors::Authors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authors)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    ui->title->move(this->width()/2-ui->title->width()/2,10);
    ui->labelPhys->move(this->width()-ui->labelCmc->width()-10,10);
    ui->labelCmc->move(10,10);
    ui->labelFacultyLeft->move(ui->labelCmc->x()+10,ui->labelCmc->y()+ui->labelCmc->height()+10);
    ui->labelFacultyRight->move(ui->labelPhys->x()+10,ui->labelPhys->y()+ui->labelPhys->height()+10);
    ui->widget->move(this->width()/2-ui->widget->width()/2,ui->title->y()+170);
    ui->year->move(this->width()/2-ui->year->width()/2,this->height()-50);
    ui->back->move(this->width()-100,this->height()-80);
}

Authors::~Authors()
{
    delete ui;
}

void Authors::on_back_clicked()
{
    delete this;
}
