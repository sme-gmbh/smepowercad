#ifndef CAD_BASIC_FACE_H
#define CAD_BASIC_FACE_H

#include "caditem.h"

class CAD_basic_face : public CADitem
{
public:
    CAD_basic_face();
    virtual ~CAD_basic_face();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    QVector3D p1;
    QVector3D p2;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;

private:

};

#endif //CAD_BASIC_FACE_H
