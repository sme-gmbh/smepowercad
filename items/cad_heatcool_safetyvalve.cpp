#include "cad_heatcool_safetyvalve.h"

CAD_heatcool_safetyValve::CAD_heatcool_safetyValve() : CADitem(CADitem::HeatCool_SafetyValve)
{
    this->description = "Heat/Cool|Safety valve";

}

QList<CADitem::ItemType> CAD_heatcool_safetyValve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_safetyValve::wizardImage()
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

void CAD_heatcool_safetyValve::calculate()
{

}

void CAD_heatcool_safetyValve::processWizardInput()
{

}
