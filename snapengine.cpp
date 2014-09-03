#include "snapengine.h"

SnapEngine::SnapEngine(ItemDB *itemDB, QObject *parent) :
    QThread(parent)
{
    this->itemDB = itemDB;
}

SnapEngine::~SnapEngine()
{

}

void SnapEngine::setUp(qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, SnapEngine::CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view)
{
    snap_vertex_points.clear();

    this->zoomFactor = zoomFactor;
    this->centerOfViewInScene = centerOfViewInScene;
    this->displayCenter = displayCenter;
    this->cuttingplane = cuttingplane;
    this->height_of_intersection = height_of_intersection;
    this->depth_of_view = depth_of_view;
}

QPointF SnapEngine::mapFromScene(QVector3D scenePoint)
{
    QPointF displayCoords;

    scenePoint -= centerOfViewInScene;
    scenePoint *= zoomFactor;

    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            displayCoords.setX(scenePoint.z());
            displayCoords.setY(-scenePoint.y());
            break;
        }
        case CuttingPlane_nX:
        {
            displayCoords.setX(scenePoint.y());
            displayCoords.setY(-scenePoint.z());
            break;
        }
        case CuttingPlane_pY:
        {
            displayCoords.setX(scenePoint.x());
            displayCoords.setY(-scenePoint.z());
            break;
        }
        case CuttingPlane_nY:
        {
            displayCoords.setX(scenePoint.z());
            displayCoords.setY(-scenePoint.x());
            break;
        }
        case CuttingPlane_pZ:
        {
            displayCoords.setX(scenePoint.y());
            displayCoords.setY(-scenePoint.x());
            break;
        }
        case CuttingPlane_nZ:
        {
            displayCoords.setX(scenePoint.x());
            displayCoords.setY(-scenePoint.y());
            break;
        }
        case CuttingPlane_ISO_001:
        {
            displayCoords.setX(scenePoint.x() - 0.5 * scenePoint.y());
            displayCoords.setY(-scenePoint.z() + 0.5 * scenePoint.y());
            break;
        }
    }

    displayCoords += QPointF(displayCenter);

    return displayCoords;
}

qreal SnapEngine::heightInScene(QVector3D scenePoint)
{
    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            return scenePoint.x();
            break;
        }
        case CuttingPlane_nX:
        {
            return scenePoint.x();
            break;
        }
        case CuttingPlane_pY:
        {
            return scenePoint.y();
            break;
        }
        case CuttingPlane_nY:
        {
            return scenePoint.y();
            break;
        }
        case CuttingPlane_pZ:
        {
            return scenePoint.z();
            break;
        }
        case CuttingPlane_nZ:
        {
            return scenePoint.z();
            break;
        }
        case CuttingPlane_ISO_001:
        {
            return scenePoint.y();
            break;
        }
    }
}

void SnapEngine::run()
{

}

void SnapEngine::snapProcessing(QList<Layer*> layers, QPoint mousePos)
{
    foreach (Layer* layer, layers)
    {
        if (!layer->on)
            continue;

        foreach (CADitem* item, layer->items)
        {
            foreach (QVector3D snap_vertex, item->snap_vertices)
            {
                qreal h1 = heightInScene(snap_vertex);
                if (h1 < height_of_intersection)
                    continue;

                if ((mapFromScene(snap_vertex) - mousePos).manhattanLength() < 10)
                    snap_vertex_points.append(mapFromScene(snap_vertex));
            }
        }
        snapProcessing(layer->subLayers, mousePos);
    }
}

