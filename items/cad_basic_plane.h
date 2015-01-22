#ifndef CAD_BASIC_PLANE_H
#define CAD_BASIC_PLANE_H

#include "caditem.h"

class CAD_basic_plane : public CADitem
{
public:
    CAD_basic_plane();
    virtual ~CAD_basic_plane();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    qreal a, b;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

};

#endif // CAD_BASIC_PLANE_H
