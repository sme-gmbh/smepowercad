#ifndef CAD_BASIC_ARC_H
#define CAD_BASIC_ARC_H

#include "caditem.h"
#include <QList>
#include <qmath.h>

class CAD_basic_arc : public CADitem
{
public:
    CAD_basic_arc();
    virtual ~CAD_basic_arc();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();


    QVector3D center;
    qreal radius;
    qreal centralAngle;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;
    QList<QVector3D> arc;

};

#endif // CAD_BASIC_ARC_H
