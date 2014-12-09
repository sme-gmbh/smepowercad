#include "glwidget.h"

//#define PI 3.1415926535897

GLWidget::GLWidget(QWidget *parent, ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier *itemGripModifier, QGLFormat glFormat) :
    QGLWidget(glFormat, parent)
  // Qt 5
  //    m_context(0),
  //    m_device(0)
{
    //    setSurfaceType(QWidget::OpenGLSurface);
    //    qDebug() << "Created GLWidget";
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->itemGripModifier = itemGripModifier;
    this->mousePos = QPoint();

    this->translationOffset = QPoint();
    this->zoomFactor = 1.0;
    this->centerOfViewInScene = QVector3D();
    this->displayCenter = QPoint();
    this->cuttingplane = CuttingPlane_nZ;
    this->height_of_intersection = QVector3D(0.0, 0.0, 0.0);
    this->depth_of_view = QVector3D(0.0, 0.0, 0.0);
    this->render_solid = true;
    this->render_outline = true;
    this->cameraPosition = QVector3D();
    this->lookAtPosition = QVector3D(0.0f, 0.0f, 0.0f);
    this->matrix_modelview.setToIdentity();
    this->matrix_projection.setToIdentity();
    this->matrix_rotation.setToIdentity();
    this->matrix_rotation_old.setToIdentity();
    this->matrix_arcball.setToIdentity();
    this->arcballRadius = 500.0;
    this->matrix_glSelect.setToIdentity();
    this->matrix_all.setToIdentity();

    this->pickActive = false;
    this->cursorShown = true;
    this->arcballShown = false;
    this->snapMode = SnapNo;
    this->item_lastHighlight = NULL;

    slot_update_settings();

    this->setMouseTracking(true);

    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);

//    initializeGL();

    //    shaderProgram->setAttributeArray(shader_colorLocation, );
    //    shaderProgram->enableAttributeArray(shader_colorLocation);
    connect(&timer_findItemAtPosition, SIGNAL(timeout()), this, SLOT(slot_timer_findItemAtPosition_triggered()));
    timer_findItemAtPosition.setInterval(200);
    timer_findItemAtPosition.setSingleShot(true);
}

GLWidget::~GLWidget()
{
    //    qDebug() << "GLWidget destroyed";

    //    glDeleteLists(tile_list, 1);
    shaderProgram->release();
    shaderProgram->removeAllShaders();
    delete shader_1_vert;
    delete shader_1_frag;
    delete shaderProgram;
}

QPointF GLWidget::mapFromScene(QVector3D &scenePoint)
{
    //    QVector4D screenCoords;

    //    screenCoords = matrix_all * scenePoint;

    qreal x;
    qreal y;

    QVector4D row0 = matrix_all.row(0);
    QVector4D row1 = matrix_all.row(1);

    x = row0.x() * scenePoint.x() + row0.y() * scenePoint.y() + row0.z() * scenePoint.z() + row0.w();
    y = row1.x() * scenePoint.x() + row1.y() * scenePoint.y() + row1.z() * scenePoint.z() + row1.w();

    //    QPointF pixelCoords = screenCoords.toPointF() ;

    //    pixelCoords.setX((pixelCoords.x() / 2.0) * this->width());
    //    pixelCoords.setY((pixelCoords.y() / 2.0) * this->height());

    //    return pixelCoords;

    return QPointF(x / 2.0 * this->width(), y / 2.0 * this->height());
}

void GLWidget::updateMatrixAll()
{
    matrix_all = matrix_projection * matrix_glSelect * matrix_modelview * matrix_rotation;
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
    else
        return Qt::IntersectsItemShape;
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
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(rot_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(rot_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(rot_z, 0.0, 0.0, 1.0);
    updateMatrixAll();
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

void GLWidget::slot_itemDeleted(CADitem *item)
{
    if (item == item_lastHighlight)
    {
        item_lastHighlight = NULL;
        snapMode = SnapNo;
    }
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
    //    rot_x += -((float)a / 15.0);
    //    rot_y += -((float)b / 15.0);
    //    rot_z += -((float)c / 15.0);

    updateMatrixAll();
    emit signal_matrix_rotation_changed(matrix_rotation);
    slot_repaint();
}

void GLWidget::slot_update_settings()
{
    _backgroundColor = settings.value("Design_Colors_backgroundColor", QVariant::fromValue(QColor().black())).value<QColor>();

    _cursorSize = settings.value("Userinterface_Cursor_cursorSize", QVariant::fromValue(4500)).toInt();
    _cursorWidth = settings.value("Userinterface_Cursor_cursorLineWidth", QVariant::fromValue(1)).toInt();
    _cursorPickboxSize = settings.value("Userinterface_Cursor_cursorPickboxSize", QVariant::fromValue(11)).toInt() * 2 + 1;
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

void GLWidget::slot_timer_findItemAtPosition_triggered()
{
    //    // Item highlighting
    //    highlightClear();
    //    highlightItemAtPosition(mousePos);

    //    // Object Snap
    //    if (item_lastHighlight != NULL)
    //    {
    //        // Basepoint snap
    //        QList<QVector3D> snap_basepoints;
    //        if ((mapFromScene(item_lastHighlight->snap_basepoint) - mousePos).manhattanLength() < 50)
    //            snap_basepoints.append(item_lastHighlight->snap_basepoint);

    //        // Endpoint / Vertex snap
    //        QList<QVector3D> snap_vertex_points;
    //        foreach (QVector3D snap_vertex, item_lastHighlight->snap_vertices)
    //        {
    //            if ((mapFromScene(snap_vertex) - mousePos).manhattanLength() < 10)
    //                snap_vertex_points.append(snap_vertex);
    //        }

    //        // Center Snap
    //        QList<QVector3D> snap_center_points;
    //        foreach (QVector3D snap_center, item_lastHighlight->snap_center)
    //        {
    //            if ((mapFromScene(snap_center) - mousePos).manhattanLength() < 10)
    //                snap_center_points.append(snap_center);
    //        }

    //        if (!snap_basepoints.isEmpty())
    //        {
    //            this->set_snap_mode(GLWidget::SnapBasepoint);
    //            this->set_snapPos(snap_basepoints.at(0));
    //        }
    //        else if (!snap_vertex_points.isEmpty())
    //        {
    //            this->set_snap_mode(GLWidget::SnapEndpoint);
    //            this->set_snapPos(snap_vertex_points.at(0));
    //        }
    //        else if (!snap_center_points.isEmpty())
    //        {
    //            this->set_snap_mode(GLWidget::SnapCenter);
    //            this->set_snapPos(snap_center_points.at(0));
    //        }
    //        else
    //        {
    //            this->set_snap_mode(GLWidget::SnapNo);
    //        }

    //        emit signal_snapFired(this->snapPos_scene, this->snapMode);
    //    }
    //    else
    //    {
    //        this->set_snap_mode(GLWidget::SnapNo);
    //        emit signal_snapFired(this->snapPos_scene, this->snapMode);
    //    }
    //    slot_repaint();
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

    //    translationOffset += (mousePos - QPoint(this->width() / 2, this->height() / 2) - translationOffset) * (1.0 - zoomStep);
    translationOffset += (mousePos - translationOffset) * (1.0 - zoomStep);

    event->accept();

    // todo: Map to scene
    //    emit signal_sceneCoordinateChanged(mapToScene(event->pos()));
    slot_repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    mousePos = event->pos();// - QPoint(1, 1);
    mousePos.setY((this->height() - 1) - mousePos.y());
    mousePos -= QPoint(this->width() / 2, this->height() / 2);
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
        QVector3D rotationEnd = pointOnSphere( mousePos );
        double angle = acos( QVector3D::dotProduct( rotationStart, rotationEnd ));
        QVector4D axis4D = matrix_rotation_old.transposed() * QVector4D(QVector3D::crossProduct(rotationStart, rotationEnd), 0.0);

        matrix_arcball.setToIdentity();
        matrix_arcball.translate(this->lookAtPosition);
        matrix_arcball.rotate(angle/PI*180,axis4D.toVector3D());
        matrix_arcball.translate(-1.0 * this->lookAtPosition);

        matrix_rotation = matrix_rotation_old * matrix_arcball;

        updateMatrixAll();
        emit signal_matrix_rotation_changed(matrix_rotation);
    }

    if ((event->buttons() == 0) && (this->pickActive == false))
    {
        //        timer_findItemAtPosition.start();
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

            // Flange snap
            QList<QVector3D> snap_flanges;
            foreach(QVector3D snap_flange, item_lastHighlight->snap_flanges)
            {
                if ((mapFromScene(snap_flange) - mousePos).manhattanLength() < 10)
                    snap_flanges.append(snap_flange);
            }

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

            if (!snap_flanges.isEmpty())
            {
                this->set_snap_mode(GLWidget::SnapFlange);
                this->set_snapPos(snap_flanges.at(0));
            }
            else if (!snap_basepoints.isEmpty())
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

QVector3D GLWidget::pointOnSphere(QPoint pointOnScreen)
{
    QPoint lookAtScreenCoords = mapFromScene(lookAtPosition).toPoint();
    double x = pointOnScreen.x();
    double y = pointOnScreen.y();
    double center_x = lookAtScreenCoords.x();
    double center_y = lookAtScreenCoords.y();
    QVector3D v;
    v.setX((x - center_x) / arcballRadius);
    v.setY((y - center_y) / arcballRadius);
    double r = v.x() * v.x() + v.y() * v.y();
    if (r > 1.0d)
    {
        v.normalize();
    }
    else
    {
        v.setZ( sqrt(1.0d - r) );
    }
    return v;
}

//void GLWidget::updateArcball(int steps)
//{
//    QPoint arcballDelta = (mousePos - arcballPosOld);
//    //    QVector3D v = getArcBallVector(arcballPos.x(), arcballPos.y());
//    //    QVector3D u = getArcBallVector(arcballPosOld.x(), arcballPosOld.y());

//    //    qreal angle = qAcos(qMin(1.0, QVector3D::dotProduct(u, v)));

//    //    QVector3D rotAxis = QVector3D::crossProduct(v, u);

//    //    QMatrix4x4 eyeToObject = arcballRotationMatrix;

//    //    QVector3D objSpaceRotAx = eyeToObject * rotAxis;
//}

//QVector3D GLWidget::getArcBallVector(int x, int y)
//{
//    QVector3D pt = QVector3D(2.0 * x / this->width() - 1.0, 2.0 * y / this->height() - 1.0, 0);
//    pt.setY(pt.y() * -1);

//    float xySquared = qPow(pt.x(), 2) + qPow(pt.y(), 2);

//    if (xySquared <= 1.0)
//        pt.setZ(sqrt(1.0 - xySquared));
//    else
//        pt.normalize();

//    return pt;
//}

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
        rotationStart = pointOnSphere( mousePos );
        matrix_rotation_old = matrix_rotation;
        this->arcballShown = true;
    }

    // Object drawing and manipulation
    if (event->buttons() == Qt::LeftButton)
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            if (this->snapMode != SnapNo)
            {
                this->lookAtPosition = this->snapPos_scene;
            }
            event->accept();
            return;
        }

        // Check if there is a currently active command
        if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Move)
        {
            if (snapMode != SnapNo)
            {
                CADitem* item = this->itemGripModifier->getItem();
                item->wizardParams.insert("Position x", QVariant::fromValue(snapPos_scene.x()));
                item->wizardParams.insert("Position y", QVariant::fromValue(snapPos_scene.y()));
                item->wizardParams.insert("Position z", QVariant::fromValue(snapPos_scene.z()));
                item->processWizardInput();
                item->calculate();
                this->itemGripModifier->finishGrip();
                this->slot_repaint();
            }
        }
        else if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Copy)
        {
            if (snapMode != SnapNo)
            {
                CADitem* item = this->itemGripModifier->getItem();
                CADitem* newItem = this->itemDB->drawItem(item->layerName, item->getType());
                newItem->wizardParams = item->wizardParams;
                newItem->wizardParams.insert("Position x", QVariant::fromValue(snapPos_scene.x()));
                newItem->wizardParams.insert("Position y", QVariant::fromValue(snapPos_scene.y()));
                newItem->wizardParams.insert("Position z", QVariant::fromValue(snapPos_scene.z()));
                newItem->processWizardInput();
                newItem->calculate();
                this->itemGripModifier->finishGrip();
                this->slot_repaint();
            }
        }

        // Pickbox
        //        if (!this->overlay->isPickActive())
        //            this->overlay->pickStart();

        else if ((this->item_lastHighlight != NULL) && (!this->isPickActive()))   // There is an item beyond the cursor, so if it is clicked, select it.
        {
            if (event->modifiers() && Qt::ShiftModifier)
                selectionRemoveItem(item_lastHighlight);
            else if (snapMode == SnapFlange)
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Append, QCursor::pos(), snapPos_scene);
            }
            else
                selectionAddItem(item_lastHighlight);
        }
        else if (!this->isPickActive())
            this->pickStart();
        else
        {
            QList<CADitem*> pickedItems = this->itemsAtPosition(this->selection().center(), this->selection().width(), this->selection().height());
            if (this->selectionMode() == Qt::IntersectsItemShape)
                selectionAddItems(pickedItems);
            else if (this->selectionMode() == Qt::ContainsItemShape)
            {
                QSet<CADitem*> surroundingItems;
                surroundingItems.unite(this->itemsAtPosition(((this->selection().topLeft() + this->selection().topRight()) / 2), this->selection().width(), 2).toSet());
                surroundingItems.unite(this->itemsAtPosition(((this->selection().bottomLeft() + this->selection().bottomRight()) / 2), this->selection().width(), 2).toSet());
                surroundingItems.unite(this->itemsAtPosition(((this->selection().topLeft() + this->selection().bottomLeft()) / 2), 2, this->selection().height()).toSet());
                surroundingItems.unite(this->itemsAtPosition(((this->selection().topRight() + this->selection().bottomRight()) / 2), 2, this->selection().height()).toSet());
                selectionAddItems(pickedItems.toSet().subtract(surroundingItems).toList());
            }

            this->pickEnd();
            event->accept();
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

    if (event->button() == Qt::RightButton)
    {
        this->arcballShown = false;
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
        if (this->itemGripModifier->getActiveGrip() != ItemGripModifier::Grip_None)
        {
            this->itemGripModifier->finishGrip();
            this->slot_repaint();
        }
        break;
    case Qt::Key_C:                         // Copy item
        if (item_lastHighlight != NULL)
        {
            if (snapMode == SnapBasepoint)
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Copy, QCursor::pos(), snapPos_scene);
                this->slot_repaint();
            }
        }
        break;
    case Qt::Key_E:                         // Edit item
        if (item_lastHighlight != NULL)
        {
            this->itemWizard->showWizard(item_lastHighlight);
        }
        break;
    case Qt::Key_F:                         // Flange item
        if (item_lastHighlight != NULL)
        {
            if (snapMode == SnapFlange)
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Append, QCursor::pos(), snapPos_scene);
            }
        }
        break;
    case Qt::Key_L:                         // Change length of item
        if (item_lastHighlight != NULL)
        {
            this->itemGripModifier->setItem(item_lastHighlight);
            this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Length, QCursor::pos(), snapPos_scene);
        }
        break;
    case Qt::Key_M:                         // Move item
        if (item_lastHighlight != NULL)
        {
            if (snapMode == SnapBasepoint)
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Move, QCursor::pos(), snapPos_scene);
                this->slot_repaint();
            }
        }
        break;
    case Qt::Key_X:                         // Turn item around x axis
        if (item_lastHighlight != NULL)
        {
            item_lastHighlight->angle_x += 45.0;
            if (item_lastHighlight->angle_x > 359.0) item_lastHighlight->angle_x = 0.0;
            item_lastHighlight->wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(item_lastHighlight->angle_x));
            item_lastHighlight->processWizardInput();
            item_lastHighlight->calculate();
            slot_repaint();

            qDebug() << "Bin da";
        }
        break;
    case Qt::Key_Y:                         // Turn item around y axis
        if (item_lastHighlight != NULL)
        {
            item_lastHighlight->angle_y += 45.0;
            if (item_lastHighlight->angle_y > 359.0) item_lastHighlight->angle_y = 0.0;
            item_lastHighlight->wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(item_lastHighlight->angle_y));
            item_lastHighlight->processWizardInput();
            item_lastHighlight->calculate();
            slot_repaint();
        }
        break;
    case Qt::Key_Z:                         // Turn item around z axis
        if (item_lastHighlight != NULL)
        {
            item_lastHighlight->angle_z += 45.0;
            if (item_lastHighlight->angle_z > 359.0) item_lastHighlight->angle_z = 0.0;
            item_lastHighlight->wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(item_lastHighlight->angle_z));
            item_lastHighlight->processWizardInput();
            item_lastHighlight->calculate();
            slot_repaint();
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

    matrix_projection.setToIdentity();
    matrix_projection.scale(2.0 / (qreal)this->width(), 2.0 / (qreal)this->height(), 1.0);
    matrix_projection.translate(-0.5, -0.5);

    updateMatrixAll();
    slot_repaint();
    event->accept();
}

