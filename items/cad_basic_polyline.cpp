#include "cad_basic_polyline.h"
#include <QDebug>
#include <QPen>

CAD_basic_polyline::CAD_basic_polyline() : CADitem(CADitem::Basic_Polyline)
{
    this->description = "Basic|Polyline";

    this->widthByBlock = false;
    this->widthByLayer = true;

    processWizardInput();
    calculate();
}

CAD_basic_polyline::~CAD_basic_polyline()
{

}

QList<CADitem::ItemType> CAD_basic_polyline::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_polyline::wizardImage()
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

void CAD_basic_polyline::calculate()
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

    //    this->snap_center = ((M3dVector)(M3dVector(p1) *0.5 + M3dVector(p2) * 0,5)).toM3dPoint();
}

void CAD_basic_polyline::processWizardInput()
{

}
