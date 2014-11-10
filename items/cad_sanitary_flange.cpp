#include "cad_sanitary_flange.h"

CAD_sanitary_flange::CAD_sanitary_flange() : CADitem(CADitem::Sanitary_Flange)
{
    this->description = "Sanitary|Flange";

}

QList<CADitem::ItemType> CAD_sanitary_flange::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_flange::wizardImage()
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

void CAD_sanitary_flange::calculate()
{

}

void CAD_sanitary_flange::processWizardInput()
{

}
