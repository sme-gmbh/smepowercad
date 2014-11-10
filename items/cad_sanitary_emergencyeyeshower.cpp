#include "cad_sanitary_emergencyeyeshower.h"

CAD_sanitary_emergencyEyeShower::CAD_sanitary_emergencyEyeShower() : CADitem(CADitem::Sanitary_EmergencyEyeShower)
{
    this->description = "Sanitary|Emergency Eye Shower";

}

QList<CADitem::ItemType> CAD_sanitary_emergencyEyeShower::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_emergencyEyeShower::wizardImage()
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

void CAD_sanitary_emergencyEyeShower::calculate()
{

}

void CAD_sanitary_emergencyEyeShower::processWizardInput()
{

}
