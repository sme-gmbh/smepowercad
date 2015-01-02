#include "cad_mainll_subll.h"
#include "glwidget.h"

CAD_mainll_subll::CAD_mainll_subll() : CADitem(CADitem::mainul_subul)
{
    this->description = "mainspaced|subspaced";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

//    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    arrayBufVertices.create();
//    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufFaces.create();
//    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufLines.create();
//    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);
   
    processWizardInput();
    calculate();
}

CAD_mainll_subll::~CAD_mainll_subll()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitem::ItemType> CAD_mainll_subll::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_mainll_subll::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

void CAD_mainll_subll::calculate()
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
}

void CAD_mainll_subll::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
}

//void CAD_mainll_subll::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices.bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces.bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces.release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
//        glwidget->glLineWidth(1.0);
                                      
//        indexBufLines.bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines.release();
//     }                          
                                                                                           
//     arrayBufVertices.release();
//}
