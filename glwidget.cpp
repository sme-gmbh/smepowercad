#include "glwidget.h"

#define PI 3.1415926535897

GLWidget::GLWidget(QWidget *parent, ItemDB *itemDB, ItemWizard *itemWizard) :
    QGLWidget(QGLFormat(QGL::SampleBuffers|QGL::AlphaChannel), parent)
{
//    qDebug() << "Created GLWidget";
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->mousePos = QPoint();

    this->translationOffset = QPoint();
    this->zoomFactor = 1.0;
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
    this->matrix_modelview.setToIdentity();
    this->matrix_projection.setToIdentity();
    this->arcballRotationMatrix.setToIdentity();

    this->pickActive = false;
    this->cursorShown = true;
    this->snapMode = SnapCenter;
    this->item_lastHighlight = NULL;

    slot_update_settings();

    this->setMouseTracking(true);

    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
//    this->setAttribute(Qt::WA_OpaquePaintEvent);

    makeCurrent();

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 0.1 };
//    glShadeModel (GL_FLAT);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0};
    GLfloat light_position[] = { 50.0, 15.0, -5000.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

//    tile_list = glGenLists(1);
//    glNewList(tile_list, GL_COMPILE);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//    }
//    glEnd();
//    glEndList();
}

GLWidget::~GLWidget()
{
//    qDebug() << "GLWidget destroyed";

    glDeleteLists(tile_list, 1);
}

