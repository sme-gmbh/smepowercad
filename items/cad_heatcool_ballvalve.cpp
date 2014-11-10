#include "cad_heatcool_ballvalve.h"

CAD_heatcool_ballValve::CAD_heatcool_ballValve() : CADitem(CADitem::HeatCool_BallValve)
{
    this->description = "Heat/Cool|Ballvalve";

}

QList<CADitem::ItemType> CAD_heatcool_ballValve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_ballValve::wizardImage()
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

void CAD_heatcool_ballValve::calculate()
{

}

void CAD_heatcool_ballValve::processWizardInput()
{

}
