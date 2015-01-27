#include "cad_air_ductteeconnector.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_air_ductTeeConnector::CAD_air_ductTeeConnector() : CADitem(CADitemTypes::Air_DuctTeeConnector)
{
//    for(int i = 0; i < 2; i++)
//    {
//        for(int j = 0; j < 12; j++)
//        {
//            for(int k = 0; k < 2; k++)
//            {
//                vertices_turn1[i][j][k] = QVector3D(0.0, 0.0, 0.0);
//                vertices_turn2[i][j][k] = QVector3D(0.0, 0.0, 0.0);

//            }
//        }
//    }
//    for(int i = 0; i < 4; i++)
//        vertices_backside[i] = QVector3D(0.0, 0.0, 0.0);

    endcap_1 = new CAD_basic_duct();
    endcap_2 = new CAD_basic_duct();
    endcap_3 = new CAD_basic_duct();
    flange_1 = new CAD_basic_duct();
    flange_2 = new CAD_basic_duct();
    flange_3 = new CAD_basic_duct();

    this->subItems.append(endcap_1);
    this->subItems.append(endcap_2);
    this->subItems.append(endcap_3);
    this->subItems.append(flange_1);
    this->subItems.append(flange_2);
    this->subItems.append(flange_3);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("b2", 300.0);
    wizardParams.insert("b3", 500.0);
    wizardParams.insert("l", 2600.0);

    wizardParams.insert("r1", 500.0);
    wizardParams.insert("r2", 500.0);

    wizardParams.insert("m", 1000.0);
    wizardParams.insert("n", 1100.0);

    wizardParams.insert("e",   0.0);
    wizardParams.insert("u",  500.0);
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

CAD_air_ductTeeConnector::~CAD_air_ductTeeConnector()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductTeeConnector::flangable_items()
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
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductTeeConnector::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTeeConnector::iconPath()
{
    return ":/icons/cad_air/cad_air_ductteeconnector.svg";
}

QString CAD_air_ductTeeConnector::domain()
{
    return "Air";
}

QString CAD_air_ductTeeConnector::description()
{
    return "Air|Duct T-Connector";
}

void CAD_air_ductTeeConnector::calculate()
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

    endcap_1->wizardParams.insert("Position x", position.x());
    endcap_1->wizardParams.insert("Position y", position.y());
    endcap_1->wizardParams.insert("Position z", position.z());
    endcap_1->wizardParams.insert("Angle x", angle_x);
    endcap_1->wizardParams.insert("Angle y", angle_y);
    endcap_1->wizardParams.insert("Angle z", angle_z);
    endcap_1->wizardParams.insert("l", u);
    endcap_1->wizardParams.insert("b", b);
    endcap_1->wizardParams.insert("a", a);
    endcap_1->wizardParams.insert("s", s);
    endcap_1->processWizardInput();
    endcap_1->calculate();

    flange_1->wizardParams.insert("Position x", position.x());
    flange_1->wizardParams.insert("Position y", position.y());
    flange_1->wizardParams.insert("Position z", position.z());
    flange_1->wizardParams.insert("Angle x", angle_x);
    flange_1->wizardParams.insert("Angle y", angle_y);
    flange_1->wizardParams.insert("Angle z", angle_z);
    flange_1->wizardParams.insert("l", fe);
    flange_1->wizardParams.insert("b", b + 2 * ff);
    flange_1->wizardParams.insert("a", a + 2 * ff);
    flange_1->wizardParams.insert("s", ff);
    flange_1->processWizardInput();
    flange_1->calculate();

    QVector3D position_e2 = position + matrix_rotation * QVector3D(l, b/2 - e - b2/2, 0);
    endcap_2->wizardParams.insert("Position x", position_e2.x());
    endcap_2->wizardParams.insert("Position y", position_e2.y());
    endcap_2->wizardParams.insert("Position z", position_e2.z());
    endcap_2->wizardParams.insert("Angle x", angle_x);
    endcap_2->wizardParams.insert("Angle y", angle_y);
    endcap_2->wizardParams.insert("Angle z", angle_z+180);
    endcap_2->wizardParams.insert("l", u);
    endcap_2->wizardParams.insert("b", b2);
    endcap_2->wizardParams.insert("a", a);
    endcap_2->wizardParams.insert("s", s);
    endcap_2->processWizardInput();
    endcap_2->calculate();

    flange_2->wizardParams.insert("Position x", position_e2.x());
    flange_2->wizardParams.insert("Position y", position_e2.y());
    flange_2->wizardParams.insert("Position z", position_e2.z());
    flange_2->wizardParams.insert("Angle x", angle_x);
    flange_2->wizardParams.insert("Angle y", angle_y);
    flange_2->wizardParams.insert("Angle z", angle_z+180);
    flange_2->wizardParams.insert("l", fe);
    flange_2->wizardParams.insert("b", b2 + 2 * ff);
    flange_2->wizardParams.insert("a", a + 2 * ff);
    flange_2->wizardParams.insert("s", ff);
    flange_2->processWizardInput();
    flange_2->calculate();

    QVector3D position_e3 = position + matrix_rotation * QVector3D(n + b3/2, b/2 -e - b2 - m, 0);
    endcap_3->wizardParams.insert("Position x", position_e3.x());
    endcap_3->wizardParams.insert("Position y", position_e3.y());
    endcap_3->wizardParams.insert("Position z", position_e3.z());
    endcap_3->wizardParams.insert("Angle x", angle_x);
    endcap_3->wizardParams.insert("Angle y", angle_y);
    endcap_3->wizardParams.insert("Angle z", angle_z+90);
    endcap_3->wizardParams.insert("l", u);
    endcap_3->wizardParams.insert("b", b3);
    endcap_3->wizardParams.insert("a", a);
    endcap_3->wizardParams.insert("s", s);
    endcap_3->processWizardInput();
    endcap_3->calculate();

    flange_3->wizardParams.insert("Position x", position_e3.x());
    flange_3->wizardParams.insert("Position y", position_e3.y());
    flange_3->wizardParams.insert("Position z", position_e3.z());
    flange_3->wizardParams.insert("Angle x", angle_x);
    flange_3->wizardParams.insert("Angle y", angle_y);
    flange_3->wizardParams.insert("Angle z", angle_z+90);
    flange_3->wizardParams.insert("l", fe);
    flange_3->wizardParams.insert("b", b3 + 2 * ff);
    flange_3->wizardParams.insert("a", a + 2 * ff);
    flange_3->wizardParams.insert("s", ff);
    flange_3->processWizardInput();
    flange_3->calculate();

    snap_flanges.append(position);
    snap_flanges.append(position_e2);
    snap_flanges.append(position_e3);


    QVector3D vertices[52];

    vertices[0] = position + matrix_rotation * QVector3D(this->u, -b/2, -a/2);
    vertices[1] = position + matrix_rotation * QVector3D(this->u, -b/2, a/2);

    vertices[24] = position + matrix_rotation * QVector3D(n + b3, -b/2 - r1, -a/2);
    vertices[25]= position + matrix_rotation * QVector3D(n + b3, -b/2 - r1, a/2);

    int x = 2;
    int y = 26;
    QMatrix4x4 matrix_turn;
    for (int i = 0; i < 10; i++)
    {
        qreal angle_turn = 9.0 * i;

        matrix_turn.setToIdentity();
        matrix_turn.rotate(-angle_turn, 0.0, 0.0, 1.0);

        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1, -a/2) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2, 0.0, -a/2) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        y++;
        x++;
        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1, a/2) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2, 0.0, a/2) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        x++;
        y++;
    }
    qDebug() << x << " " << y;
    vertices[22] = position + matrix_rotation * QVector3D(n, -b/2 - r1, -a/2);
    vertices[23] = position + matrix_rotation * QVector3D(n, -b/2 - r1, a/2);
    vertices[46] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2, -a/2);
    vertices[47] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2, a/2);


    vertices[48] = position + matrix_rotation * QVector3D(u, b/2, a/2);
    vertices[49] = position + matrix_rotation * QVector3D(l - u, b/2 - e, a/2);
    vertices[50] = position + matrix_rotation * QVector3D(l - u, b/2 - e, -a/2);
    vertices[51] = position + matrix_rotation * QVector3D(u, b/2, -a/2);


    static GLushort indicesFaces[] = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,0xABCD,
        24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,0xABCD,
        48,49,51,50,0xABCD,
        51,50,0,46,2,44,4,42,6,40,8,38,10,36,12,34,14,32,16,30,18,28,20,26,22,24,0xABCD,
        48,49,1,47,3,45,5,43,7,41,9,39,11,37,13,35,15,33,17,31,19,29,21,27,23,25,0xABCD
    };

    static GLushort indicesLines[] = {
        0,2,2,4,4,6,6,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,
        1,3,3,5,5,7,7,9,9,11,11,13,13,15,15,17,17,19,19,21,21,23,
        24,26,26,28,28,30,30,32,32,34,34,36,36,38,38,40,40,42,42,44,44,46,
        25,27,27,29,29,31,31,33,33,35,35,37,37,39,39,41,41,43,43,45,45,47,
        48,49,50,51,
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
        23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47
    };


    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(flange_1->pos_bot_1);
    boundingBox.enterVertex(flange_1->pos_top_1);
    boundingBox.enterVertex(flange_1->pos_bot_2);
    boundingBox.enterVertex(flange_1->pos_top_2);
    boundingBox.enterVertex(flange_1->pos_bot_3);
    boundingBox.enterVertex(flange_1->pos_top_3);
    boundingBox.enterVertex(flange_1->pos_bot_4);
    boundingBox.enterVertex(flange_1->pos_top_4);

    boundingBox.enterVertex(flange_2->pos_bot_1);
    boundingBox.enterVertex(flange_2->pos_top_1);
    boundingBox.enterVertex(flange_2->pos_bot_2);
    boundingBox.enterVertex(flange_2->pos_top_2);
    boundingBox.enterVertex(flange_2->pos_bot_3);
    boundingBox.enterVertex(flange_2->pos_top_3);
    boundingBox.enterVertex(flange_2->pos_bot_4);
    boundingBox.enterVertex(flange_2->pos_top_4);

    boundingBox.enterVertex(flange_3->pos_bot_1);
    boundingBox.enterVertex(flange_3->pos_top_1);
    boundingBox.enterVertex(flange_3->pos_bot_2);
    boundingBox.enterVertex(flange_3->pos_top_2);
    boundingBox.enterVertex(flange_3->pos_bot_3);
    boundingBox.enterVertex(flange_3->pos_top_3);
    boundingBox.enterVertex(flange_3->pos_bot_4);
    boundingBox.enterVertex(flange_3->pos_top_4);

    this->snap_vertices.append(endcap_1->pos_bot_2);
    this->snap_vertices.append(endcap_1->pos_bot_3);
    this->snap_vertices.append(endcap_1->pos_top_2);
    this->snap_vertices.append(endcap_1->pos_top_3);

    this->snap_vertices.append(endcap_2->pos_bot_2);
    this->snap_vertices.append(endcap_2->pos_bot_3);
    this->snap_vertices.append(endcap_2->pos_top_2);
    this->snap_vertices.append(endcap_2->pos_top_3);

    this->snap_vertices.append(endcap_3->pos_bot_2);
    this->snap_vertices.append(endcap_3->pos_bot_3);
    this->snap_vertices.append(endcap_3->pos_top_2);
    this->snap_vertices.append(endcap_3->pos_top_3);


}