void GLWidget::slot_set_cuttingplane_values_changed(qreal height, qreal depth)
{
    this->height_of_intersection.setZ(height);
    this->depth_of_view.setZ(depth);

    shaderProgram->setUniformValue(shader_Height_of_intersection_location, this->height_of_intersection);
    shaderProgram->setUniformValue(shader_Depth_of_view_location,  this->height_of_intersection - this->depth_of_view);

    slot_repaint();
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

    //    qreal screenRatio = (qreal)this->width() / (qreal)this->height();

    matrix_modelview.setToIdentity();
    matrix_modelview.translate(translationOffset.x(), translationOffset.y(), 0.0);
    matrix_modelview.scale(this->zoomFactor, this->zoomFactor, 1.0 / 100000.0);
    updateMatrixAll();

    //    shaderProgram->setUniformValue(shader_matrixLocation, matrix_projection * matrix_modelview * matrix_rotation);
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);


    qglClearColor(_backgroundColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0.0, 0.0, width(), height());
    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
//    glDepthFunc(GL_GREATER);
    glDepthRange(1,0);
    glPolygonOffset(0.0, 3.0);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);




    //    QGLFramebufferObjectFormat format;
    //    format.setSamples(4);
    //    //format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
    //    QGLFramebufferObject* fbo = new QGLFramebufferObject(50, 50, format);

    //    QPainter fbo_painter(fbo);
    //    fbo_painter.setPen(Qt::cyan);
    //    fbo_painter.drawText(30, 30, "Hello World");
    //    fbo_painter.end();

    //    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glEnable(GL_TEXTURE_2D);


    shaderProgram->setUniformValue(shader_useClippingXLocation, (GLint)0);   // Enable X-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingYLocation, (GLint)0);   // Enable Y-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingZLocation, (GLint)1);   // Enable Z-Clipping Plane

    glName = 0;
    paintContent(itemDB->layers);
    shaderProgram->setUniformValue(shader_useClippingXLocation,(GLint)0);
    shaderProgram->setUniformValue(shader_useClippingYLocation,(GLint)0);
    shaderProgram->setUniformValue(shader_useClippingZLocation,(GLint)0);

    restoreGLState();

    // Overlay
    saveGLState();
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthRange(1,0);
    glViewport(0, 0, width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set a matrix to the shader that does not rotate or scale, just transform to screen coordinate system
    QMatrix4x4 matrix_coordinateBoxScale;
    matrix_coordinateBoxScale.translate(90 - this->width() / 2, 90 - this->height() / 2);
    matrix_coordinateBoxScale.scale(40.0, 40.0, 0.1);
    QMatrix4x4 matrix_rotationOnly = matrix_rotation;
    matrix_rotationOnly.setColumn(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    shaderProgram->setUniformValue(shader_matrixLocation,matrix_projection * matrix_coordinateBoxScale * matrix_rotationOnly);
    glEnable(GL_DEPTH_TEST);

    // Coordinate orientation display
    QImage textImage(80, 80, QImage::Format_ARGB32);
    QPainter painter(&textImage);
    painter.setPen(Qt::white);
    QFont font_big;
    font_big.setPixelSize(25);
    QFont font_small;
    font_small.setPixelSize(12);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLuint textureID;
    setUseTexture(true);

    // Bottom face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Z+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking up");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    setPaintingColor(QColor(50, 50, 255));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3f(-1, -1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3f( 1, -1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3f( 1,  1, -1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3f(-1,  1, -1);
    glEnd();
    this->deleteTexture(textureID);

    // Top face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Z-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking down");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3f( 1, -1, 1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3f(-1, -1, 1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3f(-1,  1, 1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3f( 1,  1, 1);
    glEnd();
    this->deleteTexture(textureID);

    // Front face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Y-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking south");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    setPaintingColor(QColor(10, 110, 10));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i(-1,  1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i( 1,  1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i( 1,  1,  1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i(-1,  1,  1);
    glEnd();
    this->deleteTexture(textureID);

    // Back face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Y+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking north");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i(-1, -1, -1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i( 1, -1, -1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i( 1, -1,  1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i(-1, -1,  1);
    glEnd();
    this->deleteTexture(textureID);

    // Left face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "X+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking east");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    setPaintingColor(QColor(150, 0, 0));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i(-1, -1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i(-1,  1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i(-1,  1,  1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i(-1, -1,  1);
    glEnd();
    this->deleteTexture(textureID);

    // Right face
    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "X-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking west");
    textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i( 1, -1, -1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i( 1,  1, -1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i( 1,  1,  1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i( 1, -1,  1);
    glEnd();
    this->deleteTexture(textureID);

    setUseTexture(false);
    painter.end();


//    QVector4D xAxis = QVector4D(50.0,  0.0,  0.0, 0.0);
//    QVector4D yAxis = QVector4D(0.0,  50.0,  0.0, 0.0);
//    QVector4D zAxis = QVector4D(0.0,   0.0, 50.0, 0.0);
//    glBegin(GL_LINES);
//    glLineWidth(1.0);
//    setPaintingColor(Qt::red);
//    glVertex3i(-width() / 2 + 50, -height() / 2 + 50, 0);
//    glVertex3i((GLint)xAxis.x() - width() / 2 + 50, (GLint)xAxis.y() - height() / 2 + 50, 0);
//    setPaintingColor(QColor(50, 255, 50));
//    glVertex3i(-width() / 2 + 50, -height() / 2 + 50, 0);
//    glVertex3i((GLint)yAxis.x() - width() / 2 + 50, (GLint)yAxis.y() - height() / 2 + 50, 0);
//    setPaintingColor(QColor(50, 50, 255));
//    glVertex3i(-width() / 2 + 50, -height() / 2 + 50, 0);
//    glVertex3i((GLint)zAxis.x() - width() / 2 + 50, (GLint)zAxis.y() - height() / 2 + 50, 0);
//    glEnd();

    // Set a matrix to the shader that does not rotate or scale, just transform to screen coordinate system
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_projection);
    glDisable(GL_DEPTH_TEST);

    if (this->cursorShown)
    {
        // Cursor lines
        glLineWidth((GLfloat)_cursorWidth);
        setPaintingColor(Qt::white);
        glBegin(GL_LINES);
        glVertex3i(mousePos.x() - _cursorSize, mousePos.y(), 0);
        glVertex3i(mousePos.x() + _cursorSize, mousePos.y(), 0);
        glVertex3i(mousePos.x(), mousePos.y() - _cursorSize, 0);
        glVertex3i(mousePos.x(), mousePos.y() + _cursorSize, 0);
        glEnd();

        // Cursor Pickbox
        glLineWidth(_cursorPickboxLineWidth);
        setPaintingColor(_cursorPickboxColor);
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
                setPaintingColor(_pickboxFillColorLeft);
            else
                setPaintingColor(_pickboxFillColorRight);

            glLineWidth(_pickboxOutlineWidth);

            QRect rect = this->selection();
            glBegin(GL_QUADS);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();

            if (this->pickStartPos.x() < this->mousePos.x())
                setPaintingColor(_pickboxOutlineColorLeft);
            else
                setPaintingColor(_pickboxOutlineColorRight);
            glBegin(GL_LINE_LOOP);
            glVertex3i(rect.bottomLeft().x(), rect.bottomLeft().y(), 0);
            glVertex3i(rect.bottomRight().x(), rect.bottomRight().y(), 0);
            glVertex3i(rect.topRight().x(), rect.topRight().y(), 0);
            glVertex3i(rect.topLeft().x(), rect.topLeft().y(), 0);
            glEnd();
        }
    }


    //draw Arcball
    if(arcballShown)
    {
        QPointF lookAtScreenCoords = mapFromScene(lookAtPosition);
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < 60; i++ )
        {
            glVertex3f(arcballRadius * qSin(i * PI / 30.0) + lookAtScreenCoords.x(), arcballRadius * qCos(i * PI / 30.0) + lookAtScreenCoords.y(), 0.0);
        }
        glEnd();
    }

    QString infoText;
    QRect focusRect = QRect(0, 0, _snapIndicatorSize, _snapIndicatorSize);

    if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Move)
    {
        QString itemDescription = "[" + this->itemGripModifier->getItem()->description + "]";
        QVector3D pos = this->itemGripModifier->getItem()->position;
        QString itemPosition_from = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", pos.x(), pos.y(), pos.z());
        infoText = "Move " + itemDescription + itemPosition_from;
        if (snapMode != SnapNo)
            infoText += " to\n";
        focusRect.moveCenter(this->mousePos);
    }
    if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Copy)
    {
        QString itemDescription = "[" + this->itemGripModifier->getItem()->description + "]";
        QVector3D pos = this->itemGripModifier->getItem()->position;
        QString itemPosition_from = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", pos.x(), pos.y(), pos.z());
        infoText = "Copy " + itemDescription + itemPosition_from;
        if (snapMode != SnapNo)
            infoText += " to\n";
        focusRect.moveCenter(this->mousePos);
    }
    if ((snapMode != SnapNo) && (item_lastHighlight != NULL))
    {
        focusRect.moveCenter(this->snapPos_screen);
        glLineWidth(1);

        switch (snapMode)
        {
        case SnapBasepoint:
        {
            setPaintingColor(Qt::red);
            glBegin(GL_LINE_LOOP);
            glVertex2i(focusRect.bottomLeft().x(), focusRect.bottomLeft().y());
            glVertex2i(focusRect.bottomRight().x(), focusRect.bottomRight().y());
            glVertex2i(focusRect.topLeft().x(), focusRect.topLeft().y());
            glVertex2i(focusRect.topRight().x(), focusRect.topRight().y());
            glEnd();
            infoText.append(tr("Basepoint"));

//            paintTextInfoBox(textAnchorPos, snapText, textAnchorType);

            break;
        }
        case SnapFlange:
        {
            setPaintingColor(Qt::red);
            glBegin(GL_LINES);
            glVertex2i(focusRect.left(), focusRect.top());
            glVertex2i(focusRect.left(), focusRect.bottom());
            glVertex2i(focusRect.left(), (focusRect.center().y() + focusRect.top()) / 2);
            glVertex2i(focusRect.right(), (focusRect.center().y() + focusRect.top()) / 2);
            glVertex2i(focusRect.left(), (focusRect.center().y() + focusRect.bottom()) / 2);
            glVertex2i(focusRect.right(), (focusRect.center().y() + focusRect.bottom()) / 2);
            glEnd();
            infoText.append("Flange");

//            paintTextInfoBox(textAnchorPos, snapText, textAnchorType);
            break;
        }
        case SnapEndpoint:
        {
            setPaintingColor(Qt::red);
            glBegin(GL_LINE_LOOP);
            glVertex2i(focusRect.bottomLeft().x(), focusRect.bottomLeft().y());
            glVertex2i(focusRect.bottomRight().x(), focusRect.bottomRight().y());
            glVertex2i(focusRect.topRight().x(), focusRect.topRight().y());
            glVertex2i(focusRect.topLeft().x(), focusRect.topLeft().y());
            glEnd();
            infoText.append("Endpoint/Vertex");

//            paintTextInfoBox(textAnchorPos, snapText, textAnchorType);

            break;
        }
        case SnapCenter:
        {
            setPaintingColor(Qt::red);
            glBegin(GL_LINES);
            glVertex2i(focusRect.left(), focusRect.top());
            glVertex2i(focusRect.right(), focusRect.bottom());
            glVertex2i(this->snapPos_screen.x() - 5, this->snapPos_screen.y() + 5);
            glVertex2i(this->snapPos_screen.x() + 5, this->snapPos_screen.y() - 5);
            glEnd();
            infoText.append("Center");

//            paintTextInfoBox(textAnchorPos, snapText, textAnchorType);

            break;
        }
        case SnapNo:
        {
            break;
        }
        }

        QString itemDescription = "[" + item_lastHighlight->description + "]";
        QString itemPosition = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", this->snapPos_scene.x(), this->snapPos_scene.y(), this->snapPos_scene.z());
        infoText += " of " + itemDescription + itemPosition;
    }

    if (!infoText.isEmpty())
    {
        BoxVertex textAnchorType;
        QPoint textAnchorPos;
        if      (this->snapPos_screen.x() > 0.0 && this->snapPos_screen.y() > 0.0)
        {
            textAnchorPos = focusRect.topLeft();
            textAnchorType = topRight;
        }
        else if (this->snapPos_screen.x() > 0.0 && this->snapPos_screen.y() < 0.0)
        {
            textAnchorPos = focusRect.bottomLeft();
            textAnchorType = bottomRight;
        }
        else if (this->snapPos_screen.x() < 0.0 && this->snapPos_screen.y() > 0.0)
        {
            textAnchorPos = focusRect.topRight();
            textAnchorType = topLeft;
        }
        else if (this->snapPos_screen.x() < 0.0 && this->snapPos_screen.y() < 0.0)
        {
            textAnchorPos = focusRect.bottomRight();
            textAnchorType = bottomLeft;
        }

        paintTextInfoBox(textAnchorPos, infoText, textAnchorType);
    }

    glFlush();

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

