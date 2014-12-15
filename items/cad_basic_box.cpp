#include "cad_basic_box.h"

CAD_basic_box::CAD_basic_box() : CADitem(CADitem::Basic_Box)
{
    this->description = "Basic|Box";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Size x", QVariant::fromValue(1.0));
    wizardParams.insert("Size y", QVariant::fromValue(1.0));
    wizardParams.insert("Size z", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_box::~CAD_basic_box()
{

}

QList<CADitem::ItemType> CAD_basic_box::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_box::wizardImage()
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

void CAD_basic_box::calculate()
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

    this->snap_vertices.append(pos_bot_1);
    this->snap_vertices.append(pos_bot_2);
    this->snap_vertices.append(pos_bot_3);
    this->snap_vertices.append(pos_bot_4);
    this->snap_vertices.append(pos_top_1);
    this->snap_vertices.append(pos_top_2);
    this->snap_vertices.append(pos_top_3);
    this->snap_vertices.append(pos_top_4);

    // Bottom edge centers
    this->snap_center.append((pos_bot_1 + pos_bot_2) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_bot_3) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_bot_4) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_bot_1) / 2.0);
    // Top edge centers
    this->snap_center.append((pos_top_1 + pos_top_2) / 2.0);
    this->snap_center.append((pos_top_2 + pos_top_3) / 2.0);
    this->snap_center.append((pos_top_3 + pos_top_4) / 2.0);
    this->snap_center.append((pos_top_4 + pos_top_1) / 2.0);
    // Connection edge centers
    this->snap_center.append((pos_bot_1 + pos_top_1) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_top_2) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_top_3) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_top_4) / 2.0);

    // Calculate face normal vectors
    this->normal_bot   = (pos_bot_1 - pos_top_1).normalized();
    this->normal_top   = (pos_top_1 - pos_bot_1).normalized();
    this->normal_left  = (pos_bot_1 - pos_bot_2).normalized();
    this->normal_right = (pos_bot_2 - pos_bot_1).normalized();
    this->normal_front = (pos_bot_2 - pos_bot_3).normalized();
    this->normal_back  = (pos_bot_3 - pos_bot_2).normalized();
}

void CAD_basic_box::processWizardInput()
{
    QVector3D size;

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    size.setX(wizardParams.value("Size x").toDouble());
    size.setY(wizardParams.value("Size y").toDouble());
    size.setZ(wizardParams.value("Size z").toDouble());

    pos_bot_1 = position;
    pos_bot_2 = position;
    pos_bot_3 = position;
    pos_bot_4 = position;
    pos_top_1 = position;
    pos_top_2 = position;
    pos_top_3 = position;
    pos_top_4 = position;

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    pos_bot_1 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_2 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_3 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_4 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_top_1 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_2 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_3 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_4 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());

}
