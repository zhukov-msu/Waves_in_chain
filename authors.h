#ifndef AUTHORS_H
#define AUTHORS_H

#include <QWidget>

namespace Ui {
class Authors;
}

class Authors : public QWidget
{
    Q_OBJECT

public:
    explicit Authors(QWidget *parent = 0);
    ~Authors();

private slots:
    void on_back_clicked();

private:
    Ui::Authors *ui;
};

#endif // AUTHORS_H
