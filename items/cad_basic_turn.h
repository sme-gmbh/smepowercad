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
    virtual void calculate();
    virtual void processWizardInput();

    QMatrix4x4 matrix_rotation;

    qreal radius_pipe;
    qreal radius_turn;
    qreal angle_turn;
    qreal wallThickness;

    QVector3D vertices[2][21][51];
//    QVector3D direction;
};

#endif // CAD_BASIC_TURN_H
