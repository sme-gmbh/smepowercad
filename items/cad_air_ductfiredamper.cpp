#include "cad_air_ductfiredamper.h"
#include "itemdb.h"

CAD_air_ductFireDamper::CAD_air_ductFireDamper() : CADitem(CADitemTypes::Air_DuctFireDamper)
{
    this->flange_duct_left = new CAD_basic_duct();
    this->flange_duct_middle = new CAD_basic_box();
    this->flange_duct_right = new CAD_basic_duct();
    this->main_duct = new CAD_basic_duct();
    this->function = new CAD_basic_box();
    this->subItems.append(flange_duct_left);
    this->subItems.append(flange_duct_middle);
    this->subItems.append(flange_duct_right);
    this->subItems.append(main_duct);
    this->subItems.append(function);

    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("h", QVariant::fromValue(25.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("g", QVariant::fromValue(35.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("l3", QVariant::fromValue(70.0));
    wizardParams.insert("l2", QVariant::fromValue(10.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_air_ductFireDamper::~CAD_air_ductFireDamper()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductFireDamper::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransition);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductFireDamper::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductFireDamper::iconPath()
{
    return ":/icons/cad_air/cad_air_ductfiredamper.svg";
}

QString CAD_air_ductFireDamper::domain()
{
    return "Air";
}

QString CAD_air_ductFireDamper::description()
{
    return "Air|Duct fire damper";
}

void CAD_air_ductFireDamper::calculate()
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

    QVector3D position_fl = position + matrix_rotation * QVector3D(-l3, 0.0, 0.0);
    flange_duct_left->wizardParams.insert("Position x", QVariant::fromValue(position_fl.x()));
    flange_duct_left->wizardParams.insert("Position y", QVariant::fromValue(position_fl.y()));
    flange_duct_left->wizardParams.insert("Position z", QVariant::fromValue(position_fl.z()));
    flange_duct_left->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_duct_left->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_duct_left->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_duct_left->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_duct_left->wizardParams.insert("b", QVariant::fromValue(b + 2 * ff));
    flange_duct_left->wizardParams.insert("a", QVariant::fromValue(a + 2 * ff));
    flange_duct_left->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_duct_left->processWizardInput();
    flange_duct_left->calculate();

    QVector3D position_md = position + matrix_rotation * QVector3D(l2 / 2, 0.0, 0.0);
    flange_duct_middle->wizardParams.insert("Position x", QVariant::fromValue(position_md.x()));
    flange_duct_middle->wizardParams.insert("Position y", QVariant::fromValue(position_md.y()));
    flange_duct_middle->wizardParams.insert("Position z", QVariant::fromValue(position_md.z()));
    flange_duct_middle->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_duct_middle->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_duct_middle->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_duct_middle->wizardParams.insert("Size x", QVariant::fromValue(l2));
    flange_duct_middle->wizardParams.insert("Size y", QVariant::fromValue(g));
    flange_duct_middle->wizardParams.insert("Size z", QVariant::fromValue(h));
    flange_duct_middle->processWizardInput();
    flange_duct_middle->calculate();

    QVector3D position_fr = position + matrix_rotation * QVector3D(l - l3 - fe, 0.0, 0.0);
    flange_duct_right->wizardParams.insert("Position x", QVariant::fromValue(position_fr.x()));
    flange_duct_right->wizardParams.insert("Position y", QVariant::fromValue(position_fr.y()));
    flange_duct_right->wizardParams.insert("Position z", QVariant::fromValue(position_fr.z()));
    flange_duct_right->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_duct_right->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_duct_right->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_duct_right->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_duct_right->wizardParams.insert("b", QVariant::fromValue(b + 2 * ff));
    flange_duct_right->wizardParams.insert("a", QVariant::fromValue(a + 2 * ff));
    flange_duct_right->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_duct_right->processWizardInput();
    flange_duct_right->calculate();

    QVector3D position_main = position + matrix_rotation * QVector3D(-l3, 0.0, 0.0);
    main_duct->wizardParams.insert("Position x", QVariant::fromValue(position_main.x()));
    main_duct->wizardParams.insert("Position y", QVariant::fromValue(position_main.y()));
    main_duct->wizardParams.insert("Position z", QVariant::fromValue(position_main.z()));
    main_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    main_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    main_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    main_duct->wizardParams.insert("l", QVariant::fromValue(l));
    main_duct->wizardParams.insert("b", QVariant::fromValue(b));
    main_duct->wizardParams.insert("a", QVariant::fromValue(a));
    main_duct->wizardParams.insert("s", QVariant::fromValue(s));
    main_duct->processWizardInput();
    main_duct->calculate();

    QVector3D position_f = position + matrix_rotation * QVector3D(-l3 / 2, -0.6 * b, 0.0);
    function->wizardParams.insert("Position x", QVariant::fromValue(position_f.x()));
    function->wizardParams.insert("Position y", QVariant::fromValue(position_f.y()));
    function->wizardParams.insert("Position z", QVariant::fromValue(position_f.z()));
    function->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    function->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    function->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    function->wizardParams.insert("Size x", QVariant::fromValue(0.3 * l3));
    function->wizardParams.insert("Size y", QVariant::fromValue(0.2 * b));
    function->wizardParams.insert("Size z", QVariant::fromValue(0.5 * a));
    function->processWizardInput();
    function->calculate();

    // tbd: not all vertices are needed here!
    boundingBox.enterVertex(flange_duct_left->pos_top_1);
    boundingBox.enterVertex(flange_duct_left->pos_top_2);
    boundingBox.enterVertex(flange_duct_left->pos_top_3);
    boundingBox.enterVertex(flange_duct_left->pos_top_4);
    boundingBox.enterVertex(flange_duct_left->pos_bot_1);
    boundingBox.enterVertex(flange_duct_left->pos_bot_2);
    boundingBox.enterVertex(flange_duct_left->pos_bot_3);
    boundingBox.enterVertex(flange_duct_left->pos_bot_4);

    boundingBox.enterVertex(flange_duct_right->pos_top_1);
    boundingBox.enterVertex(flange_duct_right->pos_top_2);
    boundingBox.enterVertex(flange_duct_right->pos_top_3);
    boundingBox.enterVertex(flange_duct_right->pos_top_4);
    boundingBox.enterVertex(flange_duct_right->pos_bot_1);
    boundingBox.enterVertex(flange_duct_right->pos_bot_2);
    boundingBox.enterVertex(flange_duct_right->pos_bot_3);
    boundingBox.enterVertex(flange_duct_right->pos_bot_4);

    this->snap_flanges.append(position);
    this->snap_vertices.append(main_duct->snap_vertices);
    this->snap_flanges.append(main_duct->snap_flanges);

}

void CAD_air_ductFireDamper::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    a = wizardParams.value("a").toDouble();
    h = wizardParams.value("h").toDouble();
    b = wizardParams.value("b").toDouble();
    g = wizardParams.value("g").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    s = wizardParams.value("s").toDouble();

}
