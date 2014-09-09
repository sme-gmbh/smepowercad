#include "glwidget.h"

#define PI 3.1415926535897

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
    this->cameraPosition = QVector3D();

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
        qreal dx = mouseMoveDelta.x()/5.0f;
        qreal dy = mouseMoveDelta.y()/5.0f;

        #define PI 3.14159265

//        rot_x += dx * cos(rot_y*PI/180.0) - dy;
//        rot_y += dx * cos(rot_x*PI/180.0) * cos(rot_z*PI/180.0) + dy * sin(rot_z*PI/180.0);
        //rot_z += dx + dy;

        rot_x += -dy;
        rot_y += -dx;
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

    // Item highlighting (experimental)
    highlightClear();
    highlightItemAtPosition(mousePos);


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
    makeCurrent();
    saveGLState();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-(double)this->width(), (double)this->width(), -(double)this->height(), (double)this->height(), this->width(), 1000000);
    GLfloat screenRatio = (qreal)this->width() / (qreal)this->height();
    glOrtho(-100000 * screenRatio, 100000 * screenRatio, -100000, 100000, -100000, 100000);
    glTranslatef(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glLoadMatrixd();
    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);

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
        // Cursor lines
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2);
        glColor4ub(255, 255, 255, 255);
        glBegin(GL_LINES);
        glVertex3i(0, mousePos.y(), 0);
        glVertex3i(this->width() - 1, mousePos.y(), 0);
        glVertex3i(mousePos.x(), 0, 0);
        glVertex3i(mousePos.x(), this->height() - 1, 0);
        glEnd();

        // Cursor Pickbox
        glLineWidth(1);
        glColor4ub(200, 255, 200, 150);
        QRect pickRect = QRect(0, 0, 11, 11);
        pickRect.moveCenter(mousePos);
        glBegin(GL_LINE_LOOP);
        glVertex3i(pickRect.bottomLeft().x(), pickRect.bottomLeft().y(), 0);
        glVertex3i(pickRect.bottomRight().x(), pickRect.bottomRight().y(), 0);
        glVertex3i(pickRect.topRight().x(), pickRect.topRight().y(), 0);
        glVertex3i(pickRect.topLeft().x(), pickRect.topLeft().y(), 0);
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

                glBegin(GL_LINE_LOOP);
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
    quint32 glName = 0;

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

            item->index = glName;
            glLoadName(glName);
            glName++;

            // Paint it
            switch (item->getType())
            {
            case CADitem::Basic_Point:
                break;
            case CADitem::Basic_Line:
                if (this->render_outline)
                    paintLine(layer, (CAD_basic_line*)item);
                break;
            case CADitem::Basic_Polyline:
                if (this->render_outline)
                    paintPolyLine(layer, (CAD_basic_polyline*)item);
                break;
            case CADitem::Basic_Circle:
                if (this->render_outline)
                    paintBasicCircle(layer, (CAD_basic_circle*)item);
                break;
            case CADitem::Basic_Arc:
                break;
            case CADitem::Basic_Face:
                paintFace(layer, (CAD_basic_3Dface*)item);
                break;
            case CADitem::Basic_Plane:
                break;
            case CADitem::Basic_Box:
                paintBasicBox(layer, (CAD_basic_box*)item);
                break;
            case CADitem::Basic_Cylinder:
                paintBasicCylinder(layer, (CAD_basic_cylinder*)item);
                break;
            case CADitem::Basic_Sphere:
                paintBasicSphere(layer, (CAD_basic_sphere*)item);
                break;

            case CADitem::Arch_LevelSlab:
                paintArchLevelSlab(layer, (CAD_arch_levelSlab*)item);
                break;
            case CADitem::Arch_Wall_loadBearing:
                paintArchWallLoadBearing(layer, (CAD_arch_wall_loadBearing*)item);
                break;
            case CADitem::Arch_Wall_nonLoadBearing:
                paintArchWallNonLoadBearing(layer, (CAD_arch_wall_nonLoadBearing*)item);
                break;
            case CADitem::Arch_BlockOut:
                paintArchBlockOut(layer, (CAD_arch_blockOut*)item);
                break;
            case CADitem::Arch_Support:
                break;
            case CADitem::Arch_Beam:
                break;
            case CADitem::Arch_Door:
                paintArchDoor(layer, (CAD_arch_door*)item);
                break;
            case CADitem::Arch_Window:
                paintArchWindow(layer, (CAD_arch_window*)item);
                break;

            case CADitem::Air_Duct:
                paintAirDuct(layer, (CAD_air_duct*)item);
                break;
            case CADitem::Air_Pipe:
                paintAirPipe(layer, (CAD_air_pipe*)item);
                break;                
            case CADitem::Air_DuctFireResistant:
                break;
            case CADitem::Air_DuctTurn:
                paintAirDuctTurn(layer, (CAD_air_ductTurn*)item);
                break;
            case CADitem::Air_PipeTurn:
                paintAirPipeTurn(layer, (CAD_air_pipeTurn*)item);
                break;
            case CADitem::Air_PipeReducer:
                paintAirPipeReducer(layer, (CAD_air_pipeReducer*)item);
                break;
            case CADitem::Air_PipeTeeConnector:
                paintAirPipeTeeConnector(layer, (CAD_air_pipeTeeConnector*)item);
                break;
            case CADitem::Air_DuctTeeConnector:
                break;
            case CADitem::Air_DuctTransition:
                break;
            case CADitem::Air_DuctTransitionRectRound:
                break;
            case CADitem::Air_DuctYpiece:
                break;
            case CADitem::Air_DuctEndPlate:
                break;
            case CADitem::Air_DuctEndPlate:
                break;
            case CADitem::Air_PipeEndCap:
                break;
            case CADitem::Air_ThrottleValve:
                break;
            case CADitem::Air_MultiLeafDamper:
                break;
            case CADitem::Air_PressureReliefDamper:
                break;
            case CADitem::Air_PipeFireDamper:
                break;
            case CADitem::Air_DuctFireDamper:
                break;
            case CADitem::Air_DuctVolumetricFlowController:
                break;
            case CADitem::Air_PipeVolumetricFlowController:
                break;
            case CADitem::Air_HeatExchangerWaterAir:
                break;
            case CADitem::Air_HeatExchangerAirAir:
                break;
            case CADitem::Air_CanvasFlange:
                break;
            case CADitem::Air_Filter:
                break;
            case CADitem::Air_PipeSilencer:
                break;
            case CADitem::Air_DuctBaffleSilencer:
                break;
            case CADitem::Air_Fan:
                break;
            case CADitem::Air_Humidifier:
                break;
            case CADitem::Air_EmptyCabinet:
                break;
            case CADitem::Air_EquipmentFrame:
                break;

            case CADitem::HeatCool_Adjustvalve:
                paintHeatCoolAdjustvalve(layer, (CAD_heatcool_adjustvalve*)item);
                break;
            case CADitem::HeatCool_Chiller:
                paintHeatCoolChiller(layer, (CAD_heatcool_chiller*)item);
                break;
            case CADitem::HeatCool_Controlvalve:
                paintHeatCoolControlvalve(layer, (CAD_heatcool_controlvalve*)item);
                break;
            case CADitem::HeatCool_CoolingTower:
                paintHeatCoolCoolingTower(layer, (CAD_heatcool_coolingTower*)item);
                break;
            case CADitem::HeatCool_HeatExchanger:
                paintHeatCoolHeatExchanger(layer, (CAD_heatcool_heatExchanger*)item);
                break;
            case CADitem::HeatCool_Pipe:
                paintHeatCoolPipe(layer, (CAD_heatcool_pipe*)item);
                break;
            case CADitem::HeatCool_Pump:
                paintHeatCoolPump(layer, (CAD_heatcool_pump*)item);
                break;
            case CADitem::HeatCool_Sensor:
                paintHeatCoolSensor(layer, (CAD_heatcool_sensor*)item);
                break;
            case CADitem::HeatCool_PipeTurn:
                break;
            case CADitem::HeatCool_PipeReducer:
                break;
            case CADitem::HeatCool_PipeTeeConnector:
                break;
            case CADitem::HeatCool_PipeEndCap:
                break;
            case CADitem::HeatCool_Flange:
                break;
            case CADitem::HeatCool_ExpansionChamber:
                break;
            case CADitem::HeatCool_Boiler:
                break;
            case CADitem::HeatCool_WaterHeater:
                break;
            case CADitem::HeatCool_StorageBoiler:
                break;
            case CADitem::HeatCool_Radiator:
                break;
            case CADitem::HeatCool_Filter:
                break;
            case CADitem::HeatCool_BallValve:
                break;
            case CADitem::HeatCool_ButterflyValve:
                break;
            case CADitem::HeatCool_SafetyValve:
                break;
            case CADitem::HeatCool_Flowmeter:
                break;

            case CADitem::Sprinkler_CompressedAirWaterContainer:
                paintSprinklerCompressedAirWaterContainer(layer, (CAD_sprinkler_compressedAirWaterContainer*)item);
                break;
            case CADitem::Sprinkler_Distribution:
                paintSprinklerDistribution(layer, (CAD_sprinkler_distribution*)item);
                break;
            case CADitem::Sprinkler_Head:
                paintSprinklerHead(layer, (CAD_sprinkler_head*)item);
                break;
            case CADitem::Sprinkler_Pipe:
                paintSprinklerPipe(layer, (CAD_sprinkler_pipe*)item);
                break;
            case CADitem::Sprinkler_Pump:
                paintSprinklerPump(layer, (CAD_sprinkler_pump*)item);
                break;
            case CADitem::Sprinkler_TeeConnector:
                paintSprinklerTeeConnector(layer, (CAD_sprinkler_teeConnector*)item);
                break;
            case CADitem::Sprinkler_Valve:
                paintSprinklerValve(layer, (CAD_sprinkler_valve*)item);
                break;
            case CADitem::Sprinkler_WetAlarmValve:
                paintSprinklerWetAlarmValve(layer, (CAD_sprinkler_wetAlarmValve*)item);
                break;
            case CADitem::Sprinkler_ZoneCheck:
                paintSprinklerZoneCheck(layer, (CAD_sprinkler_zoneCheck*)item);
                break;

            case CADitem::Electrical_Cabinet:
                paintElectricalCabinet(layer, (CAD_electrical_cabinet*)item);
                break;
            case CADitem::Electrical_CableTray:
                paintElectricalCabletray(layer, (CAD_electrical_cableTray*)item);
                break;
            }
        }
        paintContent(layer->subLayers);
    }
}

