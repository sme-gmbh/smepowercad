#include "cad_arch_levelslab.h"

CAD_arch_levelSlab::CAD_arch_levelSlab() : CADitem(CADitem::Arch_LevelSlab)
{
    this->description = "Architecture|Level slab";
    slab = new CAD_basic_box();
    subItems.append(slab);
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("a", QVariant::fromValue(500));
    wizardParams.insert("b", QVariant::fromValue(4000));
    wizardParams.insert("l", QVariant::fromValue(4000));

    processWizardInput();
    calculate();
}

CAD_arch_levelSlab::~CAD_arch_levelSlab()
{

}

QList<CADitem::ItemType> CAD_arch_levelSlab::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_levelSlab::wizardImage()
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

void CAD_arch_levelSlab::calculate()
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
    QVector3D position_sl = position + matrix_rotation * (QVector3D(l/2, b/2, a/2));
    slab->wizardParams.insert("Position x", QVariant::fromValue(position_sl.x()));
    slab->wizardParams.insert("Position y", QVariant::fromValue(position_sl.y()));
    slab->wizardParams.insert("Position z", QVariant::fromValue(position_sl.z()));
    slab->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    slab->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    slab->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    slab->wizardParams.insert("Size x", QVariant::fromValue(l));
    slab->wizardParams.insert("Size y", QVariant::fromValue(b));
    slab->wizardParams.insert("Size z", QVariant::fromValue(a));
    slab->processWizardInput();
    slab->calculate();

    this->snap_vertices.append(slab->snap_vertices);

    this->boundingBox = slab->boundingBox;
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, b)));
}

void CAD_arch_levelSlab::processWizardInput()
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
