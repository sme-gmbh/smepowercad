#ifndef CAD_BASIC_CIRCLE_H
#define CAD_BASIC_CIRCLE_H

#include "caditem.h"
#include <QList>

class CAD_basic_circle : public CADitem
{
public:
    CAD_basic_circle();
    virtual ~CAD_basic_circle();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    QVector3D center;
    qreal radius;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;
    //QList<QVector3D> circle;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_BASIC_CIRCLE_H
