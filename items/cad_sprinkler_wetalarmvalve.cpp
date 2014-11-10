#include "cad_sprinkler_wetalarmvalve.h"

CAD_sprinkler_wetAlarmValve::CAD_sprinkler_wetAlarmValve() : CADitem(CADitem::Sprinkler_WetAlarmValve)
{
    this->description = "Sprinkler|Wet alarm valve";

}

QList<CADitem::ItemType> CAD_sprinkler_wetAlarmValve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_wetAlarmValve::wizardImage()
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

void CAD_sprinkler_wetAlarmValve::calculate()
{

}

void CAD_sprinkler_wetAlarmValve::processWizardInput()
{

}
