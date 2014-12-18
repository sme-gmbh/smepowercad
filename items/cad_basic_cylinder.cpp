#include "cad_basic_cylinder.h"

CAD_basic_cylinder::CAD_basic_cylinder() : CADitem(CADitem::Basic_Cylinder)
{
    this->description = "Basic|Cylinder";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Radius", QVariant::fromValue(1.0));
    wizardParams.insert("Height", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_cylinder::~CAD_basic_cylinder()
{

}

QList<CADitem::ItemType> CAD_basic_cylinder::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_cylinder::wizardImage()
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

void CAD_basic_cylinder::calculate()
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

    this->snap_basepoint = this->center_base;
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,0.0, this->height));

    vertices_bottom.clear();
    vertices_top.clear();
    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;
        linePos = center_base;

        linePos += matrix_rotation * QVector3D(sin(angle) * radius, cos(angle) * radius, 0.0);
        vertices_bottom.append(linePos);
        linePos += matrix_rotation * QVector3D(0.0,0.0, this->height);
        vertices_top.append(linePos);
    }
}

void CAD_basic_cylinder::processWizardInput()
{
    center_base.setX(wizardParams.value("Position x").toDouble());
    center_base.setY(wizardParams.value("Position y").toDouble());
    center_base.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    radius = wizardParams.value("Radius").toDouble();
    height = wizardParams.value("Height").toDouble();

}

void CAD_basic_cylinder::paint(GLWidget *glwidget)
{

}