void GLWidget::setVertex(QVector3D pos)
{
    shaderProgram->setAttributeValue(shader_vertexLocation, pos);
}

void GLWidget::setVertex(QPoint pos)
{
    //    setVertex(QVector3D((qreal)pos.x(), (qreal)pos.y(), 0.0));
    glVertex2i((GLint)pos.x(), (GLint)pos.y());
}

void GLWidget::setPaintingColor(QColor color)
{
    vertex_color = QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    shaderProgram->setAttributeValue(shader_colorLocation, vertex_color);
}

void GLWidget::setTextureCoords(QPoint coord)
{
    shaderProgram->setAttributeValue(shader_textureCoordLocation, QVector4D((qreal)coord.x(), (qreal)coord.y(), 0.0, 0.0));
}

void GLWidget::setTextureCoords(qreal x, qreal y, qreal z)
{
    shaderProgram->setAttributeValue(shader_textureCoordLocation, x, y, z, 0.0);
}

void GLWidget::setUseTexture(bool on)
{
    GLint use;
    if (on)
        use = 1;
    else use = 0;
    shaderProgram->setUniformValue(shader_useTextureLocation, use);
}

void GLWidget::paintTextInfoBox(QPoint pos, QString text, BoxVertex anchor, QFont font, QColor colorText, QColor colorBackground, QColor colorOutline)
{
    // Calculate text box size
    QFontMetrics fm(font);
    int textHeight = 0;
    int textWidth = 0;
    foreach (QString line, text.split('\n'))
    {
        QRect lineRect = fm.boundingRect(line);
        textHeight += lineRect.height();
        if (lineRect.width() > textWidth)
            textWidth = lineRect.width();
    }
    QRect boundingRect;
    boundingRect.setWidth(textWidth);
    boundingRect.setHeight(textHeight);
    boundingRect.adjust(-5, -5, 5, 5);
    boundingRect.moveTopLeft(QPoint(0, 0));

    // Text as texture
    QImage textImage(boundingRect.width(), boundingRect.height(), QImage::Format_ARGB32);
    textImage.fill(colorBackground);
    QPainter painter(&textImage);
    painter.setPen(colorText);
    painter.setFont(font);
    painter.drawText(boundingRect, Qt::AlignCenter, text);
    painter.end();
    switch(anchor)
    {
    case topLeft:
        boundingRect.moveBottomLeft(pos);
        break;
    case topRight:
        boundingRect.moveBottomRight(pos);
        break;
    case bottomLeft:
        boundingRect.moveTopLeft(pos);
        break;
    case bottomRight:
        boundingRect.moveTopRight(pos);
        break;
    }

    GLuint textureID = this->bindTexture(textImage, GL_TEXTURE_2D, GL_RGBA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //    qDebug() << textureID;

    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, textureID);
    //    glActiveTexture(0);
    setUseTexture(true);


    // Draw background
    setPaintingColor(Qt::transparent);
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 1.0, 0.0);
    setVertex(boundingRect.bottomLeft());
    setTextureCoords(1.0, 1.0, 0.0);
    setVertex(boundingRect.bottomRight());
    setTextureCoords(1.0, 0.001, 0.0);
    setVertex(boundingRect.topRight());
    setTextureCoords(0.0, 0.001, 0.0);
    setVertex(boundingRect.topLeft());
    glEnd();
    setUseTexture(false);
    this->deleteTexture(textureID);


    //    // Draw outline
    setPaintingColor(colorOutline);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(boundingRect.bottomLeft().x(), boundingRect.bottomLeft().y());
    glVertex2i(boundingRect.bottomRight().x(), boundingRect.bottomRight().y());
    glVertex2i(boundingRect.topRight().x(), boundingRect.topRight().y());
    glVertex2i(boundingRect.topLeft().x(), boundingRect.topLeft().y());
    glEnd();
}

void GLWidget::paintContent(QList<Layer*> layers)
{
    //    qDebug() << "GLWidget::paintContent: painting"<< layers.count() << "layers...";
    foreach (Layer* layer, layers)
    {
        if ((itemDB->layerSoloActive) && (!layer->solo))
            continue;

        if ((!layer->on) && (!layer->solo))
            continue;

        paintItems(layer->items, layer);
        paintContent(layer->subLayers);
    }
}

void GLWidget::paintItems(QList<CADitem*> items, Layer* layer, bool checkBoundingBox, bool isSubItem)
{
    foreach (CADitem* item, items)
    {
        if(checkBoundingBox)
        {
            // Global culling performance test
            //                    // Exclude all items from painting that do not reach the canvas with their boundingRect
            //                    int screen_x_min = -this->width() / 2;
            //                    //            int screen_x_max =  this->width() / 2;
            //                    int screen_y_min = -this->height() / 2;
            //                    //            int screen_y_max =  this->height() / 2;

            //                    int p_x_min =  100000;
            //                    int p_x_max = -100000;
            //                    int p_y_min =  100000;
            //                    int p_y_max = -100000;


            //                    for (int i=0; i < 8; i++)
            //                    {
            //                        QVector3D boxPoint = item->boundingBox.p(i);
            //                        QPointF screen_p = mapFromScene(boxPoint);    // Remark: INEFFICIENT!

            //                        if (screen_p.x() < p_x_min)     p_x_min = screen_p.x();
            //                        if (screen_p.x() > p_x_max)     p_x_max = screen_p.x();
            //                        if (screen_p.y() < p_y_min)     p_y_min = screen_p.y();
            //                        if (screen_p.y() > p_y_max)     p_y_max = screen_p.y();
            //                    }

            //                    QRect screenRect;
            //                    QRect itemRect;

            //                    screenRect = QRect(screen_x_min, screen_y_min, this->width(), this->height());
            //                    itemRect = QRect(p_x_min, p_y_min, (p_x_max - p_x_min), (p_y_max - p_y_min));


            //                    if (!screenRect.intersects(itemRect))
            //                    {
            //                        if(!isSubItem)
            //                            glName++;
            //                        continue;
            //                    }
        }



        if(!isSubItem)
            glName++;
        glLoadName(glName);
        item->index = glName;

        // Paint it
        switch (item->getType())
        {
        case CADitem::None:
            break;

        case CADitem::Basic_Arc:
            if (this->render_outline)
                paintBasicArc( layer, (CAD_basic_arc*)item);
            break;
        case CADitem::Basic_Box:
            paintBasicBox(layer, (CAD_basic_box*)item);
            break;
        case CADitem::Basic_Circle:
            if (this->render_outline)
                paintBasicCircle(layer, (CAD_basic_circle*)item);
            break;
        case CADitem::Basic_Cylinder:
            paintBasicCylinder(layer, (CAD_basic_cylinder*)item);
            break;
        case CADitem::Basic_Duct:
            paintBasicDuct(layer, (CAD_basic_duct*)item);
            break;
        case CADitem::Basic_Face:
            paintBasicFace(layer, (CAD_basic_3Dface*)item);
            break;
        case CADitem::Basic_Line:
            if (this->render_outline)
                paintBasicLine(layer, (CAD_basic_line*)item);
            break;
        case CADitem::Basic_Pipe:
            paintBasicPipe(layer, (CAD_basic_pipe*)item);
            break;
        case CADitem::Basic_Plane:
            paintBasicPlane(layer, (CAD_basic_plane*)item);
            break;
        case CADitem::Basic_Polyline:
            if (this->render_outline)
                paintBasicPolyLine(layer, (CAD_basic_polyline*)item);
            break;
        case CADitem::Basic_Point:
            break;
        case CADitem::Basic_Sphere:
            paintBasicSphere(layer, (CAD_basic_sphere*)item);
            break;
        case CADitem::Basic_Turn:
            paintBasicTurn(layer, (CAD_basic_turn*)item);
            break;

        case CADitem::Arch_Beam:
            break;
        case CADitem::Arch_BlockOut:
            paintArchBlockOut(layer, (CAD_arch_blockOut*)item);
            break;
        case CADitem::Arch_BoredPile:
            paintArchBoredPile(layer, (CAD_arch_boredPile*)item);
            break;
        case CADitem::Arch_Door:
            paintArchDoor(layer, (CAD_arch_door*)item);
            break;
        case CADitem::Arch_Foundation:
            break;
        case CADitem::Arch_Grating:
            break;
        case CADitem::Arch_LevelSlab:
            paintArchLevelSlab(layer, (CAD_arch_levelSlab*)item);
            break;
        case CADitem::Arch_Support:
            break;
        case CADitem::Arch_Wall_loadBearing:
            paintArchWallLoadBearing(layer, (CAD_arch_wall_loadBearing*)item);
            break;
        case CADitem::Arch_Wall_nonLoadBearing:
            paintArchWallNonLoadBearing(layer, (CAD_arch_wall_nonLoadBearing*)item);
            break;
        case CADitem::Arch_Window:
            paintArchWindow(layer, (CAD_arch_window*)item);
            break;

        case CADitem::Air_CanvasFlange:
            paintAirCanvasFlange(layer, (CAD_air_canvasFlange*)item);
            break;
        case CADitem::Air_Duct:
            paintAirDuct(layer, (CAD_air_duct*)item);
            break;
        case CADitem::Air_DuctBaffleSilencer:
            paintAirDuctBaffleSilencer(layer, (CAD_air_ductBaffleSilencer*)item);
            break;
        case CADitem::Air_DuctEndPlate:
            paintAirDuctEndPlate(layer, (CAD_air_ductEndPlate*)item);
            break;
        case CADitem::Air_DuctFireDamper:
            paintAirDuctFireDamper(layer, (CAD_air_ductFireDamper*)item);
            break;
        case CADitem::Air_DuctFireResistant:
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
        case CADitem::Air_DuctTurn:
            paintAirDuctTurn(layer, (CAD_air_ductTurn*)item);
            break;
        case CADitem::Air_DuctVolumetricFlowController:
            paintAirDuctVolumetricFlowController(layer, (CAD_air_ductVolumetricFlowController*)item);
            break;
        case CADitem::Air_DuctYpiece:
            paintAirDuctYpiece(layer, (CAD_air_ductYpiece*)item);
            break;
        case CADitem::Air_EmptyCabinet:
            paintAirEmptyCabinet(layer, (CAD_air_emptyCabinet*)item);
            break;
        case CADitem::Air_EquipmentFrame:
            paintAirEquipmentFrame(layer, (CAD_air_equipmentFrame*)item);
            break;
        case CADitem::Air_Fan:
            paintAirFan(layer, (CAD_air_fan*)item);
            break;
        case CADitem::Air_Filter:
            paintAirFilter(layer, (CAD_air_filter*)item);
            break;
        case CADitem::Air_HeatExchangerAirAir:
            paintAirHeatExchangerAirAir(layer, (CAD_air_heatExchangerAirAir*)item);
            break;
        case CADitem::Air_HeatExchangerWaterAir:
            paintAirHeatExchangerWaterAir(layer, (CAD_air_heatExchangerWaterAir*)item);
            break;
        case CADitem::Air_Humidifier:
            paintAirHumidifier(layer, (CAD_air_humidifier*)item);
            break;
        case CADitem::Air_MultiLeafDamper:
            paintAirMultiLeafDamper(layer, (CAD_air_multiLeafDamper*)item);
            break;
        case CADitem::Air_Pipe:
            paintAirPipe(layer, (CAD_air_pipe*)item);
            break;
        case CADitem::Air_PipeEndCap:
            paintAirPipeEndCap(layer, (CAD_air_pipeEndCap*)item);
            break;
        case CADitem::Air_PipeFireDamper:
            paintAirPipeFireDamper(layer, (CAD_air_pipeFireDamper*)item);
            break;
        case CADitem::Air_PipeReducer:
            paintAirPipeReducer(layer, (CAD_air_pipeReducer*)item);
            break;
        case CADitem::Air_PipeSilencer:
            paintAirPipeSilencer(layer, (CAD_air_pipeSilencer*)item);
            break;
        case CADitem::Air_PipeTeeConnector:
            paintAirPipeTeeConnector(layer, (CAD_air_pipeTeeConnector*)item);
            break;
       case CADitem::Air_PipeTurn:
            paintAirPipeTurn(layer, (CAD_air_pipeTurn*)item);
            break;
        case CADitem::Air_PipeVolumetricFlowController:
            paintAirPipeVolumetricFlowController(layer, (CAD_air_pipeVolumetricFlowController*)item);
            break;
        case CADitem::Air_PressureReliefDamper:
            paintAirPressureReliefDamper(layer, (CAD_air_pressureReliefDamper*)item);
            break;
        case CADitem::Air_ThrottleValve:
            paintAirThrottleValve(layer, (CAD_air_throttleValve*)item);
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

        case CADitem::Sanitary_Pipe:
            paintSanitaryPipe(layer, (CAD_sanitary_pipe*)item);
            break;
        case CADitem::Sanitary_PipeTurn:
            paintSanitaryPipeTurn(layer, (CAD_sanitary_pipeTurn*)item);
            break;
        case CADitem::Sanitary_PipeReducer:
            paintSanitaryPipeReducer(layer, (CAD_sanitary_pipeReducer*)item);
            break;
        case CADitem::Sanitary_PipeTeeConnector:
            paintSanitaryPipeTeeConnector(layer, (CAD_sanitary_pipeTeeConnector*)item);
            break;
        case CADitem::Sanitary_PipeEndCap:
            paintSanitaryPipeEndCap(layer, (CAD_sanitary_pipeEndCap*)item);
            break;
        case CADitem::Sanitary_Flange:
            paintSanitaryFlange(layer, (CAD_sanitary_flange*)item);
            break;
        case CADitem::Sanitary_ElectricWaterHeater:
            paintSanitaryElectricWaterHeater(layer, (CAD_sanitary_electricWaterHeater*)item);
            break;
        case CADitem::Sanitary_WashBasin:
            paintSanitaryWashBasin(layer, (CAD_sanitary_washBasin*)item);
            break;
        case CADitem::Sanitary_Sink:
            paintSanitarySink(layer, (CAD_sanitary_sink*)item);
            break;
        case CADitem::Sanitary_Shower:
            paintSanitaryShower(layer, (CAD_sanitary_shower*)item);
            break;
        case CADitem::Sanitary_EmergencyShower:
            paintSanitaryEmergencyShower(layer, (CAD_sanitary_emergencyShower*)item);
            break;
        case CADitem::Sanitary_EmergencyEyeShower:
            paintSanitaryEmergencyEyeShower(layer, (CAD_sanitary_emergencyEyeShower*)item);
            break;
        case CADitem::Sanitary_LiftingUnit:
            paintSanitaryLiftingUnit(layer, (CAD_sanitary_liftingUnit*)item);
            break;
        }

        paintItems(item->subItems, layer, false, true);
    }
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
        if (color_pen.value() > 127)
        {
            color_pen.setHsv(color_pen.hsvHue(), color_pen.hsvSaturation(), color_pen.value() - 100);
        }
        else
        {
            color_pen.setHsv(color_pen.hsvHue(), color_pen.hsvSaturation(), color_pen.value() + 100);
        }
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
        if (color_brush.value() > 127)
        {
            color_brush.setHsv(color_brush.hsvHue(), color_brush.hsvSaturation(), color_brush.value() - 100);
        }
        else
        {
            color_brush.setHsv(color_brush.hsvHue(), color_brush.hsvSaturation(), color_brush.value() + 100);
        }
    }

    return color_brush;
}

