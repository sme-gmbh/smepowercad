#include "cad_arch_support.h"

CAD_arch_support::CAD_arch_support() : CADitem(CADitem::Arch_Support)
{
    this->description = "Architecture|Support";
    support = new CAD_basic_box();
    this->subItems.append(support);
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("a", QVariant::fromValue(3000));
    wizardParams.insert("b", QVariant::fromValue(300));
    wizardParams.insert("l", QVariant::fromValue(300));

    processWizardInput();
    calculate();
}

CAD_arch_support::~CAD_arch_support()
{

}

QList<CADitem::ItemType> CAD_arch_support::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_support::wizardImage()
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

void CAD_arch_support::calculate()
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

    QVector3D position_sp = position + matrix_rotation * (QVector3D(0.0, 0.0, a/2));
    support->wizardParams.insert("Position x", QVariant::fromValue(position_sp.x()));
    support->wizardParams.insert("Position y", QVariant::fromValue(position_sp.y()));
    support->wizardParams.insert("Position z", QVariant::fromValue(position_sp.z()));
    support->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    support->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    support->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    support->wizardParams.insert("Size x", QVariant::fromValue(l));
    support->wizardParams.insert("Size y", QVariant::fromValue(b));
    support->wizardParams.insert("Size z", QVariant::fromValue(a));
    support->processWizardInput();
    support->calculate();

    this->snap_vertices.append(support->snap_vertices);
    this->boundingBox = support->boundingBox;
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, b)));
}

void CAD_arch_support::processWizardInput()
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
