#include "geometrydisplay.h"

#include <QDebug>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <qmath.h>

GeometryDisplay::GeometryDisplay(ItemDB *itemDB, QWidget *parent) :
    QDockWidget(parent)
{
    qDebug() << "GeometryDisplay created";
    this->itemDB = itemDB;
    overlay = NULL;
    cuttingplane = CuttingPlane_nZ; // Look downwards
    height_of_intersection = 0.0;
    depth_of_view = 0.0;
    repaintNeeded = true;

    titleWidget = new GeometryDisplayTitle();
    connect(this, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), titleWidget, SLOT(slot_sceneCoordinatesChanged(QVector3D)));
    this->setTitleBarWidget(titleWidget);

    mousePosOld = QPoint();

    zoomFactor = 1.0;
    centerOfViewInScene = QVector3D();
    rot_x = rot_y = rot_z = 0.0f;

    // Framebuffer and Renderengine construction

    framebufferImage = NULL;
    framebufferImage_dBuffer = NULL;

    // Create 1(3) Renderengines
    for (int i=0; i<1; i++)
    {
        GeometryRenderengine* new_renderEngine = new GeometryRenderengine(this, itemDB);
        renderEngines.append(new_renderEngine);
        connect(new_renderEngine, SIGNAL(signal_renderingDone(QImage*)), this, SLOT(slot_renderEngineFinished(QImage*)));
    }

    // Create a SnapEngine
    this->snapEngine = new SnapEngine(itemDB, this);

    glwidget = new GLWidget(0, itemDB);
    //glwidget->resize(100, 100);
    this->setWidget(glwidget);

    //this->overlay = new Overlay(this);
    //this->overlay->resize(this->size());
    //this->setWidget(this->overlay);   // working, but looses space (titlebar)
    this->resize(400, 250);
    this->setFloating(false);
    this->setMinimumSize(400, 250);
    this->setMaximumSize(4096, 3072);   // 4k ultra hd
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

//    this->setAttribute(Qt::WA_OpaquePaintEvent);    // Background widget will not be rendered
    //this->setStyleSheet("color: rgb(200,200,255);");


    // TODO: Title bar widget
    //this->setTitleBarWidget();


    this->setMouseTracking(true);
    this->setWindowText();
}

GeometryDisplay::~GeometryDisplay()
{
    delete this->overlay;
    delete this->glwidget;
    if (framebufferImage != NULL)
        delete framebufferImage;

    if (framebufferImage_dBuffer != NULL)
        delete framebufferImage_dBuffer;

    foreach (GeometryRenderengine* renderEngine, renderEngines)
        delete renderEngine;

    delete snapEngine;

    qDebug() << "GeometryDisplay destroyed";
}

void GeometryDisplay::slot_timer_50ms_fired()
{
    // TODO: Nur wenn sich der Wert ändertt, d.h. auch nur, wenn sich der Cursor im Fenster befindet!
//    emit signal_selectionCountChanged(this->selectionCount());
}

void GeometryDisplay::slot_changeCuttingplane()
{
    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            cuttingplane = CuttingPlane_nX;
            break;
        }
        case CuttingPlane_nX:
        {
            cuttingplane = CuttingPlane_pY;
            break;
        }
        case CuttingPlane_pY:
        {
            cuttingplane = CuttingPlane_nY;
            break;
        }
        case CuttingPlane_nY:
        {
            cuttingplane = CuttingPlane_pZ;
            break;
        }
        case CuttingPlane_pZ:
        {
            cuttingplane = CuttingPlane_nZ;
            break;
        }
        case CuttingPlane_nZ:
        {
            cuttingplane = CuttingPlane_ISO_001;
            break;
        }
        case CuttingPlane_ISO_001:
        {
            cuttingplane = CuttingPlane_pX;
            break;
        }
    }

    setWindowText();
    slot_redrawScene();
}