void CAD_air_ductTeeConnector::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    this->a = wizardParams.value("a").toDouble();
    this->b = wizardParams.value("b").toDouble();
    this->b2 = wizardParams.value("b2").toDouble();
    this->e = wizardParams.value("e").toDouble();
    this->b3 = wizardParams.value("b3").toDouble();
    this->l = wizardParams.value("l").toDouble();
    this->m = wizardParams.value("m").toDouble();
    this->n = wizardParams.value("n").toDouble();
    this->r1 = wizardParams.value("r1").toDouble();
    this->r2 = wizardParams.value("r2").toDouble();
    this->u = wizardParams.value("u").toDouble();
    this->ff = wizardParams.value("ff").toDouble();
    this->fe = wizardParams.value("fe").toDouble();
    this->s = wizardParams.value("s").toDouble();

    if(fabs((u + r1 + b) - (e + b2 + m)) > 10E-8)
        qDebug() << "This item can not be drawn! (u + r1 + b) != (e + d + m)";

    if(fabs((n + b3 + r2 + u) - l) > 10E-8)
        qDebug() << "This item can not be drawn! (n + h + r2 + u) != l";

}

//void CAD_air_ductTeeConnector::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();
//    int a;
////    int b;

//    int count_a = 12;
////    int count_b = 2;

