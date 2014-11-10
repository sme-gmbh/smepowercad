#include "cad_sprinkler_head.h"

CAD_sprinkler_head::CAD_sprinkler_head() : CADitem(CADitem::Sprinkler_Head)
{
    this->description = "Sprinkler|Head";

}

QList<CADitem::ItemType> CAD_sprinkler_head::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_head::wizardImage()
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

void CAD_sprinkler_head::calculate()
{

}

void CAD_sprinkler_head::processWizardInput()
{

}
