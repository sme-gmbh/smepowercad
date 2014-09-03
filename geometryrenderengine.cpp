#include "geometryrenderengine.h"
#include "geometrydisplay.h"

#include <QDebug>
#include <QPen>
#include <QColor>
#include <QVector3D>

GeometryRenderengine::GeometryRenderengine(QObject *parent, ItemDB *itemDB) :
    QThread(parent)
{
    this->itemDB = itemDB;  // ItemDB is needed later for block retrieval
    framebufferImage = NULL;
    frameBufferSize = QSize(0, 0);
    frameBufferWidth = 0;
    frameBufferHeight = 0;
    frameBufferSizeNew = QSize(1, 0);    // Set sizes different, so the generation of the frameBufferImage is forced at first render call
    connect(this, SIGNAL(finished()), this, SLOT(slot_renderingDone()));
}

GeometryRenderengine::~GeometryRenderengine()
{
    if (framebufferImage != NULL)
        delete framebufferImage;
}

void GeometryRenderengine::setFrameBufferImageSize(QSize size)
{
    frameBufferSizeNew = size;
}

void GeometryRenderengine::addLayer(Layer* layer)
{
    this->layers.append(layer);
}

void GeometryRenderengine::renderLayers(QPoint translationOffset, qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view)
{
    if (frameBufferSizeNew != frameBufferSize)
    {
        frameBufferSize = frameBufferSizeNew;
        if (framebufferImage != NULL)
            delete framebufferImage;
        this->framebufferImage = new QImage(frameBufferSize, QImage::Format_ARGB32_Premultiplied);
        this->depthbufferImage.reserve(frameBufferSize.width() * frameBufferSize.height());
        this->frameBufferWidth = frameBufferSize.width();
        this->frameBufferHeight = frameBufferSize.height();
    }
//    this->layers = layers;
    this->translationOffset = translationOffset;
    this->zoomFactor = zoomFactor;
    this->centerOfViewInScene = centerOfViewInScene;
    this->displayCenter = displayCenter;
    this->cuttingplane = cuttingplane;
    this->height_of_intersection = height_of_intersection;
    this->depth_of_view = depth_of_view;
    this->start();
}

void GeometryRenderengine::run()
{
//    qDebug() << "GeometryRenderengine: run";

    framebufferImage->fill(Qt::transparent);
    QPainter bufferPainter(framebufferImage);
//    bufferPainter.setCompositionMode(QPainter::CompositionMode_Lighten);
    bufferPainter.setRenderHints(
                QPainter::Antialiasing | QPainter::TextAntialiasing |
                QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform, true);

    bufferPainter.translate(translationOffset);
    paintContent(&bufferPainter, layers);

    bufferPainter.end();
}

void GeometryRenderengine::slot_renderingDone()
{
    emit signal_renderingDone(framebufferImage);
    layers.clear();
//    qDebug() << "GeometryRenderengine: finished";
}

QPointF GeometryRenderengine::mapFromScene(QVector3D scenePoint)
{
//    return ((GeometryDisplay*)this->parent())->mapFromScene(scenePoint);
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

//    displayCoords *= zoomFactor;
    displayCoords += QPointF(displayCenter);

    return displayCoords;

}

qreal GeometryRenderengine::heightInScene(QVector3D scenePoint)
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
    return 0.0;
}


