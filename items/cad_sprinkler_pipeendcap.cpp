#include "cad_sprinkler_pipeendcap.h"

CAD_sprinkler_pipeEndCap::CAD_sprinkler_pipeEndCap() : CADitem(CADitem::Sprinkler_PipeEndCap)
{
    this->description = "Sprinkler|Pipe End Cap";

}

QList<CADitem::ItemType> CAD_sprinkler_pipeEndCap::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_pipeEndCap::wizardImage()
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

void CAD_sprinkler_pipeEndCap::calculate()
{

}

void CAD_sprinkler_pipeEndCap::processWizardInput()
{

}
