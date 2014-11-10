#include "cad_air_ductturn.h"

CAD_air_ductTurn::CAD_air_ductTurn() : CADitem(CADitem::Air_DuctTurn)
{
    this->description = "Air|Duct turn";
    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Radius (r)"), QVariant::fromValue(10.0));
    wizardParams.insert(QObject::tr("Width 1 (b)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Width 2 (d)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Height (a)"), QVariant::fromValue(5.0));

    wizardParams.insert(QObject::tr("Endcap 1 (f)"), QVariant::fromValue(5.0));
    wizardParams.insert(QObject::tr("Endcap 2 (e)"), QVariant::fromValue(5.0));

    wizardParams.insert(QObject::tr("Wall thickness"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Flange size"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Angle (alpha)"), QVariant::fromValue(1.0));


    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
}

QList<CADitem::ItemType> CAD_air_ductTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductTurn::wizardImage()
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


void CAD_air_ductTurn::calculate()
{

}

void CAD_air_ductTurn::processWizardInput()
{
    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());

    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

    wall_thickness = wizardParams.value(QObject::tr("Wall thickness")).toDouble();;
   flange_size = wizardParams.value(QObject::tr("Flange size")).toDouble();;

    radius = wizardParams.value(QObject::tr("Radius (r)")).toDouble();;
    width_1 = wizardParams.value(QObject::tr("Width 1 (b)")).toDouble();;
    width_2 = wizardParams.value(QObject::tr("Width 2 (d)")).toDouble();;
    height = wizardParams.value(QObject::tr("Height (a)")).toDouble();;
    angle = wizardParams.value(QObject::tr("Angle (alpha)")).toDouble();;
    endcap_1 = wizardParams.value(QObject::tr("Endcap 1 (f)")).toDouble();;
    endcap_2 = wizardParams.value(QObject::tr("Endcap 2 (e)")).toDouble();;

}
