#include "cad_air_pipe.h"

CAD_air_pipe::CAD_air_pipe() : CADitem(CADitem::Air_Pipe)
{
    this->description = "Air|Pipe";

}

QList<CADitem::ItemType> CAD_air_pipe::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pipe::wizardImage()
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

void CAD_air_pipe::calculate()
{

}

void CAD_air_pipe::processWizardInput()
{

}
