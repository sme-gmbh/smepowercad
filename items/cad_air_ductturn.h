#ifndef CAD_AIR_DUCTTURN_H
#define CAD_AIR_DUCTTURN_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTurn : public CADitem
{
public:
    CAD_air_ductTurn();
    virtual ~CAD_air_ductTurn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    qreal s;
    qreal ff, fe;

    qreal r;
    qreal b;
    qreal b2;
    qreal a;
    qreal alpha;
    qreal  g;
    qreal e;

//    QVector3D vertices[2][13][5];

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

    CAD_basic_duct *flange_left_duct;
    CAD_basic_duct *flange_right_duct;
};

#endif // CAD_AIR_DUCTTURN_H
