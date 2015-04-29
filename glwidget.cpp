/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent, ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier *itemGripModifier) :
    QOpenGLWidget(parent)
{
    //    qDebug() << "Created GLWidget";
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->itemGripModifier = itemGripModifier;
    this->mousePos = QPoint();
    mouse_lastMidPress_dateTime = QDateTime::fromMSecsSinceEpoch(0);

    this->translationOffset = QPoint();
    this->zoomFactor = 1.0;
    this->centerOfViewInScene = QVector3D();
    this->displayCenter = QPoint();
    this->cuttingplane = CuttingPlane_nZ;
    this->height_of_intersection = QVector3D(0.0, 0.0, 100000.0);
    this->depth_of_view = QVector3D(0.0, 0.0, 200000.0);
    this->render_solid = true;
    this->render_outline = true;
    this->render_maintenance_area = true;
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
    this->rendertime_ns_per_frame = ULONG_MAX;

    this->pickActive = false;
    this->cursorShown = true;
    this->arcballShown = false;
    this->snapMode = SnapNo;
    this->item_lastHighlight = NULL;
    this->selectItemsByColor = false;

    slot_update_settings();

    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    this->setAttribute(Qt::WA_ForceUpdatesDisabled, true);
    this->setAttribute(Qt::WA_Hover, false);
    this->setAttribute(Qt::WA_MouseTracking, true);
    this->setAttribute(Qt::WA_NoMousePropagation, true);
}

GLWidget::~GLWidget()
{
//    qDebug() << "GLWidget destroyed";
    makeCurrent();
    openGLTimerQuery->destroy();
    shaderProgram->release();
    shaderProgram->removeAllShaders();
    texture_cube1->destroy();
    texture_cube2->destroy();
    texture_cube3->destroy();
    texture_cube4->destroy();
    texture_cube5->destroy();
    texture_cube6->destroy();
    delete fbo_select;
    delete fbo_renderImage;
    delete openGLTimerQuery;
    delete shader_1_vert;
    delete shader_1_lines_geom;
    delete shader_1_triangles_geom;
    delete shader_1_frag;
    delete shader_2_vert;
    delete shader_2_frag;
    delete shaderProgram_lines;
    delete shaderProgram_triangles;
    delete shaderProgram_overlay;
    delete texture_cube1;
    delete texture_cube2;
    delete texture_cube3;
    delete texture_cube4;
    delete texture_cube5;
    delete texture_cube6;
}

QPointF GLWidget::mapFromScene(QVector3D &scenePoint)
{
    qreal x;
    qreal y;

    QVector4D row0 = matrix_all.row(0);
    QVector4D row1 = matrix_all.row(1);

    x = row0.x() * scenePoint.x() + row0.y() * scenePoint.y() + row0.z() * scenePoint.z() + row0.w();
    y = row1.x() * scenePoint.x() + row1.y() * scenePoint.y() + row1.z() * scenePoint.z() + row1.w();

    return QPointF(x / 2.0 * this->width(), y / 2.0 * this->height());
}

void GLWidget::updateMatrixAll()
{
    matrix_all = matrix_projection * matrix_glSelect * matrix_modelview * matrix_rotation;
}

