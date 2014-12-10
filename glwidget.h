#ifndef GLWIDGET_H
#define GLWIDGET_H

// Qt 4
#include <QGLWidget>
#include <QGLFramebufferObject>
#include <QGLShader>
#include <QGLShaderProgram>

// Qt 5
//#include <QtOpenGL/QGLWidget>
//#include <QOpenGLContext>
//#include <QOpenGLFramebufferObject>
//#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_4_3_Compatibility>
//#include <QOpenGLPaintDevice>
//#include <QOpenGLShader>
//#include <QOpenGLShaderProgram>

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

#include <qmath.h>
#include <qgl.h>
#include <GL/glu.h>

#include "itemdb.h"
#include "itemwizard.h"
#include "itemgripmodifier.h"
#include "snapengine.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};


    explicit GLWidget(QWidget *parent, ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier* itemGripModifier, QGLFormat glFormat);
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
    qreal zoomFactor;
    QVector3D centerOfViewInScene;  // in coordsOnScene
    QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget
    QVector3D cameraPosition;
    QVector3D lookAtPosition;

    QVector3D centerOfRotationSphere;
    QVector3D rotationStart;
//    float rot_x, rot_y, rot_z;

    QPoint arcballPosOld;
    QMatrix4x4 matrix_arcball;
    qreal arcballRadius;


//    QMatrix4x4 matrix_projection;


    bool render_solid;
    bool render_outline;

    GLuint tile_list;

    QMap<GLuint, CADitem*> glNameMap;
    quint32 glName;
    QTimer timer_findItemAtPosition;

    // Overlay
    QPoint mousePos;
    QPoint pickStartPos;
    bool pickActive;
    bool cursorShown;
    bool arcballShown;
    SnapMode snapMode;
//    int snapIndex;      // The index of the current snap in the snap liest of the object currently highlighted
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
// Qt 4
    QGLShaderProgram* shaderProgram;
    QGLShader* shader_1_frag;
    QGLShader* shader_1_vert;
