#include "cad_sanitary_liftingunit.h"

CAD_sanitary_liftingUnit::CAD_sanitary_liftingUnit() : CADitem(CADitem::Sanitary_LiftingUnit)
{
    this->description = "Sanitary|Lifting Unit";

}

QList<CADitem::ItemType> CAD_sanitary_liftingUnit::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_liftingUnit::wizardImage()
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

void CAD_sanitary_liftingUnit::calculate()
{

}

void CAD_sanitary_liftingUnit::processWizardInput()
{

}