void GLWidget::moveCursor(QPoint pos)
{
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
    // Selection rect must be from topleft to bottomright

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
    Q_UNUSED(on);
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

QMatrix4x4 GLWidget::getMatrix_all()
{
    return this->matrix_all;
}

QMatrix4x4 GLWidget::getMatrix_modelview()
{
    return this->matrix_modelview;
}

QMatrix4x4 GLWidget::getMatrix_rotation()
{
    return this->matrix_rotation;
}

void GLWidget::render_image(QPainter* painter, int x, int y, int size_x, int size_y, QMatrix4x4 matrix_modelview, QMatrix4x4 matrix_rotation, bool showTiles)
{
    QMatrix4x4 matrix_tile;

    // Check if image size can be rendered in framebuffer or if tile rendering is needed
    int tile_pos_x;
    int tile_pos_y;
    int tile_size_x = this->width();
    int tile_size_y = this->height();
    int num_tiles_x = size_x / tile_size_x + 1;
    int num_tiles_y = size_y / tile_size_y + 1;

    qreal zoom = qMin((qreal)size_x / (qreal)this->width(), (qreal)size_y / (qreal)this->height());


    // Set cutting planes to requested state for image rendering - tbd.
//    this->height_of_intersection.setZ(height);
//    this->depth_of_view.setZ(depth);

    // Render it **************************************************************

    painter->save();
    painter->setClipRect(x, y, size_x, size_y);

    for (int current_tile_x = 0; current_tile_x < num_tiles_x; current_tile_x++)
    {
        for (int current_tile_y = 0; current_tile_y < num_tiles_y; current_tile_y++)
        {
            tile_pos_x = tile_size_x * current_tile_x;
            tile_pos_y = tile_size_y * current_tile_y;


            // Set matrix to requested state for tile image rendering
            matrix_tile.setToIdentity();
            matrix_tile.translate((-(qreal)current_tile_x) * (qreal)tile_size_x / zoom, ((qreal)current_tile_y + 1.0) * (qreal)tile_size_y / zoom, 0);

            QMatrix4x4 matrix_projection_tile;
            matrix_projection_tile.setToIdentity();
            matrix_projection_tile.translate(-1.0, -1.0);
            matrix_projection_tile.scale(zoom, zoom, 1.0);
            matrix_projection_tile.translate(+1.0, -1.0);
            matrix_projection_tile.scale(2.0 / (qreal)this->width(), 2.0 / (qreal)this->height(), 1.0);

            this->matrix_all = matrix_projection_tile * matrix_tile * matrix_modelview * matrix_rotation;

            makeCurrent();

            if (fbo_renderImage->size() != QSize(tile_size_x, tile_size_y))
            {
                QOpenGLFramebufferObjectFormat format = fbo_renderImage->format();
                delete fbo_renderImage;
                fbo_renderImage = new QOpenGLFramebufferObject(tile_size_x, tile_size_y, format);
            }

            fbo_renderImage->bind();

            glDepthFunc(GL_LEQUAL);
            glDepthRange(1,0);
            glDisable(GL_BLEND);
            glEnable(GL_MULTISAMPLE);
            glDisable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_ALPHA_TEST);
            glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            paintContent(itemDB->layers);
            QImage image_tile = fbo_renderImage->toImage(true);

            painter->drawImage(x + tile_pos_x, y + tile_pos_y, image_tile);
            fbo_renderImage->release();

            doneCurrent();
        }
    }

    if (showTiles)
    {
        painter->setPen(Qt::blue);
        for (int current_tile_x = 0; current_tile_x < num_tiles_x; current_tile_x++)
        {
            for (int current_tile_y = 0; current_tile_y < num_tiles_y; current_tile_y++)
            {
                tile_pos_x = tile_size_x * current_tile_x;
                tile_pos_y = tile_size_y * current_tile_y;


                painter->drawRect(x + tile_pos_x, y + tile_pos_y, tile_size_x, tile_size_y);
            }
        }
    }

    // Rendering done *********************************************************

    // Change cutting planes back to previous state - tbd.
//    this->height_of_intersection.setZ(height);
//    this->depth_of_view.setZ(depth);

    // Change painter back to previous state
    painter->restore();

    // Change matrix back back to previous state
    this->updateMatrixAll();
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

    // rot tbd.
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

void GLWidget::wheelEvent(QWheelEvent* event)
{
    qreal zoomStep = 1.15;

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

    translationOffset += (mousePos - translationOffset) * (1.0 - zoomStep);

    event->accept();

    slot_repaint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "mouseMove";

    mousePos = event->pos();
    mousePos.setY((this->height() - 1) - mousePos.y());
    mousePos -= QPoint(this->width() / 2, this->height() / 2);
    QPoint mouseMoveDelta = mousePos - mousePosOld;
    mousePosOld = mousePos;

    if (event->buttons() == Qt::LeftButton)
    {

    }

    if (event->buttons() == Qt::MidButton)
    {
        translationOffset += mouseMoveDelta;
        if (mouseMoveDelta.manhattanLength() > 20)  // Do not zoom_pan_all if double clicked and mouse was moved
        {
            QDateTime veryLongAgo = QDateTime::fromMSecsSinceEpoch(0);
            mouse_lastMidPress_dateTime = veryLongAgo;
        }
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

//    emit signal_mouseMoved(QVector3D(mousePos.x(), mousePos.y(), 0));

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
        QDateTime now = QDateTime::currentDateTime();
        quint64 msecs_sinceLastMidPress = this->mouse_lastMidPress_dateTime.msecsTo(now);
        this->mouse_lastMidPress_dateTime = now;

        if (msecs_sinceLastMidPress < 500)
        {
            this->zoom_pan_showAll();
        }
        else
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
        slot_repaint();
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

        if (this->itemGripModifier == NULL)
        {
            event->accept();
            return;
        }

        // Check if there is a currently active command
        if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Move)
        {
            if (snapMode != SnapNo)
            {
                this->itemGripModifier->moveItemsTo(snapPos_scene);
                this->slot_repaint();
            }
        }
        else if (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Copy)
        {
            if (snapMode != SnapNo)
            {
                this->itemGripModifier->copyItemsTo(snapPos_scene);
                this->slot_repaint();
            }
        }

        // Pickbox
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
            QList<CADitem*> pickedItems = this->itemsAtPosition_v2(this->selection().center(), this->selection().width(), this->selection().height());
            if (this->selectionMode() == Qt::IntersectsItemShape)
                selectionAddItems(pickedItems);
            else if (this->selectionMode() == Qt::ContainsItemShape)
            {
                QSet<CADitem*> surroundingItems;
                surroundingItems.unite(this->itemsAtPosition_v2(((this->selection().topLeft() + this->selection().topRight()) / 2), this->selection().width(), 2).toSet());
                surroundingItems.unite(this->itemsAtPosition_v2(((this->selection().bottomLeft() + this->selection().bottomRight()) / 2), this->selection().width(), 2).toSet());
                surroundingItems.unite(this->itemsAtPosition_v2(((this->selection().topLeft() + this->selection().bottomLeft()) / 2), 2, this->selection().height()).toSet());
                surroundingItems.unite(this->itemsAtPosition_v2(((this->selection().topRight() + this->selection().bottomRight()) / 2), 2, this->selection().height()).toSet());
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
    if (this->itemGripModifier == NULL)
    {
        event->accept();
        return;
    }

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
    case Qt::Key_A:                         // Render a test image
    {
        QImage testImage(this->width() * 2, this->height() * 2, QImage::Format_ARGB32_Premultiplied);
        QPainter painter (&testImage);
        this->render_image(&painter, 0, 0, testImage.width(), testImage.height(), this->matrix_modelview, this->matrix_rotation);
        painter.end();
        QString filename = QFileDialog::getSaveFileName(this, tr("Save captured image"), "", "PNG image file (*.png)");
        testImage.save(filename, "PNG", -1);
        break;
    }
    case Qt::Key_C:                         // Copy item
        if ((this->selection_itemList.count() > 0) && (item_lastHighlight != NULL))   // more than one item
        {
            if ((snapMode == SnapBasepoint) || (snapMode == SnapFlange) || (snapMode == SnapEndpoint))
            {
                QList<CADitem*> itemsToDo = this->selection_itemList;
                if (QMessageBox::question(this, tr("Copy items"), tr("You are going to copy ") + QString().setNum(itemsToDo.count()) + "item(s). Proceed?")
                        == QMessageBox::Yes)
                {
                    this->itemGripModifier->setItems(itemsToDo);
                    if (event->modifiers() & Qt::ShiftModifier)
                        this->itemGripModifier->activateGrip(ItemGripModifier::Grip_CopyMulti, QCursor::pos(), snapPos_scene);
                    else
                        this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Copy, QCursor::pos(), snapPos_scene);
                    this->slot_repaint();
                }
            }
        }
        else if (item_lastHighlight != NULL)        // only one item
        {
            if ((snapMode == SnapBasepoint) || (snapMode == SnapFlange) || (snapMode == SnapEndpoint))
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                if (event->modifiers() & Qt::ShiftModifier)
                    this->itemGripModifier->activateGrip(ItemGripModifier::Grip_CopyMulti, QCursor::pos(), snapPos_scene);
                else
                    this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Copy, QCursor::pos(), snapPos_scene);
                this->slot_repaint();
            }
        }
        break;
    case Qt::Key_E:                         // Edit item
        if (item_lastHighlight != NULL)
        {
            if (this->itemWizard != NULL)
                this->itemWizard->showWizard(item_lastHighlight, itemDB);
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
        if ((this->selection_itemList.count() > 0) && (item_lastHighlight != NULL))   // more than one item
        {
            if ((snapMode == SnapBasepoint) || (snapMode == SnapFlange) || (snapMode == SnapEndpoint))
            {
                QList<CADitem*> itemsToDo = this->selection_itemList;
                if (QMessageBox::question(this, tr("Moving items"), tr("You are going to move ") + QString().setNum(itemsToDo.count()) + "item(s). Proceed?")
                        == QMessageBox::Yes)
                {
                    this->itemGripModifier->setItems(itemsToDo);
                    this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Move, QCursor::pos(), snapPos_scene);
                    this->slot_repaint();
                }
            }
        }
        else if (item_lastHighlight != NULL)        // only one item
        {
            if ((snapMode == SnapBasepoint) || (snapMode == SnapFlange) || (snapMode == SnapEndpoint))
            {
                this->itemGripModifier->setItem(item_lastHighlight);
                this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Move, QCursor::pos(), snapPos_scene);
                this->slot_repaint();
            }
        }
        break;
    case Qt::Key_R:
        if ((this->selection_itemList.count() > 0) && (item_lastHighlight != NULL))
        {
            if (snapMode != SnapNo)
            {
                QList<CADitem*> itemsToDo = this->selection_itemList;
                if (QMessageBox::question(this, tr("Rotating items"), tr("You are going to rotate ") + QString().setNum(itemsToDo.count()) + "item(s). Proceed?")
                        == QMessageBox::Yes)
                {
                    this->itemGripModifier->setItems(itemsToDo);
                    this->itemGripModifier->activateGrip(ItemGripModifier::Grip_Rotate_aroundPoint, QCursor::pos(), snapPos_scene);
                    this->slot_repaint();
                }
            }
        }
        break;
    case Qt::Key_X:                         // Turn item around x axis
        if (item_lastHighlight != NULL)
        {
//            item_lastHighlight->angle_x += 45.0;
//            if (item_lastHighlight->angle_x > 359.0) item_lastHighlight->angle_x = 0.0;
//            item_lastHighlight->wizardParams.insert("Angle x", (item_lastHighlight->angle_x));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            QMatrix4x4 matrix_old = item_lastHighlight->matrix_rotation;
            QMatrix4x4 m;
            m.setToIdentity();
            m.rotate(45.0, 1.0, 0.0, 0.0);
            QVector3D angles = MAngleCalculations().anglesFromMatrix(m * matrix_old);
            WizardParams newParams;
            newParams.insert("Angle x", (angles.x()));
            newParams.insert("Angle y", (angles.y()));
            newParams.insert("Angle z", (angles.z()));
            itemDB->setRestorePoint();
            itemDB->modifyItem_withRestorePoint(item_lastHighlight, newParams);
//            item_lastHighlight->wizardParams.insert("Angle x", (angles.x()));
//            item_lastHighlight->wizardParams.insert("Angle y", (angles.y()));
//            item_lastHighlight->wizardParams.insert("Angle z", (angles.z()));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            slot_repaint();
        }
        break;
    case Qt::Key_Y:                         // Turn item around y axis
        if (item_lastHighlight != NULL)
        {
//            item_lastHighlight->angle_y += 45.0;
//            if (item_lastHighlight->angle_y > 359.0) item_lastHighlight->angle_y = 0.0;
//            item_lastHighlight->wizardParams.insert("Angle y", (item_lastHighlight->angle_y));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            QMatrix4x4 matrix_old = item_lastHighlight->matrix_rotation;
            QMatrix4x4 m;
            m.setToIdentity();
            m.rotate(45.0, 0.0, 1.0, 0.0);
            QVector3D angles = MAngleCalculations().anglesFromMatrix(m * matrix_old);
            WizardParams newParams;
            newParams.insert("Angle x", (angles.x()));
            newParams.insert("Angle y", (angles.y()));
            newParams.insert("Angle z", (angles.z()));
            itemDB->setRestorePoint();
            itemDB->modifyItem_withRestorePoint(item_lastHighlight, newParams);
//            item_lastHighlight->wizardParams.insert("Angle x", (angles.x()));
//            item_lastHighlight->wizardParams.insert("Angle y", (angles.y()));
//            item_lastHighlight->wizardParams.insert("Angle z", (angles.z()));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            slot_repaint();
        }
        break;
    case Qt::Key_Z:                         // Turn item around z axis
        if (event->modifiers() & Qt::ControlModifier)
        {
            this->itemDB->makeRestore();
        }
        else if (item_lastHighlight!= NULL)
        {
//            item_lastHighlight->angle_z += 45.0;
//            if (item_lastHighlight->angle_z > 359.0) item_lastHighlight->angle_z = 0.0;
//            item_lastHighlight->wizardParams.insert("Angle z", (item_lastHighlight->angle_z));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            QMatrix4x4 matrix_old = item_lastHighlight->matrix_rotation;
            QMatrix4x4 m;
            m.setToIdentity();
            m.rotate(45.0, 0.0, 0.0, 1.0);
            QVector3D angles = MAngleCalculations().anglesFromMatrix(m * matrix_old);
            WizardParams newParams;
            newParams.insert("Angle x", (angles.x()));
            newParams.insert("Angle y", (angles.y()));
            newParams.insert("Angle z", (angles.z()));
            itemDB->setRestorePoint();
            itemDB->modifyItem_withRestorePoint(item_lastHighlight, newParams);
//            item_lastHighlight->wizardParams.insert("Angle x", (angles.x()));
//            item_lastHighlight->wizardParams.insert("Angle y", (angles.y()));
//            item_lastHighlight->wizardParams.insert("Angle z", (angles.z()));
//            item_lastHighlight->processWizardInput();
//            item_lastHighlight->calculate();
            slot_repaint();
        }
        break;
    case Qt::Key_Delete:
        if (this->selection_itemList.count() > 0)
        {
            QList<CADitem*> itemsToDelete = this->selection_itemList;

            if (QMessageBox::question(this, tr("Deleting items"), tr("You are going to delete ") + QString().setNum(itemsToDelete.count()) + "item(s). Proceed?")
                    == QMessageBox::Yes)
            {
                selectionClear();
                itemDB->setRestorePoint();
                itemDB->deleteItems_withRestorePoint(itemsToDelete);
                slot_repaint();
            }
        }
        break;
    }

    event->accept();
}

