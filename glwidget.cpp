#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent, ItemDB *itemDB) :
    QGLWidget(QGLFormat(QGL::SampleBuffers|QGL::AlphaChannel), parent)
{
    this->itemDB = itemDB;
    this->mousePos = QPoint();

    this->translationOffset = QPoint();
    this->zoomFactor = 10.0;
    this->centerOfViewInScene = QVector3D();
    this->displayCenter = QPoint();
    this->cuttingplane = CuttingPlane_nZ;
    this->height_of_intersection = 0.0;
    this->depth_of_view = 1000000.0;
    this->rot_x = 0.0;
    this->rot_y = 0.0;
    this->rot_z = 0.0;
    this->render_solid = true;
    this->render_outline = true;



    makeCurrent();

    this->pickActive = false;
    this->cursorShown = true;
    this->snapMode = SnapCenter;

    this->setMouseTracking(true);

    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//    this->setAttribute(Qt::WA_OpaquePaintEvent);

    // Create a SnapEngine
//    this->snapEngine = new SnapEngine(itemDB, this);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glShadeModel (GL_FLAT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0};
    GLfloat light_position[] = { -5.0, 15.0, 25.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);




    tile_list = glGenLists(1);
    glNewList(tile_list, GL_COMPILE);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    }
    glEnd();
    glEndList();
}

GLWidget::~GLWidget()
{
    glDeleteLists(tile_list, 1);
}

void GLWidget::setup(QPoint translationOffset, qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, GLWidget::CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view, qreal rot_x, qreal rot_y, qreal rot_z)
{
    this->translationOffset = translationOffset;
    this->zoomFactor = zoomFactor;
    this->centerOfViewInScene = centerOfViewInScene;
    this->displayCenter = displayCenter;
    this->cuttingplane = cuttingplane;
    this->height_of_intersection = height_of_intersection;
    this->depth_of_view = depth_of_view;
    this->rot_x = rot_x;
    this->rot_y = rot_y;
    this->rot_z = rot_z;
}

void GLWidget::moveCursor(QPoint pos)
{
//    qDebug() << "GLWidget: moveCursor " << pos.x() << " " << pos.y();
    this->mousePos = pos;
    this->cursorShown = true;
    slot_repaint();
}

void GLWidget::hideCursor()
{
    this->cursorShown = false;
    slot_repaint();
}

void GLWidget::pickStart()
{
    this->pickActive = true;
    this->pickStartPos = this->mousePos;
}

void GLWidget::pickEnd()
{
    this->pickActive = false;
    slot_repaint();
}

bool GLWidget::isPickActive()
{
    return this->pickActive;
}

QRect GLWidget::selection()
{
    // Selection rect muss immer von topleft noch bottomright gehen

    QPoint topLeft;
    topLeft.setX(qMin(this->pickStartPos.x(), this->mousePos.x()));
    topLeft.setY(qMin(this->pickStartPos.y(), this->mousePos.y()));

    QPoint bottomRight;
    bottomRight.setX(qMax(this->pickStartPos.x(), this->mousePos.x()));
    bottomRight.setY(qMax(this->pickStartPos.y(), this->mousePos.y()));

    return QRect(topLeft, bottomRight);
}

Qt::ItemSelectionMode GLWidget::selectionMode()
{
    if (this->mousePos.x() - this->pickStartPos.x() > 0)
        return Qt::ContainsItemShape;
        //return Qt::ContainsItemBoundingRect;
    else
        return Qt::IntersectsItemShape;
        //return Qt::IntersectsItemBoundingRect;
}

void GLWidget::snap_enable(bool on)
{

}

void GLWidget::set_snap_mode(SnapMode mode)
{
    this->snapMode = mode;
}

void GLWidget::set_snapPos(QPoint snapPos)
{
    this->snapPos = snapPos;
}

