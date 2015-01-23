#include "cad_air_equipmentframe.h"
#include "itemdb.h"

CAD_air_equipmentFrame::CAD_air_equipmentFrame() : CADitem(CADitemTypes::Air_EquipmentFrame)
{
    wizardParams.insert("Position x", (0.0));
    wizardParams.insert("Position y", (0.0));
    wizardParams.insert("Position z", (0.0));
    wizardParams.insert("Angle x", (0.0));
    wizardParams.insert("Angle y", (0.0));
    wizardParams.insert("Angle z", (0.0));

    processWizardInput();
    calculate();
}

CAD_air_equipmentFrame::~CAD_air_equipmentFrame()
{

}

QList<CADitemTypes::ItemType> CAD_air_equipmentFrame::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_equipmentFrame::wizardImage()
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

QString CAD_air_equipmentFrame::iconPath()
{
    return ":/icons/cad_air/cad_air_equipmentframe.svg";
}

QString CAD_air_equipmentFrame::domain()
{
    return "Air";
}

QString CAD_air_equipmentFrame::description()
{
    return "Air|Equipment frame";
}

void CAD_air_equipmentFrame::calculate()
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

void CAD_air_equipmentFrame::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}
