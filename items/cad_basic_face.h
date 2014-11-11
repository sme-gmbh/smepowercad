#ifndef CAD_BASIC_FACE_H
#define CAD_BASIC_FACE_H

#include "caditem.h"

class CAD_basic_face : public CADitem
{
public:
    CAD_basic_face();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    QVector3D p1;
    QVector3D p2;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;

private:

};

#endif //CAD_BASIC_FACE_H
