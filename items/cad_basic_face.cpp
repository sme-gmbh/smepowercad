#include "cad_basic_face.h"
#include "itemdb.h"
#include "glwidget.h"
#include <QDebug>
#include <QPen>

CAD_basic_face::CAD_basic_face() : CADitem(CADitemTypes::Basic_Face)
{
    widthByLayer = false;
    widthByBlock = false;

    this->wizardParams.insert("Position x1", (0.0));
    this->wizardParams.insert("Position y1", (0.0));
    this->wizardParams.insert("Position z1", (0.0));

    this->wizardParams.insert("Width", (1.0));
    processWizardInput();
    calculate();
}

CAD_basic_face::~CAD_basic_face()
{

}

QList<CADitemTypes::ItemType> CAD_basic_face::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_face::wizardImage()
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

QString CAD_basic_face::iconPath()
{
    return ":/icons/cad_basic/cad_basic_3Dface.svg";
}

QString CAD_basic_face::domain()
{
    return "Basic";
}

QString CAD_basic_face::description()
{
    return "Basic|Face";
}

void CAD_basic_face::calculate()
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
}

void CAD_basic_face::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}

void CAD_basic_face::paint(GLWidget *glwidget)
{

}
