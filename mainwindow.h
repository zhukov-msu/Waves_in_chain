#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "demowidget.h"
#include "monochaindemo.h"
#include "doublechaindemo.h"
#include "wavetraindemo.h"
#include "authors.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MonoChainDemo * monochain_w;
    DoubleChainDemo * doublechain_w;
    WaveTrainDemo * trainchain_w;
    Authors * authors;

public slots:
    void quit();
private slots:
    void on_buttonPresentation1_clicked();
    void on_buttonPresentation2_clicked();
    void on_authors_clicked();
};

#endif // MAINWINDOW_H
