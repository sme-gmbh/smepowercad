#include "cad_basic_pipe.h"

CAD_basic_pipe::CAD_basic_pipe() : CADitem(CADitem::Basic_Pipe)
{
    vertices_inner_bottom = QList<QVector3D>();
    vertices_outer_bottom = QList<QVector3D>();
    vertices_outer_top = QList<QVector3D>();
    vertices_inner_top = QList<QVector3D>();

    this->description = "Basic|Pipe";
    radius = 1.0;

    length = 20.0;

    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("l", QVariant::fromValue(20.0));
    wizardParams.insert("d", QVariant::fromValue(10.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_pipe::~CAD_basic_pipe()
{

}

QList<CADitem::ItemType> CAD_basic_pipe::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Basic_Pipe);
    flangable_items.append(CADitem::Basic_Turn);
    return flangable_items;
}

QImage CAD_basic_pipe::wizardImage()
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

void CAD_basic_pipe::calculate()
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

    this->snap_vertices.append(this->position + this->direction);
    this->snap_center.append(this->position + this->direction * 0.5);
    this->snap_flanges.append(this->snap_vertices);
    this->boundingBox.enterVertex(this->position);
    this->boundingBox.enterVertex(this->position + this->direction);


    vertices_inner_bottom.clear();
    vertices_inner_top.clear();
    vertices_outer_bottom.clear();
    vertices_outer_top.clear();

    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;

        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += position;
        vertices_outer_bottom.append(linePos);
        this->boundingBox.enterVertex(linePos);
        vertices_inner_bottom.append(linePos + (position - linePos).normalized() * wallThickness);
        QVector3D pos_top = position + matrix_rotation * QVector3D(length, 0.0, 0.0);
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += pos_top;
        vertices_outer_top.append(linePos);
        this->boundingBox.enterVertex(linePos);
        vertices_inner_top.append(linePos + (pos_top - linePos).normalized() * wallThickness);
    }






}

void CAD_basic_pipe::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    wallThickness = wizardParams.value("s").toDouble();
    length = wizardParams.value("l").toDouble();
    radius = wizardParams.value("d").toDouble() / 2.0;

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    direction =  matrix_rotation * QVector3D(1.0, 0.0, 0.0) * length;
}

void CAD_basic_pipe::paint(GLWidget *glwidget)
{

}
