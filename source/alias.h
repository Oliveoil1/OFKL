#ifndef ALIAS_H
#define ALIAS_H

#include <QString>



class Alias
{
public:
    Alias(QString _alias, QString _fullPath, QString _paramters, QString _fileType);
    QString alias;
    QString fullPath;
    QString parameters;
    QString fileType;
};

#endif // ALIAS_H
