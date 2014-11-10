#include "cad_sanitary_washbasin.h"

CAD_sanitary_washBasin::CAD_sanitary_washBasin() : CADitem(CADitem::Sanitary_WashBasin)
{
    this->description = "Sanitary|Wash Basin";

}

QList<CADitem::ItemType> CAD_sanitary_washBasin::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_washBasin::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_sanitary_washBasin::calculate()
{

}

void CAD_sanitary_washBasin::processWizardInput()
{

}
