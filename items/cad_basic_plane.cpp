#include "cad_basic_plane.h"

CAD_basic_plane::CAD_basic_plane() : CADitem(CADitem::Basic_Plane)
{
    this->description = "Basic|Plane";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Length (A)", QVariant::fromValue(10.0));
    wizardParams.insert("Width (B)", QVariant::fromValue(10.0));


    processWizardInput();
    calculate();
}

CAD_basic_plane::~CAD_basic_plane()
{

}

QList<CADitem::ItemType> CAD_basic_plane::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_plane::wizardImage()
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

void CAD_basic_plane::calculate()
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
    vertices[0] = position + matrix_rotation * QVector3D(a, 0.0, 0.0);
    vertices[1] = position + matrix_rotation * QVector3D(a, b, 0.0);
    vertices[2] = position + matrix_rotation * QVector3D(0.0,  b, 0.0);
    vertices[3] = position + matrix_rotation * QVector3D(0.0,  0.0, 0.0);

    boundingBox.enterVertex(vertices[0]);
    boundingBox.enterVertex(vertices[1]);
    boundingBox.enterVertex(vertices[2]);
    boundingBox.enterVertex(vertices[3]);

    this->snap_vertices.append(vertices[0]);
    this->snap_vertices.append(vertices[1]);
    this->snap_vertices.append(vertices[2]);
    this->snap_vertices.append(vertices[3]);
}

void CAD_basic_plane::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    a = wizardParams.value("Length (A)").toDouble();
    b = wizardParams.value("Width (B)").toDouble();

}

void CAD_basic_plane::paint(GLWidget *glwidget)
{

}
