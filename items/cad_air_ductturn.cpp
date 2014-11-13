#include "cad_air_ductturn.h"

CAD_air_ductTurn::CAD_air_ductTurn() : CADitem(CADitem::Air_DuctTurn)
{
    this->description = "Air|Duct turn";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("Radius (r)", QVariant::fromValue(10.0));
    wizardParams.insert("Width 1 (b)", QVariant::fromValue(5.0));
    wizardParams.insert("Width 2 (d)", QVariant::fromValue(5.0));
    wizardParams.insert("Height (a)", QVariant::fromValue(5.0));

    wizardParams.insert("Endcap 1 (f)", QVariant::fromValue(5.0));
    wizardParams.insert("Endcap 2 (e)", QVariant::fromValue(5.0));

    wizardParams.insert("Wall thickness", QVariant::fromValue(1.0));
    wizardParams.insert("Flange size", QVariant::fromValue(1.0));
    wizardParams.insert("Angle (alpha)", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_air_ductTurn::~CAD_air_ductTurn()
{

}

QList<CADitem::ItemType> CAD_air_ductTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitem::Air_DuctYpiece);
    flangable_items.append(CADitem::Air_Filter);
    flangable_items.append(CADitem::Air_HeatExchangerAirAir);
    flangable_items.append(CADitem::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitem::Air_MultiLeafDamper);
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
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
}

void CAD_air_ductTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    wall_thickness = wizardParams.value("Wall thickness").toDouble();;
   flange_size = wizardParams.value("Flange size").toDouble();;

    radius = wizardParams.value("Radius (r)").toDouble();;
    width_1 = wizardParams.value("Width 1 (b)").toDouble();;
    width_2 = wizardParams.value("Width 2 (d)").toDouble();;
    height = wizardParams.value("Height (a)").toDouble();;
    angle = wizardParams.value("Angle (alpha)").toDouble();;
    endcap_1 = wizardParams.value("Endcap 1 (f)").toDouble();;
    endcap_2 = wizardParams.value("Endcap 2 (e)").toDouble();;

}
