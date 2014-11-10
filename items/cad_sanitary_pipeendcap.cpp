#include "cad_sanitary_pipeendcap.h"

CAD_sanitary_pipeEndCap::CAD_sanitary_pipeEndCap() : CADitem(CADitem::Sanitary_PipeEndCap)
{
    this->description = "Sanitary|Pipe End Cap";

}

QList<CADitem::ItemType> CAD_sanitary_pipeEndCap::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_pipeEndCap::wizardImage()
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

void CAD_sanitary_pipeEndCap::calculate()
{

}

void CAD_sanitary_pipeEndCap::processWizardInput()
{

}
