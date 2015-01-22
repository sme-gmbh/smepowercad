#ifndef CAD_BASIC_SPHERE_H
#define CAD_BASIC_SPHERE_H

#include "caditem.h"

class CAD_basic_sphere : public CADitem
{
public:
    CAD_basic_sphere();
    virtual ~CAD_basic_sphere();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

//    QVector3D pos;
    qreal radius;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_BASIC_SPHERE_H
