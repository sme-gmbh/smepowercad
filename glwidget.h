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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <QOpenGLPaintDevice>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLTimerQuery>

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QMessageBox>
#include <QFont>
#include <QFontMetrics>
#include <QSettings>
#include <QTimer>
#include <QColor>
#include <QRgb>
#include <QImage>
#include <QPainter>
#include <QFileDialog>

#include <qmath.h>

#include "itemdb.h"
#include "itemwizard.h"
#include "itemgripmodifier.h"


class GLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_3_Compatibility
{
    Q_OBJECT
public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};


    explicit GLWidget(QWidget *parent, ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier* itemGripModifier);
    ~GLWidget();

    QPointF mapFromScene(QVector3D &scenePoint);

    // Overlay
    void moveCursor(QPoint pos);
    void hideCursor();
    void pickStart();
    void pickEnd();
    bool isPickActive();
    QRect selection();
    Qt::ItemSelectionMode selectionMode();

    enum SnapMode{SnapBasepoint, SnapFlange, SnapEndpoint, SnapCenter, SnapNo};
    void snap_enable(bool on);
    void set_snap_mode(SnapMode mode);
    void set_snapPos(QVector3D snapPos_screen);
    void set_WorldRotation(float rot_x, float rot_y, float rot_z);
    QMatrix4x4 getMatrix_all();

    QImage render_image(int size_x, int size_y, QMatrix4x4 matrix_modelview, QMatrix4x4 matrix_rotation);


    QStringList getOpenGLinfo();

private:
// Qt 5
//    QOpenGLContext *m_context;
//    QOpenGLPaintDevice *m_device;
    ItemDB* itemDB;
    ItemWizard *itemWizard;
    ItemGripModifier* itemGripModifier;
    QSettings settings;
    QPoint mousePosOld;
    CuttingPlane cuttingplane;
    QVector3D height_of_intersection;
    QVector3D depth_of_view;
//    QList<Layer*> layers;
    QPoint translationOffset;
    QVector3D centerOfViewInScene;  // in coordsOnScene
    QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget
    QVector3D cameraPosition;
    QVector3D lookAtPosition;

    QVector3D centerOfRotationSphere;
    QVector3D rotationStart;

    QPoint arcballPosOld;
    QMatrix4x4 matrix_arcball;
    qreal arcballRadius;

    QMap<GLuint, CADitem*> glNameMap;
    quint32 glName;

    // Overlay
    QPoint mousePos;
    QPoint pickStartPos;
    bool pickActive;
    bool cursorShown;
    bool arcballShown;
    SnapMode snapMode;
    QPoint snapPos_screen;
    QVector3D snapPos_scene;

    CADitem* item_lastHighlight;
    QList<CADitem*> selection_itemList;

signals:
    void signal_highlightItem(CADitem* item);
    void signal_snapFired(QVector3D snapPos_scene, int snapMode);

public slots:
    void slot_highlightItem(CADitem* item);
    void slot_snapTo(QVector3D snapPos_scene, int snapMode);
    void slot_changeSelection(QList<CADitem *> selectedItems);
    void slot_itemDeleted(CADitem* item);


private:
    // **** settings ****
    QColor _backgroundColor;

    int _cursorSize;
    int _cursorWidth;
    int _cursorPickboxSize;
    int _cursorPickboxLineWidth;
    QColor _cursorPickboxColor;

    int _snapIndicatorSize;

    int _pickboxOutlineWidth;
    QColor _pickboxOutlineColorLeft;
    QColor _pickboxOutlineColorRight;
    QColor _pickboxFillColorLeft;
    QColor _pickboxFillColorRight;


    // Drawing
    // OpenGL
