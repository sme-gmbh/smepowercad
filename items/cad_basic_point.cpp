#include "cad_basic_point.h"
#include "itemdb.h"

CAD_basic_point::CAD_basic_point() : CADitem(CADitemTypes::Basic_Point)
{
    wizardParams.insert("Position x", (0.0));
    wizardParams.insert("Position y", (0.0));
    wizardParams.insert("Position z", (0.0));

    processWizardInput();
    calculate();
}

CAD_basic_point::~CAD_basic_point()
{

}

QList<CADitemTypes::ItemType> CAD_basic_point::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_point::wizardImage()
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

QString CAD_basic_point::iconPath()
{
    return ":/icons/cad_basic/cad_basic_point.svg";
}

QString CAD_basic_point::domain()
{
    return "Basic";
}

QString CAD_basic_point::description()
{
    return "Basic|Point";
}

void CAD_basic_point::calculate()
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

void CAD_basic_point::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
}

void CAD_basic_point::paint(GLWidget *glwidget)
{

}