void GLWidget::set_WorldRotation(float rot_x, float rot_y, float rot_z)
{
    this->rot_x = rot_x;
    this->rot_y = rot_y;
    this->rot_z = rot_z;
    slot_repaint();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    qreal zoomStep = 1.15;

//    centerOfViewInScene = mapToScene(event->pos());
//    displayCenter = event->pos();

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

//    frameBufferSourceRect.adjust(- frameBufferSourceRect.width() * cursorPosF_normal.x() * (1.0 / zoomStep - 1.0),
//                                 - frameBufferSourceRect.height() * cursorPosF_normal.y() * (1.0 / zoomStep - 1.0),
//                                   frameBufferSourceRect.width() * (1.0 - cursorPosF_normal.x()) * (1.0 / zoomStep - 1.0),
//                                   frameBufferSourceRect.height() * (1.0 - cursorPosF_normal.y()) * (1.0 / zoomStep - 1.0));


    event->accept();

    // todo: Map to scene
//    emit signal_sceneCoordinateChanged(mapToScene(event->pos()));
    slot_repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    mousePos = event->pos();// - QPoint(1, 1);
    mousePos.setY((this->height() - 1) - mousePos.y());
    QPoint mouseMoveDelta = mousePos - mousePosOld;
    mousePosOld = mousePos;

    // Update mouse coordinates and scene coordinates
    //todo
//    if (!(event->buttons() & Qt::MidButton))
//        emit signal_sceneCoordinateChanged(mapToScene(mousePos));

    if (event->buttons() == Qt::LeftButton)
    {

    }

//    if (event->buttons() == Qt::MidButton)
//    {
//        centerOfViewInScene = mapToScene(displayCenter - (mouseMoveDelta));

//        qreal deltaZoom_inv = zoomFactor_atCurrentFrame / zoomFactor;         // Fraction of zoom of last rendered frame in relation to current zoom
//        frameBufferSourceRect.translate(- mouseMoveDelta.x() * deltaZoom_inv, - mouseMoveDelta.y() * deltaZoom_inv);
//    slot_repaint();
//    }

    if (event->buttons() == Qt::RightButton)
    {
        rot_x += mouseMoveDelta.y()/5.0f;
        rot_y += mouseMoveDelta.x()/5.0f;
    }
/*
    snapEngine->setUp(zoomFactor, centerOfViewInScene, displayCenter, (SnapEngine::CuttingPlane)cuttingplane, height_of_intersection, depth_of_view);
    snapEngine->snapProcessing(itemDB->layers, mousePos);
    if (!snapEngine->snap_vertex_points.isEmpty())
    {
        QPointF snapPoint = snapEngine->snap_vertex_points.at(0);
        this->set_snap_mode(GLWidget::SnapEndpoint);
        this->set_snapPos(snapPoint.toPoint());
    }
    else
    {
        this->set_snap_mode(GLWidget::SnapNo);
    }
*/
    this->cursorShown = true;

    slot_repaint();

    event->accept();
}

void GLWidget::enterEvent(QEvent *event)
{
    this->setCursor(Qt::BlankCursor);
    this->cursorShown = true;

    event->accept();
}

void GLWidget::leaveEvent(QEvent *event)
{
    this->hideCursor();

    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
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
//        if (!this->overlay->isPickActive())
//            this->overlay->pickStart();
        if (!this->isPickActive())
            this->pickStart();
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
            this->pickEnd();
            return;
        }

    }
    event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::BlankCursor);

    if (event->button() == Qt::MidButton)
    {
        this->setCursor(Qt::BlankCursor);
        slot_repaint();
    }

    event->accept();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
        if (this->pickActive)
        {
            this->pickEnd();
            break;
        }
//        if (this->selectedItems.count() > 0)
//        {
//            this->selection_deselectAllItems();
//        }
        break;
    }

    event->accept();
}

void GLWidget::resizeEvent(QResizeEvent *event)
{
    displayCenter = QPoint(this->width(), this->height()) / 2;

    slot_repaint();
    event->accept();
}


