#include "cad_air_ducttransition.h"

CAD_air_ductTransition::CAD_air_ductTransition() : CADitem(CADitem::Air_DuctTransition)
{
    this->description = "Air|Duct transition rect/rect";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("b2", QVariant::fromValue(20.0));
    wizardParams.insert("a2", QVariant::fromValue(30.0));

    wizardParams.insert("e", QVariant::fromValue(0.0));
    wizardParams.insert("f", QVariant::fromValue(0.0));

    wizardParams.insert("u", QVariant::fromValue(5.0));

    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));



    transition_duct = new CAD_basic_duct();
    endcap_left_duct = new CAD_basic_duct();
    endcap_right_duct = new CAD_basic_duct();
    flange_left_duct = new CAD_basic_duct();
    flange_right_duct = new CAD_basic_duct();
    subItems.append(transition_duct);
    subItems.append(endcap_left_duct);
    subItems.append(endcap_right_duct);
    subItems.append(flange_left_duct);
    subItems.append(flange_right_duct);


    processWizardInput();
    calculate();
}

CAD_air_ductTransition::~CAD_air_ductTransition()
{
}

QList<CADitem::ItemType> CAD_air_ductTransition::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
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

QImage CAD_air_ductTransition::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTransition::iconPath()
{
    return ":/icons/cad_air/cad_air_ducttransition.svg";
}

QString CAD_air_ductTransition::domain()
{
    return "Air";
}

void CAD_air_ductTransition::calculate()
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

//    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;
//    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c  + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;

//    pos_top_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -0.0) * a + position;
//    pos_top_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_top_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_top_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * b + QVector3D(0.0, 0.0,  0.0) * a + position;

    // Left end duct
    QVector3D position_led = position + QVector3D(0.0, 0.0, 0.0);
    // Right end duct
    QVector3D position_red = position + matrix_rotation * QVector3D(l, (b - b2) / 2 + e, (a - a2) / 2 + f);
    // Left flange duct
    QVector3D position_lfd = position + QVector3D(0, 0.0, 0.0);
    // Right flange duct
    QVector3D position_rfd = position + matrix_rotation * QVector3D(l, (b - b2) / 2 + e, (a - a2) / 2 + f);




    endcap_left_duct->wizardParams.insert("Position x", QVariant::fromValue(position_led.x()));
    endcap_left_duct->wizardParams.insert("Position y", QVariant::fromValue(position_led.y()));
    endcap_left_duct->wizardParams.insert("Position z", QVariant::fromValue(position_led.z()));
    endcap_left_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    endcap_left_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    endcap_left_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    endcap_left_duct->wizardParams.insert("l", QVariant::fromValue(endcap));
    endcap_left_duct->wizardParams.insert("b", QVariant::fromValue(b));
    endcap_left_duct->wizardParams.insert("a", QVariant::fromValue(a));
    endcap_left_duct->wizardParams.insert("s", QVariant::fromValue(s));
    endcap_left_duct->processWizardInput();
    endcap_left_duct->calculate();


    endcap_right_duct->wizardParams.insert("Position x", QVariant::fromValue(position_red.x()));//position.x()+l));
    endcap_right_duct->wizardParams.insert("Position y", QVariant::fromValue(position_red.y()));//position.y()+(b-d)/2+e));
    endcap_right_duct->wizardParams.insert("Position z", QVariant::fromValue(position_red.z()));//position.z()+(a-c)/2+f));
    endcap_right_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    endcap_right_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    endcap_right_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    endcap_right_duct->wizardParams.insert("l", QVariant::fromValue(endcap));
    endcap_right_duct->wizardParams.insert("b", QVariant::fromValue(b2));
    endcap_right_duct->wizardParams.insert("a", QVariant::fromValue(a2));
    endcap_right_duct->wizardParams.insert("s", QVariant::fromValue(s));
    endcap_right_duct->processWizardInput();
    endcap_right_duct->calculate();

    flange_left_duct->wizardParams.insert("Position x", QVariant::fromValue(position_lfd.x()));
    flange_left_duct->wizardParams.insert("Position y", QVariant::fromValue(position_lfd.y()));
    flange_left_duct->wizardParams.insert("Position z", QVariant::fromValue(position_lfd.z()));
    flange_left_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_left_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_left_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    flange_left_duct->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_left_duct->wizardParams.insert("b", QVariant::fromValue(b+2*ff));
    flange_left_duct->wizardParams.insert("a", QVariant::fromValue(a+2*ff));
    flange_left_duct->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_left_duct->processWizardInput();
    flange_left_duct->calculate();

    flange_right_duct->wizardParams.insert("Position x", QVariant::fromValue(position_rfd.x()));//position.x()+l));
    flange_right_duct->wizardParams.insert("Position y", QVariant::fromValue(position_rfd.y()));//position.y()+(b-d)/2+e));
    flange_right_duct->wizardParams.insert("Position z", QVariant::fromValue(position_rfd.z()));//position.z()+(a-c)/2+f));
    flange_right_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_right_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_right_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    flange_right_duct->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_right_duct->wizardParams.insert("b", QVariant::fromValue(b2+2*ff));
    flange_right_duct->wizardParams.insert("a", QVariant::fromValue(a2+2*ff));
    flange_right_duct->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_right_duct->processWizardInput();
    flange_right_duct->calculate();

    transition_duct->wizardParams.insert("Position x", QVariant::fromValue(position.x()+endcap));
    transition_duct->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    transition_duct->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    transition_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    transition_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    transition_duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    transition_duct->wizardParams.insert("l", QVariant::fromValue(l -2*endcap));
    transition_duct->wizardParams.insert("b", QVariant::fromValue(b));
    transition_duct->wizardParams.insert("a", QVariant::fromValue(a));
    transition_duct->wizardParams.insert("s", QVariant::fromValue(s));
    transition_duct->processWizardInput();
    transition_duct->calculate();


    QVector3D vertices[] = {
        //set Outer Points
        endcap_left_duct->pos_bot_1,
        endcap_right_duct->pos_bot_4,
        endcap_right_duct->pos_bot_1,
        endcap_left_duct->pos_bot_4,
        endcap_left_duct->pos_top_1,
        endcap_right_duct->pos_top_4,
        endcap_right_duct->pos_top_1,
        endcap_left_duct->pos_top_4,
        //set inner Points
        endcap_right_duct->inner_pos_bot_4,
        endcap_left_duct->inner_pos_bot_1,
        endcap_left_duct->inner_pos_bot_4,
        endcap_right_duct->inner_pos_bot_1,
        endcap_right_duct->inner_pos_top_4,
        endcap_left_duct->inner_pos_top_1,
        transition_duct->inner_pos_top_3,
        endcap_right_duct->inner_pos_top_1,
        endcap_left_duct->inner_pos_top_4
    };
