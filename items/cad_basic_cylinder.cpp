#include "cad_basic_cylinder.h"

CAD_basic_cylinder::CAD_basic_cylinder() : CADitem(CADitem::Basic_Cylinder)
{
    this->description = "Basic|Cylinder";
    radius = 1.0;
    center_base = QVector3D();
    height = 1.0;

    wizardParams.insert(QObject::tr("Center base x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center base y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center base z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Height"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Radius"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
    //    wizardParams.insert(QObject::tr("Name"), QVariant::fromValue(QString()));
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

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_basic_cylinder::calculate()
{
    this->snap_basepoint = this->center_base;
}

void CAD_basic_cylinder::processWizardInput()
{
    center_base.setX(wizardParams.value(QObject::tr("Center base x")).toDouble());
    center_base.setY(wizardParams.value(QObject::tr("Center base y")).toDouble());
    center_base.setZ(wizardParams.value(QObject::tr("Center base z")).toDouble());
    height = wizardParams.value(QObject::tr("Height")).toDouble();
    radius = wizardParams.value(QObject::tr("Radius")).toDouble();
    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

}