void GLWidget::paintEvent(QPaintEvent *event)
{
    saveGLState();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-(double)this->width(), (double)this->width(), -(double)this->height(), (double)this->height(), this->width(), 1000000);
    qreal screenRatio = (qreal)this->width() / (qreal)this->height();
    glOrtho(-100000 * screenRatio, 100000 * screenRatio, -100000, 100000, -100000, 100000);
    glTranslatef(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);
    glScaled(this->zoomFactor, this->zoomFactor, this->zoomFactor);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);


//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    glColor4ub(255, 255, 255, 255);

//    glCallList(tile_list);


    paintContent(itemDB->layers);

    restoreGLState();

//    glFlush();

    // Overlay
    saveGLState();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, this->width(), 0, this->height(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (this->cursorShown)
    {
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2);
        glColor4ub(255, 255, 255, 255);
        glBegin(GL_LINES);
        glVertex3i(0, mousePos.y(), 0);
        glVertex3i(this->width() - 1, mousePos.y(), 0);
        glVertex3i(mousePos.x(), 0, 0);
        glVertex3i(mousePos.x(), this->height() - 1, 0);
        glEnd();


        if (this->pickActive)
        {
            if (this->pickStartPos.x() < this->mousePos.x())
                glColor4ub(127, 127, 255, 127);
            else
                glColor4ub(127, 255, 127, 127);
            glLineWidth(2);
            QRect rect = this->selection();
            glBegin(GL_QUADS);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();

            if (this->pickStartPos.x() < this->mousePos.x())
                glColor4ub(40, 40, 255, 255);
            else
                glColor4ub(40, 255, 40, 255);
            glBegin(GL_LINE_LOOP);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();
        }

        if (snapMode != SnapNo)
        {
//            pen.setColor(Qt::red);
//            pen.setStyle(Qt::SolidLine);
//            pen.setWidth(2);
//            painter.setPen(pen);
//            painter.setBrush(Qt::NoBrush);

            glColor4ub(255, 0, 0, 255);

            switch (snapMode)
            {
            case SnapBasepoint:
            {
//                QRect focusRect = QRect(0, 0, 3, 3);
//                focusRect.moveCenter(this->snapPos);
//                painter.drawRect(focusRect);

//                painter.drawLine(this->snapPos, this->snapPos + QPoint(5, -5));
//                painter.drawText(this->snapPos + QPoint(7, -7), "Basepoint");
                break;
            }
            case SnapEndpoint:
            {
                QRect focusRect = QRect(0, 0, 11, 11);
                focusRect.moveCenter(this->snapPos);
//                painter.drawRect(focusRect);
//                painter.drawText(this->snapPos + QPoint(7, -7), "Endpoint/Vertex");

                glBegin(GL_LINE_STRIP);
                glVertex3i(focusRect.bottomLeft().x(), focusRect.bottomLeft().y(), 0);
                glVertex3i(focusRect.bottomRight().x(), focusRect.bottomRight().y(), 0);
                glVertex3i(focusRect.topRight().x(), focusRect.topRight().y(), 0);
                glVertex3i(focusRect.topLeft().x(), focusRect.topLeft().y(), 0);
                break;
            }
            case SnapCenter:
            {
//                QRect focusRect = QRect(0, 0, 3, 3);
//                focusRect.moveCenter(this->snapPos);
//                painter.drawRect(focusRect);

//                painter.drawLine(this->snapPos - QPoint(5, -5), this->snapPos + QPoint(5, -5));
//                painter.drawText(this->snapPos + QPoint(7, -7), "Center");


                break;
            }
            case SnapNo:
            {
                break;
            }
            }

            glEnd();
        }

    }


    restoreGLState();
    event->accept();
}

void GLWidget::slot_repaint()
{
    repaint();
    updateGL();
}

void GLWidget::slot_wireframe(bool on)
{
    this->render_outline = on;
    slot_repaint();
}

void GLWidget::slot_solid(bool on)
{
    this->render_solid = on;
    slot_repaint();
}

