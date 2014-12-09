#include "cad_air_ductypiece.h"

CAD_air_ductYpiece::CAD_air_ductYpiece() : CADitem(CADitem::Air_DuctYpiece)
{
    endcap_1 = new CAD_basic_duct;
    endcap_2 = new CAD_basic_duct;
    endcap_3 = new CAD_basic_duct;
    flange_1 = new CAD_basic_duct;
    flange_2 = new CAD_basic_duct;
    flange_3 = new CAD_basic_duct;
    subItems.append(endcap_1);
    subItems.append(endcap_2);
    subItems.append(endcap_3);
    subItems.append(flange_1);
    subItems.append(flange_2);
    subItems.append(flange_3);

    this->description = "Air|Duct Y-Piece";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));


    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("a2", QVariant::fromValue(30.0));
    wizardParams.insert("b2", QVariant::fromValue(10.0));
    wizardParams.insert("e", QVariant::fromValue(0.0));
    wizardParams.insert("f", QVariant::fromValue(0.0));
    wizardParams.insert("b3", QVariant::fromValue(10.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("m", QVariant::fromValue(10.0));
    wizardParams.insert("u", QVariant::fromValue(5.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));


    processWizardInput();
    calculate();
}

CAD_air_ductYpiece::~CAD_air_ductYpiece()
{

}

QList<CADitem::ItemType> CAD_air_ductYpiece::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
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

QImage CAD_air_ductYpiece::wizardImage()
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

void CAD_air_ductYpiece::calculate()
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

    QVector3D position_e1 = position + matrix_rotation * QVector3D(0.0, 0.0, 0.0);
    QVector3D position_f1 = position + matrix_rotation * QVector3D(0.0, 0.0, 0.0);
    QVector3D position_e2 = position + matrix_rotation * QVector3D(l, b/2 + e - b2/2, a/2 + f - a2/2);
    QVector3D position_e3 = position + matrix_rotation * QVector3D(l, b/2 + e - b2 - m - b3/2, a/2 + f - a2/2);

    endcap_1->wizardParams.insert("Position x", QVariant::fromValue(position_e1.x()));
    endcap_1->wizardParams.insert("Position y", QVariant::fromValue(position_e1.y()));
    endcap_1->wizardParams.insert("Position z", QVariant::fromValue(position_e1.z()));
    endcap_1->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    endcap_1->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    endcap_1->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    endcap_1->wizardParams.insert("l", QVariant::fromValue(u));
    endcap_1->wizardParams.insert("b", QVariant::fromValue(b));
    endcap_1->wizardParams.insert("a", QVariant::fromValue(a));
    endcap_1->wizardParams.insert("s", QVariant::fromValue(s));
    endcap_1->processWizardInput();
    endcap_1->calculate();

    flange_1->wizardParams.insert("Position x", QVariant::fromValue(position_f1.x()));
    flange_1->wizardParams.insert("Position y", QVariant::fromValue(position_f1.y()));
    flange_1->wizardParams.insert("Position z", QVariant::fromValue(position_f1.z()));
    flange_1->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_1->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_1->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_1->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_1->wizardParams.insert("b", QVariant::fromValue(b + 2 * ff));
    flange_1->wizardParams.insert("a", QVariant::fromValue(a + 2 * ff));
    flange_1->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_1->processWizardInput();
    flange_1->calculate();

    endcap_2->wizardParams.insert("Position x", QVariant::fromValue(position_e2.x()));
    endcap_2->wizardParams.insert("Position y", QVariant::fromValue(position_e2.y()));
    endcap_2->wizardParams.insert("Position z", QVariant::fromValue(position_e2.z()));
    endcap_2->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    endcap_2->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    endcap_2->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    endcap_2->wizardParams.insert("l", QVariant::fromValue(u));
    endcap_2->wizardParams.insert("b", QVariant::fromValue(b2));
    endcap_2->wizardParams.insert("a", QVariant::fromValue(a2));
    endcap_2->wizardParams.insert("s", QVariant::fromValue(s));
    endcap_2->processWizardInput();
    endcap_2->calculate();

    flange_2->wizardParams.insert("Position x", QVariant::fromValue(position_e2.x()));
    flange_2->wizardParams.insert("Position y", QVariant::fromValue(position_e2.y()));
    flange_2->wizardParams.insert("Position z", QVariant::fromValue(position_e2.z()));
    flange_2->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_2->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_2->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_2->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_2->wizardParams.insert("b", QVariant::fromValue(b2 + 2 * ff));
    flange_2->wizardParams.insert("a", QVariant::fromValue(a2 + 2 * ff));
    flange_2->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_2->processWizardInput();
    flange_2->calculate();

    endcap_3->wizardParams.insert("Position x", QVariant::fromValue(position_e3.x()));
    endcap_3->wizardParams.insert("Position y", QVariant::fromValue(position_e3.y()));
    endcap_3->wizardParams.insert("Position z", QVariant::fromValue(position_e3.z()));
    endcap_3->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    endcap_3->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    endcap_3->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    endcap_3->wizardParams.insert("l", QVariant::fromValue(u));
    endcap_3->wizardParams.insert("b", QVariant::fromValue(b3));
    endcap_3->wizardParams.insert("a", QVariant::fromValue(a2));
    endcap_3->wizardParams.insert("s", QVariant::fromValue(s));
    endcap_3->processWizardInput();
    endcap_3->calculate();

    flange_3->wizardParams.insert("Position x", QVariant::fromValue(position_e3.x()));
    flange_3->wizardParams.insert("Position y", QVariant::fromValue(position_e3.y()));
    flange_3->wizardParams.insert("Position z", QVariant::fromValue(position_e3.z()));
    flange_3->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_3->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_3->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    flange_3->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_3->wizardParams.insert("b", QVariant::fromValue(b3 + 2 * ff));
    flange_3->wizardParams.insert("a", QVariant::fromValue(a2 + 2 * ff));
    flange_3->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_3->processWizardInput();
    flange_3->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position_e2);
    this->snap_flanges.append(position_e3);

    splitPoint[0] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (f - a2) / 2);
    splitPoint[1] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (a + f) / 2);

    boundingBox.enterVertex(flange_1->pos_bot_1);
    boundingBox.enterVertex(flange_1->pos_bot_2);
    boundingBox.enterVertex(flange_1->pos_bot_3);
    boundingBox.enterVertex(flange_1->pos_bot_4);
    boundingBox.enterVertex(flange_1->pos_top_1);
    boundingBox.enterVertex(flange_1->pos_top_2);
    boundingBox.enterVertex(flange_1->pos_top_3);
    boundingBox.enterVertex(flange_1->pos_top_4);

    boundingBox.enterVertex(flange_2->pos_bot_1);
    boundingBox.enterVertex(flange_2->pos_bot_2);
    boundingBox.enterVertex(flange_2->pos_bot_3);
    boundingBox.enterVertex(flange_2->pos_bot_4);
    boundingBox.enterVertex(flange_2->pos_top_1);
    boundingBox.enterVertex(flange_2->pos_top_2);
    boundingBox.enterVertex(flange_2->pos_top_3);
    boundingBox.enterVertex(flange_2->pos_top_4);


    boundingBox.enterVertex(flange_3->pos_bot_1);
    boundingBox.enterVertex(flange_3->pos_bot_2);
    boundingBox.enterVertex(flange_3->pos_bot_3);
    boundingBox.enterVertex(flange_3->pos_bot_4);
    boundingBox.enterVertex(flange_3->pos_top_1);
    boundingBox.enterVertex(flange_3->pos_top_2);
    boundingBox.enterVertex(flange_3->pos_top_3);
    boundingBox.enterVertex(flange_3->pos_top_4);

    this->snap_vertices.append(endcap_1->pos_bot_1);
    this->snap_vertices.append(endcap_1->pos_bot_4);
    this->snap_vertices.append(endcap_1->pos_top_1);
    this->snap_vertices.append(endcap_1->pos_top_4);
    this->snap_vertices.append(endcap_2->pos_bot_2);
    this->snap_vertices.append(endcap_2->pos_bot_3);
    this->snap_vertices.append(endcap_2->pos_top_2);
    this->snap_vertices.append(endcap_2->pos_top_3);
    this->snap_vertices.append(endcap_3->pos_bot_2);
    this->snap_vertices.append(endcap_3->pos_bot_3);
    this->snap_vertices.append(endcap_3->pos_top_2);
    this->snap_vertices.append(endcap_3->pos_top_3);

}

void CAD_air_ductYpiece::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a  = wizardParams.value("a").toDouble();
    b  = wizardParams.value("b").toDouble();
    a2  = wizardParams.value("a2").toDouble();
    b2  = wizardParams.value("b2").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    b3 = wizardParams.value("b3").toDouble();
    l = wizardParams.value("l").toDouble();
    m = wizardParams.value("m").toDouble();
    u = wizardParams.value("u").toDouble();
    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();


}
