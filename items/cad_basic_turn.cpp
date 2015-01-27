#include "cad_basic_turn.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_turn::CAD_basic_turn() : CADitem(CADitemTypes::Basic_Turn)
{
    radius_pipe = 5.0;
    radius_turn = 20.0;
    angle_turn = 90.0;

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("Outer diameter", 100.0);
    wizardParams.insert("Turn radius",    200.0);
    wizardParams.insert("Turn angle",      90.0);
    wizardParams.insert("s",               10.0);

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

CAD_basic_turn::~CAD_basic_turn()
{

}

QList<CADitemTypes::ItemType> CAD_basic_turn::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Basic_Turn);
    flangable_items.append(CADitemTypes::Basic_Pipe);
    return flangable_items;
}

QImage CAD_basic_turn::wizardImage()
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

QString CAD_basic_turn::iconPath()
{
    return ":/icons/cad_basic/cad_basic_turn.svg";
}

QString CAD_basic_turn::domain()
{
    return "Basic";
}

QString CAD_basic_turn::description()
{
    return "Basic|Turn";
}

void CAD_basic_turn::calculate()
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

    this->snap_basepoint = this->position;
    this->snap_flanges.append(this->position);

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    // Vertex data
    int a;
    int b;

    //    int count_a = 11;
    //    int count_b = 21;

    QVector3D vertices[352];
    // s iteration
    QMatrix4x4 matrix_turn;
    for (int w = 0; w <= 1; w++)
    {
        // Turn angle iteration
        a = 0;
        b = 0;
        for (qreal i=0.0; i < 1.01; i += 0.10)    // 10 edges (11 vertices)
        {
            qreal angle_turn = this->angle_turn * i;

            matrix_turn.setToIdentity();
            matrix_turn.translate(this->radius_turn, 0.0, 0.0);
            matrix_turn.rotate(-angle_turn, 0.0, 0.0, 1.0);

            // Pipe angle iteration

            for (qreal j=0.0; j < 1.0; j += 0.0625)    // 16 edges
            {
                qreal angle_pipe = 2 * PI * j;
                QVector3D linePos;

                if (w == 0)
                    linePos = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->radius_turn + sin(angle_pipe) * (this->radius_pipe), cos(angle_pipe) * (this->radius_pipe)) + QVector3D(-this->radius_turn, -this->radius_turn, 0.0));
                else
                    linePos = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->radius_turn + sin(angle_pipe) * (this->radius_pipe - this->wallThickness), cos(angle_pipe) * (this->radius_pipe - this->wallThickness)) + QVector3D(-this->radius_turn, -this->radius_turn, 0.0));
                linePos += this->position;

                vertices[176*w + 16*a + b] = linePos;
                this->boundingBox.enterVertex(linePos);
                b++;
            }
            b = 0;
            a++;
        }
    }

    QVector3D endOfTurn = this->position + this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->radius_turn, 0.0) + QVector3D(-this->radius_turn, -this->radius_turn, 0.0));
    this->snap_vertices.append(endOfTurn);
    this->snap_flanges.append(endOfTurn);


    static GLushort indicesFaces[768];

    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 16; i++)
        {
            indicesFaces[35 * j + 2*i] = j * 16 + i;
            indicesFaces[35 * j + 2*i+1] = j * 16 + i + 16;
        }
        indicesFaces[35 * j + 32] = j * 16 + 0;
        indicesFaces[35 * j + 33] = j * 16 + 16;
        indicesFaces[35 * j + 34] = 0xABCD;
    }

    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 16; i++)
        {
            indicesFaces[350 + 35 * j + 2*i] = 176 + j * 16 + i;
            indicesFaces[350 + 35 * j + 2*i+1] = 176 + j * 16 + i + 16;
        }
        indicesFaces[350 + 35 * j + 32] = 176 + j * 16 + 0;
        indicesFaces[350 + 35 * j + 33] = 176 + j * 16 + 16;
        indicesFaces[350 + 35 * j + 34] = 0xABCD;
    }

        for(int i = 0; i < 16; i++)
        {
            indicesFaces[700 + 2*i] = i;
            indicesFaces[700 + 2*i + 1] = 176 + i;
        }
        indicesFaces[732] = 0;
        indicesFaces[733] = 176;
        indicesFaces[734] = 0xABCD;

        for(int i = 0; i < 16; i++)
        {
            indicesFaces[735 + 2*i] = 160 + i;
            indicesFaces[735 + 2*i + 1] = 336 + i;
        }
        indicesFaces[765] = 160;
        indicesFaces[766] = 336;
        indicesFaces[767] = 0xABCD;

    static GLushort indicesLines[1760];
    //radial
    for(int i = 0; i < 11; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            indicesLines[32 * i + 2 * j] = 16 * i + j;
            indicesLines[32 * i + 2 * j + 1] = 16 * i + j + 1;
        }
        indicesLines[32 * i + 30] = 16 * i;
        indicesLines[32 * i + 31] = 16 * i+ 15;

    }
    for(int i = 0; i < 11; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            indicesLines[352 + 32*i + 2*j] = 176 + 16 * i + j;
            indicesLines[352 + 32*i + 2*j + 1] = 176 + 16 * i + j + 1;
        }
        indicesLines[352 + 32 * i + 30] = 176 + 16 * i;
        indicesLines[352 + 32 * i + 31] = 176 + 16 * i+ 15;
    }
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            indicesLines[704 + 32 * i + 2 * j] = 16 * i + j;
            indicesLines[704 + 32 * i + 2 * j + 1] = 16 * i + j + 16;
        }
    }
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            indicesLines[1024 + 32*i + 2*j] = 176 + 16 * i + j;
            indicesLines[1024 + 32*i + 2*j + 1] = 176 + 16 * i + j + 16;
        }
    }



    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));
}

