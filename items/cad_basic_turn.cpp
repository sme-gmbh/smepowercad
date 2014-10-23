#include "cad_basic_turn.h"

CAD_basic_turn::CAD_basic_turn() : CADitem(CADitem::Basic_Turn)
{
    this->description = "Basic|Turn";
    radius_pipe = 5.0;
    radius_turn = 20.0;
    angle_turn = 90.0;

    wizardParams.insert(QObject::tr("Center base x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center base y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center base z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Turn radius"), QVariant::fromValue(20.0));
    wizardParams.insert(QObject::tr("Turn angle"), QVariant::fromValue(90.0));
    wizardParams.insert(QObject::tr("Outer diameter"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
}

void CAD_basic_turn::calculate()
{
    this->snap_basepoint = this->position;
//    this->snap_vertices.append(this->position + this->direction);

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

void CAD_basic_turn::processWizardInput()
{
    position.setX(wizardParams.value(QObject::tr("Center base x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center base y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center base z")).toDouble());
    radius_turn = wizardParams.value(QObject::tr("Turn radius")).toDouble();
    angle_turn = wizardParams.value(QObject::tr("Turn angle")).toDouble();
    radius_pipe = wizardParams.value(QObject::tr("Outer diameter")).toDouble() / 2.0;
    wallThickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();
    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
//    direction =  matrix_rotation * QVector3D(0.0, 0.0, 1.0) * length;
}
