#include "cad_sanitary_pipeturn.h"

CAD_sanitary_pipeTurn::CAD_sanitary_pipeTurn() : CADitem(CADitem::Sanitary_PipeTurn)
{
    this->description = "Sanitary|Pipe Turn";

}

QList<CADitem::ItemType> CAD_sanitary_pipeTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_pipeTurn::wizardImage()
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

void CAD_sanitary_pipeTurn::calculate()
{

}

void CAD_sanitary_pipeTurn::processWizardInput()
{

}