void CAD_basic_turn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    wallThickness = wizardParams.value("s").toDouble();
    radius_turn = wizardParams.value("Turn radius").toDouble();
    angle_turn = wizardParams.value("Turn angle").toDouble();
    radius_pipe = wizardParams.value("Outer diameter").toDouble() / 2.0;

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    //    direction =  matrix_rotation * QVector3D(0.0, 0.0, 1.0) * length;
}

//void CAD_basic_turn::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    int a;
//    int b;

//    int count_a = 11;
//    int count_b = 21;

//    // level of detail
//    int lod = 1;

//    // s iteration
//    for (int w = 0; w <= 1; w++)
//    {
//        if (glwidget->render_solid)
//        {
//            glwidget->setPaintingColor(color_brush);

//            // Outer and inner surfaces
//            for (a=lod; a < count_a; a += lod)
//            {
//                if (a >= count_a) a = (count_a - 1);
//                glwidget->glBegin(GL_QUADS);
//                for (b=lod; b < count_b; b += lod)
//                {
//                    if (b >= count_b) b = (count_b - 1);

//                    QVector3D vertex_1 = vertices[w][a][b - lod];
//                    QVector3D vertex_2 = vertices[w][a - lod][b - lod];
//                    QVector3D vertex_3 = vertices[w][a - lod][b];
//                    QVector3D vertex_4 = vertices[w][a][b];

//                    glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                    glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                    glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                    glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
//                }
//                glwidget->glEnd();
//            }
//        }

//        if (glwidget->render_outline)
//        {
//            glwidget->setPaintingColor(color_pen);
//            glwidget->glLineWidth(1.0);

//            // Rings
//            for (a=0; a < count_a; a += lod)
//            {
//                if (a >= count_a) a = (count_a - 1);

//                glwidget->glBegin(GL_LINE_STRIP);
//                for (b=0; b < count_b; b += lod)
//                {
//                    if (b >= count_b) b = (count_b - 1);
//                    QVector3D linePos = vertices[w][a][b];
//                    glwidget->glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//                }
//                glwidget->glEnd();
//            }

//            // Lines
//            for (b=0; b < (count_b - 1); b += lod)
//            {
//                if (b >= count_b) b = (count_b - 1);
//                glwidget->glBegin(GL_LINE_STRIP);
//                for (a=0; a < count_a; a += lod)
//                {
//                    if (a >= count_a) a = (count_a - 1);

//                    QVector3D linePos = vertices[w][a][b];
//                    glwidget->glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//                }
//                glwidget->glEnd();
//            }
//        }
//    }

//    // Front and rear faces (discs)
//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush);

//        for (a=0; a < count_a; a+=(count_a - 1))
//        {
//            glwidget->glBegin(GL_QUADS);
//            for (b=lod; b < count_b; b += lod)
//            {
//                if (b >= count_b) b = (count_b - 1);
//                QVector3D vertex_1 = vertices[0][a][b - lod];
//                QVector3D vertex_2 = vertices[1][a][b - lod];
//                QVector3D vertex_3 = vertices[1][a][b];
//                QVector3D vertex_4 = vertices[0][a][b];

//                glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
//            }
//            glwidget->glEnd();
//        }
//    }
//}


void CAD_basic_turn::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

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