// Qt 5
//    QOpenGLShaderProgram* shaderProgram;
//    QOpenGLShader* shader_1_frag;
//    QOpenGLShader* shader_1_vert;
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

    QVector4D vertex_color;
    QVector3D vertex_position;
    QMatrix4x4 matrix_projection;
    QMatrix4x4 matrix_modelview;
    QMatrix4x4 matrix_rotation;
    QMatrix4x4 matrix_rotation_old;
    QMatrix4x4 matrix_glSelect;
    QMatrix4x4 matrix_all;

    void saveGLState();
    void restoreGLState();

    void setVertex(QVector3D pos);
    void setVertex(QPoint pos);
    void setPaintingColor(QColor color);
    void setTextureCoords(QPoint coord);
    void setTextureCoords(qreal x, qreal y, qreal z);
    void setUseTexture(bool on);
    void paintContent(QList<Layer*> layers);
    void paintItems(QList<CADitem *> items, Layer *layer, bool checkBoundingBox = true, bool isSubItem = false);

    void updateArcball(int steps);
    QVector3D getArcBallVector(int x, int y);
    QVector3D pointOnSphere( QPoint pointOnScreen );

    void updateMatrixAll();

    QColor getColorPen(CADitem* item, Layer *layer);
    QColor getColorBrush(CADitem* item, Layer *layer);

    typedef enum {
        topLeft,
        topRight,
        bottomLeft,
        bottomRight
    } BoxVertex;


    void paintTextInfoBox(QPoint pos, QString text, BoxVertex anchor, QFont font = QFont(), QColor colorText = QColor(255, 255, 30, 255), QColor colorBackground = QColor(0, 0, 0, 230), QColor colorOutline = QColor(200, 200, 200, 150));

    void paintBasicLine(Layer* layer, CAD_basic_line* item);
    void paintBasicPolyLine(Layer *layer, CAD_basic_polyline *item);
    void paintBasicPlane(Layer *layer, CAD_basic_plane *item);
    void paintBasicFace(Layer *layer, CAD_basic_3Dface *item);
    void paintBasicCircle(Layer *layer, CAD_basic_circle *item);
    void paintBasicBox(Layer *layer, CAD_basic_box *item);
    void paintBasicCylinder(Layer *layer, CAD_basic_cylinder *item);
    void paintBasicPipe(Layer *layer, CAD_basic_pipe *item);
    void paintBasicTurn(Layer *layer, CAD_basic_turn *item);
    void paintBasicSphere(Layer *layer, CAD_basic_sphere *item);
    void paintBasicArc(Layer *layer, CAD_basic_arc *item);
    void paintBasicDuct(Layer *layer, CAD_basic_duct *item);

    void paintArchBlockOut(Layer *layer, CAD_arch_blockOut *item);
    void paintArchBoredPile(Layer *layer, CAD_arch_boredPile *item);
    void paintArchDoor(Layer *layer, CAD_arch_door *item);
    void paintArchLevelSlab(Layer *layer, CAD_arch_levelSlab *item);
    void paintArchWallLoadBearing(Layer *layer, CAD_arch_wall_loadBearing *item);
    void paintArchWallNonLoadBearing(Layer *layer, CAD_arch_wall_nonLoadBearing *item);
    void paintArchWindow(Layer *layer, CAD_arch_window *item);

    void paintAirDuct(Layer *layer, CAD_air_duct *item);
    void paintAirPipe(Layer *layer, CAD_air_pipe *item);
    void paintAirDuctTurn(Layer *layer, CAD_air_ductTurn *item);
    void paintAirPipeTurn(Layer *layer, CAD_air_pipeTurn *item);
    void paintAirPipeReducer(Layer *layer, CAD_air_pipeReducer *item);
    void paintAirPipeTeeConnector(Layer *layer, CAD_air_pipeTeeConnector *item);
    void paintAirDuctTeeConnector(Layer *layer, CAD_air_ductTeeConnector *item);
    void paintAirDuctTransiton(Layer *layer, CAD_air_ductTransition *item);
    void paintAirDuctTransitionRectRound(Layer *layer, CAD_air_ductTransitionRectRound *item);
    void paintAirDuctYpiece(Layer *layer, CAD_air_ductYpiece *item);
    void paintAirDuctEndPlate(Layer *layer, CAD_air_ductEndPlate *item);
    void paintAirPipeEndCap(Layer *layer, CAD_air_pipeEndCap *item);
    void paintAirThrottleValve(Layer *layer, CAD_air_throttleValve *item);
    void paintAirMultiLeafDamper(Layer *layer, CAD_air_multiLeafDamper *item);
    void paintAirPressureReliefDamper(Layer *layer, CAD_air_pressureReliefDamper *item);
    void paintAirPipeFireDamper(Layer *layer, CAD_air_pipeFireDamper *item);
    void paintAirDuctFireDamper(Layer *layer, CAD_air_ductFireDamper *item);
    void paintAirDuctVolumetricFlowController(Layer *layer, CAD_air_ductVolumetricFlowController *item);
    void paintAirPipeVolumetricFlowController(Layer *layer, CAD_air_pipeVolumetricFlowController *item);
    void paintAirHeatExchangerWaterAir(Layer *layer, CAD_air_heatExchangerWaterAir *item);
    void paintAirHeatExchangerAirAir(Layer *layer, CAD_air_heatExchangerAirAir *item);
    void paintAirCanvasFlange(Layer *layer, CAD_air_canvasFlange *item);
    void paintAirFilter(Layer *layer, CAD_air_filter *item);
    void paintAirPipeSilencer(Layer *layer, CAD_air_pipeSilencer *item);
    void paintAirDuctBaffleSilencer(Layer *layer, CAD_air_ductBaffleSilencer *item);
    void paintAirFan(Layer *layer, CAD_air_fan *item);
    void paintAirHumidifier(Layer *layer, CAD_air_humidifier *item);
    void paintAirEmptyCabinet(Layer *layer, CAD_air_emptyCabinet *item);
    void paintAirEquipmentFrame(Layer *layer, CAD_air_equipmentFrame *item);

    void paintHeatCoolAdjustvalve(Layer *layer, CAD_heatcool_adjustvalve *item);
    void paintHeatCoolChiller(Layer *layer, CAD_heatcool_chiller *item);
    void paintHeatCoolControlvalve(Layer *layer, CAD_heatcool_controlvalve *item);
    void paintHeatCoolCoolingTower(Layer *layer, CAD_heatcool_coolingTower *item);
    void paintHeatCoolHeatExchanger(Layer *layer, CAD_heatcool_heatExchanger *item);
    void paintHeatCoolPipe(Layer *layer, CAD_heatcool_pipe *item);
    void paintHeatCoolPump(Layer *layer, CAD_heatcool_pump *item);
    void paintHeatCoolSensor(Layer *layer, CAD_heatcool_sensor *item);
    void paintHeatCoolPipeTurn(Layer *layer, CAD_heatcool_pipeTurn *item);
    void paintHeatCoolPipeReducer(Layer *layer, CAD_heatcool_pipeReducer *item);
    void paintHeatCoolPipeTeeConnector(Layer *layer, CAD_heatcool_pipeTeeConnector *item);
    void paintHeatCoolPipeEndCap(Layer *layer, CAD_heatcool_pipeEndCap *item);
    void paintHeatCoolFlange(Layer *layer, CAD_heatcool_flange *item);
    void paintHeatCoolExpansionChamber(Layer *layer, CAD_heatcool_expansionChamber *item);
    void paintHeatCoolBoiler(Layer *layer, CAD_heatcool_boiler *item);
    void paintHeatCoolWaterHeater(Layer *layer, CAD_heatcool_waterHeater *item);
    void paintHeatCoolStorageBoiler(Layer *layer, CAD_heatcool_storageBoiler *item);
    void paintHeatCoolRadiator(Layer *layer, CAD_heatcool_radiator *item);
    void paintHeatCoolFilter(Layer *layer, CAD_heatcool_filter *item);
    void paintHeatCoolBallValve(Layer *layer, CAD_heatcool_ballValve *item);
    void paintHeatCoolButterflyValve(Layer *layer, CAD_heatcool_butterflyValve *item);
    void paintHeatCoolSafteyValve(Layer *layer, CAD_heatcool_safetyValve *item);
    void paintHeatCoolFlowmeter(Layer *layer, CAD_heatcool_flowmeter *item);

    void paintSprinklerCompressedAirWaterContainer(Layer *layer, CAD_sprinkler_compressedAirWaterContainer *item);
    void paintSprinklerDistribution(Layer *layer, CAD_sprinkler_distribution *item);
    void paintSprinklerHead(Layer *layer, CAD_sprinkler_head *item);
    void paintSprinklerPipe(Layer *layer, CAD_sprinkler_pipe *item);
    void paintSprinklerPump(Layer *layer, CAD_sprinkler_pump *item);
    void paintSprinklerTeeConnector(Layer *layer, CAD_sprinkler_teeConnector *item);
    void paintSprinklerValve(Layer *layer, CAD_sprinkler_valve *item);
    void paintSprinklerWetAlarmValve(Layer *layer, CAD_sprinkler_wetAlarmValve *item);
    void paintSprinklerZoneCheck(Layer *layer, CAD_sprinkler_zoneCheck *item);
    void paintSprinklerPipeTurn(Layer *layer, CAD_sprinkler_pipeTurn *item);
    void paintSprinklerPipeEndCap(Layer *layer, CAD_sprinkler_pipeEndCap *item);
    void paintSprinklerPipeReducer(Layer *layer, CAD_sprinkler_pipeReducer *item);

    void paintElectricalCabinet(Layer *layer, CAD_electrical_cabinet *item);
    void paintElectricalCabletray(Layer *layer, CAD_electrical_cableTray *item);

    void paintSanitaryPipe(Layer *layer, CAD_sanitary_pipe *item);
    void paintSanitaryPipeTurn(Layer *layer, CAD_sanitary_pipeTurn *item);
    void paintSanitaryPipeReducer(Layer *layer, CAD_sanitary_pipeReducer *item);
    void paintSanitaryPipeTeeConnector(Layer *layer, CAD_sanitary_pipeTeeConnector *item);
    void paintSanitaryPipeEndCap(Layer *layer, CAD_sanitary_pipeEndCap *item);
    void paintSanitaryFlange(Layer *layer, CAD_sanitary_flange *item);
    void paintSanitaryElectricWaterHeater(Layer *layer, CAD_sanitary_electricWaterHeater *item);
    void paintSanitaryWashBasin(Layer *layer, CAD_sanitary_washBasin *item);
    void paintSanitarySink(Layer *layer, CAD_sanitary_sink *item);
    void paintSanitaryShower(Layer *layer, CAD_sanitary_shower *item);
    void paintSanitaryEmergencyShower(Layer *layer, CAD_sanitary_emergencyShower *item);
    void paintSanitaryEmergencyEyeShower(Layer *layer, CAD_sanitary_emergencyEyeShower *item);
    void paintSanitaryLiftingUnit(Layer *layer, CAD_sanitary_liftingUnit *item);

    QList<CADitem *> itemsAtPosition(QPoint pos, int size_x, int size_y);
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
    virtual void resizeGL();
    virtual void paintGL();

    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

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
    void slot_timer_findItemAtPosition_triggered();
};

#endif // GLWIDGET_H