void GeometryDisplay::setCuttingPlane(CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view)
{
    this->cuttingplane = cuttingplane;
    this->height_of_intersection = height_of_intersection;
    this->depth_of_view = depth_of_view;
    slot_redrawScene();
}

void GeometryDisplay::slot_redrawScene()
{
    repaintNeeded = true;
    //repaint();
    update();
}

QVector3D GeometryDisplay::mapToScene(QPoint displayPoint)
{
    QVector3D sceneCoords;

    displayPoint -= displayCenter;

    qreal scene_upwardAxis = (qreal)(-displayPoint.y()) / zoomFactor;
    qreal scene_sidewardAxis = (qreal)(displayPoint.x()) / zoomFactor;

    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            sceneCoords.setX(height_of_intersection);
            sceneCoords.setY(scene_upwardAxis);
            sceneCoords.setZ(scene_sidewardAxis);
            break;
        }
        case CuttingPlane_nX:
        {
            sceneCoords.setX(height_of_intersection);
            sceneCoords.setY(scene_sidewardAxis);
            sceneCoords.setZ(scene_upwardAxis);
            break;
        }
        case CuttingPlane_pY:
        {
            sceneCoords.setX(scene_sidewardAxis);
            sceneCoords.setY(height_of_intersection);
            sceneCoords.setZ(scene_upwardAxis);
            break;
        }
        case CuttingPlane_nY:
        {
            sceneCoords.setX(scene_upwardAxis);
            sceneCoords.setY(height_of_intersection);
            sceneCoords.setZ(scene_sidewardAxis);
            break;
        }
        case CuttingPlane_pZ:
        {
            sceneCoords.setX(scene_upwardAxis);
            sceneCoords.setY(scene_sidewardAxis);
            sceneCoords.setZ(height_of_intersection);
            break;
        }
        case CuttingPlane_nZ:
        {
            sceneCoords.setX(scene_sidewardAxis);
            sceneCoords.setY(scene_upwardAxis);
            sceneCoords.setZ(height_of_intersection);
            break;
        }
        case CuttingPlane_ISO_001:
        {
        QMatrix4x4 matrix(1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0);
        QVector3D  vector;
        matrix * vector;

            sceneCoords.setX(scene_sidewardAxis);
            sceneCoords.setY(0.0);
            sceneCoords.setZ(scene_upwardAxis);
            break;
        }
    }

    sceneCoords += centerOfViewInScene;

    return sceneCoords;
}

QPointF GeometryDisplay::mapFromScene(QVector3D scenePoint)
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

void GeometryDisplay::wheelEvent(QWheelEvent* event)
{
    qreal zoomStep = 1.15;

    centerOfViewInScene = mapToScene(event->pos());
    displayCenter = event->pos();

    QPointF cursorPosF_normal = QPointF((qreal)event->pos().x() / (this->width() - 1), (qreal)event->pos().y() / (this->height() - 1));

    int steps = abs(event->delta() / 8 / 15);

    // Scale the view
    if(event->delta() > 0)
    {

    }
    else
    {
        zoomStep = 1.0 / zoomStep;
    }

    zoomStep = qPow(zoomStep, steps);
    zoomFactor *= zoomStep;

    frameBufferSourceRect.adjust(- frameBufferSourceRect.width() * cursorPosF_normal.x() * (1.0 / zoomStep - 1.0),
                                 - frameBufferSourceRect.height() * cursorPosF_normal.y() * (1.0 / zoomStep - 1.0),
                                   frameBufferSourceRect.width() * (1.0 - cursorPosF_normal.x()) * (1.0 / zoomStep - 1.0),
                                   frameBufferSourceRect.height() * (1.0 - cursorPosF_normal.y()) * (1.0 / zoomStep - 1.0));


    event->accept();
    emit signal_sceneCoordinateChanged(mapToScene(event->pos()));
    slot_redrawScene();
}

