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
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();


    qreal radius_pipe;
    qreal radius_turn;
    qreal angle_turn;
    qreal wallThickness;

    QVector3D vertices[2][11][21];
//    QVector3D direction;
};

#endif // CAD_BASIC_TURN_H
