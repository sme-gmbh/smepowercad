#include "cad_basic_pipe.h"

CAD_basic_pipe::CAD_basic_pipe() : CADitem(CADitem::Basic_Pipe)
{
    this->description = "Basic|Pipe";
    radius = 1.0;

    length = 20.0;

    wizardParams.insert(QObject::tr("Position x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Position y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Position z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Length"), QVariant::fromValue(20.0));
    wizardParams.insert(QObject::tr("Outer diameter"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));

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

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_basic_pipe::calculate()
{
    this->boundingBox.reset();
    this->snap_basepoint = this->position;
    this->snap_vertices.append(this->position + this->direction);
    this->snap_center.append(this->position + this->direction * 0.5);
    this->snap_flanges.append(this->snap_vertices);
    this->boundingBox.enterVertex(this->position);
    this->boundingBox.enterVertex(this->position + this->direction);    // tbd...
}

void CAD_basic_pipe::processWizardInput()
{
    position.setX(wizardParams.value(QObject::tr("Position x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Position y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Position z")).toDouble());
    length = wizardParams.value(QObject::tr("Length")).toDouble();
    radius = wizardParams.value(QObject::tr("Outer diameter")).toDouble() / 2.0;
    wallThickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();
    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    direction =  matrix_rotation * QVector3D(0.0, 1.0, 0.0) * length;
}