void GeometryDisplay::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos() - QPoint(1, 1);
    QPoint mouseMoveDelta = event->pos() - mousePosOld;
    mousePosOld = event->pos();

    // Update mouse coordinates and scene coordinates
    if (!(event->buttons() & Qt::MidButton))
        emit signal_sceneCoordinateChanged(mapToScene(mousePos));

    if (event->buttons() == Qt::LeftButton)
    {

    }

    if (event->buttons() == Qt::MidButton)
    {
        centerOfViewInScene = mapToScene(displayCenter - (mouseMoveDelta));

        qreal deltaZoom_inv = zoomFactor_atCurrentFrame / zoomFactor;         // Fraction of zoom of last rendered frame in relation to current zoom
        frameBufferSourceRect.translate(- mouseMoveDelta.x() * deltaZoom_inv, - mouseMoveDelta.y() * deltaZoom_inv);
        repaint();
        //slot_redrawScene();
    }

    if (event->buttons() == Qt::RightButton)
    {
        rot_x += mouseMoveDelta.y()/5.0f;
        rot_y += mouseMoveDelta.x()/5.0f;
    }

    snapEngine->setUp(zoomFactor, centerOfViewInScene, displayCenter, (SnapEngine::CuttingPlane)cuttingplane, height_of_intersection, depth_of_view);
    snapEngine->snapProcessing(itemDB->layers, mousePos);
    if (!snapEngine->snap_vertex_points.isEmpty())
    {
        QPointF snapPoint = snapEngine->snap_vertex_points.at(0);
//        overlay->set_snap_mode(Overlay::SnapEndpoint);
        glwidget->set_snap_mode(GLWidget::SnapEndpoint);
//        overlay->set_snapPos(snapPoint.toPoint());
        glwidget->set_snapPos(snapPoint.toPoint());
    }
    else
    {
//        overlay->set_snap_mode(Overlay::SnapNo);
        glwidget->set_snap_mode(GLWidget::SnapNo);
//        overlay->set_snapPos(mousePos);
    }

    //this->overlay->moveCursor(mousePos);
    glwidget->moveCursor(mousePos);
    slot_redrawScene();

    event->accept();
}

void GeometryDisplay::enterEvent(QEvent *event)
{
    QDockWidget::enterEvent(event);
    this->setCursor(Qt::BlankCursor);

    event->accept();
}

void GeometryDisplay::leaveEvent(QEvent *event)
{
    QDockWidget::leaveEvent(event);
    //this->overlay->hideCursor();
    glwidget->hideCursor();

    event->accept();
}

void GeometryDisplay::mousePressEvent(QMouseEvent *event)
{
    QDockWidget::mousePressEvent(event);

    if (event->buttons() == Qt::MidButton)
    {
        this->setCursor(Qt::OpenHandCursor);
    }
    else
        this->setCursor(Qt::BlankCursor);


    if (event->buttons() == Qt::RightButton)
    {

    }

    // Object drawing and manipulation
    if (event->buttons() == Qt::LeftButton)
    {
        // Check if there is a currently active command
        if (false)
        {

        }

        // Pickbox
        if (!this->overlay->isPickActive())
            this->overlay->pickStart();
        if (!this->glwidget->isPickActive())
            this->glwidget->pickStart();
        else
        {
            // Selection of items finished
//            QList<QGraphicsItem*> new_selectedItems = this->items(this->overlay->selection(), this->overlay->selectionMode());
//            qDebug() << QString("Selection finished: ") + QString().setNum(new_selectedItems.count()) + " items found.";

//            foreach (QGraphicsItem* new_selectedItem, new_selectedItems)
//            {
//                // Ignore items that have been selected already
//                if (this->selectedItems.contains(new_selectedItem))
//                {
//                    // Check if user wants to deselect it using the shift button
//                    if (event->modifiers() & Qt::ShiftModifier)
//                        this->selection_deselectSingleItem(new_selectedItem);
//                    continue;
//                }

//                if (!(event->modifiers() & Qt::ShiftModifier))
//                    this->selection_selectSingleItem(new_selectedItem);
//            }

//            this->overlay->pickEnd();
            this->glwidget->pickEnd();
            return;
        }

    }
    event->accept();
}

void GeometryDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::BlankCursor);

    if (event->button() == Qt::MidButton)
    {
        this->setCursor(Qt::BlankCursor);
        slot_redrawScene();
    }

    event->accept();
}

void GeometryDisplay::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
//        if (this->overlay->isPickActive())
        if (this->glwidget->isPickActive())
        {
//            this->overlay->pickEnd();
            this->glwidget->pickEnd();
            break;
        }
//        if (this->selectedItems.count() > 0)
//        {
//            this->selection_deselectAllItems();
//        }
        break;
        case Qt::Key_C:
        {
            slot_changeCuttingplane();
        }
        break;
    }

    event->accept();
}

void GeometryDisplay::resizeEvent(QResizeEvent *event)
{
    displayCenter = QPoint(this->width(), this->height()) / 2;

    QDockWidget::resizeEvent(event);
//    if (this->overlay != NULL)
    if (this->glwidget!= NULL)
    {
//        this->overlay->resize(event->size());
        this->glwidget->resize(event->size());
    }

    if (this->glwidget != NULL)
    {
        //this->glwidget->resize(event->size());
    }

    if (framebufferImage != NULL)
        delete framebufferImage;
    framebufferImage = new QImage(event->size() * 3, QImage::Format_ARGB32_Premultiplied);

    if (framebufferImage_dBuffer != NULL)
        delete framebufferImage_dBuffer;
    framebufferImage_dBuffer = new QImage(event->size() * 3, QImage::Format_ARGB32_Premultiplied);


    foreach (GeometryRenderengine* renderEngine, renderEngines)
        renderEngine->setFrameBufferImageSize(event->size() * 3);

    slot_redrawScene();
    event->accept();
}

void GeometryDisplay::paintEvent(QPaintEvent *event)
{
    event->accept();

    glwidget->setup(QPoint(0, 0), zoomFactor, centerOfViewInScene, displayCenter, (GLWidget::CuttingPlane)cuttingplane, height_of_intersection, depth_of_view, rot_x, rot_y, rot_z);
    glwidget->updateGL();
    glwidget->repaint();

    return;
//    qDebug() << "GeometryDisplay: paintEvent" << event->region();    QDockWidget::paintEvent(event);

    if (framebufferImage_dBuffer == NULL)
        return;
    if (framebufferImage == NULL)
        return;
    if (framebufferImage_dBuffer->size().isNull())
        return;
    if (framebufferImage->size().isNull())
        return;

    // Check if there is a pending render process that could prevent starting a new one
    bool allRenderEnginesReady = true;
    foreach (GeometryRenderengine* renderEngine, renderEngines)
    {
        if (renderEngine->isRunning())
        {
            allRenderEnginesReady = false;
            break;
        }
    }

    if (repaintNeeded && allRenderEnginesReady)
    {
        zoomFactor_atRenderStart = zoomFactor;

        framebufferImage_dBuffer->fill(QColor(20, 20, 20));

        // Decide which layer is rendered by which engine
        int numLayers = itemDB->layers.count();
        int currentLayer = 0;
        int numEngines = renderEngines.count();
        int currentEngine = 0;

        foreach (GeometryRenderengine* renderEngine, renderEngines)
        {
            currentEngine++;
            while(((qreal)currentLayer / (qreal)numLayers) < ((qreal)(currentEngine) / (qreal)numEngines))   // Load balancing
            {
                if (currentLayer >= numLayers)
                    break;

                renderEngine->addLayer(itemDB->layers.at(currentLayer));
                currentLayer++;
            }
        }

        foreach (GeometryRenderengine* renderEngine, renderEngines)
        {
            renderEngine->renderLayers(QPoint(this->width(), this->height()), zoomFactor, centerOfViewInScene, displayCenter, (GeometryRenderengine::CuttingPlane)cuttingplane, height_of_intersection, depth_of_view);
        }

        repaintNeeded = false;
    }

    QPainter screenPainter(this);
    screenPainter.setRenderHints(
                QPainter::Antialiasing | QPainter::TextAntialiasing |
                QPainter::HighQualityAntialiasing, false);
    screenPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    screenPainter.drawImage(this->rect(), *framebufferImage, frameBufferSourceRect);
    paintAnchor(&screenPainter);

    screenPainter.end();


    event->accept();
}