void GLWidget::paintBasicLine(Layer* layer, CAD_basic_line *item)
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
    //    glColor4f(color_pen.redF(), color_pen.greenF(), color_pen.blueF(), color_pen.alphaF());
    setPaintingColor(color_pen);
    glBegin(GL_LINES);
    glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
    glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());

    glEnd();
}

void GLWidget::paintBasicPolyLine(Layer *layer, CAD_basic_polyline *item)
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
            setPaintingColor(color_pen);

            glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
            glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());



            p1 = p2;
        }
    }
    glEnd();
}

void GLWidget::paintBasicPlane(Layer *layer, CAD_basic_plane *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {
        setPaintingColor(color_brush);
        glBegin(GL_QUADS);
        for(int k = 0; k < 4; k++)
            glVertex3f((GLfloat)item->vertices[k].x(), (GLfloat)item->vertices[k].y(), (GLfloat)item->vertices[k].z());
        glEnd();
    }
    if (this->render_outline)
    {
        setPaintingColor(color_pen);
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
            glVertex3f((GLfloat)item->vertices[k].x(), (GLfloat)item->vertices[k].y(), (GLfloat)item->vertices[k].z());
        glEnd();
    }
}

void GLWidget::paintBasicFace(Layer *layer, CAD_basic_3Dface *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {
        glBegin(GL_POLYGON);
        setPaintingColor(color_brush);
        foreach (CAD_basic_3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }

    if (this->render_outline)
    {
        glBegin(GL_LINE_LOOP);
        setPaintingColor(color_pen);
        foreach (CAD_basic_3Dface::Vertex vertex, item->vertices)
        {
            glVertex3f((GLfloat)vertex.pos.x(), (GLfloat)vertex.pos.y(), (GLfloat)vertex.pos.z());
        }
        glEnd();
    }
}

void GLWidget::paintBasicArc(Layer *layer, CAD_basic_arc *item)
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

    setPaintingColor(color_pen);
    glLineWidth(penWidth);
    glBegin(GL_LINE_STRIP);
    for (qreal i=0.0; i < 1.0; i += 0.01)    // 100 edges
    {
        qreal angle = item->centralAngle/180.0f*PI * i;
        QVector3D linePos;
        linePos = item->center;

        linePos += QVector3D(sin(angle) * item->radius, cos(angle) * item->radius, 0.0);

        glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
    }

    glEnd();
    glEnd();
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

    setPaintingColor(color_pen);
    glLineWidth(penWidth);
    glBegin(GL_LINE_LOOP);
    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
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
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {
        glBegin(GL_QUADS);
        setPaintingColor(color_brush);

        // Bottom face
        glNormal3f((GLfloat)item->normal_bot.x(), (GLfloat)item->normal_bot.y(), (GLfloat)item->normal_bot.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());

        // Top face
        glNormal3f((GLfloat)item->normal_top.x(), (GLfloat)item->normal_top.y(), (GLfloat)item->normal_top.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());

        // Front face
        glNormal3f((GLfloat)item->normal_front.x(), (GLfloat)item->normal_front.y(), (GLfloat)item->normal_front.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());

        // Left face
        glNormal3f((GLfloat)item->normal_left.x(), (GLfloat)item->normal_left.y(), (GLfloat)item->normal_left.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());

        // Right face
        glNormal3f((GLfloat)item->normal_right.x(), (GLfloat)item->normal_right.y(), (GLfloat)item->normal_right.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());

        // Back face
        glNormal3f((GLfloat)item->normal_back.x(), (GLfloat)item->normal_back.y(), (GLfloat)item->normal_back.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());

        glEnd();
    }

    if (this->render_outline)
    {
        setPaintingColor(color_pen);
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
        setPaintingColor(color_brush);
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
        setPaintingColor(color_pen);
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

void GLWidget::paintBasicPipe(Layer *layer, CAD_basic_pipe *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {


        // Vertical connections (faces)
        setPaintingColor(color_brush);
        glBegin(GL_QUADS);
        // Outer cylinder
        QVector3D last_vertex_bottom = item->vertices_outer_bottom.at(item->vertices_outer_bottom.count() - 1);
        QVector3D last_vertex_top = item->vertices_outer_top.at(item->vertices_outer_bottom.count() - 1);
        for (int i = 0; i < item->vertices_outer_bottom.count(); i++)
        {
            QVector3D vertex_bottom = item->vertices_outer_bottom.at(i);
            QVector3D vertex_top = item->vertices_outer_top.at(i);
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
            glVertex3f((GLfloat)last_vertex_top.x(), (GLfloat)last_vertex_top.y(), (GLfloat)last_vertex_top.z());
            glVertex3f((GLfloat)vertex_top.x(), (GLfloat)vertex_top.y(), (GLfloat)vertex_top.z());
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            last_vertex_bottom = vertex_bottom;
            last_vertex_top = vertex_top;
        }
        // Inner cylinder
        last_vertex_bottom = item->vertices_inner_bottom.at(item->vertices_inner_bottom.count() - 1);
        last_vertex_top = item->vertices_inner_top.at(item->vertices_inner_top.count() - 1);
        for (int i = 0; i < item->vertices_inner_bottom.count(); i++)
        {
            QVector3D vertex_bottom = item->vertices_inner_bottom.at(i);
            QVector3D vertex_top = item->vertices_inner_top.at(i);
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
            glVertex3f((GLfloat)last_vertex_top.x(), (GLfloat)last_vertex_top.y(), (GLfloat)last_vertex_top.z());
            glVertex3f((GLfloat)vertex_top.x(), (GLfloat)vertex_top.y(), (GLfloat)vertex_top.z());
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            last_vertex_bottom = vertex_bottom;
            last_vertex_top = vertex_top;
        }
        // End disks
        // Pipe length iteration
        for (int l = 0; l <= 1; l++)
        {

            QVector3D last_vertex_outer = item->vertices_outer_bottom.at(item->vertices_outer_bottom.count() - 1);
            QVector3D last_vertex_inner = item->vertices_inner_bottom.at(item->vertices_inner_bottom.count() - 1);
            if (l == 1)
            {
                last_vertex_outer = item->vertices_outer_top.at(item->vertices_outer_top.count() - 1);
                last_vertex_inner = item->vertices_inner_top.at(item->vertices_inner_top.count() - 1);
            }

            for (int i = 0; i < item->vertices_outer_bottom.count(); i++)
            {

                QVector3D vertex_outer = item->vertices_outer_bottom.at(i);
                QVector3D vertex_inner = item->vertices_inner_bottom.at(i);
                if (l == 0)
                {
                    glVertex3f((GLfloat)last_vertex_inner.x(), (GLfloat)last_vertex_inner.y(), (GLfloat)last_vertex_inner.z());
                    glVertex3f((GLfloat)last_vertex_outer.x(), (GLfloat)last_vertex_outer.y(), (GLfloat)last_vertex_outer.z());
                    glVertex3f((GLfloat)vertex_outer.x(), (GLfloat)vertex_outer.y(), (GLfloat)vertex_outer.z());
                    glVertex3f((GLfloat)vertex_inner.x(), (GLfloat)vertex_inner.y(), (GLfloat)vertex_inner.z());
                }
                else
                {
                    vertex_outer = item->vertices_outer_top.at(i);
                    vertex_inner = item->vertices_inner_top.at(i);
                    glVertex3f((GLfloat)last_vertex_inner.x(), (GLfloat)last_vertex_inner.y(), (GLfloat)last_vertex_inner.z());
                    glVertex3f((GLfloat)last_vertex_outer.x(), (GLfloat)last_vertex_outer.y(), (GLfloat)last_vertex_outer.z());
                    glVertex3f((GLfloat)vertex_outer.x(), (GLfloat)vertex_outer.y(), (GLfloat)vertex_outer.z());
                    glVertex3f((GLfloat)vertex_inner.x(), (GLfloat)vertex_inner.y(), (GLfloat)vertex_inner.z());
                }

                last_vertex_outer = vertex_outer;
                last_vertex_inner = vertex_inner;
            }
        }
        glEnd();
    }

    if (this->render_outline)
    {
        setPaintingColor(color_pen);
        glLineWidth(1.0);


        // Vertical connections (lines)
        glBegin(GL_LINES);
        // Outer cylinder
        QVector3D last_vertex_bottom = item->vertices_outer_bottom.at(item->vertices_outer_bottom.count() - 1);
        for (int i = 0; i < item->vertices_outer_bottom.count(); i++)
        {
            QVector3D vertex_bottom = item->vertices_outer_bottom.at(i);
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
            glVertex3f((GLfloat)(last_vertex_bottom.x() + item->direction.x()), (GLfloat)(last_vertex_bottom.y() + item->direction.y()), (GLfloat)(last_vertex_bottom.z()  + item->direction.z()));
            glVertex3f((GLfloat)(vertex_bottom.x() + item->direction.x()), (GLfloat)(vertex_bottom.y() + item->direction.y()), (GLfloat)(vertex_bottom.z() + item->direction.z()));
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            last_vertex_bottom = vertex_bottom;
        }
        // Inner cylinder
        last_vertex_bottom = item->vertices_inner_bottom.at(item->vertices_inner_bottom.count() - 1);
        for (int i = 0; i < item->vertices_inner_bottom.count(); i++)
        {
            QVector3D vertex_bottom = item->vertices_inner_bottom.at(i);
            glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
            glVertex3f((GLfloat)(last_vertex_bottom.x() + item->direction.x()), (GLfloat)(last_vertex_bottom.y() + item->direction.y()), (GLfloat)(last_vertex_bottom.z()  + item->direction.z()));
            glVertex3f((GLfloat)(vertex_bottom.x() + item->direction.x()), (GLfloat)(vertex_bottom.y() + item->direction.y()), (GLfloat)(vertex_bottom.z() + item->direction.z()));
            glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
            last_vertex_bottom = vertex_bottom;
        }
        glEnd();
        //Radial connections (lines)
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < item->vertices_inner_bottom.count(); i++)
        {
            glVertex3f((GLfloat)item->vertices_inner_bottom.at(i).x(), (GLfloat)item->vertices_inner_bottom.at(i).y(), (GLfloat)item->vertices_inner_bottom.at(i).z());
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < item->vertices_outer_bottom.count(); i++)
        {
            glVertex3f((GLfloat)item->vertices_outer_bottom.at(i).x(), (GLfloat)item->vertices_outer_bottom.at(i).y(), (GLfloat)item->vertices_outer_bottom.at(i).z());
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < item->vertices_inner_top.count(); i++)
        {
            glVertex3f((GLfloat)item->vertices_inner_top.at(i).x(), (GLfloat)item->vertices_inner_top.at(i).y(), (GLfloat)item->vertices_inner_top.at(i).z());
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < item->vertices_outer_top.count(); i++)
        {
            glVertex3f((GLfloat)item->vertices_outer_top.at(i).x(), (GLfloat)item->vertices_outer_top.at(i).y(), (GLfloat)item->vertices_outer_top.at(i).z());
        }
        glEnd();
        // s iteration
//        for (int w = 0; w <= 1; w++)
//        {

////            // Bottom circle
////            glBegin(GL_LINE_LOOP);
////            for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
////            {
////                qreal angle = 2 * PI * i;
////                QVector3D linePos;

////                if (w == 0)
////                    linePos = item->matrix_rotation * QVector3D(0.0, sin(angle) * item->radius, cos(angle) * item->radius);
////                else
////                    linePos = item->matrix_rotation * QVector3D(0.0, sin(angle) * (item->radius - item->wallThickness), cos(angle) * (item->radius - item->wallThickness));
////                linePos += item->position;
////                item->vertices_outer_bottom_lines.append(linePos);

////                glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
////            }
////            glEnd();


//            // Top circle
//            glBegin(GL_LINE_LOOP);
//            for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
//            {
//                qreal angle = 2 * PI * i;
//                QVector3D linePos;
//                if (w == 0)
//                    linePos = item->matrix_rotation * QVector3D(0.0, sin(angle) * item->radius, cos(angle) * item->radius);
//                else
//                    linePos = item->matrix_rotation * QVector3D(0.0, sin(angle) * (item->radius - item->wallThickness), cos(angle) * (item->radius - item->wallThickness));
//                linePos += item->position + item->direction;

//                glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//            }
//            glEnd();

            // Vertical connections

//            glBegin(GL_LINES);
//            foreach (QVector3D vertex_bottom, item->vertices_outer_bottom_lines)
//            {
//                glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//                glVertex3f((GLfloat)(vertex_bottom.x() + item->direction.x()), (GLfloat)(vertex_bottom.y() + item->direction.y()), (GLfloat)(vertex_bottom.z() + item->direction.z()));
//            }
//            glEnd();
//        }

        // Center line
        glLineWidth(3.0);
        glLineStipple(2, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINES);
        glVertex3f((GLfloat)item->position.x(), (GLfloat)item->position.y(), (GLfloat)item->position.z());
        glVertex3f((GLfloat)(item->position.x() + item->direction.x()), (GLfloat)(item->position.y() + item->direction.y()), (GLfloat)(item->position.z() + item->direction.z()));
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
}

void GLWidget::paintBasicTurn(Layer *layer, CAD_basic_turn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    int a;
    int b;

    int count_a = 11;
    int count_b = 21;

    // level of detail
    int lod = 1;

    // s iteration
    for (int w = 0; w <= 1; w++)
    {
        if (this->render_solid)
        {
            setPaintingColor(color_brush);

            // Outer and inner surfaces
            for (a=lod; a < count_a; a += lod)
            {
                if (a >= count_a) a = (count_a - 1);
                glBegin(GL_QUADS);
                for (b=lod; b < count_b; b += lod)
                {
                    if (b >= count_b) b = (count_b - 1);

                    QVector3D vertex_1 = item->vertices[w][a][b - lod];
                    QVector3D vertex_2 = item->vertices[w][a - lod][b - lod];
                    QVector3D vertex_3 = item->vertices[w][a - lod][b];
                    QVector3D vertex_4 = item->vertices[w][a][b];

                    glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                    glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                    glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                    glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
                }
                glEnd();
            }
        }

        if (this->render_outline)
        {
            setPaintingColor(color_pen);
            glLineWidth(1.0);

            // Rings
            for (a=0; a < count_a; a += lod)
            {
                if (a >= count_a) a = (count_a - 1);

                glBegin(GL_LINE_STRIP);
                for (b=0; b < count_b; b += lod)
                {
                    if (b >= count_b) b = (count_b - 1);
                    QVector3D linePos = item->vertices[w][a][b];
                    glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
                }
                glEnd();
            }

            // Lines
            for (b=0; b < (count_b - 1); b += lod)
            {
                if (b >= count_b) b = (count_b - 1);
                glBegin(GL_LINE_STRIP);
                for (a=0; a < count_a; a += lod)
                {
                    if (a >= count_a) a = (count_a - 1);

                    QVector3D linePos = item->vertices[w][a][b];
                    glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
                }
                glEnd();
            }
        }
    }

    // Front and rear faces (discs)
    if (this->render_solid)
    {
        setPaintingColor(color_brush);

        for (a=0; a < count_a; a+=(count_a - 1))
        {
            glBegin(GL_QUADS);
            for (b=lod; b < count_b; b += lod)
            {
                if (b >= count_b) b = (count_b - 1);
                QVector3D vertex_1 = item->vertices[0][a][b - lod];
                QVector3D vertex_2 = item->vertices[1][a][b - lod];
                QVector3D vertex_3 = item->vertices[1][a][b];
                QVector3D vertex_4 = item->vertices[0][a][b];

                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
            }
            glEnd();
        }
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
        setPaintingColor(color_brush);
        gluSphere(sphere, radius, 32, 32);
    }
    if (this->render_outline)
    {
        setPaintingColor(color_pen);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gluSphere(sphere, radius * 1.001, 32, 32);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glPopMatrix();
    gluDeleteQuadric(sphere);
}

void GLWidget::paintBasicDuct(Layer *layer, CAD_basic_duct *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {
        glBegin(GL_QUADS);
        setPaintingColor(color_brush);

        // Bottom face
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());

        // Top face
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());

        // Side Faces
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());

        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());

        // Inner Bottom Face
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());
        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());

        // Inner Top Face
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());

        // Inner Side Faces
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());

        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());

        // Front faces
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());

        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());

        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());

        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());

        // Back faces
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());

        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());

        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());

        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());




        glEnd();
    }

    if (this->render_outline)
    {
        setPaintingColor(color_pen);
        glLineWidth(1.5);

        glBegin(GL_LINE_LOOP);

        // Bottom face
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        // Top face
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        // Side Faces
        glVertex3f((GLfloat)item->pos_bot_1.x(), (GLfloat)item->pos_bot_1.y(), (GLfloat)item->pos_bot_1.z());
        glVertex3f((GLfloat)item->pos_bot_2.x(), (GLfloat)item->pos_bot_2.y(), (GLfloat)item->pos_bot_2.z());
        glVertex3f((GLfloat)item->pos_top_2.x(), (GLfloat)item->pos_top_2.y(), (GLfloat)item->pos_top_2.z());
        glVertex3f((GLfloat)item->pos_top_1.x(), (GLfloat)item->pos_top_1.y(), (GLfloat)item->pos_top_1.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->pos_bot_3.x(), (GLfloat)item->pos_bot_3.y(), (GLfloat)item->pos_bot_3.z());
        glVertex3f((GLfloat)item->pos_bot_4.x(), (GLfloat)item->pos_bot_4.y(), (GLfloat)item->pos_bot_4.z());
        glVertex3f((GLfloat)item->pos_top_4.x(), (GLfloat)item->pos_top_4.y(), (GLfloat)item->pos_top_4.z());
        glVertex3f((GLfloat)item->pos_top_3.x(), (GLfloat)item->pos_top_3.y(), (GLfloat)item->pos_top_3.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        // Inner Bottom Face
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());
        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        // Inner Top Face
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        // Inner Side Faces
        glVertex3f((GLfloat)item->inner_pos_bot_1.x(), (GLfloat)item->inner_pos_bot_1.y(), (GLfloat)item->inner_pos_bot_1.z());
        glVertex3f((GLfloat)item->inner_pos_bot_2.x(), (GLfloat)item->inner_pos_bot_2.y(), (GLfloat)item->inner_pos_bot_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_2.x(), (GLfloat)item->inner_pos_top_2.y(), (GLfloat)item->inner_pos_top_2.z());
        glVertex3f((GLfloat)item->inner_pos_top_1.x(), (GLfloat)item->inner_pos_top_1.y(), (GLfloat)item->inner_pos_top_1.z());
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->inner_pos_bot_3.x(), (GLfloat)item->inner_pos_bot_3.y(), (GLfloat)item->inner_pos_bot_3.z());
        glVertex3f((GLfloat)item->inner_pos_bot_4.x(), (GLfloat)item->inner_pos_bot_4.y(), (GLfloat)item->inner_pos_bot_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_4.x(), (GLfloat)item->inner_pos_top_4.y(), (GLfloat)item->inner_pos_top_4.z());
        glVertex3f((GLfloat)item->inner_pos_top_3.x(), (GLfloat)item->inner_pos_top_3.y(), (GLfloat)item->inner_pos_top_3.z());
        glEnd();

    }
}