//    //draw turns
//    // s iteration
//    for (int w = 0; w <= 0; w++)
//    {
//        if (glwidget->render_solid)
//        {
//            glwidget->setPaintingColor(color_brush);

//            for (a=1; a < count_a; a ++)
//            {

//                QVector3D vertex_1 = vertices_turn1[w][a][0];
//                QVector3D vertex_2 = vertices_turn1[w][a - 1][0];
//                QVector3D vertex_3 = vertices_turn1[w][a - 1][1];
//                QVector3D vertex_4 = vertices_turn1[w][a][1];

//                glwidget->glBegin(GL_QUADS);
//                glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
//                glwidget->glEnd();
//            }
//            for (a=1; a < count_a; a ++)
//            {

//                QVector3D vertex_1 = vertices_turn2[w][a][0];
//                QVector3D vertex_2 = vertices_turn2[w][a - 1][0];
//                QVector3D vertex_3 = vertices_turn2[w][a - 1][1];
//                QVector3D vertex_4 = vertices_turn2[w][a][1];

//                glwidget->glBegin(GL_QUADS);
//                glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
//                glwidget->glEnd();
//            }
//            //draw backside
//            glwidget->glBegin(GL_QUADS);
//            glwidget->glVertex3f((GLfloat)vertices_backside[0].x(), (GLfloat)vertices_backside[0].y(), (GLfloat)vertices_backside[0].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[1].x(), (GLfloat)vertices_backside[1].y(), (GLfloat)vertices_backside[1].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[2].x(), (GLfloat)vertices_backside[2].y(), (GLfloat)vertices_backside[2].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[3].x(), (GLfloat)vertices_backside[3].y(), (GLfloat)vertices_backside[3].z());
//            glwidget->glEnd();