void GLWidget::slot_set_cuttingplane_values_changed(qreal height, qreal depth)
{
    qDebug() << "GLWidget::slot_set_cuttingplane_values_changed";
    this->height_of_intersection.setZ(height);
    this->depth_of_view.setZ(depth);

    slot_repaint();
}

void GLWidget::paintGL()
{
    if (this->size().isNull())
        return;

    if (openGLTimerQuery->isResultAvailable())
    {
        rendertime_ns_per_frame = (quint64)openGLTimerQuery->waitForResult();
//        qDebug() << "rendertime [ns]:" << rendertime_ns_per_frame << "FPS:" << 1e9 / rendertime_ns_per_frame << "@" << QCursor::pos();
    }
    else
//        qDebug() << "render without time" << "@" << QCursor::pos();

        openGLTimerQuery->begin();

    matrix_modelview.setToIdentity();
    matrix_modelview.translate(translationOffset.x(), translationOffset.y(), 0.0);
    matrix_modelview.scale(this->zoomFactor, this->zoomFactor, 1.0 / 100000.0);
    updateMatrixAll();

//    shaderProgram = shaderProgram_lines;
//    shaderProgram->bind();

//    shaderProgram = shaderProgram_triangles;
//    shaderProgram->bind();
//    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);
//    setUseTexture(false);

    glClearColor(_backgroundColor.redF(), _backgroundColor.greenF(), _backgroundColor.blueF(), _backgroundColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(1,0);
    glPolygonOffset(0.0, 3.0);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);

    paintContent(itemDB->layers);   // After this: TRIANGLE SHADER IS ACTIVE!

    // Overlay
    shaderProgram = shaderProgram_overlay;
    shaderProgram->bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthRange(1,0);
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
    setUseTexture(true);

    // Bottom face
    texture_cube1->bind();
    setPaintingColor(QColor(50, 50, 255));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3f(-1, -1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3f( 1, -1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3f( 1,  1, -1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3f(-1,  1, -1);
    glEnd();
    texture_cube1->release();


    // Top face
    texture_cube2->bind();
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3f( 1, -1, 1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3f(-1, -1, 1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3f(-1,  1, 1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3f( 1,  1, 1);
    glEnd();
    texture_cube2->release();


    // Front face
    texture_cube3->bind();
    setPaintingColor(QColor(10, 110, 10));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i(-1,  1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i( 1,  1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i( 1,  1,  1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i(-1,  1,  1);
    glEnd();
    texture_cube3->release();


    // Back face
    texture_cube4->bind();
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i(-1, -1, -1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i( 1, -1, -1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i( 1, -1,  1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i(-1, -1,  1);
    glEnd();
    texture_cube4->release();


    // Left face
    texture_cube5->bind();
    setPaintingColor(QColor(150, 0, 0));
    glBegin(GL_QUADS);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i(-1, -1, -1);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i(-1,  1, -1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i(-1,  1,  1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i(-1, -1,  1);
    glEnd();
    texture_cube5->release();


    // Right face
    texture_cube6->bind();
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 1.0, 0.0);
    glVertex3i( 1, -1, -1);
    setTextureCoords(1.0, 1.0, 0.0);
    glVertex3i( 1,  1, -1);
    setTextureCoords(1.0, 0.0, 0.0);
    glVertex3i( 1,  1,  1);
    setTextureCoords(0.0, 0.0, 0.0);
    glVertex3i( 1, -1,  1);
    glEnd();
    texture_cube6->release();


    setUseTexture(false);
    glDisable(GL_DEPTH_TEST);

    // Set a matrix to the shader that does not rotate or scale, just transform to screen coordinate system
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_projection);

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

        // draw Arcball
        if(arcballShown)
        {
            QPointF lookAtScreenCoords = mapFromScene(lookAtPosition);
            setPaintingColor(QColor(255, 200, 0));
            glLineWidth(3.0);
            glBegin(GL_LINES);
            glVertex3f(lookAtScreenCoords.x() - 15, lookAtScreenCoords.y()     , 0);
            glVertex3f(lookAtScreenCoords.x() + 15, lookAtScreenCoords.y()     , 0);
            glVertex3f(lookAtScreenCoords.x()     , lookAtScreenCoords.y() - 15, 0);
            glVertex3f(lookAtScreenCoords.x()     , lookAtScreenCoords.y() + 15, 0);
            glEnd();

            glLineWidth(2.0);
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < 60; i++ )
            {
                glVertex3f(arcballRadius * qSin(i * PI / 30.0) + lookAtScreenCoords.x(), arcballRadius * qCos(i * PI / 30.0) + lookAtScreenCoords.y(), 0.0);
            }
            glEnd();
        }
    }


    QString infoText;
    QRect focusRect = QRect(0, 0, _snapIndicatorSize, _snapIndicatorSize);

    if ((this->itemGripModifier != NULL) && (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Move))
    {
        QString itemDescription = "[" + this->itemGripModifier->getItemDescription() + "]";
        QVector3D pos = this->itemGripModifier->getScenePosSource();
        QString itemPosition_from = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", pos.x(), pos.y(), pos.z());
        infoText = "Move " + itemDescription + itemPosition_from;
        if (snapMode != SnapNo)
            infoText += " to\n";
        focusRect.moveCenter(this->mousePos);
    }
    if ((this->itemGripModifier != NULL) && (this->itemGripModifier->getActiveGrip() == ItemGripModifier::Grip_Copy))
    {
        QString itemDescription = "[" + this->itemGripModifier->getItemDescription() + "]";
        QVector3D pos = this->itemGripModifier->getScenePosSource();
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
            infoText.append("Flange ");
            int flangeIndex = item_lastHighlight->snap_flanges.indexOf(snapPos_scene) + 1;
            infoText.append(QString().setNum(flangeIndex));
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
            break;
        }
        case SnapNo:
        {
            break;
        }
        }

        QString itemDescription = "[" + item_lastHighlight->description() + "]";
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

//    glFlush();

    openGLTimerQuery->end();
}

void GLWidget::slot_repaint()
{
    update();
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

void GLWidget::setVertex(QVector3D pos)
{
    shaderProgram->setAttributeValue(shader_vertexLocation, pos);
}

void GLWidget::setVertex(QPoint pos)
{
    glVertex2i((GLint)pos.x(), (GLint)pos.y());
}

void GLWidget::setPaintingColor(QColor color)
{
    if (selectItemsByColor)
    {
        color.setRed(  (glName >> 16) & 0xff);
        color.setGreen((glName >>  8) & 0xff);
        color.setBlue( (glName)       & 0xff);
        color.setAlpha(255);
        vertex_color = QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        shaderProgram->setAttributeValue(shader_colorLocation, vertex_color);
    }
    else
    {
        QVector4D vertex_color_new = QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        vertex_color = vertex_color_new;
        shaderProgram->setAttributeValue(shader_colorLocation, vertex_color);
    }
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

    QOpenGLTexture* texture = new QOpenGLTexture(textImage, QOpenGLTexture::DontGenerateMipMaps);
    texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->bind();
    setUseTexture(true);


    // Draw background
    setPaintingColor(Qt::transparent);
    glBegin(GL_QUADS);
    setTextureCoords(0.0, 0.001, 0.0);
    setVertex(boundingRect.bottomLeft());
    setTextureCoords(1.0, 0.001, 0.0);
    setVertex(boundingRect.bottomRight());
    setTextureCoords(1.0, 1.0, 0.0);
    setVertex(boundingRect.topRight());
    setTextureCoords(0.0, 1.0, 0.0);
    setVertex(boundingRect.topLeft());
    glEnd();
    setUseTexture(false);
    texture->release();
    delete texture;


    // Draw outline
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
    bool render_outline_shadow = render_outline;
    bool render_solid_shadow = render_solid;

    // ********** LINES SHADER ON ********
    shaderProgram = shaderProgram_lines;
    shaderProgram->bind();

    shaderProgram->setUniformValue(shader_useClippingXLocation, (GLint)0);   // Enable X-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingYLocation, (GLint)0);   // Enable Y-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingZLocation, (GLint)0);   // Enable Z-Clipping Plane
    shaderProgram->setUniformValue(shader_Height_of_intersection_location, this->height_of_intersection);
    shaderProgram->setUniformValue(shader_Depth_of_view_location,  this->height_of_intersection - this->depth_of_view);
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);
    this->glEnable(GL_PRIMITIVE_RESTART);
    this->glPrimitiveRestartIndex(0xABCD);

    glName = 1;

    if (render_outline)
    {
        render_solid = false;
        paintLayers(layers);
    }
    render_solid = render_solid_shadow;

    shaderProgram->setUniformValue(shader_useClippingXLocation, (GLint)0);   // Enable X-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingYLocation, (GLint)0);   // Enable Y-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingZLocation, (GLint)0);   // Enable Z-Clipping Plane

    setUseTexture(false);

    // ********** TRIANGLES SHADER ON ********
    shaderProgram = shaderProgram_triangles;
    shaderProgram->bind();

    shaderProgram->setUniformValue(shader_useClippingXLocation, (GLint)0);   // Enable X-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingYLocation, (GLint)0);   // Enable Y-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingZLocation, (GLint)1);   // Enable Z-Clipping Plane
    shaderProgram->setUniformValue(shader_Height_of_intersection_location, this->height_of_intersection);
    shaderProgram->setUniformValue(shader_Depth_of_view_location,  this->height_of_intersection - this->depth_of_view);
    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);
    this->glEnable(GL_PRIMITIVE_RESTART);
    this->glPrimitiveRestartIndex(0xABCD);

    setUseTexture(false);

    glName = 1;

    if (render_solid)
    {
        render_outline = false;
        paintLayers(layers);
    }
    render_outline = render_outline_shadow;

    shaderProgram->setUniformValue(shader_useClippingXLocation, (GLint)0);   // Enable X-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingYLocation, (GLint)0);   // Enable Y-Clipping Plane
    shaderProgram->setUniformValue(shader_useClippingZLocation, (GLint)0);   // Enable Z-Clipping Plane
}

void GLWidget::paintLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        if ((itemDB->layerSoloActive) && (!layer->solo))
            continue;

        if ((!layer->on) && (!layer->solo))
            continue;

        paintItems(layer->items, layer);
        paintLayers(layer->subLayers);
    }
}

void GLWidget::paintItems(QList<CADitem*> items, Layer* layer, bool checkBoundingBox, bool isSubItem)
{
    foreach (CADitem* item, items)
    {
        if(checkBoundingBox)
        {
            //Global culling performance test
            //Exclude all items from painting that do not reach the canvas with their boundingRect
            int screen_x_min = -this->width() / 2;
//            int screen_x_max =  this->width() / 2;
            int screen_y_min = -this->height() / 2;
//            int screen_y_max =  this->height() / 2;

            int p_x_min =  100000;
            int p_x_max = -100000;
            int p_y_min =  100000;
            int p_y_max = -100000;


            for (int i=0; i < 8; i++)
            {
                QVector3D boxPoint = item->boundingBox.p(i);
                QPointF screen_p = mapFromScene(boxPoint);

                if (screen_p.x() < p_x_min)     p_x_min = screen_p.x();
                if (screen_p.x() > p_x_max)     p_x_max = screen_p.x();
                if (screen_p.y() < p_y_min)     p_y_min = screen_p.y();
                if (screen_p.y() > p_y_max)     p_y_max = screen_p.y();
            }

            QRect screenRect;
            QRect itemRect;

            screenRect = QRect(screen_x_min, screen_y_min, this->width(), this->height());
            itemRect = QRect(p_x_min, p_y_min, (p_x_max - p_x_min), (p_y_max - p_y_min));


            if (!screenRect.intersects(itemRect))
            {
                if(!isSubItem)
                    glName++;
                continue;
            }
        }



        if(!isSubItem)
            glName++;

        item->index = glName;
        if (item->isMaintenanceArea)
        {
            bool renderSolidShadow = this->render_solid;
            this->render_solid = false;
            item->paint(this);
            this->render_solid = renderSolidShadow;
        }
        else
            item->paint(this);
        if (item->subItems.count() > 0)
            paintItems(item->subItems, layer, false, true);
    }
}

QList<CADitem*> GLWidget::itemsAtPosition_v2(QPoint pos, int size_x, int size_y)
{
    makeCurrent();

//    qDebug() << "highlight";

    if (fbo_select->size() != QSize(size_x, size_y))
    {
//        qDebug() << "fbo resize" << QSize(size_x, size_y);
        QOpenGLFramebufferObjectFormat format = fbo_select->format();
        delete fbo_select;
        fbo_select = new QOpenGLFramebufferObject(size_x, size_y, format);
    }

    matrix_glSelect.setToIdentity();
    matrix_glSelect.translate(-(qreal)(pos.x() + (this->width() - size_x) / 2), -(qreal)(pos.y() + (this->height() - size_y) / 2), 0.0);
    updateMatrixAll();
//    shaderProgram->setUniformValue(shader_matrixLocation, matrix_all);

    fbo_select->bind();

    glDepthFunc(GL_LEQUAL);
    glDepthRange(1,0);
    glDisable(GL_BLEND);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    selectItemsByColor = true;
    paintContent(itemDB->layers);
    selectItemsByColor = false;

    QImage image = fbo_select->toImage(false);

    fbo_select->release();


    matrix_glSelect.setToIdentity();
    updateMatrixAll();
    doneCurrent();

    QList<CADitem*> foundItems;
    QMap<quint32,quint32> itemsDistMap;

    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            QRgb pixel = image.pixel(x, y);
            if ((pixel & 0xffffff) != 0)
            {
                quint32 itemName;
                itemName = (quint32)pixel & 0xffffff;
                itemsDistMap.insertMulti(qAbs(size_x/2 - x) + qAbs(size_y/2 - y), itemName);
            }
        }
    }

    QList<quint32> processedNames;
    foreach(quint32 itemName, itemsDistMap.values())
    {
        if (!processedNames.contains(itemName))
        {
            CADitem* item = itemsAtPosition_processLayers(itemDB->layers, itemName);
            foundItems.append(item);
            processedNames.append(itemName);
        }
    }

    return foundItems;
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
    QList<CADitem*> itemList = this->itemsAtPosition_v2(pos, _cursorPickboxSize, _cursorPickboxSize);
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

void GLWidget::zoom_pan_showAll()
{
    M3dBoundingBox boundingBox_scene;
    boundingBox_scene.reset();
    zoom_pan_showAll_processLayers(itemDB->layers, &boundingBox_scene);

    QList<QVector3D> vertices = boundingBox_scene.getVertices();

    M3dBoundingBox boundingBox_screen;
    boundingBox_screen.reset();

    foreach (QVector3D vertex, vertices)
    {
        QPointF screenPoint = this->mapFromScene(vertex);
        boundingBox_screen.enterVertex(QVector3D(screenPoint));
    }


    // Set zoom to fit all items in screen
    qreal current_width  = boundingBox_screen.x_max - boundingBox_screen.x_min;
    qreal current_height = boundingBox_screen.y_max - boundingBox_screen.y_min;

    qreal zoom_x = (qreal)this->width()  / current_width;
    qreal zoom_y = (qreal)this->height() / current_height;

    qreal zoom_final = qMin(zoom_x, zoom_y) * 0.99;

    this->zoomFactor *= zoom_final;

    // Calculate new center after zooming
    matrix_modelview.setToIdentity();
    matrix_modelview.translate(translationOffset.x(), translationOffset.y(), 0.0);
    matrix_modelview.scale(this->zoomFactor, this->zoomFactor, 1.0 / 100000.0);
    updateMatrixAll();

    boundingBox_screen.reset();
    foreach (QVector3D vertex, vertices)
    {
        QPointF screenPoint = this->mapFromScene(vertex);
        boundingBox_screen.enterVertex(QVector3D(screenPoint));
    }

    // Move center of scene to center of screen
    QPointF center_scene = QPointF((boundingBox_screen.x_min + boundingBox_screen.x_max) / 2.0, (boundingBox_screen.y_min + boundingBox_screen.y_max) / 2.0);
    QPointF center_screen = QPointF(0.0, 0.0);
    QPointF center_delta = center_screen - center_scene;

    this->translationOffset += center_delta.toPoint();

}

void GLWidget::zoom_pan_showAll_processLayers(QList<Layer *> layers, M3dBoundingBox* boundingBox)
{
    foreach (Layer* layer, layers)
    {
        zoom_pan_showAll_processItems(layer->items, boundingBox);
        zoom_pan_showAll_processLayers(layer->subLayers, boundingBox);
    }
}

void GLWidget::zoom_pan_showAll_processItems(QList<CADitem *> items, M3dBoundingBox* boundingBox)
{
    foreach (CADitem* item, items)
    {
        boundingBox->enterVertices(item->boundingBox.getVertices());
    }
}

void GLWidget::initializeGL()
{
    qDebug() << "initializeGL()";

    initializeOpenGLFunctions();
    makeCurrent();

    openGLTimerQuery = new QOpenGLTimerQuery(this);
    openGLTimerQuery->create();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE | GL_EMISSION);
    //    glEnableClientState(GL_VERTEX_ARRAY);

    shader_1_vert = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!shader_1_vert->compileSourceFile(":/shaders/shader_1.vert"))
        QMessageBox::critical(this, "Shader compiler", "Vertex shader failed to compile!");

    shader_1_triangles_geom = new QOpenGLShader(QOpenGLShader::Geometry);
    if (!shader_1_triangles_geom->compileSourceFile(":/shaders/shader_1_triangles.geom"))
        QMessageBox::critical(this, "Shader compiler", "Triangle geometry shader failed to compile!");

    shader_1_lines_geom = new QOpenGLShader(QOpenGLShader::Geometry);
    if (!shader_1_lines_geom->compileSourceFile(":/shaders/shader_1_lines.geom"))
        QMessageBox::critical(this, "Shader compiler", "Lines geometry shader failed to compile!");

    shader_1_frag = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!shader_1_frag->compileSourceFile(":/shaders/shader_1.frag"))
        QMessageBox::critical(this, "Shader compiler", "Fragment shader failed to compile!");

    shader_2_vert = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!shader_2_vert->compileSourceFile(":/shaders/shader_2.vert"))
        QMessageBox::critical(this, "Shader compiler", "Vertex shader 2 failed to compile!");

    shader_2_frag = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!shader_2_frag->compileSourceFile(":/shaders/shader_2.frag"))
        QMessageBox::critical(this, "Shader compiler", "Fragment shader 2 failed to compile!");

    // ********** LINE SHADER **********
    shaderProgram_lines = new QOpenGLShaderProgram(this);
    shaderProgram_lines->addShader(shader_1_vert);
    shaderProgram_lines->addShader(shader_1_lines_geom);
    shaderProgram_lines->addShader(shader_1_frag);
    if (!shaderProgram_lines->link())
    {
        QMessageBox::critical(this, "Shader linker", QString("Line Shader failed to link!\n\n") + shaderProgram->log());
    }
    if (!shaderProgram_lines->bind())
    {
        QMessageBox::critical(this, "Shader program", "Line Shaderprogram could not be bound to gl context!");
    }

    // ********** TRIANGLE SHADER **********
    shaderProgram_triangles = new QOpenGLShaderProgram(this);
    shaderProgram_triangles->addShader(shader_1_vert);
    shaderProgram_triangles->addShader(shader_1_triangles_geom);
    shaderProgram_triangles->addShader(shader_1_frag);
    if (!shaderProgram_triangles->link())
    {
        QMessageBox::critical(this, "Shader linker", QString("Triangle Shader failed to link!\n\n") + shaderProgram->log());
    }
    if (!shaderProgram_triangles->bind())
    {
        QMessageBox::critical(this, "Shader program", "Triangle Shaderprogram could not be bound to gl context!");
    }

    // ********** OVERLAY SHADER **********
    shaderProgram_overlay = new QOpenGLShaderProgram(this);
    shaderProgram_overlay->addShader(shader_2_vert);
    shaderProgram_overlay->addShader(shader_2_frag);
    if (!shaderProgram_overlay->link())
    {
        QMessageBox::critical(this, "Shader linker", QString("Overlay Shader failed to link!\n\n") + shaderProgram->log());
    }
    if (!shaderProgram_overlay->bind())
    {
        QMessageBox::critical(this, "Shader program", "Overlay Shaderprogram could not be bound to gl context!");
    }


    shaderProgram = shaderProgram_triangles;


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

//    qDebug() << "vertex location" << shader_vertexLocation;
//    qDebug() << "matrix location" << shader_matrixLocation;
//    qDebug() << "color location" << shader_colorLocation;
//    qDebug() << "texture coord location" << shader_textureCoordLocation;
//    qDebug() << "use texture location" << shader_useTextureLocation;
//    qDebug() << "use clippingX location" << shader_useClippingXLocation;
//    qDebug() << "use clippingY location" << shader_useClippingYLocation;
//    qDebug() << "use clippingZ location" << shader_useClippingZLocation;
//    qDebug() << "depth of view location" << shader_Depth_of_view_location;
//    qDebug() << "height of intersection location" << shader_Height_of_intersection_location;

    texture_cube1 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube1->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube1->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture_cube2 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube2->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube2->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture_cube3 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube3->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube3->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture_cube4 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube4->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube4->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture_cube5 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube5->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube5->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture_cube6 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture_cube6->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    texture_cube6->setWrapMode(QOpenGLTexture::ClampToEdge);

    QImage textImage(80, 80, QImage::Format_ARGB32);
    QPainter painter(&textImage);
    painter.setPen(Qt::white);
    QFont font_big;
    font_big.setPixelSize(25);
    QFont font_small;
    font_small.setPixelSize(12);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Z+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking up");
    texture_cube1->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Z-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking down");
    texture_cube2->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Y-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking south");
    texture_cube3->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "Y+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking north");
    texture_cube4->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "X+");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking east");
    texture_cube5->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    textImage.fill(Qt::black);
    painter.setFont(font_big);
    painter.drawText(textImage.rect(), Qt::AlignCenter, "X-");
    painter.setFont(font_small);
    painter.drawText(textImage.rect(), Qt::AlignHCenter | Qt::AlignBottom, "looking west");
    texture_cube6->setData(textImage, QOpenGLTexture::DontGenerateMipMaps);

    painter.end();

    QOpenGLFramebufferObjectFormat format;
    format.setSamples(1);
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setInternalTextureFormat(GL_RGBA);
    format.setMipmap(false);
    format.setTextureTarget(GL_TEXTURE_2D);

    fbo_select = new QOpenGLFramebufferObject(_cursorPickboxSize, _cursorPickboxSize, format);

    format.setSamples(8);
    fbo_renderImage = new QOpenGLFramebufferObject(this->width(), this->height(), format);
}

    void GLWidget::resizeGL(int w, int h)
    {
        displayCenter = QPoint(w, h) / 2;

        matrix_projection.setToIdentity();
        matrix_projection.scale(2.0 / (qreal)w, 2.0 / (qreal)h, 1.0);
        matrix_projection.translate(-0.5, -0.5);

        updateMatrixAll();
        slot_repaint();
    }
