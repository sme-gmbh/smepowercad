#include "cad_sanitary_sink.h"

CAD_sanitary_sink::CAD_sanitary_sink() : CADitem(CADitem::Sanitary_Sink)
{
    this->description = "Sanitary|Sink";

}

QList<CADitem::ItemType> CAD_sanitary_sink::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_sink::wizardImage()
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

void CAD_sanitary_sink::calculate()
{

}

void CAD_sanitary_sink::processWizardInput()
{

}
