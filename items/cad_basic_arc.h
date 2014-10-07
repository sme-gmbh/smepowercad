#ifndef CAD_BASIC_ARC_H
#define CAD_BASIC_ARC_H

#include "caditem.h"
#include <qmath.h>;

class CAD_basic_arc : public CADitem
{
public:
    CAD_basic_arc();
    virtual void calculate();
    virtual void processWizardInput();


    QVector3D center;
    qreal radius;
    qreal centralAngle;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;


};

#endif // CAD_BASIC_ARC_H