//            //draw lower side
//            glwidget->glBegin(GL_TRIANGLE_FAN);
//            QVector3D vertex_1 = (vertices_backside[0] + vertices_backside[1]) / 2;
//            glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[0].x(), (GLfloat)vertices_backside[0].y(), (GLfloat)vertices_backside[0].z());
//            for (a=0; a < count_a; a ++)
//            {
//                QVector3D vertex_a = vertices_turn1[w][a][1];
//                glwidget->glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
//            }
//            for (a=0; a < count_a; a ++)
//            {
//                QVector3D vertex_a = vertices_turn2[w][a][1];
//                glwidget->glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
//            }

//            glwidget->glVertex3f((GLfloat)vertices_backside[1].x(), (GLfloat)vertices_backside[1].y(), (GLfloat)vertices_backside[1].z());
//            glwidget->glEnd();

//            //draw upper side

//            glwidget->glBegin(GL_TRIANGLE_FAN);
//            QVector3D vertex_2 = (vertices_backside[2] + vertices_backside[3]) / 2;
//            glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[3].x(), (GLfloat)vertices_backside[3].y(), (GLfloat)vertices_backside[3].z());
//            for (a=0; a < count_a; a ++)
//            {
//                QVector3D vertex_a = vertices_turn1[0][a][0];
//                glwidget->glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
//            }
//            for (a=0; a < count_a; a ++)
//            {
//                QVector3D vertex_a = vertices_turn2[0][a][0];
//                glwidget->glVertex3f((GLfloat)vertex_a.x(), (GLfloat)vertex_a.y(), (GLfloat)vertex_a.z());
//            }

//            glwidget->glVertex3f((GLfloat)vertices_backside[2].x(), (GLfloat)vertices_backside[2].y(), (GLfloat)vertices_backside[2].z());
//            glwidget->glEnd();


//        }



//        if (glwidget->render_outline)
//        {
//            glwidget->setPaintingColor(color_pen);
//            glwidget->glLineWidth(1.0);



//            for (a=1; a < count_a; a ++)
//            {
//                glwidget->glBegin(GL_LINE_STRIP);
//                QVector3D vertex_1 = vertices_turn1[w][a][0];
//                QVector3D vertex_2 = vertices_turn1[w][a - 1][0];
//                QVector3D vertex_3 = vertices_turn1[w][a - 1][1];
//                QVector3D vertex_4 = vertices_turn1[w][a][1];

//                glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());
//                glwidget->glEnd();
//                glwidget->glBegin(GL_LINE_STRIP);
//                vertex_1 = vertices_turn2[w][a][0];
//                vertex_2 = vertices_turn2[w][a - 1][0];
//                vertex_3 = vertices_turn2[w][a - 1][1];
//                vertex_4 = vertices_turn2[w][a][1];

//                glwidget->glVertex3f((GLfloat)vertex_1.x(), (GLfloat)vertex_1.y(), (GLfloat)vertex_1.z());
//                glwidget->glVertex3f((GLfloat)vertex_2.x(), (GLfloat)vertex_2.y(), (GLfloat)vertex_2.z());
//                glwidget->glVertex3f((GLfloat)vertex_3.x(), (GLfloat)vertex_3.y(), (GLfloat)vertex_3.z());
//                glwidget->glVertex3f((GLfloat)vertex_4.x(), (GLfloat)vertex_4.y(), (GLfloat)vertex_4.z());

//                glwidget->glEnd();
//            }

//            glwidget->glBegin(GL_LINE_LOOP);
//            glwidget->glVertex3f((GLfloat)vertices_backside[0].x(), (GLfloat)vertices_backside[0].y(), (GLfloat)vertices_backside[0].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[1].x(), (GLfloat)vertices_backside[1].y(), (GLfloat)vertices_backside[1].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[2].x(), (GLfloat)vertices_backside[2].y(), (GLfloat)vertices_backside[2].z());
//            glwidget->glVertex3f((GLfloat)vertices_backside[3].x(), (GLfloat)vertices_backside[3].y(), (GLfloat)vertices_backside[3].z());
//            glwidget->glEnd();
//        }
//        // This breaks the pipeline... is it needed?

//    }
//}

void CAD_air_ductTeeConnector::paint(GLWidget *glwidget)
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