void GLWidget::paintArchBlockOut(Layer *layer, CAD_arch_blockOut *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchBoredPile(Layer *layer ,CAD_arch_boredPile *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintArchDoor(Layer *layer, CAD_arch_door *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
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


    int a;
    int b;

    int count_a = 13;
    int count_b = 5;

    // level of detail
    int lod = 1;

    // s iteration
    for (int w = 0; w <= 1; w++)
    {
        if (this->render_solid)
        {
            setPaintingColor(color_brush);

            // Outer and inner surfaces
            for (a=lod; a < count_a; a += lod)
            {
                if (a >= count_a) a = (count_a - 1);
                glBegin(GL_QUADS);
                for (b=lod; b < count_b; b += lod)
                {
                    if (b >= count_b) b = (count_b - 1);

                    QVector3D vertex_1 = item->vertices[w][a][b - lod];
                    QVector3D vertex_2 = item->vertices[w][a - lod][b - lod];
                    QVector3D vertex_3 = item->vertices[w][a - lod][b];
                    QVector3D vertex_4 = item->vertices[w][a][b];

                    glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                    glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                    glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                    glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
                }
                glEnd();
            }
        }

        if (this->render_outline)
        {
            setPaintingColor(color_pen);
            glLineWidth(1.0);

            // Rings
            for (a=0; a < count_a; a += lod)
            {
                if (a >= count_a) a = (count_a - 1);

                glBegin(GL_LINE_STRIP);
                for (b=0; b < count_b; b += lod)
                {
                    if (b >= count_b) b = (count_b - 1);
                    QVector3D linePos = item->vertices[w][a][b];
                    glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
                }
                glEnd();
            }

            // Lines
            for (b=0; b < (count_b - 1); b += lod)
            {
                if (b >= count_b) b = (count_b - 1);
                glBegin(GL_LINE_STRIP);
                for (a=0; a < count_a; a += lod)
                {
                    if (a >= count_a) a = (count_a - 1);

                    QVector3D linePos = item->vertices[w][a][b];
                    glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
                }
                glEnd();
            }
        }
    }

    // Front and rear faces (discs)
    if (this->render_solid)
    {
        setPaintingColor(color_brush);

        for (a=0; a < count_a; a+=(count_a - 1))
        {
            glBegin(GL_QUADS);
            for (b=lod; b < count_b; b += lod)
            {
                if (b >= count_b) b = (count_b - 1);
                QVector3D vertex_1 = item->vertices[0][a][b - lod];
                QVector3D vertex_2 = item->vertices[1][a][b - lod];
                QVector3D vertex_3 = item->vertices[1][a][b];
                QVector3D vertex_4 = item->vertices[0][a][b];

                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
            }
            glEnd();
        }
    }
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

    int a;
    int b;

    int count_a = 12;
    int count_b = 2;

    //draw turns
    // s iteration
    for (int w = 0; w <= 0; w++)
    {
        if (this->render_solid)
        {
            setPaintingColor(color_brush);

            for (a=1; a < count_a; a ++)
            {

                QVector3D vertex_1 = item->vertices_turn1[w][a][0];
                QVector3D vertex_2 = item->vertices_turn1[w][a - 1][0];
                QVector3D vertex_3 = item->vertices_turn1[w][a - 1][1];
                QVector3D vertex_4 = item->vertices_turn1[w][a][1];

                glBegin(GL_QUADS);
                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
                glEnd();
            }
            for (a=1; a < count_a; a ++)
            {

                QVector3D vertex_1 = item->vertices_turn2[w][a][0];
                QVector3D vertex_2 = item->vertices_turn2[w][a - 1][0];
                QVector3D vertex_3 = item->vertices_turn2[w][a - 1][1];
                QVector3D vertex_4 = item->vertices_turn2[w][a][1];

                glBegin(GL_QUADS);
                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
                glEnd();
            }
            //draw backside
            glBegin(GL_QUADS);
            glVertex3f((GLfloat)item->vertices_backside[0].x(), (GLfloat)item->vertices_backside[0].y(), (GLfloat)item->vertices_backside[0].z());
            glVertex3f((GLfloat)item->vertices_backside[1].x(), (GLfloat)item->vertices_backside[1].y(), (GLfloat)item->vertices_backside[1].z());
            glVertex3f((GLfloat)item->vertices_backside[2].x(), (GLfloat)item->vertices_backside[2].y(), (GLfloat)item->vertices_backside[2].z());
            glVertex3f((GLfloat)item->vertices_backside[3].x(), (GLfloat)item->vertices_backside[3].y(), (GLfloat)item->vertices_backside[3].z());
            glEnd();

            //draw lower side
            glBegin(GL_TRIANGLE_FAN);
            QVector3D vertex_1 = (item->vertices_backside[0] + item->vertices_backside[1]) / 2;
            glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
            glVertex3f((GLfloat)item->vertices_backside[0].x(), (GLfloat)item->vertices_backside[0].y(), (GLfloat)item->vertices_backside[0].z());
            for (a=0; a < count_a; a ++)
            {
                QVector3D vertex_a = item->vertices_turn1[w][a][1];
                glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
            }
            for (a=0; a < count_a; a ++)
            {
                QVector3D vertex_a = item->vertices_turn2[w][a][1];
                glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
            }

            glVertex3f((GLfloat)item->vertices_backside[1].x(), (GLfloat)item->vertices_backside[1].y(), (GLfloat)item->vertices_backside[1].z());
            glEnd();

            //draw upper side

            glBegin(GL_TRIANGLE_FAN);
            QVector3D vertex_2 = (item->vertices_backside[2] + item->vertices_backside[3]) / 2;
            glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
            glVertex3f((GLfloat)item->vertices_backside[3].x(), (GLfloat)item->vertices_backside[3].y(), (GLfloat)item->vertices_backside[3].z());
            for (a=0; a < count_a; a ++)
            {
                QVector3D vertex_a = item->vertices_turn1[0][a][0];
                glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
            }
            for (a=0; a < count_a; a ++)
            {
                QVector3D vertex_a = item->vertices_turn2[0][a][0];
                glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
            }

            glVertex3f((GLfloat)item->vertices_backside[2].x(), (GLfloat)item->vertices_backside[2].y(), (GLfloat)item->vertices_backside[2].z());
            glEnd();


        }



        if (this->render_outline)
        {
            setPaintingColor(color_pen);
            glLineWidth(1.0);



            for (a=1; a < count_a; a ++)
            {
                glBegin(GL_LINE_STRIP);


                QVector3D vertex_1 = item->vertices_turn1[w][a][0];
                QVector3D vertex_2 = item->vertices_turn1[w][a - 1][0];
                QVector3D vertex_3 = item->vertices_turn1[w][a - 1][1];
                QVector3D vertex_4 = item->vertices_turn1[w][a][1];

                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
                glEnd();
                glBegin(GL_LINE_STRIP);
                vertex_1 = item->vertices_turn2[w][a][0];
                vertex_2 = item->vertices_turn2[w][a - 1][0];
                vertex_3 = item->vertices_turn2[w][a - 1][1];
                vertex_4 = item->vertices_turn2[w][a][1];

                glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
                glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
                glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
                glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());

                glEnd();
            }
        }
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->vertices_backside[0].x(), (GLfloat)item->vertices_backside[0].y(), (GLfloat)item->vertices_backside[0].z());
        glVertex3f((GLfloat)item->vertices_backside[1].x(), (GLfloat)item->vertices_backside[1].y(), (GLfloat)item->vertices_backside[1].z());
        glVertex3f((GLfloat)item->vertices_backside[2].x(), (GLfloat)item->vertices_backside[2].y(), (GLfloat)item->vertices_backside[2].z());
        glVertex3f((GLfloat)item->vertices_backside[3].x(), (GLfloat)item->vertices_backside[3].y(), (GLfloat)item->vertices_backside[3].z());
        glEnd();
    }
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

    int count_k = 32;
    if (this->render_solid)
    {
        setPaintingColor(color_brush);

        //front face round
        glBegin(GL_QUADS);
        for(int k = 0; k < count_k - 1; k++)
        {
            glVertex3f((GLfloat)item->roundside[0][0][k].x(), (GLfloat)item->roundside[0][0][k].y(), (GLfloat)item->roundside[0][0][k].z());
            glVertex3f((GLfloat)item->roundside[0][1][k].x(), (GLfloat)item->roundside[0][1][k].y(), (GLfloat)item->roundside[0][1][k].z());
            glVertex3f((GLfloat)item->roundside[0][1][k+1].x(), (GLfloat)item->roundside[0][1][k+1].y(), (GLfloat)item->roundside[0][1][k+1].z());
            glVertex3f((GLfloat)item->roundside[0][0][k+1].x(), (GLfloat)item->roundside[0][0][k+1].y(), (GLfloat)item->roundside[0][0][k+1].z());
        }
        glVertex3f((GLfloat)item->roundside[0][0][count_k - 1].x(), (GLfloat)item->roundside[0][0][count_k - 1].y(), (GLfloat)item->roundside[0][0][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[0][1][count_k - 1].x(), (GLfloat)item->roundside[0][1][count_k - 1].y(), (GLfloat)item->roundside[0][1][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[0][1][0].x(), (GLfloat)item->roundside[0][1][0].y(), (GLfloat)item->roundside[0][1][0].z());
        glVertex3f((GLfloat)item->roundside[0][0][0].x(), (GLfloat)item->roundside[0][0][0].y(), (GLfloat)item->roundside[0][0][0].z());

        //outer faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glVertex3f((GLfloat)item->roundside[0][0][k].x(), (GLfloat)item->roundside[0][0][k].y(), (GLfloat)item->roundside[0][0][k].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(), (GLfloat)item->roundside[1][0][k].y(), (GLfloat)item->roundside[1][0][k].z());
            glVertex3f((GLfloat)item->roundside[1][0][k+1].x(), (GLfloat)item->roundside[1][0][k+1].y(), (GLfloat)item->roundside[1][0][k+1].z());
            glVertex3f((GLfloat)item->roundside[0][0][k+1].x(), (GLfloat)item->roundside[0][0][k+1].y(), (GLfloat)item->roundside[0][0][k+1].z());
        }
        glVertex3f((GLfloat)item->roundside[0][0][count_k - 1].x(), (GLfloat)item->roundside[0][0][count_k - 1].y(), (GLfloat)item->roundside[0][0][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][0][count_k - 1].x(), (GLfloat)item->roundside[1][0][count_k - 1].y(), (GLfloat)item->roundside[1][0][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][0][0].x(), (GLfloat)item->roundside[1][0][0].y(), (GLfloat)item->roundside[1][0][0].z());
        glVertex3f((GLfloat)item->roundside[0][0][0].x(), (GLfloat)item->roundside[0][0][0].y(), (GLfloat)item->roundside[0][0][0].z());


        //inner faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glVertex3f((GLfloat)item->roundside[0][1][k].x(), (GLfloat)item->roundside[0][1][k].y(), (GLfloat)item->roundside[0][1][k].z());
            glVertex3f((GLfloat)item->roundside[1][1][k].x(), (GLfloat)item->roundside[1][1][k].y(), (GLfloat)item->roundside[1][1][k].z());
            glVertex3f((GLfloat)item->roundside[1][1][k+1].x(), (GLfloat)item->roundside[1][1][k+1].y(), (GLfloat)item->roundside[1][1][k+1].z());
            glVertex3f((GLfloat)item->roundside[0][1][k+1].x(), (GLfloat)item->roundside[0][1][k+1].y(), (GLfloat)item->roundside[0][1][k+1].z());
        }
        glVertex3f((GLfloat)item->roundside[0][1][count_k - 1].x(), (GLfloat)item->roundside[0][1][count_k - 1].y(), (GLfloat)item->roundside[0][1][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][1][count_k - 1].x(), (GLfloat)item->roundside[1][1][count_k - 1].y(), (GLfloat)item->roundside[1][1][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][1][0].x(), (GLfloat)item->roundside[1][1][0].y(), (GLfloat)item->roundside[1][1][0].z());
        glVertex3f((GLfloat)item->roundside[0][1][0].x(), (GLfloat)item->roundside[0][1][0].y(), (GLfloat)item->roundside[0][1][0].z());


        //inner faces rect
        glBegin(GL_QUADS);
        glVertex3f((GLfloat)item->rectside[0][1][0].x(), (GLfloat)item->rectside[0][1][0].y(), (GLfloat)item->rectside[0][1][0].z());
        glVertex3f((GLfloat)item->rectside[1][1][0].x(), (GLfloat)item->rectside[1][1][0].y(), (GLfloat)item->rectside[1][1][0].z());
        glVertex3f((GLfloat)item->rectside[1][1][1].x(), (GLfloat)item->rectside[1][1][1].y(), (GLfloat)item->rectside[1][1][1].z());
        glVertex3f((GLfloat)item->rectside[0][1][1].x(), (GLfloat)item->rectside[0][1][1].y(), (GLfloat)item->rectside[0][1][1].z());

        glVertex3f((GLfloat)item->rectside[0][1][1].x(), (GLfloat)item->rectside[0][1][1].y(), (GLfloat)item->rectside[0][1][1].z());
        glVertex3f((GLfloat)item->rectside[1][1][1].x(), (GLfloat)item->rectside[1][1][1].y(), (GLfloat)item->rectside[1][1][1].z());
        glVertex3f((GLfloat)item->rectside[1][1][2].x(), (GLfloat)item->rectside[1][1][2].y(), (GLfloat)item->rectside[1][1][2].z());
        glVertex3f((GLfloat)item->rectside[0][1][2].x(), (GLfloat)item->rectside[0][1][2].y(), (GLfloat)item->rectside[0][1][2].z());

        glVertex3f((GLfloat)item->rectside[0][1][2].x(), (GLfloat)item->rectside[0][1][2].y(), (GLfloat)item->rectside[0][1][2].z());
        glVertex3f((GLfloat)item->rectside[1][1][2].x(), (GLfloat)item->rectside[1][1][2].y(), (GLfloat)item->rectside[1][1][2].z());
        glVertex3f((GLfloat)item->rectside[1][1][3].x(), (GLfloat)item->rectside[1][1][3].y(), (GLfloat)item->rectside[1][1][3].z());
        glVertex3f((GLfloat)item->rectside[0][1][3].x(), (GLfloat)item->rectside[0][1][3].y(), (GLfloat)item->rectside[0][1][3].z());

        glVertex3f((GLfloat)item->rectside[0][1][3].x(), (GLfloat)item->rectside[0][1][3].y(), (GLfloat)item->rectside[0][1][3].z());
        glVertex3f((GLfloat)item->rectside[1][1][3].x(), (GLfloat)item->rectside[1][1][3].y(), (GLfloat)item->rectside[1][1][3].z());
        glVertex3f((GLfloat)item->rectside[1][1][0].x(), (GLfloat)item->rectside[1][1][0].y(), (GLfloat)item->rectside[1][1][0].z());
        glVertex3f((GLfloat)item->rectside[0][1][0].x(), (GLfloat)item->rectside[0][1][0].y(), (GLfloat)item->rectside[0][1][0].z());

        //outer faces rect
        glVertex3f((GLfloat)item->rectside[0][0][0].x(), (GLfloat)item->rectside[0][0][0].y(), (GLfloat)item->rectside[0][0][0].z());
        glVertex3f((GLfloat)item->rectside[1][0][0].x(), (GLfloat)item->rectside[1][0][0].y(), (GLfloat)item->rectside[1][0][0].z());
        glVertex3f((GLfloat)item->rectside[1][0][1].x(), (GLfloat)item->rectside[1][0][1].y(), (GLfloat)item->rectside[1][0][1].z());
        glVertex3f((GLfloat)item->rectside[0][0][1].x(), (GLfloat)item->rectside[0][0][1].y(), (GLfloat)item->rectside[0][0][1].z());

        glVertex3f((GLfloat)item->rectside[0][0][1].x(), (GLfloat)item->rectside[0][0][1].y(), (GLfloat)item->rectside[0][0][1].z());
        glVertex3f((GLfloat)item->rectside[1][0][1].x(), (GLfloat)item->rectside[1][0][1].y(), (GLfloat)item->rectside[1][0][1].z());
        glVertex3f((GLfloat)item->rectside[1][0][2].x(), (GLfloat)item->rectside[1][0][2].y(), (GLfloat)item->rectside[1][0][2].z());
        glVertex3f((GLfloat)item->rectside[0][0][2].x(), (GLfloat)item->rectside[0][0][2].y(), (GLfloat)item->rectside[0][0][2].z());

        glVertex3f((GLfloat)item->rectside[0][0][2].x(), (GLfloat)item->rectside[0][0][2].y(), (GLfloat)item->rectside[0][0][2].z());
        glVertex3f((GLfloat)item->rectside[1][0][2].x(), (GLfloat)item->rectside[1][0][2].y(), (GLfloat)item->rectside[1][0][2].z());
        glVertex3f((GLfloat)item->rectside[1][0][3].x(), (GLfloat)item->rectside[1][0][3].y(), (GLfloat)item->rectside[1][0][3].z());
        glVertex3f((GLfloat)item->rectside[0][0][3].x(), (GLfloat)item->rectside[0][0][3].y(), (GLfloat)item->rectside[0][0][3].z());

        glVertex3f((GLfloat)item->rectside[0][0][3].x(), (GLfloat)item->rectside[0][0][3].y(), (GLfloat)item->rectside[0][0][3].z());
        glVertex3f((GLfloat)item->rectside[1][0][3].x(), (GLfloat)item->rectside[1][0][3].y(), (GLfloat)item->rectside[1][0][3].z());
        glVertex3f((GLfloat)item->rectside[1][0][0].x(), (GLfloat)item->rectside[1][0][0].y(), (GLfloat)item->rectside[1][0][0].z());
        glVertex3f((GLfloat)item->rectside[0][0][0].x(), (GLfloat)item->rectside[0][0][0].y(), (GLfloat)item->rectside[0][0][0].z());

        //front faces rect
        glVertex3f((GLfloat)item->rectside[0][0][0].x(), (GLfloat)item->rectside[0][0][0].y(), (GLfloat)item->rectside[0][0][0].z());
        glVertex3f((GLfloat)item->rectside[0][1][0].x(), (GLfloat)item->rectside[0][1][0].y(), (GLfloat)item->rectside[0][1][0].z());
        glVertex3f((GLfloat)item->rectside[0][1][1].x(), (GLfloat)item->rectside[0][1][1].y(), (GLfloat)item->rectside[0][1][1].z());
        glVertex3f((GLfloat)item->rectside[0][0][1].x(), (GLfloat)item->rectside[0][0][1].y(), (GLfloat)item->rectside[0][0][1].z());

        glVertex3f((GLfloat)item->rectside[0][0][1].x(), (GLfloat)item->rectside[0][0][1].y(), (GLfloat)item->rectside[0][0][1].z());
        glVertex3f((GLfloat)item->rectside[0][1][1].x(), (GLfloat)item->rectside[0][1][1].y(), (GLfloat)item->rectside[0][1][1].z());
        glVertex3f((GLfloat)item->rectside[0][1][2].x(), (GLfloat)item->rectside[0][1][2].y(), (GLfloat)item->rectside[0][1][2].z());
        glVertex3f((GLfloat)item->rectside[0][0][2].x(), (GLfloat)item->rectside[0][0][2].y(), (GLfloat)item->rectside[0][0][2].z());

        glVertex3f((GLfloat)item->rectside[0][0][2].x(), (GLfloat)item->rectside[0][0][2].y(), (GLfloat)item->rectside[0][0][2].z());
        glVertex3f((GLfloat)item->rectside[0][1][2].x(), (GLfloat)item->rectside[0][1][2].y(), (GLfloat)item->rectside[0][1][2].z());
        glVertex3f((GLfloat)item->rectside[0][1][3].x(), (GLfloat)item->rectside[0][1][3].y(), (GLfloat)item->rectside[0][1][3].z());
        glVertex3f((GLfloat)item->rectside[0][0][3].x(), (GLfloat)item->rectside[0][0][3].y(), (GLfloat)item->rectside[0][0][3].z());

        glVertex3f((GLfloat)item->rectside[0][0][3].x(), (GLfloat)item->rectside[0][0][3].y(), (GLfloat)item->rectside[0][0][3].z());
        glVertex3f((GLfloat)item->rectside[0][1][3].x(), (GLfloat)item->rectside[0][1][3].y(), (GLfloat)item->rectside[0][1][3].z());
        glVertex3f((GLfloat)item->rectside[0][1][0].x(), (GLfloat)item->rectside[0][1][0].y(), (GLfloat)item->rectside[0][1][0].z());
        glVertex3f((GLfloat)item->rectside[0][0][0].x(), (GLfloat)item->rectside[0][0][0].y(), (GLfloat)item->rectside[0][0][0].z());
        glEnd();

        //triangle rect-round-transition
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f((GLfloat)item->rectside[1][0][1].x(),   (GLfloat)item->rectside[1][0][1].y(),   (GLfloat)item->rectside[1][0][1].z());
        for(int k = 0; k < 8; k++)
        {
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f((GLfloat)item->rectside[1][0][2].x(),   (GLfloat)item->rectside[1][0][2].y(),   (GLfloat)item->rectside[1][0][2].z());
        for(int k = 8; k < 16; k++)
        {
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f((GLfloat)item->rectside[1][0][3].x(),   (GLfloat)item->rectside[1][0][3].y(),   (GLfloat)item->rectside[1][0][3].z());
        for(int k = 16; k < 24; k++)
        {
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f((GLfloat)item->rectside[1][0][0].x(),   (GLfloat)item->rectside[1][0][0].y(),   (GLfloat)item->rectside[1][0][0].z());
        for(int k = 24; k < 32; k++)
        {
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();

        glBegin(GL_QUADS);

        glVertex3f((GLfloat)item->rectside[1][0][0].x(),   (GLfloat)item->rectside[1][0][0].y(),   (GLfloat)item->rectside[1][0][0].z());
        glVertex3f((GLfloat)item->rectside[1][0][1].x(),   (GLfloat)item->rectside[1][0][1].y(),   (GLfloat)item->rectside[1][0][1].z());
        glVertex3f((GLfloat)item->roundside[1][0][0].x(),   (GLfloat)item->roundside[1][0][0].y(),   (GLfloat)item->roundside[1][0][0].z());
        glVertex3f((GLfloat)item->roundside[1][0][31].x(),   (GLfloat)item->roundside[1][0][31].y(),   (GLfloat)item->roundside[1][0][31].z());

        glVertex3f((GLfloat)item->rectside[1][0][1].x(),   (GLfloat)item->rectside[1][0][1].y(),   (GLfloat)item->rectside[1][0][1].z());
        glVertex3f((GLfloat)item->rectside[1][0][2].x(),   (GLfloat)item->rectside[1][0][2].y(),   (GLfloat)item->rectside[1][0][2].z());
        glVertex3f((GLfloat)item->roundside[1][0][8].x(),   (GLfloat)item->roundside[1][0][8].y(),   (GLfloat)item->roundside[1][0][8].z());
        glVertex3f((GLfloat)item->roundside[1][0][7].x(),   (GLfloat)item->roundside[1][0][7].y(),   (GLfloat)item->roundside[1][0][7].z());

        glVertex3f((GLfloat)item->rectside[1][0][2].x(),   (GLfloat)item->rectside[1][0][2].y(),   (GLfloat)item->rectside[1][0][2].z());
        glVertex3f((GLfloat)item->rectside[1][0][3].x(),   (GLfloat)item->rectside[1][0][3].y(),   (GLfloat)item->rectside[1][0][3].z());
        glVertex3f((GLfloat)item->roundside[1][0][16].x(),   (GLfloat)item->roundside[1][0][16].y(),   (GLfloat)item->roundside[1][0][16].z());
        glVertex3f((GLfloat)item->roundside[1][0][15].x(),   (GLfloat)item->roundside[1][0][15].y(),   (GLfloat)item->roundside[1][0][15].z());

        glVertex3f((GLfloat)item->rectside[1][0][3].x(),   (GLfloat)item->rectside[1][0][3].y(),   (GLfloat)item->rectside[1][0][3].z());
        glVertex3f((GLfloat)item->rectside[1][0][0].x(),   (GLfloat)item->rectside[1][0][0].y(),   (GLfloat)item->rectside[1][0][0].z());
        glVertex3f((GLfloat)item->roundside[1][0][24].x(),   (GLfloat)item->roundside[1][0][24].y(),   (GLfloat)item->roundside[1][0][24].z());
        glVertex3f((GLfloat)item->roundside[1][0][23].x(),   (GLfloat)item->roundside[1][0][23].y(),   (GLfloat)item->roundside[1][0][23].z());

        glEnd();


    }

    if (this->render_outline)
    {
        setPaintingColor(color_pen);

        //        //front face round
        //        for(int k = 0; k < count_k - 1; k++)
        //        {
        //            glBegin(GL_LINE_LOOP);
        //            glVertex3f((GLfloat)item->roundside[0][0][k].x(), (GLfloat)item->roundside[0][0][k].y(), (GLfloat)item->roundside[0][0][k].z());
        //            glVertex3f((GLfloat)item->roundside[0][1][k].x(), (GLfloat)item->roundside[0][1][k].y(), (GLfloat)item->roundside[0][1][k].z());
        //            glVertex3f((GLfloat)item->roundside[0][1][k+1].x(), (GLfloat)item->roundside[0][1][k+1].y(), (GLfloat)item->roundside[0][1][k+1].z());
        //            glVertex3f((GLfloat)item->roundside[0][0][k+1].x(), (GLfloat)item->roundside[0][0][k+1].y(), (GLfloat)item->roundside[0][0][k+1].z());

        //        }
        //        glVertex3f((GLfloat)item->roundside[0][0][count_k - 1].x(), (GLfloat)item->roundside[0][0][count_k - 1].y(), (GLfloat)item->roundside[0][0][count_k - 1].z());
        //        glVertex3f((GLfloat)item->roundside[0][1][count_k - 1].x(), (GLfloat)item->roundside[0][1][count_k - 1].y(), (GLfloat)item->roundside[0][1][count_k - 1].z());
        //        glVertex3f((GLfloat)item->roundside[0][1][0].x(), (GLfloat)item->roundside[0][1][0].y(), (GLfloat)item->roundside[0][1][0].z());
        //        glVertex3f((GLfloat)item->roundside[0][0][0].x(), (GLfloat)item->roundside[0][0][0].y(), (GLfloat)item->roundside[0][0][0].z());
        //        glEnd();

        //outer faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f((GLfloat)item->roundside[0][0][k].x(), (GLfloat)item->roundside[0][0][k].y(), (GLfloat)item->roundside[0][0][k].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(), (GLfloat)item->roundside[1][0][k].y(), (GLfloat)item->roundside[1][0][k].z());
            glVertex3f((GLfloat)item->roundside[1][0][k+1].x(), (GLfloat)item->roundside[1][0][k+1].y(), (GLfloat)item->roundside[1][0][k+1].z());
            glVertex3f((GLfloat)item->roundside[0][0][k+1].x(), (GLfloat)item->roundside[0][0][k+1].y(), (GLfloat)item->roundside[0][0][k+1].z());
            glEnd();
        }
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->roundside[0][0][count_k - 1].x(), (GLfloat)item->roundside[0][0][count_k - 1].y(), (GLfloat)item->roundside[0][0][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][0][count_k - 1].x(), (GLfloat)item->roundside[1][0][count_k - 1].y(), (GLfloat)item->roundside[1][0][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][0][0].x(), (GLfloat)item->roundside[1][0][0].y(), (GLfloat)item->roundside[1][0][0].z());
        glVertex3f((GLfloat)item->roundside[0][0][0].x(), (GLfloat)item->roundside[0][0][0].y(), (GLfloat)item->roundside[0][0][0].z());
        glEnd();

        //inner faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f((GLfloat)item->roundside[0][1][k].x(), (GLfloat)item->roundside[0][1][k].y(), (GLfloat)item->roundside[0][1][k].z());
            glVertex3f((GLfloat)item->roundside[1][1][k].x(), (GLfloat)item->roundside[1][1][k].y(), (GLfloat)item->roundside[1][1][k].z());
            glVertex3f((GLfloat)item->roundside[1][1][k+1].x(), (GLfloat)item->roundside[1][1][k+1].y(), (GLfloat)item->roundside[1][1][k+1].z());
            glVertex3f((GLfloat)item->roundside[0][1][k+1].x(), (GLfloat)item->roundside[0][1][k+1].y(), (GLfloat)item->roundside[0][1][k+1].z());
            glEnd();
        }
        glBegin(GL_LINE_LOOP);
        glVertex3f((GLfloat)item->roundside[0][1][count_k - 1].x(), (GLfloat)item->roundside[0][1][count_k - 1].y(), (GLfloat)item->roundside[0][1][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][1][count_k - 1].x(), (GLfloat)item->roundside[1][1][count_k - 1].y(), (GLfloat)item->roundside[1][1][count_k - 1].z());
        glVertex3f((GLfloat)item->roundside[1][1][0].x(), (GLfloat)item->roundside[1][1][0].y(), (GLfloat)item->roundside[1][1][0].z());
        glVertex3f((GLfloat)item->roundside[0][1][0].x(), (GLfloat)item->roundside[0][1][0].y(), (GLfloat)item->roundside[0][1][0].z());
        glEnd();

        //inner faces rect
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][k].x(),   (GLfloat)item->rectside[1][0][k].y(),   (GLfloat)item->rectside[1][0][k].z());
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][1][k].x(),   (GLfloat)item->rectside[1][1][k].y(),   (GLfloat)item->rectside[1][1][k].z());
        }
        glEnd();
        glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[0][1][k].x(),   (GLfloat)item->rectside[0][1][k].y(),   (GLfloat)item->rectside[0][1][k].z());
            glVertex3f((GLfloat)item->rectside[1][1][k].x(),   (GLfloat)item->rectside[1][1][k].y(),   (GLfloat)item->rectside[1][1][k].z());
        }
        glEnd();


        //outer faces rect
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[0][0][k].x(),   (GLfloat)item->rectside[0][0][k].y(),   (GLfloat)item->rectside[0][0][k].z());
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[0][1][k].x(),   (GLfloat)item->rectside[0][1][k].y(),   (GLfloat)item->rectside[0][1][k].z());
        }
        glEnd();
        glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[0][0][k].x(),   (GLfloat)item->rectside[0][0][k].y(),   (GLfloat)item->rectside[0][0][k].z());
            glVertex3f((GLfloat)item->rectside[1][0][k].x(),   (GLfloat)item->rectside[1][0][k].y(),   (GLfloat)item->rectside[1][0][k].z());
        }
        glEnd();

        //inner faces rect
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][k].x(),   (GLfloat)item->rectside[1][0][k].y(),   (GLfloat)item->rectside[1][0][k].z());
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][1][k].x(),   (GLfloat)item->rectside[1][1][k].y(),   (GLfloat)item->rectside[1][1][k].z());
        }
        glEnd();
        glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glVertex3f((GLfloat)item->rectside[0][1][k].x(),   (GLfloat)item->rectside[0][1][k].y(),   (GLfloat)item->rectside[0][1][k].z());
            glVertex3f((GLfloat)item->rectside[1][1][k].x(),   (GLfloat)item->rectside[1][1][k].y(),   (GLfloat)item->rectside[1][1][k].z());
        }
        glEnd();

        //rect-round transition

        glBegin(GL_LINES);
        for(int k = 0; k < 8; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][1].x(),   (GLfloat)item->rectside[1][0][1].y(),   (GLfloat)item->rectside[1][0][1].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();
        glBegin(GL_LINES);
        for(int k = 8; k < 16; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][2].x(),   (GLfloat)item->rectside[1][0][2].y(),   (GLfloat)item->rectside[1][0][2].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();

        glBegin(GL_LINES);
        for(int k = 16; k < 24; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][3].x(),   (GLfloat)item->rectside[1][0][3].y(),   (GLfloat)item->rectside[1][0][3].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();

        glBegin(GL_LINES);
        for(int k = 24; k < 32; k++)
        {
            glVertex3f((GLfloat)item->rectside[1][0][0].x(),   (GLfloat)item->rectside[1][0][0].y(),   (GLfloat)item->rectside[1][0][0].z());
            glVertex3f((GLfloat)item->roundside[1][0][k].x(),   (GLfloat)item->roundside[1][0][k].y(),   (GLfloat)item->roundside[1][0][k].z());

        }
        glEnd();







    }
}

