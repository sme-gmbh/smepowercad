#ifndef CAD_BASIC_BOX_H
#define CAD_BASIC_BOX_H

#include "caditem.h"

class CAD_basic_box : public CADitem
{
public:
    CAD_basic_box();
    virtual ~CAD_basic_box();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

    QVector3D size;

    QVector3D normal_bot;
    QVector3D normal_top;
    QVector3D normal_left;
    QVector3D normal_right;
    QVector3D normal_front;
    QVector3D normal_back;
};

#endif // CAD_BASIC_BOX_H
