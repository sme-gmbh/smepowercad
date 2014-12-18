#include "cad_basic_line.h"
#include <QDebug>
#include <QPen>

CAD_basic_line::CAD_basic_line() : CADitem(CADitem::Basic_Line)
{
    this->description = "Basic|Line";

    widthByLayer = false;
    widthByBlock = false;

    this->wizardParams.insert("Position x1", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position y1", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position z1", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position x2", QVariant::fromValue(1.0));
    this->wizardParams.insert("Position y2", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position z2", QVariant::fromValue(0.0));
    this->wizardParams.insert("Width", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_line::~CAD_basic_line()
{

}

QList<CADitem::ItemType> CAD_basic_line::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_line::wizardImage()
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

void CAD_basic_line::calculate()
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

    this->boundingBox.enterVertex(this->p1);
    this->boundingBox.enterVertex(this->p2);

    this->position = this->p1;
    this->snap_vertices.append(p1);
    this->snap_vertices.append(p2);

    this->snap_center.append((p1 + p2) / 2.0);
}

void CAD_basic_line::processWizardInput()
{
    this->p1.setX(wizardParams.value("Position x1").toDouble());
    this->p1.setY(wizardParams.value("Position y1").toDouble());
    this->p1.setZ(wizardParams.value("Position z1").toDouble());


    this->p2.setX(wizardParams.value("Position x2").toDouble());
    this->p2.setY(wizardParams.value("Position y2").toDouble());
    this->p2.setZ(wizardParams.value("Position z2").toDouble());

    this->width = wizardParams.value("Width").toDouble();

    this->angle_x = wizardParams.value("Angle x").toDouble();
    this->angle_y = wizardParams.value("Angle y").toDouble();
    this->angle_z = wizardParams.value("Angle z").toDouble();

}

void CAD_basic_line::paint(GLWidget *glwidget)
{

}
