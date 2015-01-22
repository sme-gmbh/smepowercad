#include "cad_basic_cylinder.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_cylinder::CAD_basic_cylinder() : CADitem(CADitemTypes::Basic_Cylinder)
{
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Radius", QVariant::fromValue(500.0));
    wizardParams.insert("Height", QVariant::fromValue(1000.0));

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

CAD_basic_cylinder::~CAD_basic_cylinder()
{
    arrayBufVertices.destroy();
    indexBufFaces.destroy();
    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_basic_cylinder::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_cylinder::wizardImage()
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

QString CAD_basic_cylinder::iconPath()
{
    return ":/icons/cad_basic/cad_basic_cylinder.svg";
}

QString CAD_basic_cylinder::domain()
{
    return "Basic";
}

QString CAD_basic_cylinder::description()
{
    return "Basic|Cylinder";
}

void CAD_basic_cylinder::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    this->snap_basepoint = this->center_base;
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,0.0, this->height));

    QVector3D vertices[100];
    int index = 0;
    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;
        linePos = center_base;

        linePos += matrix_rotation * QVector3D(sin(angle) * radius, cos(angle) * radius, 0.0);
        vertices[index] = linePos;
        index = index + 1;
        boundingBox.enterVertex(linePos);
        linePos += matrix_rotation * QVector3D(0.0,0.0, this->height);
        vertices[index] = linePos;
        index = index + 1;
        boundingBox.enterVertex(linePos);

    }



    static GLushort indicesFaces[102];
    for(int i = 0; i < 100; i++)
        indicesFaces[i] = i;
    indicesFaces[100] = 0;
    indicesFaces[101] = 1;

    static GLushort indicesLines[300];
    for(int i = 0; i < 100; i++)
    {
        indicesLines[2*i] = i;
        indicesLines[2*i+1] = i + 2;
    }
    indicesLines[199] = 1;
    indicesLines[198] = 99;
    indicesLines[197] = 0;
    indicesLines[196] = 98;
    for(int i = 0; i < 100; i++)
    {
        indicesLines[200+i] = i;
    }

    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));
}

void CAD_basic_cylinder::processWizardInput()
{
    center_base.setX(wizardParams.value("Position x").toDouble());
    center_base.setY(wizardParams.value("Position y").toDouble());
    center_base.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    radius = wizardParams.value("Radius").toDouble();
    height = wizardParams.value("Height").toDouble();

}

//void CAD_basic_cylinder::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    if (glwidget->render_solid)
//    {
//        int count = vertices_bottom.count();
//        // Vertical connections (faces)
//        glwidget->setPaintingColor(color_brush_tmp);
//        glwidget->glBegin(GL_QUADS);
//        QVector3D last_vertex_bottom = vertices_bottom.at(count - 1);
//        QVector3D last_vertex_top = vertices_top.at(count - 1);
//        for (int i = 0; i < count; i++)
//        {
//            QVector3D vertex_bottom = vertices_bottom.at(i);
//            QVector3D vertex_top = vertices_top.at(i);
//            glwidget->glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
//            glwidget->glVertex3f((GLfloat)last_vertex_top.x(), (GLfloat)last_vertex_top.y(), (GLfloat)last_vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_top.x(), (GLfloat)vertex_top.y(), (GLfloat)vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//            last_vertex_bottom = vertex_bottom;
//            last_vertex_top = vertex_top;
//        }

//        glEnd();
//    }

//    if (glwidget->render_outline)
//    {
//        int count = vertices_bottom.count();
//        glwidget->setPaintingColor(color_pen_tmp);
//        glwidget->glLineWidth(1.0);
//        // Bottom circle
//        glwidget->glBegin(GL_LINE_LOOP);
//        foreach (QVector3D linePos, vertices_bottom)
//        {
//            glwidget->glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//        }
//        glwidget->glEnd();

//        // Top circle
//        glwidget->glBegin(GL_LINE_LOOP);
//        foreach (QVector3D linePos, vertices_top)    // 50 edges
//        {
//            glwidget->glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//        }
//        glwidget->glEnd();

//        // Vertical connections
//        glwidget->glBegin(GL_LINES);
//        for (int i = 0; i < count; i++)
//        {
//            glwidget->glVertex3f((GLfloat)vertices_bottom.at(i).x(), (GLfloat)vertices_bottom.at(i).y(), (GLfloat)vertices_bottom.at(i).z());
//            glwidget->glVertex3f((GLfloat)vertices_top.at(i).x(), (GLfloat)vertices_top.at(i).y(), (GLfloat)vertices_top.at(i).z());
//        }

//        glwidget->glEnd();
//    }
//}

void CAD_basic_cylinder::paint(GLWidget *glwidget)
{
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

