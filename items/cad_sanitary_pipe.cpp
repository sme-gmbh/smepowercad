#include "cad_sanitary_pipe.h"

CAD_sanitary_pipe::CAD_sanitary_pipe() : CADitem(CADitem::Sanitary_Pipe)
{
    this->description = "Sanitary|Pipe";

}

QList<CADitem::ItemType> CAD_sanitary_pipe::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_pipe::wizardImage()
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

void CAD_sanitary_pipe::calculate()
{

}

void CAD_sanitary_pipe::processWizardInput()
{

}
