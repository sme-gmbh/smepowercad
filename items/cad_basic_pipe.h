#ifndef CAD_BASIC_PIPE_H
#define CAD_BASIC_PIPE_H

#include "caditem.h"

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class CAD_basic_pipe : public CADitem
{
public:
    CAD_basic_pipe();
    virtual ~CAD_basic_pipe();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

   QMatrix4x4 matrix_rotation;

    qreal radius;
    qreal length;
    qreal wallThickness;
    QVector3D direction;

    QList<QVector3D> vertices_outer_bottom;
    QList<QVector3D> vertices_inner_bottom;

    QList<QVector3D> vertices_outer_top;
    QList<QVector3D> vertices_inner_top;

};

#endif // CAD_BASIC_PIPE_H
