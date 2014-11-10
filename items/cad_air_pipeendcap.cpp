#include "cad_air_pipeendcap.h"

CAD_air_pipeEndCap::CAD_air_pipeEndCap() : CADitem(CADitem::Air_PipeEndCap)
{
    this->description = "Air|Pipe endcap";

}

QList<CADitem::ItemType> CAD_air_pipeEndCap::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pipeEndCap::wizardImage()
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

void CAD_air_pipeEndCap::calculate()
{

}

void CAD_air_pipeEndCap::processWizardInput()
{

}
