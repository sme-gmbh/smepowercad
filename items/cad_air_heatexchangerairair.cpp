#include "cad_air_heatexchangerairair.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_air_heatExchangerAirAir::CAD_air_heatExchangerAirAir() : CADitem(CADitemTypes::Air_HeatExchangerAirAir)
{
    this->duct = new CAD_air_duct();
    this->subItems.append(duct);
    this->seperator_1 = new CAD_basic_plane();
    this->subItems.append(seperator_1);
    this->seperator_2 = new CAD_basic_plane();
    this->subItems.append(seperator_2);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("i",  200.0);

    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",   10.0);

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

CAD_air_heatExchangerAirAir::~CAD_air_heatExchangerAirAir()
{

}

QList<CADitemTypes::ItemType> CAD_air_heatExchangerAirAir::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransition);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_heatExchangerAirAir::wizardImage()
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

QString CAD_air_heatExchangerAirAir::iconPath()
{
    return ":/icons/cad_air/cad_air_heatexchangerairair.svg";
}

QString CAD_air_heatExchangerAirAir::domain()
{
    return "Air";
}

QString CAD_air_heatExchangerAirAir::description()
{
    return "Air|Heat exchanger air/air";
}

void CAD_air_heatExchangerAirAir::calculate()
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

    QVector3D position_he = position + matrix_rotation * QVector3D(0.0, 0.0, -a/2);
    this->duct->wizardParams.insert("Position x", (position_he.x()));
    this->duct->wizardParams.insert("Position y", (position_he.y()));
    this->duct->wizardParams.insert("Position z", (position_he.z()));
    this->duct->wizardParams.insert("Angle x", (angle_x));
    this->duct->wizardParams.insert("Angle y", (angle_y));
    this->duct->wizardParams.insert("Angle z", (angle_z));
    this->duct->wizardParams.insert("s", (s));
    this->duct->wizardParams.insert("l", (l));
    this->duct->wizardParams.insert("b", (b));
    this->duct->wizardParams.insert("a", (2 * a));
    this->duct->wizardParams.insert("ff", (ff));
    this->duct->wizardParams.insert("fe", (fe));

    this->duct->processWizardInput();
    this->duct->calculate();

    QVector3D position_sp1 = position + matrix_rotation * QVector3D(0.0, -b/2 +s, -a/2);
    this->seperator_1->wizardParams.insert("Position x", (position_sp1.x()));
    this->seperator_1->wizardParams.insert("Position y", (position_sp1.y()));
    this->seperator_1->wizardParams.insert("Position z", (position_sp1.z()));
    this->seperator_1->wizardParams.insert("Angle x", (angle_x));
    this->seperator_1->wizardParams.insert("Angle y", (angle_y));
    this->seperator_1->wizardParams.insert("Angle z", (angle_z));
    this->seperator_1->wizardParams.insert("Length (A)", ((l - i) / 2));
    this->seperator_1->wizardParams.insert("Width (B)", (b-2*s));

    this->seperator_1->processWizardInput();
    this->seperator_1->calculate();

    QVector3D position_sp2 = position + matrix_rotation * QVector3D((l+i)/2, -b/2 +s, -a/2);
    this->seperator_2->wizardParams.insert("Position x", (position_sp2.x()));
    this->seperator_2->wizardParams.insert("Position y", (position_sp2.y()));
    this->seperator_2->wizardParams.insert("Position z", (position_sp2.z()));
    this->seperator_2->wizardParams.insert("Angle x", (angle_x));
    this->seperator_2->wizardParams.insert("Angle y", (angle_y));
    this->seperator_2->wizardParams.insert("Angle z", (angle_z));
    this->seperator_2->wizardParams.insert("Length (A)", ((l - i) / 2));
    this->seperator_2->wizardParams.insert("Width (B)", (b-2*s));

    this->seperator_2->processWizardInput();
    this->seperator_2->calculate();

    QVector3D vertices[8];
    vertices[0] = position + matrix_rotation * QVector3D((l-i)/2, -b/2 + s, -a/2);
    vertices[1] = position + matrix_rotation * QVector3D(l/2,     -b/2 + s, -3*a/2 + s);
    vertices[2] = position + matrix_rotation * QVector3D((l+i)/2, -b/2 + s, -a/2 );
    vertices[3] = position + matrix_rotation * QVector3D(l/2,     -b/2 + s,  a/2 - s);

    vertices[4] = position + matrix_rotation * QVector3D((l-i)/2, b/2 - s, -a/2);
    vertices[5] = position + matrix_rotation * QVector3D(l/2,     b/2 - s, -3*a/2 + s);
    vertices[6] = position + matrix_rotation * QVector3D((l+i)/2, b/2 - s, -a/2);
    vertices[7] = position + matrix_rotation * QVector3D(l/2,     b/2 - s, a/2 - s);

    GLushort indicesFaces[] = {0, 4, 1, 5, 2, 6, 3, 7, 0, 4 };
    GLushort indicesLines[] = {0,1,1,2,2,3,3,0, 4,5,5,6,6,7,7,4, 1,5, 3,7 };


    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));



    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, -a));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, -a));

    this->boundingBox = duct->boundingBox;

}

void CAD_air_heatExchangerAirAir::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    s = wizardParams.value("s").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    i = wizardParams.value("i").toDouble();

}

void CAD_air_heatExchangerAirAir::paint(GLWidget *glwidget)
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


//void CAD_air_heatExchangerAirAir::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush);
//        glwidget->glBegin(GL_QUADS);

//        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
//        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
//        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
//        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());

//        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
//        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
//        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());
//        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());

//        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
//        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
//        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
//        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());

//        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
//        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
//        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());
//        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
//        glwidget->glEnd();

//    }
//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen);
//        glwidget->glLineWidth(1.0);
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)points[0][0].x(), (GLfloat)points[0][0].y(), (GLfloat)points[0][0].z());
//        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
//        glwidget->glVertex3f((GLfloat)points[0][2].x(), (GLfloat)points[0][2].y(), (GLfloat)points[0][2].z());
//        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)points[1][0].x(), (GLfloat)points[1][0].y(), (GLfloat)points[1][0].z());
//        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
//        glwidget->glVertex3f((GLfloat)points[1][2].x(), (GLfloat)points[1][2].y(), (GLfloat)points[1][2].z());
//        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        glwidget->glVertex3f((GLfloat)points[0][1].x(), (GLfloat)points[0][1].y(), (GLfloat)points[0][1].z());
//        glwidget->glVertex3f((GLfloat)points[1][1].x(), (GLfloat)points[1][1].y(), (GLfloat)points[1][1].z());
//        glwidget->glVertex3f((GLfloat)points[0][3].x(), (GLfloat)points[0][3].y(), (GLfloat)points[0][3].z());
//        glwidget->glVertex3f((GLfloat)points[1][3].x(), (GLfloat)points[1][3].y(), (GLfloat)points[1][3].z());
//        glwidget->glEnd();
//    }
//}
