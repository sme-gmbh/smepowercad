#ifndef CAD_BASIC_POLYLINE_H
#define CAD_BASIC_POLYLINE_H

#include "caditem.h"

class CAD_basic_polyline : public CADitem
{
public:
    CAD_basic_polyline();
    virtual ~CAD_basic_polyline();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    class Vertex{
    public:
        QVector3D pos;
        qreal bulge;
        qreal widthStart;
        qreal widthEnd;
    };

    QList<Vertex> vertices;
    bool widthByLayer;
    bool widthByBlock;

private:

};

#endif // CAD_BASIC_POLYLINE_H
