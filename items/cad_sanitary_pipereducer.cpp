#include "cad_sanitary_pipereducer.h"

CAD_sanitary_pipeReducer::CAD_sanitary_pipeReducer() : CADitem(CADitem::Sanitary_PipeReducer)
{
    this->description = "Sanitary|Pipe Reducer";

}

QList<CADitem::ItemType> CAD_sanitary_pipeReducer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_pipeReducer::wizardImage()
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

void CAD_sanitary_pipeReducer::calculate()
{

}

void CAD_sanitary_pipeReducer::processWizardInput()
{

}
