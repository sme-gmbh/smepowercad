#ifndef CAD_BASIC_LINE_H
#define CAD_BASIC_LINE_H

#include "caditem.h"

class CAD_basic_line : public CADitem
{
public:
    CAD_basic_line();
    virtual ~CAD_basic_line();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
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

#endif // CADLINE_H
