#ifndef OPTIONS_H
#define OPTIONS_H

#include <QMainWindow>

namespace Ui {
class Options;
}

class Options : public QMainWindow
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

private:
    Ui::Options *ui;

private slots:
    void on_actionOpen_Data_Location_triggered();
};

#endif // OPTIONS_H
