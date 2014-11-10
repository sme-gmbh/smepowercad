#include "cad_sprinkler_pipe.h"

CAD_sprinkler_pipe::CAD_sprinkler_pipe() : CADitem(CADitem::Sprinkler_Pipe)
{
    this->description = "Sprinkler|Pipe";

}

QList<CADitem::ItemType> CAD_sprinkler_pipe::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_pipe::wizardImage()
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

void CAD_sprinkler_pipe::calculate()
{

}

void CAD_sprinkler_pipe::processWizardInput()
{

}
