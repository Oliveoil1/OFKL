#include "options.h"
#include "ui_options.h"

#include <QDesktopServices>
#include <QDir>
#include <QStandardPaths>

Options::Options(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::on_actionOpen_Data_Location_triggered()
{
    QDesktopServices::openUrl(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}
