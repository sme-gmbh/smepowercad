#include "cad_sanitary_sink.h"

CAD_sanitary_sink::CAD_sanitary_sink() : CADitem(CADitem::Sanitary_Sink)
{
    this->description = "Sanitary|Sink";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_sanitary_sink::~CAD_sanitary_sink()
{

}

QList<CADitem::ItemType> CAD_sanitary_sink::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Sanitary_ElectricWaterHeater);
    flangable_items.append(CADitem::Sanitary_EmergencyEyeShower);
    flangable_items.append(CADitem::Sanitary_EmergencyShower);
    flangable_items.append(CADitem::Sanitary_Flange);
    flangable_items.append(CADitem::Sanitary_LiftingUnit);
    flangable_items.append(CADitem::Sanitary_Pipe);
    flangable_items.append(CADitem::Sanitary_PipeEndCap);
    flangable_items.append(CADitem::Sanitary_PipeReducer);
    flangable_items.append(CADitem::Sanitary_PipeTeeConnector);
    flangable_items.append(CADitem::Sanitary_PipeTurn);
    flangable_items.append(CADitem::Sanitary_Shower);
    flangable_items.append(CADitem::Sanitary_Sink);
    flangable_items.append(CADitem::Sanitary_WashBasin);

    return flangable_items;
}

QImage CAD_sanitary_sink::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_sanitary_sink::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
}

void CAD_sanitary_sink::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}
