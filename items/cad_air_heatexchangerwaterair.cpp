#include "cad_air_heatexchangerwaterair.h"

CAD_air_heatExchangerWaterAir::CAD_air_heatExchangerWaterAir() : CADitem(CADitem::Air_HeatExchangerWaterAir)
{
    this->duct = new CAD_air_duct();
    this->pipe3 = new CAD_basic_pipe();
    this->pipe4 = new CAD_basic_pipe();
    this->exchanger = new CAD_basic_box();
    this->subItems.append(duct);
    this->subItems.append(pipe3);
    this->subItems.append(pipe4);
    this->subItems.append(exchanger);

    this->description = "Air|Heat exchanger water/air";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));

    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("l3", QVariant::fromValue(30.0));
    wizardParams.insert("l4", QVariant::fromValue(70.0));
    wizardParams.insert("d", QVariant::fromValue(20.0));
    wizardParams.insert("j", QVariant::fromValue(40.0));
    wizardParams.insert("m3", QVariant::fromValue(20.0));
    wizardParams.insert("m4", QVariant::fromValue(40.0));
    wizardParams.insert("a", QVariant::fromValue(100.0));
    wizardParams.insert("b", QVariant::fromValue(100.0));

    processWizardInput();
    calculate();
}

CAD_air_heatExchangerWaterAir::~CAD_air_heatExchangerWaterAir()
{

}

QList<CADitem::ItemType> CAD_air_heatExchangerWaterAir::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_heatExchangerWaterAir::wizardImage()
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

void CAD_air_heatExchangerWaterAir::calculate()
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

    this->duct->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    this->duct->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    this->duct->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    this->duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    this->duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    this->duct->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    this->duct->wizardParams.insert("s", QVariant::fromValue(s));
    this->duct->wizardParams.insert("l", QVariant::fromValue(l));
    this->duct->wizardParams.insert("b", QVariant::fromValue(b));
    this->duct->wizardParams.insert("a", QVariant::fromValue(a));
    this->duct->wizardParams.insert("ff", QVariant::fromValue(ff));
    this->duct->wizardParams.insert("fe", QVariant::fromValue(fe));

    this->duct->processWizardInput();
    this->duct->calculate();

    QVector3D position_p3 = position + matrix_rotation *  QVector3D(l3, -b/2, -a/2 + m3);
    pipe3->wizardParams.insert("Position x", QVariant::fromValue(position_p3.x()));
    pipe3->wizardParams.insert("Position y", QVariant::fromValue(position_p3.y()));
    pipe3->wizardParams.insert("Position z", QVariant::fromValue(position_p3.z()));
    pipe3->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    pipe3->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    pipe3->wizardParams.insert("Angle z", QVariant::fromValue(angle_z-90));
    pipe3->wizardParams.insert("l", QVariant::fromValue(j));
    pipe3->wizardParams.insert("d", QVariant::fromValue(d));
    pipe3->wizardParams.insert("s", QVariant::fromValue(s));

    pipe3->processWizardInput();
    pipe3->calculate();

    QVector3D position_p4 = position + matrix_rotation *  QVector3D(l4, -b/2, -a/2 + m4);
    pipe4->wizardParams.insert("Position x", QVariant::fromValue(position_p4.x()));
    pipe4->wizardParams.insert("Position y", QVariant::fromValue(position_p4.y()));
    pipe4->wizardParams.insert("Position z", QVariant::fromValue(position_p4.z()));
    pipe4->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    pipe4->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    pipe4->wizardParams.insert("Angle z", QVariant::fromValue(angle_z-90));
    pipe4->wizardParams.insert("l", QVariant::fromValue(j));
    pipe4->wizardParams.insert("d", QVariant::fromValue(d));
    pipe4->wizardParams.insert("s", QVariant::fromValue(s));

    pipe4->processWizardInput();
    pipe4->calculate();

    QVector3D position_p = position + matrix_rotation * QVector3D(l / 2, 0.0, 0.0);
    exchanger->wizardParams.insert("Position x", QVariant::fromValue(position_p.x()));
    exchanger->wizardParams.insert("Position y", QVariant::fromValue(position_p.y()));
    exchanger->wizardParams.insert("Position z", QVariant::fromValue(position_p.z()));
    exchanger->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    exchanger->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    exchanger->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    exchanger->wizardParams.insert("Size x", QVariant::fromValue(0.8 * l));
    exchanger->wizardParams.insert("Size y", QVariant::fromValue(0.8 * b));
    exchanger->wizardParams.insert("Size z", QVariant::fromValue(0.8 * a));
    exchanger->processWizardInput();
    exchanger->calculate();

    this->snap_flanges.append(duct->snap_flanges);
    this->snap_flanges.append(pipe3->snap_flanges.at(0));
    this->snap_flanges.append(pipe4->snap_flanges.at(0));

    this->boundingBox = duct->boundingBox;
    this->boundingBox.enterVertex(pipe3->snap_flanges.at(0));
    this->boundingBox.enterVertex(pipe4->snap_flanges.at(0));


}

void CAD_air_heatExchangerWaterAir::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    l = wizardParams.value("l").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    l4 = wizardParams.value("l4").toDouble();
    d = wizardParams.value("d").toDouble();
    j = wizardParams.value("j").toDouble();
    m3 = wizardParams.value("m3").toDouble();
    m4 = wizardParams.value("m4").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();

}