void GLWidget::paintLine(Layer* layer, CAD_basic_line *item)
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

    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0 / zoomFactor;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width / zoomFactor;
    }

    // Default width setting
    if (penWidth < 1.0)
        penWidth = 1.0;

    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();
    }

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

    glLineWidth(penWidth);
    glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
    glBegin(GL_LINES);
    glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
    glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());

    glEnd();
}

void GLWidget::paintPolyLine(Layer *layer, CAD_basic_polyline *item)
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



    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();
    }

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    glBegin(GL_LINES);
    foreach (CAD_basic_polyline::Vertex vertex, item->vertices)
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
            qreal penWidth = 1.0;
            if (item->widthByLayer)
            {
                penWidth = layer->width / 100.0 / zoomFactor;
            }
            else if (item->widthByBlock)
            {

            }
            else
            {
                penWidth = vertex.widthStart / zoomFactor;
            }

            // Default width setting
            if (penWidth < 1.0)
                penWidth = 1.0;

            glLineWidth(penWidth);
            glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());

            glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
            glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());



//            painter->drawLine(mapFromScene(p1), mapFromScene(p2));
            p1 = p2;
        }
    }
    glEnd();

    //    qDebug() << "GeometryRenderengine: Painting a polyline";
}

void GLWidget::paintFace(Layer *layer, CAD_basic_3Dface *item)
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


    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();

        if (color_brush.lightnessF() > 0.5)
            color_brush = color_brush.darker();
        else
            color_brush = color_brush.lighter();
    }


    if (this->render_solid)
    {
        glBegin(GL_POLYGON);
        glColor4f(color_brush.redF(), color_brush.greenF(), color_brush.blueF(), color_brush.alphaF());
        foreach (CAD_basic_3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }

    if (this->render_outline)
    {
        glBegin(GL_LINE_LOOP);
        glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
        foreach (CAD_basic_3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }
}

void GLWidget::paintBasicCircle(Layer *layer, CAD_basic_circle *item)
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

    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0 / zoomFactor;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width / zoomFactor;
    }

    // Default width setting
    if (penWidth < 1.0)
        penWidth = 1.0;

    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();
    }

    glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
    glLineWidth(penWidth);
    glBegin(GL_LINE_LOOP);
    for (qreal i=0.0; i < 1.0; i += 0.01)    // 100 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;
        linePos = item->center;

        linePos += QVector3D(sin(angle) * item->radius, cos(angle) * item->radius, 0.0);

        glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
    }
    glEnd();
}

