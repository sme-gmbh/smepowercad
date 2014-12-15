#include "cad_arch_beam.h"

CAD_arch_beam::CAD_arch_beam() : CADitem(CADitem::Arch_Beam)
{
    this->beam = new CAD_basic_box();
    this->subItems.append(beam);

    this->description = "Architecture|Beam";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("a", QVariant::fromValue(500.0));
    wizardParams.insert("b", QVariant::fromValue(200.0));
    wizardParams.insert("l", QVariant::fromValue(5000.0));

    processWizardInput();
    calculate();
}

CAD_arch_beam::~CAD_arch_beam()
{

}

QList<CADitem::ItemType> CAD_arch_beam::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_beam::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_arch_beam::calculate()
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

    QVector3D position_b = position + matrix_rotation * QVector3D(l / 2, b / 2, -a / 2);
    beam->wizardParams.insert("Position x", QVariant::fromValue(position_b.x()));
    beam->wizardParams.insert("Position y", QVariant::fromValue(position_b.y()));
    beam->wizardParams.insert("Position z", QVariant::fromValue(position_b.z()));
    beam->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    beam->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    beam->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    beam->wizardParams.insert("Size x", QVariant::fromValue(l));
    beam->wizardParams.insert("Size y", QVariant::fromValue(b));
    beam->wizardParams.insert("Size z", QVariant::fromValue(a));
    beam->processWizardInput();
    beam->calculate();

    this->snap_vertices.append(beam->snap_vertices);

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0,   b, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  l,   b, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  l, 0.0, 0.0));

    foreach(QVector3D vec, beam->snap_vertices)
        boundingBox.enterVertex(vec);
}

void CAD_arch_beam::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();



}
