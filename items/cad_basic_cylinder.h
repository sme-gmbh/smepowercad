#ifndef CAD_BASIC_CYLINDER_H
#define CAD_BASIC_CYLINDER_H

#include "caditem.h"
#include <QList>

class CAD_basic_cylinder : public CADitem
{
public:
    CAD_basic_cylinder();
    virtual ~CAD_basic_cylinder();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    qreal radius;
    QVector3D center_base;
    qreal height;
    QList<QVector3D> vertices_bottom, vertices_top;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;


};

#endif // CADCYLINDER_H