void GLWidget::paintBasicBox(Layer *layer, CAD_basic_box *item)
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

    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();

        if (color_brush.lightnessF() > 0.5)
            color_brush = color_brush.darker();
        else
            color_brush = color_brush.lighter();
    }

    if (this->render_solid)
    {
        glBegin(GL_QUADS);
        glColor4f(color_brush.redF(), color_brush.greenF(), color_brush.blueF(), color_brush.alphaF());

        // Bottom face
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());

        // Top face
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());

        // Front face
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());

        // Left face
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());

        // Right face
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());

        // Back face
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());

        glEnd();
    }

    if (this->render_outline)
    {
        glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
        glLineWidth(1.0);

        // Bottom face
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glEnd();

        // Top face
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glEnd();

        // Vertical edges
        glBegin(GL_LINES);
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());

        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());

        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());

        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glEnd();
    }
}

void GLWidget::paintBasicCylinder(Layer *layer, CAD_basic_cylinder *item)
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

    if (item->highlight)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();

        if (color_brush.lightnessF() > 0.5)
            color_brush = color_brush.darker();
        else
            color_brush = color_brush.lighter();
    }

    if (this->render_solid)
    {
        QList<QVector3D> vertices_bottom;

        // Bottom circle
        for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
        {
            qreal angle = 2 * PI * i;
            QVector3D linePos;
            linePos = item->center_base;

            linePos += QVector3D(sin(angle) * item->radius, cos(angle) * item->radius, 0.0);
            vertices_bottom.append(linePos);
        }

        // Vertical connections (faces)
        glColor4f(color_brush.redF(), color_brush.greenF(), color_brush.blueF(), color_brush.alphaF());
        glBegin(GL_QUADS);
        QVector3D last_vertex_bottom = vertices_bottom.at(vertices_bottom.count() - 1);
        for (int i = 0; i < vertices_bottom.count(); i++)
        {
            QVector3D vertex_bottom = vertices_bottom.at(i);
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z() + item->height);
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z() + item->height);
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            last_vertex_bottom = vertex_bottom;
        }

        glEnd();
    }

    if (this->render_outline)
    {
        QList<QVector3D> vertices_bottom;
        glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
        glLineWidth(1.0);
        // Bottom circle
        glBegin(GL_LINE_LOOP);
        for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
        {
            qreal angle = 2 * PI * i;
            QVector3D linePos;
            linePos = item->center_base;

            linePos += QVector3D(sin(angle) * item->radius, cos(angle) * item->radius, 0.0);
            vertices_bottom.append(linePos);

            glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
        }
        glEnd();

        // Top circle
        glBegin(GL_LINE_LOOP);
        for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
        {
            qreal angle = 2 * PI * i;
            QVector3D linePos;
            linePos = item->center_base + QVector3D(0.0, 0.0, item->height);

            linePos += QVector3D(sin(angle) * item->radius, cos(angle) * item->radius, 0.0);

            glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
        }
        glEnd();

        // Vertical connections
        glBegin(GL_LINES);
        foreach (QVector3D vertex_bottom, vertices_bottom)
        {
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z() + item->height);
        }

        glEnd();
    }
}

