#include "cad_arch_boredPile.h"
#include "itemdb.h"

CAD_arch_boredPile::CAD_arch_boredPile() : CADitem(CADitemTypes::Arch_BoredPile)
{
    this->pile = new CAD_basic_pipe();
    this->subItems.append(this->pile);
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));


    wizardParams.insert("d", QVariant::fromValue(1000));
    wizardParams.insert("a", QVariant::fromValue(5000));

    processWizardInput();
    calculate();
}

CAD_arch_boredPile::~CAD_arch_boredPile()
{

}

QList<CADitemTypes::ItemType> CAD_arch_boredPile::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Arch_BoredPile);
    return flangable_items;
}

QImage CAD_arch_boredPile::wizardImage()
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

QString CAD_arch_boredPile::iconPath()
{
    return ":/icons/cad_arch/cad_arch_bored_pile.svg";
}

QString CAD_arch_boredPile::domain()
{
    return "Architecture";
}

QString CAD_arch_boredPile::description()
{
    return "Architecture|BoredPile";
}

void CAD_arch_boredPile::calculate()
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
    this->snap_flanges.append(position);

    pile->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    pile->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    pile->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    pile->wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    pile->wizardParams.insert("Angle y", QVariant::fromValue(90.0));
    pile->wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    pile->wizardParams.insert("l", QVariant::fromValue(a));
    pile->wizardParams.insert("d", QVariant::fromValue(d));
    pile->wizardParams.insert("s", QVariant::fromValue(d/2));
    pile->processWizardInput();
    pile->calculate();

    this->snap_flanges.append(pile->snap_flanges);

}

void CAD_arch_boredPile::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();

}
