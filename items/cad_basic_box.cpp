#include "cad_basic_box.h"

CAD_basic_box::CAD_basic_box() : CADitem(CADitem::Basic_Box)
{
    this->description = "Basic|Box";

    pos_bot_1 = QVector3D(-1, -1,  1);
    pos_bot_2 = QVector3D( 1, -1,  1);
    pos_bot_3 = QVector3D( 1, -1, -1);
    pos_bot_4 = QVector3D(-1, -1, -1);
    pos_top_1 = QVector3D(-1,  1,  1);
    pos_top_2 = QVector3D( 1,  1,  1);
    pos_top_3 = QVector3D( 1,  1, -1);
    pos_top_4 = QVector3D(-1,  1, -1);

    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Size x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Size y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Size z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
}

void CAD_basic_box::calculate()
{
    this->snap_basepoint = ((pos_bot_1 +
                            pos_bot_2 +
                            pos_bot_3 +
                            pos_bot_4 +
                            pos_top_1 +
                            pos_top_2 +
                            pos_top_3 +
                            pos_top_4) / 8.0);

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

    qDebug() << "calculating a box";
}

void CAD_basic_box::processWizardInput()
{
    QVector3D size;

    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());
    size.setX(wizardParams.value(QObject::tr("Size x")).toDouble());
    size.setY(wizardParams.value(QObject::tr("Size y")).toDouble());
    size.setZ(wizardParams.value(QObject::tr("Size z")).toDouble());
    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    pos_bot_1 = position;
    pos_bot_2 = position;
    pos_bot_3 = position;
    pos_bot_4 = position;
    pos_top_1 = position;
    pos_top_2 = position;
    pos_top_3 = position;
    pos_top_4 = position;

//    pos_bot_1 = QVector3D(-1, -1,  1);
//    pos_bot_2 = QVector3D( 1, -1,  1);
//    pos_bot_3 = QVector3D( 1, -1, -1);
//    pos_bot_4 = QVector3D(-1, -1, -1);
//    pos_top_1 = QVector3D(-1,  1,  1);
//    pos_top_2 = QVector3D( 1,  1,  1);
//    pos_top_3 = QVector3D( 1,  1, -1);
//    pos_top_4 = QVector3D(-1,  1, -1);

    pos_bot_1 += QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
    pos_bot_2 += QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
    pos_bot_3 += QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
    pos_bot_4 += QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
    pos_top_1 += QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
    pos_top_2 += QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
    pos_top_3 += QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
    pos_top_4 += QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
}
