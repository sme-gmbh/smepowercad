#ifndef CAD_mainuu_subuu_H
#define CAD_mainuu_subuu_H

#include "caditem.h"

class CAD_mainll_subll : public CADitem
{
public:
    CAD_mainll_subll();
    virtual ~CAD_mainll_subll();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
//    virtual void paint(GLWidget* glwidget);

//    QOpenGLBuffer arrayBufVertices;
//    QOpenGLBuffer indexBufFaces;
//    QOpenGLBuffer indexBufLines;
};

#endif // CAD_mainuu_subuu_H
