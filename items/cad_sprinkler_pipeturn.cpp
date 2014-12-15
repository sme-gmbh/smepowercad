#include "cad_sprinkler_pipeturn.h"

CAD_sprinkler_pipeTurn::CAD_sprinkler_pipeTurn() : CADitem(CADitem::Sprinkler_PipeTurn)
{
    this->description = "Sprinkler|Pipe Turn";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_sprinkler_pipeTurn::~CAD_sprinkler_pipeTurn()
{

}

QList<CADitem::ItemType> CAD_sprinkler_pipeTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Sprinkler_CompressedAirWaterContainer);
    flangable_items.append(CADitem::Sprinkler_Distribution);
    flangable_items.append(CADitem::Sprinkler_Head);
    flangable_items.append(CADitem::Sprinkler_Pipe);
    flangable_items.append(CADitem::Sprinkler_PipeEndCap);
    flangable_items.append(CADitem::Sprinkler_PipeReducer);
    flangable_items.append(CADitem::Sprinkler_PipeTurn);
    flangable_items.append(CADitem::Sprinkler_Pump);
    flangable_items.append(CADitem::Sprinkler_TeeConnector);
    flangable_items.append(CADitem::Sprinkler_Valve);
    flangable_items.append(CADitem::Sprinkler_WetAlarmValve);
    flangable_items.append(CADitem::Sprinkler_ZoneCheck);

    return flangable_items;
}

QImage CAD_sprinkler_pipeTurn::wizardImage()
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

void CAD_sprinkler_pipeTurn::calculate()
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

void CAD_sprinkler_pipeTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}
