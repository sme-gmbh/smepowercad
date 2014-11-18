#include "cad_basic_duct.h"

CAD_basic_duct::CAD_basic_duct() : CADitem(CADitem::Basic_Duct)
{
    this->description = "Basic|Duct";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));


    wizardParams.insert("Wall thickness", QVariant::fromValue(1.0));
    wizardParams.insert("Length (l)", QVariant::fromValue(10.0));
    wizardParams.insert("Width (b)", QVariant::fromValue(5.0));
    wizardParams.insert("Height (a)", QVariant::fromValue(5.0));



    processWizardInput();
    calculate();
}

CAD_basic_duct::~CAD_basic_duct()
{
}

QList<CADitem::ItemType> CAD_basic_duct::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Basic_Duct);
    return flangable_items;
}

QImage CAD_basic_duct::wizardImage()
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

void CAD_basic_duct::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.translate(position);
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    matrix_rotation.translate(-position);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pos_bot_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);

    pos_top_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);

    inner_pos_bot_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0, 1.0, 1.0) * wall_thickness + position);
    inner_pos_bot_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0, 1.0, 1.0) * wall_thickness + position);
    inner_pos_bot_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0,-1.0, 1.0) * wall_thickness + position);
    inner_pos_bot_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0,-1.0, 1.0) * wall_thickness + position);

    inner_pos_top_1 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0, 1.0,-1.0) * wall_thickness + position);
    inner_pos_top_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0, 1.0,-1.0) * wall_thickness + position);
    inner_pos_top_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0,-1.0,-1.0) * wall_thickness + position);
    inner_pos_top_4 = matrix_rotation * (QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0,-1.0,-1.0) * wall_thickness + position);



    boundingBox.enterVertex(pos_bot_1);
    boundingBox.enterVertex(pos_bot_2);
    boundingBox.enterVertex(pos_bot_3);
    boundingBox.enterVertex(pos_bot_4);
    boundingBox.enterVertex(pos_top_1);
    boundingBox.enterVertex(pos_top_2);
    boundingBox.enterVertex(pos_top_3);
    boundingBox.enterVertex(pos_top_4);





    this->snap_flanges.append((pos_bot_1 + pos_top_4) / 2);
    this->snap_flanges.append((pos_bot_2 + pos_top_3) / 2);

    this->snap_vertices.append(pos_top_1);
    this->snap_vertices.append(pos_top_2);
    this->snap_vertices.append(pos_top_3);
    this->snap_vertices.append(pos_top_4);
    this->snap_vertices.append(pos_bot_1);
    this->snap_vertices.append(pos_bot_2);
    this->snap_vertices.append(pos_bot_3);
    this->snap_vertices.append(pos_bot_4);


}

void CAD_basic_duct::processWizardInput()
{

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    size.setX(wizardParams.value("Length (l)").toDouble());
    size.setY(wizardParams.value("Width (b)").toDouble());
    size.setZ(wizardParams.value("Height (a)").toDouble());


    wall_thickness = wizardParams.value("Wall thickness").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}
