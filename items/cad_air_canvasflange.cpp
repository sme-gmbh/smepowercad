#include "cad_air_canvasflange.h"

CAD_air_canvasFlange::CAD_air_canvasFlange() : CADitem(CADitem::Air_CanvasFlange)
{
    this->flange_left = new CAD_basic_duct();
    this->flange_right = new CAD_basic_duct();
    this->canvas1 = new CAD_basic_duct();
    this->canvas2 = new CAD_basic_duct();
    this->canvas3 = new CAD_basic_duct();
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(canvas1);
    this->subItems.append(canvas2);
    this->subItems.append(canvas3);
    this->description = "Air|Canvas flange";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("l", QVariant::fromValue(50.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));
    processWizardInput();
    calculate();
}

CAD_air_canvasFlange::~CAD_air_canvasFlange()
{

}

QList<CADitem::ItemType> CAD_air_canvasFlange::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_CanvasFlange);
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctBaffleSilencer);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitem::Air_DuctYpiece);
    flangable_items.append(CADitem::Air_Fan);
    flangable_items.append(CADitem::Air_Filter);
    flangable_items.append(CADitem::Air_HeatExchangerAirAir);
    flangable_items.append(CADitem::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitem::Air_Humidifier);
    flangable_items.append(CADitem::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_canvasFlange::wizardImage()
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

void CAD_air_canvasFlange::calculate()
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

    flange_left->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    flange_left->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    flange_left->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    flange_left->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_left->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_left->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_left->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_left->wizardParams.insert("b", QVariant::fromValue(b+2*ff));
    flange_left->wizardParams.insert("a", QVariant::fromValue(a+2*ff));
    flange_left->wizardParams.insert("s", QVariant::fromValue(ff + s));
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_fr = matrix_rotation * QVector3D(l - fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", QVariant::fromValue(position_fr.x()));
    flange_right->wizardParams.insert("Position y", QVariant::fromValue(position_fr.y()));
    flange_right->wizardParams.insert("Position z", QVariant::fromValue(position_fr.z()));
    flange_right->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_right->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_right->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_right->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_right->wizardParams.insert("b", QVariant::fromValue(b + 2 * ff));
    flange_right->wizardParams.insert("a", QVariant::fromValue(a + 2 * ff));
    flange_right->wizardParams.insert("s", QVariant::fromValue(ff + s));
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_ca1 = matrix_rotation * QVector3D(fe, 0.0, 0.0);
    canvas1->wizardParams.insert("Position x", QVariant::fromValue(position_ca1.x()));
    canvas1->wizardParams.insert("Position y", QVariant::fromValue(position_ca1.y()));
    canvas1->wizardParams.insert("Position z", QVariant::fromValue(position_ca1.z()));
    canvas1->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    canvas1->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    canvas1->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    canvas1->wizardParams.insert("l", QVariant::fromValue((l - 2 * fe) / 3));
    canvas1->wizardParams.insert("b", QVariant::fromValue(b));
    canvas1->wizardParams.insert("a", QVariant::fromValue(a));
    canvas1->wizardParams.insert("s", QVariant::fromValue(s));
    canvas1->processWizardInput();
    canvas1->calculate();

    QVector3D position_ca2 = matrix_rotation * QVector3D((fe + l) / 3, 0.0, 0.0);
    canvas2->wizardParams.insert("Position x", QVariant::fromValue(position_ca2.x()));
    canvas2->wizardParams.insert("Position y", QVariant::fromValue(position_ca2.y()));
    canvas2->wizardParams.insert("Position z", QVariant::fromValue(position_ca2.z()));
    canvas2->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    canvas2->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    canvas2->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    canvas2->wizardParams.insert("l", QVariant::fromValue((l - 2 * fe) / 3));
    canvas2->wizardParams.insert("b", QVariant::fromValue(b - 2 * s));
    canvas2->wizardParams.insert("a", QVariant::fromValue(a - 2 * s));
    canvas2->wizardParams.insert("s", QVariant::fromValue(s));
    canvas2->processWizardInput();
    canvas2->calculate();

    QVector3D position_ca3 = matrix_rotation * QVector3D((2 * l - fe) / 3, 0.0, 0.0);
    canvas3->wizardParams.insert("Position x", QVariant::fromValue(position_ca3.x()));
    canvas3->wizardParams.insert("Position y", QVariant::fromValue(position_ca3.y()));
    canvas3->wizardParams.insert("Position z", QVariant::fromValue(position_ca3.z()));
    canvas3->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    canvas3->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    canvas3->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    canvas3->wizardParams.insert("l", QVariant::fromValue((l - 2 * fe) / 3));
    canvas3->wizardParams.insert("b", QVariant::fromValue(b));
    canvas3->wizardParams.insert("a", QVariant::fromValue(a));
    canvas3->wizardParams.insert("s", QVariant::fromValue(s));
    canvas3->processWizardInput();
    canvas3->calculate();
}

void CAD_air_canvasFlange::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    this->b = wizardParams.value("b").toDouble();
    this->a = wizardParams.value("a").toDouble();
    this->l = wizardParams.value("l").toDouble();
    this->ff = wizardParams.value("ff").toDouble();
    this->fe = wizardParams.value("fe").toDouble();
    this->s = wizardParams.value("s").toDouble();
}
