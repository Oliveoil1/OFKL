#include "paletteloader.h"

#include <QFile>
#include <QString>
#include <QtXml>

paletteLoader::paletteLoader()
{

}

QPalette paletteLoader::loadPalette(QString filePath)
{
    QDomDocument xmlBom;
    QFile file(filePath);

    file.open(QIODevice::ReadOnly);
    xmlBom.setContent(&file);

    QDomElement root = xmlBom.documentElement();
    QString type = root.tagName();

    QDomElement component = root.firstChild().toElement(); //active, inactive, disabled

    QPalette newPalette;

    while(!component.isNull())
    {
        QDomElement colourRole = component.firstChild().toElement();

        while(!colourRole.isNull())
        {
            QString roleStr = colourRole.attribute("role");
            QString brushStyleStr;
            int colourR;
            int colourG;
            int colourB;

            QDomElement brush = colourRole.firstChild().toElement();
            brushStyleStr = brush.attribute("brushsstyle");

            QDomElement colour = brush.firstChild().toElement();
            QDomElement colourChild = colour.firstChild().toElement();



            while(!colourChild.isNull())
            {
                if(colourChild.tagName() == "red")
                {
                    colourR = colourChild.firstChild().toText().data().toInt();
                }
                if(colourChild.tagName() == "blue")
                {
                    colourB = colourChild.firstChild().toText().data().toInt();
                }
                if(colourChild.tagName() == "green")
                {
                    colourG = colourChild.firstChild().toText().data().toInt();
                }

                colourChild = colourChild.nextSibling().toElement();
            }

            QPalette::ColorGroup currentGroup;
            QPalette::ColorRole currentRole;

            if(component.tagName() == "active")
            {
                currentGroup = QPalette::Active;
            }
            if(component.tagName() == "inactive")
            {
                currentGroup = QPalette::Inactive;
            }
            if(component.tagName() == "disabled")
            {
                currentGroup = QPalette::Disabled;
            }

            if(roleStr == "WindowText")
            {
                currentRole = QPalette::WindowText;
            }
            if(roleStr == "Button")
            {
                currentRole = QPalette::Button;
            }
            if(roleStr == "Light")
            {
                currentRole = QPalette::Light;
            }
            if(roleStr == "Midlight")
            {
                currentRole = QPalette::Midlight;
            }
            if(roleStr == "Mid")
            {
                currentRole = QPalette::Mid;
            }
            if(roleStr == "Text")
            {
                currentRole = QPalette::Text;
            }
            if(roleStr == "BrightText")
            {
                currentRole = QPalette::BrightText;
            }
            if(roleStr == "ButtonText")
            {
                currentRole = QPalette::ButtonText;
            }
            if(roleStr == "Light")
            {
                currentRole = QPalette::Light;
            }
            if(roleStr == "Base")
            {
                currentRole = QPalette::Base;
            }
            if(roleStr == "Window")
            {
                currentRole = QPalette::Window;
            }
            if(roleStr == "Shadow")
            {
                currentRole = QPalette::Shadow;
            }
            if(roleStr == "AlternateBase")
            {
                currentRole = QPalette::AlternateBase;
            }
            if(roleStr == "ToolTipBase")
            {
                currentRole = QPalette::ToolTipBase;
            }
            if(roleStr == "ToolTipText")
            {
                currentRole = QPalette::ToolTipText;
            }
            if(roleStr == "PlaceholderText")
            {
                currentRole = QPalette::PlaceholderText;
            }

            newPalette.setColor(currentGroup, currentRole, QColor(colourR, colourG, colourB));

            colourRole = colourRole.nextSibling().toElement();
        }

        component = component.nextSibling().toElement();
    }

    return newPalette;
}
