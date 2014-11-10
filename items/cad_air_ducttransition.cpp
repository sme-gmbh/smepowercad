#include "cad_air_ducttransition.h"

CAD_air_ductTransition::CAD_air_ductTransition() : CADitem(CADitem::Air_DuctTransition)
{
    this->description = "Air|Duct transition rect/rect";
    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(20.0));
    wizardParams.insert(QObject::tr("Width 1 (b)"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Height 1 (a)"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Width 2 (d)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Height 2 (c)"), QVariant::fromValue(5.0));

    wizardParams.insert(QObject::tr("Offset y (e)"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Offset z (f)"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Length endcap (u)"), QVariant::fromValue(5.0));

    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Flange Size"), QVariant::fromValue(1.0));


    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));



}

QList<CADitem::ItemType> CAD_air_ductTransition::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductTransition::wizardImage()
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

void CAD_air_ductTransition::calculate()
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

    this->snap_flanges.append((pos_top_3 + pos_bot_2) / 2.0);
    this->snap_flanges.append((pos_top_4 + pos_bot_1) / 2.0);

}

void CAD_air_ductTransition::processWizardInput()
{

    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());

    qreal l = wizardParams.value(QObject::tr("Length (l)")).toDouble();
    qreal b = wizardParams.value(QObject::tr("Width 1 (b)")).toDouble();
    qreal a = wizardParams.value(QObject::tr("Height 1 (a)")).toDouble();
    qreal d = wizardParams.value(QObject::tr("Width 2 (d)")).toDouble();
    qreal c = wizardParams.value(QObject::tr("Height 2 (c)")).toDouble();

    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    wall_thickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();
    flange_size = wizardParams.value(QObject::tr("Flange size")).toDouble();
    endcap = wizardParams.value(QObject::tr("Length endcap (u)")).toDouble();
    e = wizardParams.value(QObject::tr("Offset y (e)")).toDouble();
    f = wizardParams.value(QObject::tr("Offset z (f)")).toDouble();

    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;
    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c  + QVector3D(0.0, 0.0, 0.5) * f + position;
    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;

    pos_top_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -0.0) * a + position;
    pos_top_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
    pos_top_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
    pos_top_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * b + QVector3D(0.0, 0.0,  0.0) * a + position;


}
