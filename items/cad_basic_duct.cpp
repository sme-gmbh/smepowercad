#include "cad_basic_duct.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_duct::CAD_basic_duct() : CADitem(CADitemTypes::Basic_Duct)
{
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));


    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(10.0));
    wizardParams.insert("b", QVariant::fromValue(5.0));
    wizardParams.insert("a", QVariant::fromValue(5.0));

    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices.create();
    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces.create();
    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines.create();
    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_basic_duct::~CAD_basic_duct()
{
    arrayBufVertices.destroy();
    indexBufFaces.destroy();
    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_basic_duct::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Basic_Duct);
    return flangable_items;
}

QImage CAD_basic_duct::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_duct::iconPath()
{
    return ":/icons/cad_basic/cad_basic_duct.svg";
}

QString CAD_basic_duct::domain()
{
    return "Basic";
}

QString CAD_basic_duct::description()
{
    return "Basic|Duct";
}

void CAD_basic_duct::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.translate(position);
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    matrix_rotation.translate(-position);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pos_bot_1 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);
    pos_bot_4 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + position);

    pos_top_1 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);
    pos_top_4 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + position);

    inner_pos_bot_1 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0, 1.0, 1.0) * s + position);
    inner_pos_bot_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0, 1.0, 1.0) * s + position);
    inner_pos_bot_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0,-1.0, 1.0) * s + position);
    inner_pos_bot_4 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z() + QVector3D(0.0,-1.0, 1.0) * s + position);

    inner_pos_top_1 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0, 1.0,-1.0) * s + position);
    inner_pos_top_2 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0, 1.0,-1.0) * s + position);
    inner_pos_top_3 = matrix_rotation * (QVector3D( 0.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0,-1.0,-1.0) * s + position);
    inner_pos_top_4 = matrix_rotation * (QVector3D( 1.0, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z() + QVector3D(0.0,-1.0,-1.0) * s + position);



    boundingBox.enterVertex(pos_bot_1);
    boundingBox.enterVertex(pos_bot_2);
    boundingBox.enterVertex(pos_bot_3);
    boundingBox.enterVertex(pos_bot_4);
    boundingBox.enterVertex(pos_top_1);
    boundingBox.enterVertex(pos_top_2);
    boundingBox.enterVertex(pos_top_3);
    boundingBox.enterVertex(pos_top_4);

    this->snap_flanges.append((pos_bot_2 + pos_top_3) / 2);
    this->snap_flanges.append((pos_bot_1 + pos_top_4) / 2);

    this->snap_vertices.append(pos_top_1);
    this->snap_vertices.append(pos_top_2);
    this->snap_vertices.append(pos_top_3);
    this->snap_vertices.append(pos_top_4);
    this->snap_vertices.append(pos_bot_1);
    this->snap_vertices.append(pos_bot_2);
    this->snap_vertices.append(pos_bot_3);
    this->snap_vertices.append(pos_bot_4);

    QVector3D vertices[] = {
        pos_bot_1, //0
        pos_bot_2,
        pos_bot_3,
        pos_bot_4,
        pos_top_1, //4
        pos_top_2,
        pos_top_3,
        pos_top_4,
        inner_pos_bot_1, //8
        inner_pos_bot_2,
        inner_pos_bot_3,
        inner_pos_bot_4,
        inner_pos_top_1, //12
        inner_pos_top_2,
        inner_pos_top_3,
        inner_pos_top_4
    };

    static GLushort indicesFaces[] = {
        0, 1,  4,  5,  7,  6,  3,  2,  0, 1, 0xABCD,
        8, 9,  12, 13, 15, 14, 11, 10, 8, 9, 0xABCD,
        1, 2,  9,  10, 0xABCD,
        2, 10, 6,  14, 0xABCD,
        6, 14, 5,  13, 0xABCD,
        5, 13, 1,  9, 0xABCD,
        0, 8,  3,  11, 0xABCD,
        3, 11, 7,  15, 0xABCD,
        7, 15, 4,  12,0xABCD,
        4, 12, 0,  8
    };

    static GLushort indicesLines[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7,

        8, 9,
        9, 10,
        10, 11,
        11, 8,
        12, 13,
        13, 14,
        14, 15,
        15, 12,
        8, 12,
        9, 13,
        10, 14,
        11, 15
    };


    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));
}

void CAD_basic_duct::processWizardInput()
{

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    size.setX(wizardParams.value("l").toDouble());
    size.setY(wizardParams.value("b").toDouble());
    size.setZ(wizardParams.value("a").toDouble());


    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

//void CAD_basic_duct::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush);
//        glwidget->glBegin(GL_QUADS);

//        // Bottom face
//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());

//        // Top face
//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());

//        // Side Faces
//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());

//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());

//        // Inner Bottom Face
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());

//        // Inner Top Face
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());

//        // Inner Side Faces
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());

//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());

//        // Front faces
//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());

//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());

//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());

//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());

//        // Back faces
//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());

//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());

//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());

//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());

//        glwidget->glEnd();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen);
//        glwidget->glLineWidth(1.0);

//        glwidget->glBegin(GL_LINE_LOOP);

//        // Bottom face
//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        // Top face
//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        // Side Faces
//        glwidget->glVertex3f((GLfloat)pos_bot_1.x(), (GLfloat)pos_bot_1.y(), (GLfloat)pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_2.x(), (GLfloat)pos_bot_2.y(), (GLfloat)pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_2.x(), (GLfloat)pos_top_2.y(), (GLfloat)pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)pos_top_1.x(), (GLfloat)pos_top_1.y(), (GLfloat)pos_top_1.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)pos_bot_3.x(), (GLfloat)pos_bot_3.y(), (GLfloat)pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)pos_bot_4.x(), (GLfloat)pos_bot_4.y(), (GLfloat)pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)pos_top_4.x(), (GLfloat)pos_top_4.y(), (GLfloat)pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)pos_top_3.x(), (GLfloat)pos_top_3.y(), (GLfloat)pos_top_3.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        // Inner Bottom Face
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        // Inner Top Face
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        // Inner Side Faces
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_1.x(), (GLfloat)inner_pos_bot_1.y(), (GLfloat)inner_pos_bot_1.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_2.x(), (GLfloat)inner_pos_bot_2.y(), (GLfloat)inner_pos_bot_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_2.x(), (GLfloat)inner_pos_top_2.y(), (GLfloat)inner_pos_top_2.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_1.x(), (GLfloat)inner_pos_top_1.y(), (GLfloat)inner_pos_top_1.z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_3.x(), (GLfloat)inner_pos_bot_3.y(), (GLfloat)inner_pos_bot_3.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_bot_4.x(), (GLfloat)inner_pos_bot_4.y(), (GLfloat)inner_pos_bot_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_4.x(), (GLfloat)inner_pos_top_4.y(), (GLfloat)inner_pos_top_4.z());
//        glwidget->glVertex3f((GLfloat)inner_pos_top_3.x(), (GLfloat)inner_pos_top_3.y(), (GLfloat)inner_pos_top_3.z());
//        glwidget->glEnd();
//    }
//}

void CAD_basic_duct::paint(GLWidget *glwidget)
{
    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);
    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    arrayBufVertices.bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces.bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces.release();
    }

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen_tmp);
        glwidget->glLineWidth(1.0);

        indexBufLines.bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines.release();
    }

    arrayBufVertices.release();
}