void GLWidget::paintBasicSphere(Layer *layer, CAD_basic_sphere *item)
{

}

void GLWidget::paintArchLevelSlab(Layer *layer, CAD_arch_levelSlab *item)
{

}

void GLWidget::paintArchWallLoadBearing(Layer *layer, CAD_arch_wall_loadBearing *item)
{

}

void GLWidget::paintArchWallNonLoadBearing(Layer *layer, CAD_arch_wall_nonLoadBearing *item)
{

}

void GLWidget::paintArchBlockOut(Layer *layer, CAD_arch_blockOut *item)
{

}

void GLWidget::paintArchDoor(Layer *layer, CAD_arch_door *item)
{

}

void GLWidget::paintArchWindow(Layer *layer, CAD_arch_window *item)
{

}

void GLWidget::paintAirDuct(Layer *layer, CAD_air_duct *item)
{

}

void GLWidget::paintAirPipe(Layer *layer, CAD_air_pipe *item)
{

}

void GLWidget::paintAirDuctTurn(Layer *layer, CAD_air_ductTurn *item)
{

}

void GLWidget::paintAirPipeTurn(Layer *layer, CAD_air_pipeTurn *item)
{

}

void GLWidget::paintAirPipeReducer(Layer *layer, CAD_air_pipeReducer *item)
{

}