//    //set Outer Points
//    transition_duct->pos_bot_1 = endcap_left_duct->pos_bot_1;
//    transition_duct->pos_bot_2 = endcap_right_duct->pos_bot_4;
//    transition_duct->pos_bot_3 = endcap_right_duct->pos_bot_1;
//    transition_duct->pos_bot_4 = endcap_left_duct->pos_bot_4;

//    transition_duct->pos_top_1 = endcap_left_duct->pos_top_1;
//    transition_duct->pos_top_2 = endcap_right_duct->pos_top_4;
//    transition_duct->pos_top_3 = endcap_right_duct->pos_top_1;
//    transition_duct->pos_top_4 = endcap_left_duct->pos_top_4;
//    //set Inner Points
//    transition_duct->inner_pos_bot_1 = endcap_left_duct->inner_pos_bot_1;
//    transition_duct->inner_pos_bot_2 = endcap_right_duct->inner_pos_bot_4;
//    transition_duct->inner_pos_bot_3 = endcap_right_duct->inner_pos_bot_1;
//    transition_duct->inner_pos_bot_4 = endcap_left_duct->inner_pos_bot_4;

//    transition_duct->inner_pos_top_1 = endcap_left_duct->inner_pos_top_1;
//    transition_duct->inner_pos_top_2 = endcap_right_duct->inner_pos_top_4;
//    transition_duct->inner_pos_top_3 = endcap_right_duct->inner_pos_top_1;
//    transition_duct->inner_pos_top_4 = endcap_left_duct->inner_pos_top_4;

    transition_duct->arrayBufVertices.bind();
    transition_duct->arrayBufVertices.allocate(vertices, sizeof(vertices));


    boundingBox.enterVertex(flange_left_duct->pos_bot_1);
    boundingBox.enterVertex(flange_left_duct->pos_top_1);
    boundingBox.enterVertex(flange_left_duct->pos_bot_2);
    boundingBox.enterVertex(flange_left_duct->pos_top_2);
    boundingBox.enterVertex(flange_left_duct->pos_bot_3);
    boundingBox.enterVertex(flange_left_duct->pos_top_3);
    boundingBox.enterVertex(flange_left_duct->pos_bot_4);
    boundingBox.enterVertex(flange_left_duct->pos_top_4);


    boundingBox.enterVertex(flange_right_duct->pos_bot_1);
    boundingBox.enterVertex(flange_right_duct->pos_top_1);
    boundingBox.enterVertex(flange_right_duct->pos_bot_2);
    boundingBox.enterVertex(flange_right_duct->pos_top_2);
    boundingBox.enterVertex(flange_right_duct->pos_bot_3);
    boundingBox.enterVertex(flange_right_duct->pos_top_3);
    boundingBox.enterVertex(flange_right_duct->pos_bot_4);
    boundingBox.enterVertex(flange_right_duct->pos_top_4);

    this->snap_vertices.append(endcap_left_duct->pos_bot_2);
    this->snap_vertices.append(endcap_left_duct->pos_bot_3);
    this->snap_vertices.append(endcap_left_duct->pos_top_2);
    this->snap_vertices.append(endcap_left_duct->pos_top_3);

    this->snap_vertices.append(endcap_right_duct->pos_bot_2);
    this->snap_vertices.append(endcap_right_duct->pos_bot_3);
    this->snap_vertices.append(endcap_right_duct->pos_top_2);
    this->snap_vertices.append(endcap_right_duct->pos_top_3);





    this->snap_basepoint = position;

//    this->snap_vertices.append(pos_bot_1);
//    this->snap_vertices.append(pos_bot_2);
//    this->snap_vertices.append(pos_bot_3);
//    this->snap_vertices.append(pos_bot_4);
//    this->snap_vertices.append(pos_top_1);
//    this->snap_vertices.append(pos_top_2);
//    this->snap_vertices.append(pos_top_3);
//    this->snap_vertices.append(pos_top_4);

    this->snap_flanges.append(position);
    this->snap_flanges.append(flange_right_duct->snap_flanges.at(1));

}

void CAD_air_ductTransition::processWizardInput()
{

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();
    b2 = wizardParams.value("b2").toDouble();
    a2 = wizardParams.value("a2").toDouble();

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    endcap = wizardParams.value("u").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}