void GLWidget::paintAirDuctYpiece(Layer *layer, CAD_air_ductYpiece *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);

    if (this->render_solid)
    {
        setPaintingColor(color_brush);
        glBegin(GL_QUADS);

        //top and bottom
        glVertex3f((GLfloat)item->endcap_1->pos_bot_2.x(), (GLfloat)item->endcap_1->pos_bot_2.y(), (GLfloat)item->endcap_1->pos_bot_2.z());
        glVertex3f((GLfloat)item->endcap_3->pos_bot_4.x(), (GLfloat)item->endcap_3->pos_bot_4.y(), (GLfloat)item->endcap_3->pos_bot_4.z());
        glVertex3f((GLfloat)item->endcap_3->pos_top_4.x(), (GLfloat)item->endcap_3->pos_top_4.y(), (GLfloat)item->endcap_3->pos_top_4.z());
        glVertex3f((GLfloat)item->endcap_1->pos_top_2.x(), (GLfloat)item->endcap_1->pos_top_2.y(), (GLfloat)item->endcap_1->pos_top_2.z());


        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glVertex3f((GLfloat)item->endcap_3->pos_bot_1.x(), (GLfloat)item->endcap_3->pos_bot_1.y(), (GLfloat)item->endcap_3->pos_bot_1.z());
        glVertex3f((GLfloat)item->endcap_3->pos_top_1.x(), (GLfloat)item->endcap_3->pos_top_1.y(), (GLfloat)item->endcap_3->pos_top_1.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());


        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glVertex3f((GLfloat)item->endcap_2->pos_bot_4.x(), (GLfloat)item->endcap_2->pos_bot_4.y(), (GLfloat)item->endcap_2->pos_bot_4.z());
        glVertex3f((GLfloat)item->endcap_2->pos_top_4.x(), (GLfloat)item->endcap_2->pos_top_4.y(), (GLfloat)item->endcap_2->pos_top_4.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());

        glVertex3f((GLfloat)item->endcap_1->pos_bot_3.x(), (GLfloat)item->endcap_1->pos_bot_3.y(), (GLfloat)item->endcap_1->pos_bot_3.z());
        glVertex3f((GLfloat)item->endcap_2->pos_bot_1.x(), (GLfloat)item->endcap_2->pos_bot_1.y(), (GLfloat)item->endcap_2->pos_bot_1.z());
        glVertex3f((GLfloat)item->endcap_2->pos_top_1.x(), (GLfloat)item->endcap_2->pos_top_1.y(), (GLfloat)item->endcap_2->pos_top_1.z());
        glVertex3f((GLfloat)item->endcap_1->pos_top_3.x(), (GLfloat)item->endcap_1->pos_top_3.y(), (GLfloat)item->endcap_1->pos_top_3.z());

        //front
        glVertex3f((GLfloat)item->endcap_1->pos_bot_2.x(), (GLfloat)item->endcap_1->pos_bot_2.y(), (GLfloat)item->endcap_1->pos_bot_2.z());
        glVertex3f((GLfloat)item->endcap_3->pos_bot_4.x(), (GLfloat)item->endcap_3->pos_bot_4.y(), (GLfloat)item->endcap_3->pos_bot_4.z());
        glVertex3f((GLfloat)item->endcap_3->pos_bot_1.x(), (GLfloat)item->endcap_3->pos_bot_1.y(), (GLfloat)item->endcap_3->pos_bot_1.z());
        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());

        glVertex3f((GLfloat)item->endcap_2->pos_bot_1.x(), (GLfloat)item->endcap_2->pos_bot_1.y(), (GLfloat)item->endcap_2->pos_bot_1.z());
        glVertex3f((GLfloat)item->endcap_2->pos_bot_4.x(), (GLfloat)item->endcap_2->pos_bot_4.y(), (GLfloat)item->endcap_2->pos_bot_4.z());
        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glVertex3f((GLfloat)item->endcap_1->pos_bot_3.x(), (GLfloat)item->endcap_1->pos_bot_3.y(), (GLfloat)item->endcap_1->pos_bot_3.z());
        //back
        glVertex3f((GLfloat)item->endcap_1->pos_top_2.x(), (GLfloat)item->endcap_1->pos_top_2.y(), (GLfloat)item->endcap_1->pos_top_2.z());
        glVertex3f((GLfloat)item->endcap_3->pos_top_4.x(), (GLfloat)item->endcap_3->pos_top_4.y(), (GLfloat)item->endcap_3->pos_top_4.z());
        glVertex3f((GLfloat)item->endcap_3->pos_top_1.x(), (GLfloat)item->endcap_3->pos_top_1.y(), (GLfloat)item->endcap_3->pos_top_1.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());

        glVertex3f((GLfloat)item->endcap_1->pos_top_3.x(), (GLfloat)item->endcap_1->pos_top_3.y(), (GLfloat)item->endcap_1->pos_top_3.z());
        glVertex3f((GLfloat)item->endcap_2->pos_top_1.x(), (GLfloat)item->endcap_2->pos_top_1.y(), (GLfloat)item->endcap_2->pos_top_1.z());
        glVertex3f((GLfloat)item->endcap_2->pos_top_4.x(), (GLfloat)item->endcap_2->pos_top_4.y(), (GLfloat)item->endcap_2->pos_top_4.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());

        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex3f((GLfloat)item->endcap_1->pos_bot_3.x(), (GLfloat)item->endcap_1->pos_bot_3.y(), (GLfloat)item->endcap_1->pos_bot_3.z());
        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glVertex3f((GLfloat)item->endcap_1->pos_bot_2.x(), (GLfloat)item->endcap_1->pos_bot_2.y(), (GLfloat)item->endcap_1->pos_bot_2.z());

        glVertex3f((GLfloat)item->endcap_1->pos_top_2.x(), (GLfloat)item->endcap_1->pos_top_2.y(), (GLfloat)item->endcap_1->pos_top_2.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());
        glVertex3f((GLfloat)item->endcap_1->pos_top_3.x(), (GLfloat)item->endcap_1->pos_top_3.y(), (GLfloat)item->endcap_1->pos_top_3.z());


        glEnd();

    }

    if (this->render_outline)
    {
        setPaintingColor(color_pen);
        glBegin(GL_LINES);
        //lower
        glVertex3f((GLfloat)item->endcap_1->pos_bot_2.x(), (GLfloat)item->endcap_1->pos_bot_2.y(), (GLfloat)item->endcap_1->pos_bot_2.z());
        glVertex3f((GLfloat)item->endcap_3->pos_bot_4.x(), (GLfloat)item->endcap_3->pos_bot_4.y(), (GLfloat)item->endcap_3->pos_bot_4.z());

        glVertex3f((GLfloat)item->endcap_3->pos_bot_1.x(), (GLfloat)item->endcap_3->pos_bot_1.y(), (GLfloat)item->endcap_3->pos_bot_1.z());
        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());

        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glVertex3f((GLfloat)item->endcap_2->pos_bot_4.x(), (GLfloat)item->endcap_2->pos_bot_4.y(), (GLfloat)item->endcap_2->pos_bot_4.z());

        glVertex3f((GLfloat)item->endcap_2->pos_bot_1.x(), (GLfloat)item->endcap_2->pos_bot_1.y(), (GLfloat)item->endcap_2->pos_bot_1.z());
        glVertex3f((GLfloat)item->endcap_1->pos_bot_3.x(), (GLfloat)item->endcap_1->pos_bot_3.y(), (GLfloat)item->endcap_1->pos_bot_3.z());
        //upper
        glVertex3f((GLfloat)item->endcap_1->pos_top_2.x(), (GLfloat)item->endcap_1->pos_top_2.y(), (GLfloat)item->endcap_1->pos_top_2.z());
        glVertex3f((GLfloat)item->endcap_3->pos_top_4.x(), (GLfloat)item->endcap_3->pos_top_4.y(), (GLfloat)item->endcap_3->pos_top_4.z());

        glVertex3f((GLfloat)item->endcap_3->pos_top_1.x(), (GLfloat)item->endcap_3->pos_top_1.y(), (GLfloat)item->endcap_3->pos_top_1.z());
        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());

        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());
        glVertex3f((GLfloat)item->endcap_2->pos_top_4.x(), (GLfloat)item->endcap_2->pos_top_4.y(), (GLfloat)item->endcap_2->pos_top_4.z());

        glVertex3f((GLfloat)item->endcap_2->pos_top_1.x(), (GLfloat)item->endcap_2->pos_top_1.y(), (GLfloat)item->endcap_2->pos_top_1.z());
        glVertex3f((GLfloat)item->endcap_1->pos_top_3.x(), (GLfloat)item->endcap_1->pos_top_3.y(), (GLfloat)item->endcap_1->pos_top_3.z());


        glVertex3f((GLfloat)item->splitPoint[1].x(), (GLfloat)item->splitPoint[1].y(), (GLfloat)item->splitPoint[1].z());
        glVertex3f((GLfloat)item->splitPoint[0].x(), (GLfloat)item->splitPoint[0].y(), (GLfloat)item->splitPoint[0].z());
        glEnd();
    }

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