QPointF GLWidget::mapFromScene(QVector3D scenePoint)
{
    QVector4D sceneCoords = QVector4D(scenePoint, 1.0);
    QVector4D screenCoords;

    screenCoords = matrix_modelview * matrix_projection * sceneCoords;
    QPointF pixelCoords = screenCoords.toPointF() ;

    pixelCoords.setX(((pixelCoords.x() / 2.0) + 0.5) * this->width());
    pixelCoords.setY(((pixelCoords.y() / 2.0) + 0.5) * this->height());

    return pixelCoords;
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

void GLWidget::set_snapPos(QVector3D snapPos)
{
    this->snapPos_screen = this->mapFromScene(snapPos).toPoint();
    this->snapPos_scene = snapPos;
}

void GLWidget::set_WorldRotation(float rot_x, float rot_y, float rot_z)
{
    this->rot_x = rot_x;
    this->rot_y = rot_y;
    this->rot_z = rot_z;
    slot_repaint();
}

QStringList GLWidget::getOpenGLinfo()
{
    makeCurrent();

    // get OpenGL info
    QStringList ret;
    ret << tr("Vendor") << QString((const char*)glGetString(GL_VENDOR));
    ret << "Renderer" << QString((const char*)glGetString(GL_RENDERER));
    ret << "Version" << QString((const char*)glGetString(GL_VERSION));
    ret << "GLSL Version" << QString((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    return ret;
}

void GLWidget::slot_highlightItem(CADitem *item)
{
    if (this->item_lastHighlight != item)
    {
        this->item_lastHighlight = item;
        slot_repaint();
    }
}

void GLWidget::slot_snapTo(QVector3D snapPos_scene, int snapMode)
{
    bool repaint = false;

    if (this->snapMode != snapMode)
    {
        this->snapMode = (SnapMode)snapMode;
        repaint = true;
    }

    if (this->snapPos_scene != snapPos_scene)
    {
        this->set_snapPos(snapPos_scene);
        repaint = true;
    }

    if (repaint)
    {
        slot_repaint();
    }
}

void GLWidget::slot_changeSelection(QList<CADitem *> selectedItems)
{
    this->selection_itemList = selectedItems;
//    emit signal_selectionChanged(this->selection_itemList);
    slot_repaint();
}

void GLWidget::slot_mouse3Dmoved(int x, int y, int z, int a, int b, int c)
{
    if (!cursorShown)
        return;

    // move
    translationOffset += QPoint(x/2, y/2);

    // zoom
    qreal zoomStep = 0.10;
    zoomStep = -(z * zoomStep / 8.0 / 20.0);
    if ((zoomFactor + zoomStep) <= 0)
    {
        zoomFactor += zoomStep / 100.0;
        if (zoomFactor < 0.0) zoomFactor = 0.0;
    }
    else
        zoomFactor += zoomStep;

    // rot
    rot_x += -((float)a / 15.0);
    rot_y += -((float)b / 15.0);
    rot_z += -((float)c / 15.0);


    slot_repaint();
}

void GLWidget::slot_update_settings()
{
    _backgroundColor = settings.value("Design_Colors_backgroundColor", QVariant::fromValue(QColor().black())).value<QColor>();

    _cursorSize = settings.value("Userinterface_Cursor_cursorSize", QVariant::fromValue(4500)).toInt();
    _cursorWidth = settings.value("Userinterface_Cursor_cursorLineWidth", QVariant::fromValue(1)).toInt();
    _cursorPickboxSize = settings.value("Userinterface_Cursor_cursorPickboxSize", QVariant::fromValue(11)).toInt();
    _cursorPickboxLineWidth = settings.value("Userinterface_Cursor_cursorPickboxLineWidth", QVariant::fromValue(1)).toInt();
    _cursorPickboxColor = settings.value("Userinterface_Cursor_cursorPickboxColor", QVariant::fromValue(QColor(200, 255, 200, 150))).value<QColor>();

    _snapIndicatorSize = settings.value("Userinterface_Snap_snapIndicatorSize", QVariant::fromValue(21)).toInt();

    _pickboxOutlineWidth = settings.value("Userinterface_pickbox_pickboxOutlineWidth", QVariant::fromValue(1)).toInt();
    _pickboxOutlineColorLeft = settings.value("Userinterface_pickbox_pickboxOutlineColorLeft", QVariant::fromValue(QColor(40, 255, 40, 255))).value<QColor>();
    _pickboxOutlineColorRight = settings.value("Userinterface_pickbox_pickboxOutlineColorRight", QVariant::fromValue(QColor(40, 40, 255, 255))).value<QColor>();
    _pickboxFillColorLeft = settings.value("Userinterface_pickbox_pickboxFillColorLeft", QVariant::fromValue(QColor(127, 255, 127, 127))).value<QColor>();
    _pickboxFillColorRight = settings.value("Userinterface_pickbox_pickboxFillColorRight", QVariant::fromValue(QColor(127, 127, 255, 127))).value<QColor>();

    slot_repaint();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    qreal zoomStep = 1.15;

//    centerOfViewInScene = mapToScene(event->pos());
//    displayCenter = event->pos();

//    QPointF cursorPosF_normal = QPointF((qreal)event->pos().x() / (this->width() - 1), (qreal)event->pos().y() / (this->height() - 1));

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

    translationOffset += (mousePos - QPoint(this->width() / 2, this->height() / 2) - translationOffset) * (1.0 - zoomStep);

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
    // TODO
//    if (!(event->buttons() & Qt::MidButton))
//        emit signal_sceneCoordinateChanged(mapToScene(mousePos));

    if (event->buttons() == Qt::LeftButton)
    {

    }

    if (event->buttons() == Qt::MidButton)
    {
        translationOffset += mouseMoveDelta;
    }

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


//        this->updateArcball(mouseMoveDelta.manhattanLength());

    }


    if (event->buttons() == 0)
    {
        // Item highlighting
        highlightClear();
        highlightItemAtPosition(mousePos);

        // Object Snap
        if (item_lastHighlight != NULL)
        {
            // Basepoint snap
            QList<QVector3D> snap_basepoints;
            if ((mapFromScene(item_lastHighlight->snap_basepoint) - mousePos).manhattanLength() < 50)
                snap_basepoints.append(item_lastHighlight->snap_basepoint);

            // Endpoint / Vertex snap
            QList<QVector3D> snap_vertex_points;
            foreach (QVector3D snap_vertex, item_lastHighlight->snap_vertices)
            {
                if ((mapFromScene(snap_vertex) - mousePos).manhattanLength() < 10)
                    snap_vertex_points.append(snap_vertex);
            }

            // Center Snap
            QList<QVector3D> snap_center_points;
            foreach (QVector3D snap_center, item_lastHighlight->snap_center)
            {
                if ((mapFromScene(snap_center) - mousePos).manhattanLength() < 10)
                    snap_center_points.append(snap_center);
            }

            if (!snap_basepoints.isEmpty())
            {
                this->set_snap_mode(GLWidget::SnapBasepoint);
                this->set_snapPos(snap_basepoints.at(0));
            }
            else if (!snap_vertex_points.isEmpty())
            {
                this->set_snap_mode(GLWidget::SnapEndpoint);
                this->set_snapPos(snap_vertex_points.at(0));
            }
            else if (!snap_center_points.isEmpty())
            {
                this->set_snap_mode(GLWidget::SnapCenter);
                this->set_snapPos(snap_center_points.at(0));
            }
            else
            {
                this->set_snap_mode(GLWidget::SnapNo);
            }

            emit signal_snapFired(this->snapPos_scene, this->snapMode);
        }
        else
        {
            this->set_snap_mode(GLWidget::SnapNo);
            emit signal_snapFired(this->snapPos_scene, this->snapMode);
        }
    }
    else
    {
        this->set_snap_mode(GLWidget::SnapNo);
    }

    this->cursorShown = true;

    emit signal_mouseMoved(QVector3D(mousePos.x(), mousePos.y(), 0));

    slot_repaint();
    event->accept();
}

void GLWidget::updateArcball(int steps)
{
    QPoint arcballDelta = (mousePos - arcballPosOld);
//    QVector3D v = getArcBallVector(arcballPos.x(), arcballPos.y());
//    QVector3D u = getArcBallVector(arcballPosOld.x(), arcballPosOld.y());

//    qreal angle = qAcos(qMin(1.0, QVector3D::dotProduct(u, v)));

//    QVector3D rotAxis = QVector3D::crossProduct(v, u);

//    QMatrix4x4 eyeToObject = arcballRotationMatrix;

//    QVector3D objSpaceRotAx = eyeToObject * rotAxis;

//    qDebug() << 4 * ((360 / (2 * PI)) * angle);

    arcballRotationMatrix.rotate(steps, arcballDelta.y(), arcballDelta.x(), 0.0);
}

QVector3D GLWidget::getArcBallVector(int x, int y)
{
    QVector3D pt = QVector3D(2.0 * x / this->width() - 1.0, 2.0 * y / this->height() - 1.0, 0);
    pt.setY(pt.y() * -1);

    float xySquared = qPow(pt.x(), 2) + qPow(pt.y(), 2);

    if (xySquared <= 1.0)
        pt.setZ(sqrt(1.0 - xySquared));
    else
        pt.normalize();

    return pt;
}

void GLWidget::enterEvent(QEvent *event)
{
    this->setFocus();
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
    this->setFocus();

    if (event->buttons() == Qt::MidButton)
    {
        this->setCursor(Qt::OpenHandCursor);
    }
    else
        this->setCursor(Qt::BlankCursor);


    if (event->buttons() == Qt::RightButton)
    {
        arcballPosOld = mousePos;
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

        if (this->item_lastHighlight != NULL)   // There is an item beyond the cursor, so if it is clicked, select it.
        {
            if (event->modifiers() && Qt::ShiftModifier)
                selectionRemoveItem(item_lastHighlight);
            else
                selectionAddItem(item_lastHighlight);
        }
        else if (!this->isPickActive())
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
        if (this->selection_itemList.count() > 0)
        {
            this->selectionClear();
        }
        break;
    case Qt::Key_E:                         // Edit item
        if (item_lastHighlight != NULL)
        {
            this->itemWizard->showWizard(item_lastHighlight);
        }
        break;
    case Qt::Key_Delete:
        if (this->selection_itemList.count() > 0)
        {
            QList<CADitem*> itemsToDelete = this->selection_itemList;
            selectionClear();
            itemDB->deleteItems(itemsToDelete);
            slot_repaint();
        }
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
    if (event->rect().isNull())
    {
        event->accept();
        return;
    }

    makeCurrent();

    saveGLState();

    qglClearColor(_backgroundColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat screenRatio = (qreal)this->width() / (qreal)this->height();
    //glOrtho(-10 * screenRatio, 10 * screenRatio, -10, 10, -100000, 100000);
    //glTranslatef(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0.0, 0.0, width(), height());
    glTranslatef((qreal)translationOffset.x() / (qreal)this->width() * 2, (qreal)translationOffset.y() / (qreal)this->height() * 2, /*-this->zoomFactor / (qreal)this->height()*/0.0);
    glScalef(this->zoomFactor / screenRatio / (qreal)this->height(), this->zoomFactor / (qreal)this->height(), /*this->zoomFactor / (qreal)this->height() / */ 1.0 / 100000.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);

    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);

    GLfloat glMatrix_modelview[16];
    GLfloat glMatrix_projection[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, glMatrix_modelview);
    this->matrix_modelview = QMatrix4x4(
            glMatrix_modelview[0],
            glMatrix_modelview[1],
            glMatrix_modelview[2],
            glMatrix_modelview[3],
            glMatrix_modelview[4],
            glMatrix_modelview[5],
            glMatrix_modelview[6],
            glMatrix_modelview[7],
            glMatrix_modelview[8],
            glMatrix_modelview[9],
            glMatrix_modelview[10],
            glMatrix_modelview[11],
            glMatrix_modelview[12],
            glMatrix_modelview[13],
            glMatrix_modelview[14],
            glMatrix_modelview[15]
            );
    this->matrix_modelview = this->matrix_modelview.transposed();



//    QMatrix4x4 tmpModelView;
//    tmpModelView.setToIdentity();

//    tmpModelView.translate((qreal)translationOffset.x() / (qreal)this->width() * 2, (qreal)translationOffset.y() / (qreal)this->height() * 2, 0);
//    tmpModelView.scale(this->zoomFactor / screenRatio, this->zoomFactor, this->zoomFactor);
//    tmpModelView = tmpModelView * this->arcballRotationMatrix;

//    tmpModelView = tmpModelView.transposed();
////    tmpModelView.setToIdentity();
////    tmpModelView.scale(0.1);

//    GLfloat modMatrix[16];
//    QVector4D r = tmpModelView.row(0);
//    modMatrix[0] = r.x();
//    modMatrix[1] = r.y();
//    modMatrix[2] = r.z();
//    modMatrix[3] = r.w();
//    r = tmpModelView.row(1);
//    modMatrix[4] = r.x();
//    modMatrix[5] = r.y();
//    modMatrix[6] = r.z();
//    modMatrix[7] = r.w();
//    r = tmpModelView.row(2);
//    modMatrix[8] = r.x();
//    modMatrix[9] = r.y();
//    modMatrix[10] = r.z();
//    modMatrix[11] = r.w();
//    r = tmpModelView.row(3);
//    modMatrix[12] = r.x();
//    modMatrix[13] = r.y();
//    modMatrix[14] = r.z();
//    modMatrix[15] = r.w();

//    glLoadMatrixf(modMatrix);


    glGetFloatv(GL_PROJECTION_MATRIX, glMatrix_projection);
    this->matrix_projection = QMatrix4x4(
            glMatrix_projection[0],
            glMatrix_projection[1],
            glMatrix_projection[2],
            glMatrix_projection[3],
            glMatrix_projection[4],
            glMatrix_projection[5],
            glMatrix_projection[6],
            glMatrix_projection[7],
            glMatrix_projection[8],
            glMatrix_projection[9],
            glMatrix_projection[10],
            glMatrix_projection[11],
            glMatrix_projection[12],
            glMatrix_projection[13],
            glMatrix_projection[14],
            glMatrix_projection[15]
            );

    this->matrix_projection = this->matrix_projection.transposed();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);


//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    glColor4ub(255, 255, 255, 255);

//    glCallList(tile_list);




    QGLFramebufferObjectFormat format;
    format.setSamples(4);
    //format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
    QGLFramebufferObject* fbo = new QGLFramebufferObject(50, 50, format);

    QPainter fbo_painter(fbo);
    fbo_painter.setPen(Qt::cyan);
    fbo_painter.drawText(30, 30, "Hello World");
    fbo_painter.end();

    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

//    glColor4ub(255, 0, 0, 255);
//    glCallList(tile_list);




    glName = 0;
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
        glLineWidth((GLfloat)_cursorWidth);
        glColor4ub(255, 255, 255, 255);
        glBegin(GL_LINES);
        glVertex3i(mousePos.x() - _cursorSize, mousePos.y(), 0);
        glVertex3i(mousePos.x() + _cursorSize, mousePos.y(), 0);
        glVertex3i(mousePos.x(), mousePos.y() - _cursorSize, 0);
        glVertex3i(mousePos.x(), mousePos.y() + _cursorSize, 0);
        glEnd();

        // Cursor Pickbox
        glLineWidth(_cursorPickboxLineWidth);
        qglColor(_cursorPickboxColor);
        QRect pickRect = QRect(0, 0, _cursorPickboxSize, _cursorPickboxSize);
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
                qglColor(_pickboxFillColorLeft);
            else
                qglColor(_pickboxFillColorRight);
            glLineWidth(_pickboxOutlineWidth);
            QRect rect = this->selection();
            glBegin(GL_QUADS);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();

            if (this->pickStartPos.x() < this->mousePos.x())
                qglColor(_pickboxOutlineColorLeft);
            else
                qglColor(_pickboxOutlineColorRight);
            glBegin(GL_LINE_LOOP);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();
        }
    }
    if ((snapMode != SnapNo) && (item_lastHighlight != NULL))
    {
        QString snapText;
        QString itemDescription = "[" + item_lastHighlight->description + "]";
        QString itemPosition = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", this->snapPos_scene.x(), this->snapPos_scene.y(), this->snapPos_scene.z());
        snapText = " of " + itemDescription + itemPosition;

        QRect focusRect = QRect(0, 0, _snapIndicatorSize, _snapIndicatorSize);
        focusRect.moveCenter(this->snapPos_screen);

        glLineWidth(1);

        switch (snapMode)
        {
        case SnapBasepoint:
        {
            glColor4ub(255, 0, 0, 255);
            glBegin(GL_LINE_LOOP);
            glVertex2i(focusRect.bottomLeft().x(), focusRect.bottomLeft().y());
            glVertex2i(focusRect.bottomRight().x(), focusRect.bottomRight().y());
            glVertex2i(focusRect.topLeft().x(), focusRect.topLeft().y());
            glVertex2i(focusRect.topRight().x(), focusRect.topRight().y());
            glEnd();
            snapText.prepend(tr("Basepoint"));

            paintTextInfoBox(focusRect.bottomRight(), snapText);

            break;
        }
        case SnapEndpoint:
        {
            glColor4ub(255, 0, 0, 255);
            glBegin(GL_LINE_LOOP);
            glVertex2i(focusRect.bottomLeft().x(), focusRect.bottomLeft().y());
            glVertex2i(focusRect.bottomRight().x(), focusRect.bottomRight().y());
            glVertex2i(focusRect.topRight().x(), focusRect.topRight().y());
            glVertex2i(focusRect.topLeft().x(), focusRect.topLeft().y());
            glEnd();
            snapText.prepend("Endpoint/Vertex");

            paintTextInfoBox(focusRect.bottomRight(), snapText);

            break;
        }
        case SnapCenter:
        {
            glColor4ub(255, 0, 0, 255);
            glBegin(GL_LINES);
            glVertex2i(focusRect.left(), focusRect.top());
            glVertex2i(focusRect.right(), focusRect.bottom());
            glVertex2i(this->snapPos_screen.x() - 5, this->snapPos_screen.y() + 5);
            glVertex2i(this->snapPos_screen.x() + 5, this->snapPos_screen.y() - 5);
            glEnd();
            snapText.prepend("Center");

            paintTextInfoBox(focusRect.bottomRight(), snapText);

            break;
        }
        case SnapNo:
        {
            break;
        }
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

void GLWidget::paintTextInfoBox(QPoint pos, QString text, QFont font, QColor colorText, QColor colorBackground, QColor colorOutline)
{
    // Calculate text box size
    QFontMetrics fm(font);
    QRect boundingRect = fm.boundingRect(text);
    boundingRect.moveTopLeft(pos + QPoint(5, 2));
    boundingRect.adjust(-5, -5, 5, 5);

    // Draw background
//    glColor4ub(0, 0, 0, 150);
    qglColor(colorBackground);
    glBegin(GL_QUADS);
    glVertex2i(boundingRect.bottomLeft().x(), boundingRect.bottomLeft().y());
    glVertex2i(boundingRect.bottomRight().x(), boundingRect.bottomRight().y());
    glVertex2i(boundingRect.topRight().x(), boundingRect.topRight().y());
    glVertex2i(boundingRect.topLeft().x(), boundingRect.topLeft().y());
    glEnd();

    // Draw outline
//    glColor4ub(200, 200, 200, 150);
    qglColor(colorOutline);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(boundingRect.bottomLeft().x(), boundingRect.bottomLeft().y());
    glVertex2i(boundingRect.bottomRight().x(), boundingRect.bottomRight().y());
    glVertex2i(boundingRect.topRight().x(), boundingRect.topRight().y());
    glVertex2i(boundingRect.topLeft().x(), boundingRect.topLeft().y());
    glEnd();

    // Draw text
//    glColor4ub(255, 0, 0, 255);
    qglColor(colorText);
    this->renderText(pos.x() + 5, this->height() - 1 - (pos.y() + 5),
                     text, font);
}

void GLWidget::paintContent(QList<Layer*> layers)
{
//    quint32 glName = 0;

//    qDebug() << "GLWidget::paintContent: painting"<< layers.count() << "layers...";
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
            case CADitem::Arch_Foundation:
                break;
            case CADitem::Arch_BoredPile:
                break;
            case CADitem::Arch_Grating:
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
                paintAirDuctTeeConnector(layer, (CAD_air_ductTeeConnector*)item);
                break;
            case CADitem::Air_DuctTransition:
                paintAirDuctTransiton(layer, (CAD_air_ductTransition*)item);
                break;
            case CADitem::Air_DuctTransitionRectRound:
                paintAirDuctTransitionRectRound(layer, (CAD_air_ductTransitionRectRound*)item);
                break;
            case CADitem::Air_DuctYpiece:
                paintAirDuctYpiece(layer, (CAD_air_ductYpiece*)item);
                break;
            case CADitem::Air_DuctEndPlate:
                paintAirDuctEndPlate(layer, (CAD_air_ductEndPlate*)item);
                break;
            case CADitem::Air_PipeEndCap:
                paintAirPipeEndCap(layer, (CAD_air_pipeEndCap*)item);
                break;
            case CADitem::Air_ThrottleValve:
                paintAirThrottleValve(layer, (CAD_air_throttleValve*)item);
                break;
            case CADitem::Air_MultiLeafDamper:
                paintAirMultiLeafDamper(layer, (CAD_air_multiLeafDamper*)item);
                break;
            case CADitem::Air_PressureReliefDamper:
                paintAirPressureReliefDamper(layer, (CAD_air_pressureReliefDamper*)item);
                break;
            case CADitem::Air_PipeFireDamper:
                paintAirPipeFireDamper(layer, (CAD_air_pipeFireDamper*)item);
                break;
            case CADitem::Air_DuctFireDamper:
                paintAirDuctFireDamper(layer, (CAD_air_ductFireDamper*)item);
                break;
            case CADitem::Air_DuctVolumetricFlowController:
                paintAirDuctVolumetricFlowController(layer, (CAD_air_ductVolumetricFlowController*)item);
                break;
            case CADitem::Air_PipeVolumetricFlowController:
                paintAirPipeVolumetricFlowController(layer, (CAD_air_pipeVolumetricFlowController*)item);
                break;
            case CADitem::Air_HeatExchangerWaterAir:
                paintAirHeatExchangerWaterAir(layer, (CAD_air_heatExchangerWaterAir*)item);
                break;
            case CADitem::Air_HeatExchangerAirAir:
                paintAirHeatExchangerAirAir(layer, (CAD_air_heatExchangerAirAir*)item);
                break;
            case CADitem::Air_CanvasFlange:
                paintAirCanvasFlange(layer, (CAD_air_canvasFlange*)item);
                break;
            case CADitem::Air_Filter:
                paintAirFilter(layer, (CAD_air_filter*)item);
                break;
            case CADitem::Air_PipeSilencer:
                paintAirPipeSilencer(layer, (CAD_air_pipeSilencer*)item);
                break;
            case CADitem::Air_DuctBaffleSilencer:
                paintAirDuctBaffleSilencer(layer, (CAD_air_ductBaffleSilencer*)item);
                break;
            case CADitem::Air_Fan:
                paintAirFan(layer, (CAD_air_fan*)item);
                break;
            case CADitem::Air_Humidifier:
                paintAirHumidifier(layer, (CAD_air_humidifier*)item);
                break;
            case CADitem::Air_EmptyCabinet:
                paintAirEmptyCabinet(layer, (CAD_air_emptyCabinet*)item);
                break;
            case CADitem::Air_EquipmentFrame:
                paintAirEquipmentFrame(layer, (CAD_air_equipmentFrame*)item);
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
                paintHeatCoolPipeTurn(layer, (CAD_heatcool_pipeTurn*)item);
                break;
            case CADitem::HeatCool_PipeReducer:
                paintHeatCoolPipeReducer(layer, (CAD_heatcool_pipeReducer*)item);
                break;
            case CADitem::HeatCool_PipeTeeConnector:
                paintHeatCoolPipeTeeConnector(layer, (CAD_heatcool_pipeTeeConnector*)item);
                break;
            case CADitem::HeatCool_PipeEndCap:
                paintHeatCoolPipeEndCap(layer, (CAD_heatcool_pipeEndCap*)item);
                break;
            case CADitem::HeatCool_Flange:
                paintHeatCoolFlange(layer, (CAD_heatcool_flange*)item);
                break;
            case CADitem::HeatCool_ExpansionChamber:
                paintHeatCoolExpansionChamber(layer, (CAD_heatcool_expansionChamber*)item);
                break;
            case CADitem::HeatCool_Boiler:
                paintHeatCoolBoiler(layer, (CAD_heatcool_boiler*)item);
                break;
            case CADitem::HeatCool_WaterHeater:
                paintHeatCoolWaterHeater(layer, (CAD_heatcool_waterHeater*)item);
                break;
            case CADitem::HeatCool_StorageBoiler:
                paintHeatCoolStorageBoiler(layer, (CAD_heatcool_storageBoiler*)item);
                break;
            case CADitem::HeatCool_Radiator:
                paintHeatCoolRadiator(layer, (CAD_heatcool_radiator*)item);
                break;
            case CADitem::HeatCool_Filter:
                paintHeatCoolFilter(layer, (CAD_heatcool_filter*)item);
                break;
            case CADitem::HeatCool_BallValve:
                paintHeatCoolBallValve(layer, (CAD_heatcool_ballValve*)item);
                break;
            case CADitem::HeatCool_ButterflyValve:
                paintHeatCoolButterflyValve(layer, (CAD_heatcool_butterflyValve*)item);
                break;
            case CADitem::HeatCool_SafetyValve:
                paintHeatCoolSafteyValve(layer, (CAD_heatcool_safetyValve*)item);
                break;
            case CADitem::HeatCool_Flowmeter:
                paintHeatCoolFlowmeter(layer, (CAD_heatcool_flowmeter*)item);
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
            case CADitem::Sprinkler_PipeTurn:
                paintSprinklerPipeTurn(layer, (CAD_sprinkler_pipeTurn*)item);
                break;
            case CADitem::Sprinkler_PipeReducer:
                paintSprinklerPipeReducer(layer, (CAD_sprinkler_pipeReducer*)item);
                break;
            case CADitem::Sprinkler_PipeEndCap:
                paintSprinklerPipeEndCap(layer, (CAD_sprinkler_pipeEndCap*)item);
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
    QColor color_pen = getColorPen(item, layer);

    qreal penWidth = 1.0;
    if (item->widthByLayer)
    {
        penWidth = layer->width / 100.0;
    }
    else if (item->widthByBlock)
    {

    }
    else
    {
        penWidth = item->width;
    }

    // Default width setting
    if (penWidth < 1.0)
        penWidth = 1.0;

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

    QColor color_pen = getColorPen(item, layer);

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
                penWidth = layer->width / 100.0;
            }
            else if (item->widthByBlock)
            {

            }
            else
            {
                penWidth = vertex.widthStart;
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
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

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
    QColor color_pen = getColorPen(item, layer);

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

QColor GLWidget::getColorPen(CADitem* item, Layer* layer)
{
    QColor color_pen = item->color_pen;

    if (color_pen == Qt::transparent)   // BYLAYER
    {
        color_pen = layer->pen.color();
    }
    else if (color_pen.value() < 50)
        color_pen = Qt::white;

    if (item->highlight || item->selected)
    {
        if (color_pen.lightnessF() > 0.5)
            color_pen = color_pen.darker();
        else
            color_pen = color_pen.lighter();
    }

    return color_pen;
}

QColor GLWidget::getColorBrush(CADitem* item, Layer* layer)
{
    QColor color_brush = item->color_brush;

    if (color_brush == Qt::transparent)   // BYLAYER
    {
        color_brush = layer->brush.color();
    }
    else if (color_brush.value() < 50)
        color_brush = Qt::white;

    if (item->highlight || item->selected)
    {
        if (color_brush.lightnessF() > 0.5)
            color_brush = color_brush.darker();
        else
            color_brush = color_brush.lighter();
    }

    return color_brush;
}

void GLWidget::paintBasicBox(Layer *layer, CAD_basic_box *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

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
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

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
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    GLdouble radius = (GLdouble)item->radius;

    GLUquadricObj* sphere = gluNewQuadric();

    glPushMatrix();
    glTranslatef(item->position.x(), item->position.y(), item->position.z());
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricTexture(sphere, GL_TRUE);

    if (this->render_solid)
    {
        qglColor(color_brush);
        gluSphere(sphere, radius, 32, 32);
    }
    if (this->render_outline)
    {
        qglColor(color_pen);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gluSphere(sphere, radius * 1.001, 32, 32);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glPopMatrix();
    gluDeleteQuadric(sphere);
}

void GLWidget::paintArchLevelSlab(Layer *layer, CAD_arch_levelSlab *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchWallLoadBearing(Layer *layer, CAD_arch_wall_loadBearing *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchWallNonLoadBearing(Layer *layer, CAD_arch_wall_nonLoadBearing *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchBlockOut(Layer *layer, CAD_arch_blockOut *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchDoor(Layer *layer, CAD_arch_door *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchWindow(Layer *layer, CAD_arch_window *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuct(Layer *layer, CAD_air_duct *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipe(Layer *layer, CAD_air_pipe *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctTurn(Layer *layer, CAD_air_ductTurn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeTurn(Layer *layer, CAD_air_pipeTurn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeReducer(Layer *layer, CAD_air_pipeReducer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeTeeConnector(Layer *layer, CAD_air_pipeTeeConnector *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctTeeConnector(Layer *layer, CAD_air_ductTeeConnector *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctTransiton(Layer *layer, CAD_air_ductTransition *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctTransitionRectRound(Layer *layer, CAD_air_ductTransitionRectRound *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctYpiece(Layer *layer, CAD_air_ductYpiece *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctEndPlate(Layer *layer, CAD_air_ductEndPlate *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeEndCap(Layer *layer, CAD_air_pipeEndCap *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirThrottleValve(Layer *layer, CAD_air_throttleValve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirMultiLeafDamper(Layer *layer, CAD_air_multiLeafDamper *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPressureReliefDamper(Layer *layer, CAD_air_pressureReliefDamper *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeFireDamper(Layer *layer, CAD_air_pipeFireDamper *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctFireDamper(Layer *layer, CAD_air_ductFireDamper *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctVolumetricFlowController(Layer *layer, CAD_air_ductVolumetricFlowController *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeVolumetricFlowController(Layer *layer, CAD_air_pipeVolumetricFlowController *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirHeatExchangerWaterAir(Layer *layer, CAD_air_heatExchangerWaterAir *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirHeatExchangerAirAir(Layer *layer, CAD_air_heatExchangerAirAir *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirCanvasFlange(Layer *layer, CAD_air_canvasFlange *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirFilter(Layer *layer, CAD_air_filter *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirPipeSilencer(Layer *layer, CAD_air_pipeSilencer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirDuctBaffleSilencer(Layer *layer, CAD_air_ductBaffleSilencer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirFan(Layer *layer, CAD_air_fan *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirHumidifier(Layer *layer, CAD_air_humidifier *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirEmptyCabinet(Layer *layer, CAD_air_emptyCabinet *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintAirEquipmentFrame(Layer *layer, CAD_air_equipmentFrame *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolAdjustvalve(Layer *layer, CAD_heatcool_adjustvalve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolChiller(Layer *layer, CAD_heatcool_chiller *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolControlvalve(Layer *layer, CAD_heatcool_controlvalve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolCoolingTower(Layer *layer, CAD_heatcool_coolingTower *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolHeatExchanger(Layer *layer, CAD_heatcool_heatExchanger *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPipe(Layer *layer, CAD_heatcool_pipe *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPump(Layer *layer, CAD_heatcool_pump *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolSensor(Layer *layer, CAD_heatcool_sensor *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPipeTurn(Layer *layer, CAD_heatcool_pipeTurn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPipeReducer(Layer *layer, CAD_heatcool_pipeReducer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPipeTeeConnector(Layer *layer, CAD_heatcool_pipeTeeConnector *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolPipeEndCap(Layer *layer, CAD_heatcool_pipeEndCap *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolFlange(Layer *layer, CAD_heatcool_flange *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolExpansionChamber(Layer *layer, CAD_heatcool_expansionChamber *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolBoiler(Layer *layer, CAD_heatcool_boiler *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolWaterHeater(Layer *layer, CAD_heatcool_waterHeater *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolStorageBoiler(Layer *layer, CAD_heatcool_storageBoiler *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolRadiator(Layer *layer, CAD_heatcool_radiator *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolFilter(Layer *layer, CAD_heatcool_filter *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolBallValve(Layer *layer, CAD_heatcool_ballValve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolButterflyValve(Layer *layer, CAD_heatcool_butterflyValve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolSafteyValve(Layer *layer, CAD_heatcool_safetyValve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintHeatCoolFlowmeter(Layer *layer, CAD_heatcool_flowmeter *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerCompressedAirWaterContainer(Layer *layer, CAD_sprinkler_compressedAirWaterContainer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerDistribution(Layer *layer, CAD_sprinkler_distribution *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerHead(Layer *layer, CAD_sprinkler_head *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerPipe(Layer *layer, CAD_sprinkler_pipe *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerPump(Layer *layer, CAD_sprinkler_pump *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerTeeConnector(Layer *layer, CAD_sprinkler_teeConnector *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerValve(Layer *layer, CAD_sprinkler_valve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerWetAlarmValve(Layer *layer, CAD_sprinkler_wetAlarmValve *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerZoneCheck(Layer *layer, CAD_sprinkler_zoneCheck *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerPipeTurn(Layer *layer, CAD_sprinkler_pipeTurn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerPipeEndCap(Layer *layer, CAD_sprinkler_pipeEndCap *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSprinklerPipeReducer(Layer *layer, CAD_sprinkler_pipeReducer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintElectricalCabinet(Layer *layer, CAD_electrical_cabinet *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintElectricalCabletray(Layer *layer, CAD_electrical_cableTray *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

CADitem* GLWidget::itemAtPosition(QPoint pos)
{
#define HITBUFFER_SIZE 512000
    GLuint buffer[HITBUFFER_SIZE];
    GLint viewport[4];

    saveGLState();

    //    glViewport(translationOffset.x(), translationOffset.y(), width(), height());
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
//    glTranslatef(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);

    glTranslatef((qreal)translationOffset.x() / (qreal)this->width() * 2, (qreal)translationOffset.y() / (qreal)this->height() * 2, /*-this->zoomFactor / (qreal)this->height()*/0.0);
    glScalef(this->zoomFactor / screenRatio / (qreal)this->height(), this->zoomFactor / (qreal)this->height(), /*this->zoomFactor / (qreal)this->height() / */ 1.0 / 100000.0);
//    glTranslatef((qreal)translationOffset.x() / (qreal)this->width() * 2, (qreal)translationOffset.y() / (qreal)this->height() * 2, 0);
//    glScalef(this->zoomFactor / screenRatio / (qreal)this->height(), this->zoomFactor / (qreal)this->height(), this->zoomFactor / (qreal)this->height());
//    glScaled(this->zoomFactor / screenRatio, this->zoomFactor, this->zoomFactor);
    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);

    glName = 0;
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
    // tst
    this->item_lastHighlight = item;

    if (item != NULL)
        item->highlight = true;

    emit signal_highlightItem(item);
}

void GLWidget::highlightClear()
{
    this->item_lastHighlight = NULL;
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

void GLWidget::selectionAddItem(CADitem *item)
{
    if (item != NULL)
    {
        if (selection_itemList.contains(item))
            return;
        this->selection_itemList.append(item);
        item->selected = true;
        emit signal_selectionChanged(selection_itemList);
    }
}

void GLWidget::selectionRemoveItem(CADitem *item)
{
    if (item != NULL)
    {
        if (!selection_itemList.contains(item))
            return;
        this->selection_itemList.removeOne(item);
        item->selected = false;
        emit signal_selectionChanged(selection_itemList);
    }
}

void GLWidget::selectionClear()
{
    selectionClear_processLayers(itemDB->layers);
    this->selection_itemList.clear();
    emit signal_selectionChanged(this->selection_itemList);
}

void GLWidget::selectionClear_processLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        selectionClear_processItems(layer->items);
        selectionClear_processLayers(layer->subLayers);
    }
}

void GLWidget::selectionClear_processItems(QList<CADitem *> items)
{
    foreach (CADitem* item, items)
    {
        item->selected = false;
        selectionClear_processItems(item->subItems);
    }
}