void GLWidget::saveGLState()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void GLWidget::restoreGLState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}


void GLWidget::paintContent(QList<Layer*> layers)
{


//    glBegin(GL_TRIANGLES);
//    glVertex3f(-30000.0, -2.0, 0.0);
//    glVertex3f( 30000.0, -2.0, 0.0);
//    glVertex3f( 0000.0, 2000.0, 0.0);
//    glEnd();


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
                if (this->render_outline)
                    paintLine(layer, (CADline*)item);
                break;
            case CADitem::Polyline:
                if (this->render_outline)
                    paintPolyLine(layer, (CADpolyline*)item);
                break;
            case CADitem::Circle:
                break;
            case CADitem::Arc:
                break;
            case CADitem::Face:
                paintFace(layer, (CAD3Dface*)item);
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
        paintContent(layer->subLayers);
    }
}

void GLWidget::paintLine(Layer* layer, CADline* item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    QColor color = item->color_pen;
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
//    qreal h1 = heightInScene(p1);
//    qreal h2 = heightInScene(p2);

//    if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
//        return;

//    if (h1 < height_of_intersection)
//    {
//        // modify p1
//        p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
//    }
//    else if (h2 < height_of_intersection)
//    {
//        // modify p2
//        p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
//    }

//    painter->drawLine(mapFromScene(p1), mapFromScene(p2));
//    qDebug() << "GeometryRenderengine: Painting a line";

    glLineWidth(pen.width());
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glBegin(GL_LINES);
    glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
    glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());

    glEnd();
}

void GLWidget::paintPolyLine(Layer *layer, CADpolyline *item)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    QColor color = item->color_pen;
    if (color == Qt::transparent)   // BYLAYER
    {
        color = layer->pen.color();
    }
    else if (color.value() < 50)
        color = Qt::white;
    pen.setColor(color);
//    painter->setPen(pen);

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    glBegin(GL_LINES);
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
//            qreal h1 = heightInScene(p1);
//            qreal h2 = heightInScene(p2);

//            if ((h1 < height_of_intersection) && (h2 < height_of_intersection))
//            {
//                p1 = p2;
//                continue;
//            }

//            if (h1 < height_of_intersection)
//            {
//                // modify p1
//                p1 = (p1 - p2) * ((height_of_intersection - h2) / (h1 - h2)) + p2;
//            }
//            else if (h2 < height_of_intersection)
//            {
//                // modify p2
//                p2 = (p2 - p1) * ((height_of_intersection - h1) / (h2 - h1)) + p1;
//            }




            // Points in buffer coords (screenpoints)
//            QPoint sp1 = mapFromScene(p1).toPoint() + this->translationOffset;
//            QPoint sp2 = mapFromScene(p2).toPoint() + this->translationOffset;


            glLineWidth(pen.width());
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());

            glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
            glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());



//            painter->drawLine(mapFromScene(p1), mapFromScene(p2));
            p1 = p2;
        }
    }
    glEnd();

    //    qDebug() << "GeometryRenderengine: Painting a polyline";
}

void GLWidget::paintFace(Layer *layer, CAD3Dface *item)
{
    QColor color_pen = item->color_pen;
    QColor color_brush = item->color_brush;

    if (color_pen == Qt::transparent)   // BYLAYER
    {
        color_pen = layer->pen.color();
    }
    else if (color_pen.value() < 50)
        color_pen = Qt::white;

    if (color_brush == Qt::transparent)   // BYLAYER
    {
        color_brush = layer->brush.color();
    }
    else if (color_brush.value() < 50)
        color_brush = Qt::white;


    if (this->render_solid)
    {
        glBegin(GL_POLYGON);
        glColor4f(color_brush.redF(), color_brush.greenF(), color_brush.blueF(), color_brush.alphaF());
        foreach (CAD3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }

    if (this->render_outline)
    {
        glBegin(GL_LINE_LOOP);
        glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
        foreach (CAD3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }
}