void GeometryRenderengine::paintContent(QPainter *painter, QList<Layer*> layers)
{
//    qDebug() << "GeometryRenderengine::paintContent: painting"<< layers.count() << "layers...";
    foreach (Layer* layer, layers)
    {
        if (!layer->on)
            continue;

        foreach (CADitem* item, layer->items)
        {

//            // Exclude all items from painting that do not reach the canvas with their boundingRect
//            QRect paintWindow = painter->window();
//            QVector3D p1 = item->boundingBox.p0;
//            QVector3D p2 = item->boundingBox.p0 + item->boundingBox.a1 + item->boundingBox.a2 + item->boundingBox.a3;
//            if (!paintWindow.contains(QRectF(mapFromScene(p1), mapFromScene(p2)).toRect()))
//                continue;

            // Paint it
            switch (item->getType())
            {
            case CADitem::Point:
                break;
            case CADitem::Line:
                paintLine2(painter, layer, (CADline*)item);
                break;
            case CADitem::Polyline:
                paintPolyLine(painter, layer, (CADpolyline*)item);
                break;
            case CADitem::Circle:
                break;
            case CADitem::Arc:
                break;
            case CADitem::Face:
                break;
            case CADitem::Plane:
                break;
            case CADitem::Box:
                break;
            case CADitem::Cylinder:
                break;
            case CADitem::Sphere:
                break;
            case CADitem::LevelSlab:
                break;
            case CADitem::Wall_loadBearing:
                break;
            case CADitem::Wall_nonLoadBearing:
                break;
            case CADitem::BlockOut:
                break;
            case CADitem::Door:
                break;
            case CADitem::Window:
                break;
            }
        }
        paintContent(painter, layer->subLayers);
    }
}

void GeometryRenderengine::paintPixel(int &x, int &y, qreal &depth, QRgb &color)
{
    if (depth < height_of_intersection)
        return;

    if (x < 0)
        return;

    if (y < 0)
        return;

    if (x > (frameBufferWidth - 1))
        return;

    if (y > (frameBufferHeight - 1))
        return;

    *((QRgb*)(this->framebufferImage->scanLine(y) + x * 4)) = color;
//    qDebug() << "paintPixel";
}

void GeometryRenderengine::paintLine(QPainter* painter, Layer* layer, CADline* item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0 * zoomFactor;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width * zoomFactor;
    }

    // Default width setting
    if (penWidth < 1.0)
        pen.setWidth(1);
    else
        pen.setWidthF(penWidth);

    painter->setPen(pen);

    //  Crop lines that exceed the paint area (heightOfIntersection to depthOfView)
    QVector3D p1 = item->p1;
    QVector3D p2 = item->p2;
    qreal h1 = heightInScene(p1);
    qreal h2 = heightInScene(p2);

    if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
        return;

    if (h1 < height_of_intersection)
    {
        // modify p1
        p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
    }
    else if (h2 < height_of_intersection)
    {
        // modify p2
        p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
    }

    painter->drawLine(mapFromScene(p1), mapFromScene(p2));
//    qDebug() << "GeometryRenderengine: Painting a line";
}

// Direct painting version with depth buffering
void GeometryRenderengine::paintLine2(QPainter* painter, Layer* layer, CADline* item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0 * zoomFactor;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width * zoomFactor;
    }

    // Default width setting
    if (penWidth < 1.0)
        pen.setWidth(1);
    else
        pen.setWidthF(penWidth);

//    painter->setPen(pen);

    //  Crop lines that exceed the paint area (heightOfIntersection to depthOfView)
    QVector3D p1 = item->p1;
    QVector3D p2 = item->p2;
    qreal h1 = heightInScene(p1);
    qreal h2 = heightInScene(p2);

    if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
        return;

    if (h1 < height_of_intersection)
    {
        // modify p1
        p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
    }
    else if (h2 < height_of_intersection)
    {
        // modify p2
        p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
    }

    // Points in buffer coords (screenpoints)
    QPoint sp1 = mapFromScene(p1).toPoint() + this->translationOffset;
    QPoint sp2 = mapFromScene(p2).toPoint() + this->translationOffset;

    // Check if line is inside scene
    if ((sp1.x() < 0) && (sp2.x() < 0))
        return;
    if ((sp1.x() > frameBufferWidth) && (sp2.x() > frameBufferWidth))
        return;
    if ((sp1.y() < 0) && (sp2.y() < 0))
        return;
    if ((sp1.y() > frameBufferHeight) && (sp2.y() > frameBufferHeight))
        return;


    // handle ortho lines
    // no antialiasing needed in this case
    qreal depth = 0.0;
    QRgb rgba = pen.color().rgba();
    int x0 = sp1.x();
    int y0 = sp1.y();
    int x1 = sp2.x();
    int y1 = sp2.y();

    if (x0 == x1)
    {
        for (int y = y0; y != y1;)
        {
            this->paintPixel(x0, y, depth, rgba);


            if (y0 < y1)
                y++;
            else
                y--;
        }
    }
    else if (y0 == y1)
    {
        for (int x = x0; x != x1;)
        {
            this->paintPixel(x, y0, depth, rgba);


            if (x0 < x1)
                x++;
            else
                x--;
        }
    }
    else    // non-ortho line
    {
        int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
        int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
        int err = dx+dy, e2; /* error value e_xy */

        for(;;)
        {  /* loop */
            //setPixel(x0,y0);
            this->paintPixel(x0, y0, depth, rgba);
            if (x0==x1 && y0==y1) break;
            e2 = 2*err;
            if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
            if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
        }
    }
}


