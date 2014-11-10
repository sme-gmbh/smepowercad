#include "cad_sanitary_emergencyshower.h"

CAD_sanitary_emergencyShower::CAD_sanitary_emergencyShower() : CADitem(CADitem::Sanitary_EmergencyShower)
{
    this->description = "Sanitary|Emergency Shower";

}

QList<CADitem::ItemType> CAD_sanitary_emergencyShower::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_emergencyShower::wizardImage()
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

void CAD_sanitary_emergencyShower::calculate()
{

}

void CAD_sanitary_emergencyShower::processWizardInput()
{

}
