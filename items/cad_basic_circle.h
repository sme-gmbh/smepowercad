#ifndef CAD_BASIC_CIRCLE_H
#define CAD_BASIC_CIRCLE_H

#include "caditem.h"

class CAD_basic_circle : public CADitem
{
public:
    CAD_basic_circle();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    QVector3D center;
    qreal radius;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;
};

#endif // CAD_BASIC_CIRCLE_H