void GLWidget::paintAirPipeTeeConnector(Layer *layer, CAD_air_pipeTeeConnector *item)
{

}

void GLWidget::paintHeatCoolAdjustvalve(Layer *layer, CAD_heatcool_adjustvalve *item)
{

}

void GLWidget::paintHeatCoolChiller(Layer *layer, CAD_heatcool_chiller *item)
{

}

void GLWidget::paintHeatCoolControlvalve(Layer *layer, CAD_heatcool_controlvalve *item)
{

}

void GLWidget::paintHeatCoolCoolingTower(Layer *layer, CAD_heatcool_coolingTower *item)
{

}

void GLWidget::paintHeatCoolHeatExchanger(Layer *layer, CAD_heatcool_heatExchanger *item)
{

}

void GLWidget::paintHeatCoolPipe(Layer *layer, CAD_heatcool_pipe *item)
{

}

void GLWidget::paintHeatCoolPump(Layer *layer, CAD_heatcool_pump *item)
{

}

void GLWidget::paintHeatCoolSensor(Layer *layer, CAD_heatcool_sensor *item)
{

}

void GLWidget::paintSprinklerCompressedAirWaterContainer(Layer *layer, CAD_sprinkler_compressedAirWaterContainer *item)
{

}

void GLWidget::paintSprinklerDistribution(Layer *layer, CAD_sprinkler_distribution *item)
{

}

void GLWidget::paintSprinklerHead(Layer *layer, CAD_sprinkler_head *item)
{

}

void GLWidget::paintSprinklerPipe(Layer *layer, CAD_sprinkler_pipe *item)
{

}

void GLWidget::paintSprinklerPump(Layer *layer, CAD_sprinkler_pump *item)
{

}

void GLWidget::paintSprinklerTeeConnector(Layer *layer, CAD_sprinkler_teeConnector *item)
{

}

