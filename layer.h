#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QBrush>
#include <QPen>

#include "caditem.h"

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject *parent = 0);
    bool isEmpty();

    QString name;
    QPen pen;
    QBrush brush;
    bool on;
    bool writable;
    int width;
    QString lineType;
    QList<CADitem*> items;
    QList<Layer*> subLayers;
    Layer* parentLayer;

signals:

public slots:

};

#endif // LAYER_H
