#include "cad_basic_3Dface.h"
#include <QDebug>
#include <QPen>

CAD_basic_3Dface::CAD_basic_3Dface() : CADitem(CADitem::Basic_Face)
{
    this->description = "Basic|3D face";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_basic_3Dface::~CAD_basic_3Dface()
{

}

QList<CADitem::ItemType> CAD_basic_3Dface::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_3Dface::wizardImage()
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

void CAD_basic_3Dface::calculate()
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

//    qreal min_x;
//    qreal min_y;
//    qreal min_z;
//    qreal max_x;
//    qreal max_y;
//    qreal max_z;

    int i = 0;


    foreach(Vertex vertex, vertices)
    {
        if (i == 0)
        {
//            min_x = vertex.pos.x();
//            min_y = vertex.pos.y();
//            min_z = vertex.pos.z();
//            max_x = vertex.pos.x();
//            max_y = vertex.pos.y();
//            max_z = vertex.pos.z();
            this->position = vertex.pos;
            this->snap_basepoint = vertex.pos;
        }
//        else
//        {
//            min_x = qMin(min_x, vertex.pos.x());
//            min_y = qMin(min_y, vertex.pos.y());
//            min_z = qMin(min_z, vertex.pos.z());
//            max_x = qMax(max_x, vertex.pos.x());
//            max_y = qMax(max_y, vertex.pos.y());
//            max_z = qMax(max_z, vertex.pos.z());
//        }
        this->snap_vertices.append(vertex.pos);
        this->boundingBox.enterVertex(vertex.pos);
        i++;
    }

//    this->boundingBox.p0 = QVector3D(min_x, min_y, min_z);
//    this->boundingBox.a1 = QVector3D(max_x, 0.0, 0.0);
//    this->boundingBox.a2 = QVector3D(0.0, max_y, 0.0);
//    this->boundingBox.a3 = QVector3D(0.0, 0.0, max_z);

    //    this->snap_center = ((M3dVector)(M3dVector(p1) *0.5 + M3dVector(p2) * 0,5)).toM3dPoint();
}

void CAD_basic_3Dface::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}

void CAD_basic_3Dface::paint(GLWidget *glwidget)
{

}
