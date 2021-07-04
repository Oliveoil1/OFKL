#ifndef PALETTELOADER_H
#define PALETTELOADER_H

#include <QPalette>



class paletteLoader
{
public:
    paletteLoader();

    QPalette loadPalette(QString filePath);
};

#endif // PALETTELOADER_H