void GeometryRenderengine::paintPolyLine(QPainter *painter, Layer *layer, CADpolyline *item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
    painter->setPen(pen);

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    foreach (CADpolyline::Vertex vertex, item->vertices)
    {
        if (p1 == p2)
        {
            p1 = vertex.pos;
        }
        else
        {
            p2 = vertex.pos;

            //  Crop lines that exceed the paint area (heightOfIntersection to depthOfView)
            qreal h1 = heightInScene(p1);
            qreal h2 = heightInScene(p2);

            if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
            {
                p1 = p2;
                continue;
            }

            if (h1 < height_of_intersection)
            {
                // modify p1
                p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
            }
            else if (h2 < height_of_intersection)
            {
                // modify p2
                p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
            }




            // Points in buffer coords (screenpoints)
            QPoint sp1 = mapFromScene(p1).toPoint() + this->translationOffset;
            QPoint sp2 = mapFromScene(p2).toPoint() + this->translationOffset;

            // Check if line is inside scene
            if ((sp1.x() < 0) && (sp2.x() < 0))
            {
                p1 = p2;
                continue;
            }
            if ((sp1.x() > frameBufferWidth) && (sp2.x() > frameBufferWidth))
            {
                p1 = p2;
                continue;
            }
            if ((sp1.y() < 0) && (sp2.y() < 0))
            {
                p1 = p2;
                continue;
            }
            if ((sp1.y() > frameBufferHeight) && (sp2.y() > frameBufferHeight))
            {
                p1 = p2;
                continue;
            }


            // handle ortho lines
            // no antialiasing needed in this case
            qreal depth = 0.0;
            QRgb rgba = pen.color().rgba();
            int x0 = sp1.x();
            int y0 = sp1.y();
            int x1 = sp2.x();
            int y1 = sp2.y();

            if (x0 == x1)
            {
                for (int y = y0; y != y1;)
                {
                    this->paintPixel(x0, y, depth, rgba);


                    if (y0 < y1)
                        y++;
                    else
                        y--;
                }
            }
            else if (y0 == y1)
            {
                for (int x = x0; x != x1;)
                {
                    this->paintPixel(x, y0, depth, rgba);


                    if (x0 < x1)
                        x++;
                    else
                        x--;
                }
            }
            else    // non-ortho line
            {
                int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
                int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
                int err = dx+dy, e2; /* error value e_xy */

                for(;;)
                {  /* loop */
                    //setPixel(x0,y0);
                    this->paintPixel(x0, y0, depth, rgba);
                    if (x0==x1 && y0==y1) break;
                    e2 = 2*err;
                    if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
                    if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
                }
            }




//            painter->drawLine(mapFromScene(p1), mapFromScene(p2));
            p1 = p2;
        }
    }


    //    qDebug() << "GeometryRenderengine: Painting a polyline";
}

void GeometryRenderengine::paintFace(QPainter *painter, Layer *layer, CAD3Dface *item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    QColor color = item->color;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
    painter->setPen(pen);

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    foreach (CAD3Dface::Vertex vertex, item->vertices)
    {
        if (p1 == p2)
        {
            p1 = vertex.pos;
        }
        else
        {
            p2 = vertex.pos;

        }
    }
}

void GeometryRenderengine::renderTriangle(int x0, int x1, int x2, int y0, int y1, int y2, qreal d0, qreal d1, qreal d2, QRgb &colorPen, QRgb &colorBrush)
{

}