void GLWidget::paintSprinklerValve(Layer *layer, CAD_sprinkler_valve *item)
{

}

void GLWidget::paintSprinklerWetAlarmValve(Layer *layer, CAD_sprinkler_wetAlarmValve *item)
{

}

void GLWidget::paintSprinklerZoneCheck(Layer *layer, CAD_sprinkler_zoneCheck *item)
{

}

void GLWidget::paintElectricalCabinet(Layer *layer, CAD_electrical_cabinet *item)
{

}

void GLWidget::paintElectricalCabletray(Layer *layer, CAD_electrical_cableTray *item)
{

}

CADitem* GLWidget::itemAtPosition(QPoint pos)
{
#define HITBUFFER_SIZE 512000
    GLuint buffer[HITBUFFER_SIZE];
    GLint viewport[4];

    saveGLState();

    glViewport(0, 0, width(), height());
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(HITBUFFER_SIZE, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix((GLdouble)pos.x(), (GLdouble)pos.y(), 11.0, 11.0, viewport);

    GLfloat screenRatio = (qreal)this->width() / (qreal)this->height();
    glOrtho(-100000 * screenRatio, 100000 * screenRatio, -100000, 100000, -100000, 100000);
    glTranslatef(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);

    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);
    glScaled(this->zoomFactor, this->zoomFactor, this->zoomFactor);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);


    paintContent(itemDB->layers);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    GLint hits = glRenderMode(GL_RENDER);   // Number of hits is returned by glRenderMode
    restoreGLState();

    if (hits == 0)
        return NULL;

    int i = 0;
    GLint hit = 1;
    GLuint globalMinDepth = 0xffffffff;
    GLuint glName;
    while (i < HITBUFFER_SIZE)
    {
        GLuint numberOfNames = buffer[i];
        GLuint minDepth = buffer[i + 1];
        GLuint maxDepth = buffer[i + 2];

        if (numberOfNames > 0)
        {
            if (minDepth < globalMinDepth)
            {
                glName = buffer[i + 3];
                globalMinDepth = minDepth;
            }
        }

        i += 3;
        i += numberOfNames;

        hit++;
        if (hit > hits)
            break;
    }

    if (i >= HITBUFFER_SIZE)
        QMessageBox::warning(this, "GLWidget::itemAtPosition()", "HITBUFFER_SIZE too small - too many objects in findbox!");

    CADitem* item = itemAtPosition_processLayers(itemDB->layers, glName);
    if (item)
        return item;

    return NULL;
}

CADitem *GLWidget::itemAtPosition_processLayers(QList<Layer *> layers, GLuint glName)
{
    foreach (Layer* layer, layers)
    {
        if (!layer->on)
            continue;

        CADitem* item = itemAtPosition_processItems(layer->items, glName);
        if (item)
            return item;


        item = itemAtPosition_processLayers(layer->subLayers, glName);
        if (item)
            return item;
    }

    return NULL;
}

CADitem *GLWidget::itemAtPosition_processItems(QList<CADitem *> items, GLuint glName)
{
    foreach (CADitem* item, items)
    {
        if (item->index == glName)
        {
            return item;
        }

        item = itemAtPosition_processItems(item->subItems, glName);
        if (item)
            return item;
    }

    return NULL;
}

void GLWidget::highlightItemAtPosition(QPoint pos)
{
    CADitem* item = this->itemAtPosition(pos);

    if (item == NULL)
        return;

    item->highlight = true;
}

void GLWidget::highlightClear()
{
    highlightClear_processLayers(itemDB->layers);
}

void GLWidget::highlightClear_processLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        highlightClear_processItems(layer->items);
        highlightClear_processLayers(layer->subLayers);
    }
}

void GLWidget::highlightClear_processItems(QList<CADitem *> items)
{
    foreach (CADitem* item, items)
    {
        item->highlight = false;
        highlightClear_processItems(item->subItems);
    }

}