void GeometryDisplay::slot_renderEngineFinished(QImage *frameBufferImage)
{
    QPainter painter(this->framebufferImage_dBuffer);
    painter.setRenderHints(
                QPainter::Antialiasing | QPainter::TextAntialiasing |
                QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform, false);
//    painter.setCompositionMode(QPainter::CompositionMode_Lighten);

    painter.drawImage(0, 0, *frameBufferImage);
    painter.end();

    bool allRenderEnginesReady = true;
    foreach (GeometryRenderengine* renderEngine, renderEngines)
    {
        if (renderEngine->isRunning())
        {
            allRenderEnginesReady = false;
            break;
        }
    }

    if (allRenderEnginesReady && (!repaintNeeded))
    {
        *this->framebufferImage = *this->framebufferImage_dBuffer;  // Swap buffers (dirty way...), do that in a clean way later (swap pointers)
        frameBufferSourceRect = this->rect();
        frameBufferSourceRect.translate(this->width(), this->height());    // Framebuffer is 3 times larger than screen, we want the mid part to be displayed

        zoomFactor_atCurrentFrame = zoomFactor_atRenderStart;
    }

    if (allRenderEnginesReady)
        update();      // schedule a paintEvent to get frameBufferImage drawn on screen or start rendering if it has been aborted
}

void GeometryDisplay::slot_setHeightOfIntersection(qreal height)
{
    this->height_of_intersection = height;
    slot_redrawScene();
}

void GeometryDisplay::paintAnchor(QPainter *painter)
{
    QPoint bottomLeft = this->rect().bottomLeft();

    // Anchor
    // axes colors: x = red, y = green, z = blue
    QPoint anchorPoint = bottomLeft + QPoint(10, -10);

    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            QPen pen(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "y");
            pen.setColor(QColor(120, 120, 255));
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "z");
            break;
        }
        case CuttingPlane_nX:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::green);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "y");
            break;
        }
        case CuttingPlane_pY:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            break;
        }
        case CuttingPlane_nY:
        {
            QPen pen(Qt::red);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "x");
            pen.setColor(QColor(120, 120, 255));
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "z");
            break;
        }
        case CuttingPlane_pZ:
        {
            QPen pen(Qt::red);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "x");
            pen.setColor(Qt::green);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "y");
            break;
        }
        case CuttingPlane_nZ:
        {
            QPen pen(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "y");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            break;
        }
        case CuttingPlane_ISO_001:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            pen.setColor(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, -50));
            painter->drawText(anchorPoint + QPoint(55, -55), "-y");
            break;
        }
    }
}



void GeometryDisplay::setWindowText()
{
    QString text;

    text = "Intersection ";

    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            text += "X+";
            break;
        }
        case CuttingPlane_nX:
        {
            text += "X-";
            break;
        }
        case CuttingPlane_pY:
        {
            text += "Y+";
            break;
        }
        case CuttingPlane_nY:
        {
            text += "Y-";
            break;
        }
        case CuttingPlane_pZ:
        {
            text += "Z+";
            break;
        }
        case CuttingPlane_nZ:
        {
            text += "Z-";
            break;
        }
    }

    text += QString().sprintf(" plane=%.3lf depth=%.3lf", this->height_of_intersection, this->depth_of_view);


    this->setWindowTitle(text);
}
