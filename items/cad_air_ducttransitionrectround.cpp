#include "cad_air_ducttransitionrectround.h"

CAD_air_ductTransitionRectRound::CAD_air_ductTransitionRectRound() : CADitem(CADitem::Air_DuctTransitionRectRound)
{
    this->description = "Air|Duct transition rect/round";

}

QList<CADitem::ItemType> CAD_air_ductTransitionRectRound::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductTransitionRectRound::wizardImage()
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

void CAD_air_ductTransitionRectRound::calculate()
{

}

void CAD_air_ductTransitionRectRound::processWizardInput()
{

}
