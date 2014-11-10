#include "cad_air_pipefiredamper.h"

CAD_air_pipeFireDamper::CAD_air_pipeFireDamper() : CADitem(CADitem::Air_PipeFireDamper)
{
    this->description = "Air|Pipe fire damper";

}

QList<CADitem::ItemType> CAD_air_pipeFireDamper::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pipeFireDamper::wizardImage()
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

void CAD_air_pipeFireDamper::calculate()
{

}

void CAD_air_pipeFireDamper::processWizardInput()
{

}
