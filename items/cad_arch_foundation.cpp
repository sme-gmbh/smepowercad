#include "cad_arch_foundation.h"

CAD_arch_foundation::CAD_arch_foundation() : CADitem(CADitem::Arch_Foundation)
{
    this->description = "Architecture|Foundation";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_arch_foundation::~CAD_arch_foundation()
{

}

QList<CADitem::ItemType> CAD_arch_foundation::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Arch_Foundation);
    return flangable_items;
}

QImage CAD_arch_foundation::wizardImage()
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

void CAD_arch_foundation::calculate()
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

void CAD_arch_foundation::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}