public:
    QOpenGLShaderProgram* shaderProgram;    // the current one
    QOpenGLShaderProgram* shaderProgram_lines;
    QOpenGLShaderProgram* shaderProgram_triangles;
    QOpenGLShaderProgram* shaderProgram_overlay;
    QOpenGLShader* shader_1_vert;
    QOpenGLShader* shader_1_triangles_geom;
    QOpenGLShader* shader_1_lines_geom;
    QOpenGLShader* shader_1_frag;
    QOpenGLShader* shader_2_vert;
    QOpenGLShader* shader_2_frag;
    QOpenGLTexture* texture_cube1;
    QOpenGLTexture* texture_cube2;
    QOpenGLTexture* texture_cube3;
    QOpenGLTexture* texture_cube4;
    QOpenGLTexture* texture_cube5;
    QOpenGLTexture* texture_cube6;
    int shader_vertexLocation;
    int shader_matrixLocation;
    int shader_colorLocation;
    int shader_textureCoordLocation;
    int shader_textureSamplerLocation;
    int shader_useTextureLocation;
    int shader_useClippingXLocation;
    int shader_useClippingYLocation;
    int shader_useClippingZLocation;
    int shader_Depth_of_view_location;
    int shader_Height_of_intersection_location;

    qreal zoomFactor;
    bool render_solid;
    bool render_outline;
    bool render_maintenance_area;


private:
    QOpenGLTimerQuery* openGLTimerQuery;
    quint64 rendertime_ns_per_frame;
    QVector4D vertex_color;
    QVector3D vertex_position;
    QMatrix4x4 matrix_projection;
    QMatrix4x4 matrix_modelview;
    QMatrix4x4 matrix_rotation;
    QMatrix4x4 matrix_rotation_old;
    QMatrix4x4 matrix_glSelect;
    QMatrix4x4 matrix_all;

public:
    void setVertex(QVector3D pos);
    void setVertex(QPoint pos);
    void setPaintingColor(QColor color);
    void setTextureCoords(QPoint coord);
    void setTextureCoords(qreal x, qreal y, qreal z);
    void setUseTexture(bool on);

private:
    void paintContent(QList<Layer*> layers);
    void paintLayers(QList<Layer*> layers);
    void paintItems(QList<CADitem *> items, Layer *layer, bool checkBoundingBox = true, bool isSubItem = false);

    void updateArcball(int steps);
    QVector3D getArcBallVector(int x, int y);
    QVector3D pointOnSphere( QPoint pointOnScreen );

    void updateMatrixAll();

    bool selectItemsByColor;

    typedef enum {
        topLeft,
        topRight,
        bottomLeft,
        bottomRight
    } BoxVertex;


    void paintTextInfoBox(QPoint pos, QString text, BoxVertex anchor, QFont font = QFont(), QColor colorText = QColor(255, 255, 30, 255), QColor colorBackground = QColor(0, 0, 0, 230), QColor colorOutline = QColor(200, 200, 200, 150));

    QOpenGLFramebufferObject* fbo_select;
    QOpenGLFramebufferObject* fbo_renderImage;
    QList<CADitem *> itemsAtPosition_v2(QPoint pos, int size_x, int size_y);
    CADitem *itemsAtPosition_processLayers(QList<Layer*> layers, GLuint glName);
    CADitem *itemsAtPosition_processItems(QList<CADitem*> items, GLuint glName);
    void highlightItemAtPosition(QPoint pos);
    void highlightItems(QList<CADitem*> items);
    void highlightClear();
    void highlightClear_processLayers(QList<Layer*> layers);
    void highlightClear_processItems(QList<CADitem*> items);

// Selection
    void selectionAddItem(CADitem* item);
    void selectionAddItems(QList<CADitem*> items);
    void selectionAddSubItems(QList<CADitem*> items);
    void selectionRemoveItem(CADitem* item);
    void selectionRemoveSubItems(QList<CADitem*> items);
    void selectionClear();
    void selectionClear_processLayers(QList<Layer*> layers);
    void selectionClear_processItems(QList<CADitem*> items);

signals:
    void signal_selectionChanged(QList<CADitem*> items);

// General event handlers
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

signals:
    void signal_mouseMoved(QVector3D coords);
    void signal_matrix_rotation_changed(QMatrix4x4 matrix_rotation);

public slots:
    void slot_repaint();
    void slot_wireframe(bool on);
    void slot_solid(bool on);
    void slot_mouse3Dmoved(int x, int y, int z, int a, int b, int c);

    void slot_update_settings();
    void slot_set_cuttingplane_values_changed(qreal height, qreal depth);

private slots:

};

#endif // GLWIDGET_H
