#include "cad_basic_circle.h"

CAD_basic_circle::CAD_basic_circle() : CADitem(CADitem::Basic_Circle)
{
    this->description = "Basic|Circle";
    this->width = 0.0;
    this->widthByLayer = true;
    this->widthByBlock = false;
    this->circle = QList<QVector3D>();

    wizardParams.insert("Center x", QVariant::fromValue(0.0));
    wizardParams.insert("Center y", QVariant::fromValue(0.0));
    wizardParams.insert("Center z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Radius", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_basic_circle::~CAD_basic_circle()
{

}

QList<CADitem::ItemType> CAD_basic_circle::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_circle::wizardImage()
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

void CAD_basic_circle::calculate()
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

    this->snap_basepoint = this->center;

    circle.clear();
    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;
        linePos = this->center;

        linePos += matrix_rotation * QVector3D(sin(angle) * this->radius, cos(angle) * this->radius, 0.0);
        circle.append(linePos);
    }
}

void CAD_basic_circle::processWizardInput()
{
    center.setX(wizardParams.value("Center x").toDouble());
    center.setY(wizardParams.value("Center y").toDouble());
    center.setZ(wizardParams.value("Center z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    radius = wizardParams.value("Radius").toDouble();
}
