#include "cad_air_ductvolumetricflowcontroller.h"

CAD_air_ductVolumetricFlowController::CAD_air_ductVolumetricFlowController() : CADitem(CADitem::Air_DuctVolumetricFlowController)
{
    this->description = "Air|Duct volumetric flow controller";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));

    this->duct = new CAD_air_duct();
    this->flap = new CAD_basic_box();
    this->function = new CAD_basic_box();
    this->subItems.append(duct);
    this->subItems.append(flap);
    this->subItems.append(function);

    processWizardInput();
    calculate();
}

CAD_air_ductVolumetricFlowController::~CAD_air_ductVolumetricFlowController()
{

}

QList<CADitem::ItemType> CAD_air_ductVolumetricFlowController::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctYpiece);
    flangable_items.append(CADitem::Air_Filter);
    flangable_items.append(CADitem::Air_HeatExchangerAirAir);
    flangable_items.append(CADitem::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitem::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductVolumetricFlowController::wizardImage()
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

void CAD_air_ductVolumetricFlowController::calculate()
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

    duct->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    duct->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    duct->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    duct->wizardParams.insert("s", QVariant::fromValue(s));
    duct->wizardParams.insert("l", QVariant::fromValue(l));
    duct->wizardParams.insert("b", QVariant::fromValue(b));
    duct->wizardParams.insert("a", QVariant::fromValue(a));
    duct->wizardParams.insert("ff", QVariant::fromValue(ff));
    duct->wizardParams.insert("fe", QVariant::fromValue(fe));
    duct->processWizardInput();
    duct->calculate();

    QVector3D position_f = position + matrix_rotation * QVector3D(l / 2, -0.6 * b, 0.0);
    function->wizardParams.insert("Position x", QVariant::fromValue(position_f.x()));
    function->wizardParams.insert("Position y", QVariant::fromValue(position_f.y()));
    function->wizardParams.insert("Position z", QVariant::fromValue(position_f.z()));
    function->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    function->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    function->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    function->wizardParams.insert("Size x", QVariant::fromValue(0.3 * l));
    function->wizardParams.insert("Size y", QVariant::fromValue(0.2 * b));
    function->wizardParams.insert("Size z", QVariant::fromValue(0.5 * a));
    function->processWizardInput();
    function->calculate();

    QVector3D position_fl = position + matrix_rotation * QVector3D(l / 2, 0.0, 0.0);
    flap->wizardParams.insert("Position x", QVariant::fromValue(position_fl.x()));
    flap->wizardParams.insert("Position y", QVariant::fromValue(position_fl.y()));
    flap->wizardParams.insert("Position z", QVariant::fromValue(position_fl.z()));
    flap->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flap->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flap->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flap->wizardParams.insert("Size x", QVariant::fromValue(a));
    flap->wizardParams.insert("Size y", QVariant::fromValue(b - 2 * s));
    flap->wizardParams.insert("Size z", QVariant::fromValue(0.1 * a));
    flap->processWizardInput();
    flap->calculate();
}

void CAD_air_ductVolumetricFlowController::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();

}
