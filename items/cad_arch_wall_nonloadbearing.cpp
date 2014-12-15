#include "cad_arch_wall_nonloadbearing.h"

CAD_arch_wall_nonLoadBearing::CAD_arch_wall_nonLoadBearing() : CADitem(CADitem::Arch_Wall_nonLoadBearing)
{
    this->description = "Architecture|Wall non load bearing";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("b", QVariant::fromValue(200.0));
    wizardParams.insert("l", QVariant::fromValue(10000.0));
    wizardParams.insert("a", QVariant::fromValue(3000.0));

    basic_box = new CAD_basic_box();
    subItems.append(basic_box);

    processWizardInput();
    calculate();
}

CAD_arch_wall_nonLoadBearing::~CAD_arch_wall_nonLoadBearing()
{

}

QList<CADitem::ItemType> CAD_arch_wall_nonLoadBearing::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Arch_Wall_loadBearing);
    flangable_items.append(CADitem::Arch_Wall_nonLoadBearing);
    return flangable_items;
}

QImage CAD_arch_wall_nonLoadBearing::wizardImage()
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

void CAD_arch_wall_nonLoadBearing::calculate()
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

    QVector3D center = (matrix_rotation * QVector3D(l, b, a) / 2.0) + position;
    basic_box->wizardParams.insert("Position x", QVariant::fromValue(center.x()));
    basic_box->wizardParams.insert("Position y", QVariant::fromValue(center.y()));
    basic_box->wizardParams.insert("Position z", QVariant::fromValue(center.z()));
    basic_box->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    basic_box->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    basic_box->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    basic_box->wizardParams.insert("Size x", QVariant::fromValue(l));
    basic_box->wizardParams.insert("Size y", QVariant::fromValue(b));
    basic_box->wizardParams.insert("Size z", QVariant::fromValue(a));
    basic_box->processWizardInput();
    basic_box->calculate();

    this->snap_vertices.append(basic_box->snap_vertices);
    this->snap_center.append(basic_box->snap_center);
    this->snap_flanges.append(basic_box->pos_bot_1);
    this->snap_flanges.append(basic_box->pos_bot_2);
    this->snap_flanges.append(basic_box->pos_bot_3);
    this->snap_flanges.append(basic_box->pos_bot_4);


    boundingBox = basic_box->boundingBox;
}

void CAD_arch_wall_nonLoadBearing::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    a = wizardParams.value("a").toDouble();

}
