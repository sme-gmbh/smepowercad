#include "cad_arch_blockout.h"
#include "itemdb.h"

CAD_arch_blockOut::CAD_arch_blockOut() : CADitem(CADitemTypes::Arch_BlockOut)
{
    this->blockout = new CAD_basic_box();
    this->subItems.append(blockout);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("g", 30.0);
    wizardParams.insert("h", 20.0);
    wizardParams.insert("b",  5.0);

    processWizardInput();
    calculate();
}

CAD_arch_blockOut::~CAD_arch_blockOut()
{

}

QList<CADitemTypes::ItemType> CAD_arch_blockOut::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_blockOut::wizardImage()
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

QString CAD_arch_blockOut::iconPath()
{
    return ":/icons/cad_arch/cad_arch_blockout.svg";
}

QString CAD_arch_blockOut::domain()
{
    return "Architecture";
}

QString CAD_arch_blockOut::description()
{
    return "Architecture|Blockout";
}

void CAD_arch_blockOut::calculate()
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

    qDebug() << g << h << b;
    QVector3D position_b = position + matrix_rotation * QVector3D(b / 2, g / 2, h / 2);
    blockout->wizardParams.insert("Position x", (position_b.x()));
    blockout->wizardParams.insert("Position y", (position_b.y()));
    blockout->wizardParams.insert("Position z", (position_b.z()));
    blockout->wizardParams.insert("Angle x", (angle_x));
    blockout->wizardParams.insert("Angle y", (angle_y));
    blockout->wizardParams.insert("Angle z", (angle_z));
    blockout->wizardParams.insert("Size x", (b));
    blockout->wizardParams.insert("Size y", (g));
    blockout->wizardParams.insert("Size z", (h));
    blockout->processWizardInput();
    blockout->calculate();

    this->snap_vertices.append(blockout->snap_vertices);

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0,   g, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  b,   g, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  b, 0.0, 0.0));

    foreach(QVector3D vec, blockout->snap_vertices)
        boundingBox.enterVertex(vec);
}

void CAD_arch_blockOut::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    g = wizardParams.value("g").toDouble();
    h = wizardParams.value("h").toDouble();
    b = wizardParams.value("b").toDouble();


}
