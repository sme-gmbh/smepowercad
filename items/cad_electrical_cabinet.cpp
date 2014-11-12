#include "cad_electrical_cabinet.h"

CAD_electrical_cabinet::CAD_electrical_cabinet() : CADitem(CADitem::Electrical_Cabinet)
{
    this->description = "Electrical|Cabinet";

}

QList<CADitem::ItemType> CAD_electrical_cabinet::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Electrical_Cabinet);
    return flangable_items;
}

QImage CAD_electrical_cabinet::wizardImage()
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

void CAD_electrical_cabinet::calculate()
{

}

void CAD_electrical_cabinet::processWizardInput()
{

}
