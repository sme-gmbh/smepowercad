#include "cad_basic_duct.h"

CAD_basic_duct::CAD_basic_duct() : CADitem(CADitem::Basic_Duct)
{
    this->description = "Basic|Duct";
    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Length (l)"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Width (b)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));


    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));

}

QList<CADitem::ItemType> CAD_basic_duct::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

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
    this->snap_basepoint = ((pos_bot_1 +
                                pos_bot_2 +
                                pos_bot_3 +
                                pos_bot_4 +
                                pos_top_1 +
                                pos_top_2 +
                                pos_top_3 +
                                pos_top_4) / 8.0);

    this->snap_flanges.append((pos_bot_1+pos_top_3) / 2);
    this->snap_flanges.append((pos_bot_2+pos_bot_4) / 2);


}

void CAD_basic_duct::processWizardInput()
{
    QVector3D size;

    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setX(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());

    size.setX(wizardParams.value(QObject::tr("Length (l)")).toDouble());
    size.setY(wizardParams.value(QObject::tr("Width (b)")).toDouble());
    size.setZ(wizardParams.value(QObject::tr("Height (a)")).toDouble());

    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    wall_thickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();


//    pos_bot_1 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
//    pos_bot_2 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
//    pos_bot_3 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
//    pos_bot_4 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
//    pos_top_1 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
//    pos_top_2 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z();
//    pos_top_3 = QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
//    pos_top_4 = QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z();
    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;
    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0, -1.0) * size.z() + position;

    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -1.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;
    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.0, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.0) * size.z() + position;


}
