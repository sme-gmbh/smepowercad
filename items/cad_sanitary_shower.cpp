#include "cad_sanitary_shower.h"

CAD_sanitary_shower::CAD_sanitary_shower() : CADitem(CADitem::Sanitary_Shower)
{
    this->description = "Sanitary|Shower";

}

QList<CADitem::ItemType> CAD_sanitary_shower::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_shower::wizardImage()
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

void CAD_sanitary_shower::calculate()
{

}

void CAD_sanitary_shower::processWizardInput()
{

}
