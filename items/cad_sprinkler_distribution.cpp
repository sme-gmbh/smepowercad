#include "cad_sprinkler_distribution.h"

CAD_sprinkler_distribution::CAD_sprinkler_distribution() : CADitem(CADitem::Sprinkler_Distribution)
{
    this->description = "Sprinkler|Distribution";

}

QList<CADitem::ItemType> CAD_sprinkler_distribution::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_distribution::wizardImage()
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

void CAD_sprinkler_distribution::calculate()
{

}

void CAD_sprinkler_distribution::processWizardInput()
{

}