void GLWidget::paintSanitaryPipe(Layer *layer, CAD_sanitary_pipe *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryPipeTurn(Layer *layer, CAD_sanitary_pipeTurn *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryPipeReducer(Layer *layer, CAD_sanitary_pipeReducer *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryPipeTeeConnector(Layer *layer, CAD_sanitary_pipeTeeConnector *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryPipeEndCap(Layer *layer, CAD_sanitary_pipeEndCap *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryFlange(Layer *layer, CAD_sanitary_flange *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryElectricWaterHeater(Layer *layer, CAD_sanitary_electricWaterHeater *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryWashBasin(Layer *layer, CAD_sanitary_washBasin *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitarySink(Layer *layer, CAD_sanitary_sink *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryShower(Layer *layer, CAD_sanitary_shower *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryEmergencyShower(Layer *layer, CAD_sanitary_emergencyShower *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryEmergencyEyeShower(Layer *layer, CAD_sanitary_emergencyEyeShower *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

void GLWidget::paintSanitaryLiftingUnit(Layer *layer, CAD_sanitary_liftingUnit *item)
{
    QColor color_pen = getColorPen(item, layer);
    QColor color_brush = getColorBrush(item, layer);
}

QList<CADitem*> GLWidget::itemsAtPosition(QPoint pos, int size_x, int size_y)
{
#define HITBUFFER_SIZE 512000
    GLuint buffer[HITBUFFER_SIZE];
    GLint viewport[4];

    saveGLState();


    glViewport(0, 0, width(), height());
    //    glViewport(pos.x(), pos.y(), (GLsizei)_cursorPickboxSize, (GLsizei)_cursorPickboxSize);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(HITBUFFER_SIZE, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glDisable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);

    //    glTranslatef((qreal)translationOffset.x() / (qreal)this->width() * 2, (qreal)translationOffset.y() / (qreal)this->height() * 2, 0.0);
    //    glScalef(this->zoomFactor / screenRatio / (qreal)this->height(), this->zoomFactor / (qreal)this->height(), 1.0 / 100000.0);
    //    glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
    //    glRotatef(rot_y, 0.0f, 1.0f, 0.0f);
    //    glRotatef(rot_z, 0.0f, 0.0f, 1.0f);

    matrix_glSelect.setToIdentity();
    matrix_glSelect.scale(1.0 / ((qreal)size_x / this->width()), 1.0 / ((qreal)size_y / this->height()), 1.0);
    matrix_glSelect.translate(-(qreal)pos.x(), -(qreal)pos.y(), 0.0);
    updateMatrixAll();
    //    shaderProgram->setUniformValue(shader_matrixLocation, matrix_projection * matrix_glSelect * matrix_modelview * matrix_rotation);
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);


    glDepthFunc(GL_LESS);
    glDepthRange(1,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

    glName = 0;
    paintContent(itemDB->layers);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    GLint hits = glRenderMode(GL_RENDER);   // Number of hits is returned by glRenderMode
    restoreGLState();
    matrix_glSelect.setToIdentity();
    updateMatrixAll();

    QList<CADitem*> foundItems;
    QMap<int,CADitem*> itemsDepthMap;

    if (hits == 0)
        return foundItems;

    int i = 0;
    GLint hit = 1;
//    GLuint globalMinDepth = 0xffffffff;
    GLuint glName;
    while (i < HITBUFFER_SIZE)
    {
        GLuint numberOfNames = buffer[i];
        GLuint minDepth = buffer[i + 1];
        GLuint maxDepth = buffer[i + 2];

        if (numberOfNames > 0)
        {
            glName = buffer[i + 3];
//            if (minDepth < globalMinDepth)
//            {
//                globalMinDepth = minDepth;
//            }
            CADitem* item = itemsAtPosition_processLayers(itemDB->layers, glName);
            itemsDepthMap.insertMulti(maxDepth, item);
        }

        i += 3;
        i += numberOfNames;

        hit++;
        if (hit > hits)
            break;
    }

    if (i >= HITBUFFER_SIZE)
        QMessageBox::warning(this, "GLWidget::itemAtPosition()", "HITBUFFER_SIZE too small - too many objects in findbox!");


    foundItems = itemsDepthMap.values();

    return foundItems;

//    CADitem* item = itemAtPosition_processLayers(itemDB->layers, glName);
//    if (item)
//        return item;

//    return NULL;
}

CADitem *GLWidget::itemsAtPosition_processLayers(QList<Layer *> layers, GLuint glName)
{
    foreach (Layer* layer, layers)
    {
        if (!layer->on)
            continue;

        CADitem* item = itemsAtPosition_processItems(layer->items, glName);
        if (item)
            return item;


        item = itemsAtPosition_processLayers(layer->subLayers, glName);
        if (item)
            return item;
    }

    return NULL;
}

CADitem *GLWidget::itemsAtPosition_processItems(QList<CADitem *> items, GLuint glName)
{
    foreach (CADitem* item, items)
    {
        if (item->index == glName)
        {
            return item;
        }

        item = itemsAtPosition_processItems(item->subItems, glName);
        if (item)
            return item;
    }

    return NULL;
}

void GLWidget::highlightItemAtPosition(QPoint pos)
{
    QList<CADitem*> itemList = this->itemsAtPosition(pos, _cursorPickboxSize, _cursorPickboxSize);
    CADitem* item;
    if (itemList.isEmpty())
        item = NULL;
    else
        item = itemList.at(0);

    // tst
    this->item_lastHighlight = item;

    if (item != NULL)
    {
        item->highlight = true;
        highlightItems(item->subItems);
    }

    emit signal_highlightItem(item);
}

void GLWidget::highlightItems(QList<CADitem *> items)
{
    foreach(CADitem* item, items)
    {
        item->highlight = true;
        highlightItems(item->subItems);
    }
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
        selectionAddSubItems(item->subItems);
        emit signal_selectionChanged(selection_itemList);
    }
}

void GLWidget::selectionAddItems(QList<CADitem *> items)
{
    foreach (CADitem* item, items)
    {
        if (item != NULL)
        {
            if (selection_itemList.contains(item))
                continue;
            this->selection_itemList.append(item);
            item->selected = true;
            selectionAddSubItems(item->subItems);
        }
    }
    emit signal_selectionChanged(selection_itemList);
}

void GLWidget::selectionAddSubItems(QList<CADitem *> items)
{
    foreach(CADitem * item, items) {
        item->selected = true;
        selectionAddSubItems(item->subItems);
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
        selectionRemoveSubItems(item->subItems);
        emit signal_selectionChanged(selection_itemList);
    }
}

void GLWidget::selectionRemoveSubItems(QList<CADitem *> items)
{
    foreach(CADitem * item, items) {
        item->selected = false;
        selectionRemoveSubItems(item->subItems);
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

void GLWidget::initializeGL()
{
    makeCurrent();


    //    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    //    GLfloat mat_shininess[] = { 0.2 };
    //    glShadeModel (GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE | GL_EMISSION);

    //tbd: glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    //    glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
    //    glEnable(GL_COLOR_MATERIAL);
    //    glEnable(GL_NORMALIZE);

    //    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    //    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    //    GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    //    GLfloat diffuseLight[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    //    GLfloat light_position[] = { 500.0, 15.0, -800.0, 0.0 };

    //    glLightfv(GL_LIGHT0, GL_DIFFUSE,diffuseLight);
    //    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    //    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
    //    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.3);
    //    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.8);

    //    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    shader_1_frag = new QGLShader(QGLShader::Fragment, this);
    bool shaderOk = shader_1_frag->compileSourceFile(":/shaders/test.frag");
    if (!shaderOk)
        QMessageBox::critical(this, "Shader compiler", "Fragment shader failed to compile!");

    shader_1_vert = new QGLShader(QGLShader::Vertex, this);
    shaderOk = shader_1_vert->compileSourceFile(":/shaders/shader_1.vert");

    shaderProgram = new QGLShaderProgram(this);
    shaderProgram->addShader(shader_1_vert);
    shaderProgram->addShader(shader_1_frag);
    shaderOk = shaderProgram->link();
    if (!shaderOk)
        QMessageBox::critical(this, "Shader compiler", "Vertex shader failed to compile!");

    if (!shaderOk)
    {
        QMessageBox::critical(this, "Shader linker", QString("Shader failed to link!\n\n") + shaderProgram->log());
    }


    shaderOk = shaderProgram->bind();

    if (!shaderOk)
    {
        QMessageBox::critical(this, "Shader program", "Shader could not be bound to gl context!");
    }

    shader_vertexLocation = shaderProgram->attributeLocation("VertexPosition");
    shader_matrixLocation = shaderProgram->uniformLocation("Matrix");
    shader_colorLocation = shaderProgram->attributeLocation("VertexColor");
    shader_textureCoordLocation = shaderProgram->attributeLocation("TexCoord");
    shader_textureSamplerLocation = shaderProgram->uniformLocation("uTexUnit0");
    shader_useTextureLocation = shaderProgram->uniformLocation("UseTexture");
    shader_useClippingXLocation = shaderProgram->uniformLocation("UseClippingX");
    shader_useClippingYLocation = shaderProgram->uniformLocation("UseClippingY");
    shader_useClippingZLocation = shaderProgram->uniformLocation("UseClippingZ");
    shader_Depth_of_view_location = shaderProgram->uniformLocation("Depth_of_view");
    shader_Height_of_intersection_location = shaderProgram->uniformLocation("Height_of_intersection");

    if (shader_vertexLocation < 0)
        QMessageBox::information(this, "Vertex Location invalid", QString().setNum(shader_vertexLocation));
    if (shader_colorLocation < 0)
        QMessageBox::information(this, "Color Location invalid", QString().setNum(shader_colorLocation));
    if (shader_textureCoordLocation < 0)
        QMessageBox::information(this, "Texture Coordinate Location invalid", QString().setNum(shader_textureCoordLocation));
    if (shader_matrixLocation < 0)
        QMessageBox::information(this, "Matrix Location invalid", QString().setNum(shader_matrixLocation));
    if (shader_useTextureLocation < 0)
        QMessageBox::information(this, "Use Texture Location invalid", QString().setNum(shader_useTextureLocation));
    if (shader_useClippingXLocation < 0)
        QMessageBox::information(this, "Use ClippingX Location invalid", QString().setNum(shader_useClippingXLocation));
    if (shader_useClippingYLocation < 0)
        QMessageBox::information(this, "Use ClippingY Location invalid", QString().setNum(shader_useClippingYLocation));
    if (shader_useClippingZLocation < 0)
        QMessageBox::information(this, "Use ClippingZ Location invalid", QString().setNum(shader_useClippingZLocation));
    if (shader_Depth_of_view_location < 0)
        QMessageBox::information(this, "Depth of View Location invalid", QString().setNum(shader_Depth_of_view_location));
    if (shader_Height_of_intersection_location < 0)
        QMessageBox::information(this, "Height of Intersection Location invalid", QString().setNum(shader_Height_of_intersection_location));

    qDebug() << "vertex location" << shader_vertexLocation;
    qDebug() << "matrix location" << shader_matrixLocation;
    qDebug() << "color location" << shader_colorLocation;
    qDebug() << "texture coord location" << shader_textureCoordLocation;
    qDebug() << "use texture location" << shader_useTextureLocation;
    qDebug() << "use clippingX location" << shader_useClippingXLocation;
    qDebug() << "use clippingY location" << shader_useClippingYLocation;
    qDebug() << "use clippingZ location" << shader_useClippingZLocation;
    qDebug() << "depth of view location" << shader_Depth_of_view_location;
    qDebug() << "height of intersection location" << shader_Height_of_intersection_location;
}

void GLWidget::resizeGL()
{

}

void GLWidget::paintGL()
{

}


