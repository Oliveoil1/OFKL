#include "ofkl.h"

#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <QtXml>
#include <paletteloader.h>
#include <QDir>
#include <QStandardPaths>

void createAppData()
{
    if(!QDir().exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
    {
        qInfo() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    if(!QFile().exists(QStandardPaths::locate(QStandardPaths::AppDataLocation, "aliases.csv")))
    {
        qInfo() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/aliases.csv";
        QFile aliases(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/aliases.csv");
        aliases.open(QFile::ReadWrite);
        aliases.write("alias,full_path,parameters");
        aliases.close();
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette p;

    p = paletteLoader().loadPalette(":/themes/dark.xml");

    a.setPalette(p);

    createAppData();

    Ofkl w;
    w.show();
    return a.exec();
}
