#ifndef CAD_BASIC_TURN_H
#define CAD_BASIC_TURN_H

#include "caditem.h"

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class CAD_basic_turn : public CADitem
{
public:
    CAD_basic_turn();
    virtual ~CAD_basic_turn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);


    qreal radius_pipe;
    qreal radius_turn;
    qreal angle_turn;
    qreal wallThickness;

//    QVector3D vertices[2][11][21];
//    QVector3D direction;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_BASIC_TURN_H
