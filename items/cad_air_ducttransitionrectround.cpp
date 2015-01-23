#include "cad_air_ducttransitionrectround.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_air_ductTransitionRectRound::CAD_air_ductTransitionRectRound() : CADitem(CADitemTypes::Air_DuctTransitionRectRound)
{
    this->flange_rect = new CAD_basic_duct();
    this->flange_round =new CAD_basic_pipe();
    this->subItems.append(flange_rect);
    this->subItems.append(flange_round);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("s",   1.0);
    wizardParams.insert("l", 100.0);
    wizardParams.insert("b",  30.0);
    wizardParams.insert("a",  20.0);
    wizardParams.insert("ff",  1.0);
    wizardParams.insert("fe",  1.0);
    wizardParams.insert("e",   0.0);
    wizardParams.insert("f",   0.0);
    wizardParams.insert("u",   5.0);
    wizardParams.insert("d",  20.0);

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

CAD_air_ductTransitionRectRound::~CAD_air_ductTransitionRectRound()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductTransitionRectRound::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Pipe);
    flangable_items.append(CADitemTypes::Air_PipeEndCap);
    flangable_items.append(CADitemTypes::Air_PipeFireDamper);
    flangable_items.append(CADitemTypes::Air_PipeReducer);
    flangable_items.append(CADitemTypes::Air_PipeSilencer);
    flangable_items.append(CADitemTypes::Air_PipeTeeConnector);
    flangable_items.append(CADitemTypes::Air_PipeTurn);
    flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_ductTransitionRectRound::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTransitionRectRound::iconPath()
{
    return ":/icons/cad_air/cad_air_ducttransitionrectround.svg";
}

QString CAD_air_ductTransitionRectRound::domain()
{
    return "Air";
}

QString CAD_air_ductTransitionRectRound::description()
{
    return "Air|Duct transition rect/round";
}

void CAD_air_ductTransitionRectRound::calculate()
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
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, b/2 - e - d/2, a/2 - f - d/2 ));

    QVector3D vertices[144];
    //front <-> back
    for(int i = 0; i < 2; i++)
    {
        //inner <-> outer
        for(int j = 0; j < 2; j++)
        {
            int k = 0;
            //circle
            for(qreal step = 0; step <= 1; step += 0.03125)
            {
                QVector3D rad = QVector3D(0.0, d/2 - j * s, 0.0);
                QMatrix4x4 matrix_turn = QMatrix4x4();
                matrix_turn.setToIdentity();
                matrix_turn.rotate(step * 360, 1.0, 0.0, 0.0);
                vertices[64*i + 32*j + k]= position + matrix_rotation * (matrix_turn * rad + QVector3D(l - i * u, b/2 - e - d/2, a/2 - f - d/2 ));
                boundingBox.enterVertex(vertices[62*i + 32*j + k]);
                k++;

            }
        }
    }
    //front <-> back
    for(int i = 0; i < 2; i++)
    {
        //inner <-> outer
        for(int j = 0; j < 2; j++)
        {
            vertices[128 + 8*i + 4*j] = position + matrix_rotation * QVector3D(i * u,  b/2 - j * s, -a/2 + j * s);
            vertices[128 + 8*i + 4*j + 1] = position + matrix_rotation * QVector3D(i * u,  b/2 - j * s,  a/2 - j * s);
            vertices[128 + 8*i + 4*j + 2] = position + matrix_rotation * QVector3D(i * u, -b/2 + j * s,  a/2 - j * s);
            vertices[128 + 8*i + 4*j + 3] = position + matrix_rotation * QVector3D(i * u, -b/2 + j * s, -a/2 + j * s);
            boundingBox.enterVertex(vertices[128 + 8*i + 4*j]);
            boundingBox.enterVertex(vertices[128 + 8*i + 4*j + 1]);
            boundingBox.enterVertex(vertices[128 + 8*i + 4*j + 2]);
            boundingBox.enterVertex(vertices[128 + 8*i + 4*j + 3]);
        }
    }
    static GLushort indicesFaces[376];
    for(int i = 0; i < 32; i++)
    {
        indicesFaces[2*i] = i;
        indicesFaces[2*i+1] = 64 + i;
    }
    indicesFaces[64] = 0;
    indicesFaces[65] = 64;
    indicesFaces[66] = 0xABCD;
    for(int i = 0; i < 32; i++)
    {
        indicesFaces[66 + 2*i] = 32 + i;
        indicesFaces[66 + 2*i+1] = 96 + i;
    }
    indicesFaces[130] = 32;
    indicesFaces[131] = 96;
    indicesFaces[132] = 0xABCD;
    for(int i = 0; i < 32; i++)
    {
        indicesFaces[132 + 2*i] = 0 + i;
        indicesFaces[132 + 2*i+1] = 32 + i;
    }
    indicesFaces[196] = 0;
    indicesFaces[197] = 32;
    indicesFaces[198] = 0xABCD;
    indicesFaces[199] = 128;
    indicesFaces[200] = 136;
    indicesFaces[201] = 129;
    indicesFaces[202] = 137;
    indicesFaces[203] = 130;
    indicesFaces[204] = 138;
    indicesFaces[205] = 131;
    indicesFaces[206] = 139;
    indicesFaces[207] = 128;
    indicesFaces[208] = 136;
    indicesFaces[209] = 0xABCD;
    indicesFaces[210] = 132;
    indicesFaces[211] = 140;
    indicesFaces[212] = 133;
    indicesFaces[213] = 141;
    indicesFaces[214] = 134;
    indicesFaces[215] = 142;
    indicesFaces[216] = 135;
    indicesFaces[217] = 143;
    indicesFaces[218] = 132;
    indicesFaces[219] = 140;
    indicesFaces[220] = 0xABCD;
    indicesFaces[221] = 128;
    indicesFaces[222] = 140;
    indicesFaces[223] = 129;
    indicesFaces[224] = 141;
    indicesFaces[225] = 130;
    indicesFaces[226] = 142;
    indicesFaces[227] = 131;
    indicesFaces[228] = 143;
    indicesFaces[229] = 128;
    indicesFaces[230] = 140;
    indicesFaces[231] = 0xABCD;
    for(int i = 0; i < 8; i++)
    {
        indicesFaces[232 + 4*i] = 136;
        indicesFaces[232 + 4*i + 1] = 64 + 24 + i;
        indicesFaces[232 + 4*i + 2] = 64 + 24 + i + 1;
        indicesFaces[232 + 4*i + 3] = 0xABCD;
    }
    indicesFaces[232 + 4*7+2] = 64;
    for(int i = 0; i < 8; i++)
    {
        indicesFaces[264 + 4*i] = 138;
        indicesFaces[264 + 4*i + 1] = 64 + i + 8;
        indicesFaces[264 + 4*i + 2] = 64 + i + 8 + 1;
        indicesFaces[264 + 4*i + 3] = 0xABCD;
    }
    for(int i = 0; i < 8; i++)
    {
        indicesFaces[296 + 4*i] = 137;
        indicesFaces[296 + 4*i + 1] = 64 + i;
        indicesFaces[296 + 4*i + 2] = 64 + i + 1;
        indicesFaces[296 + 4*i + 3] = 0xABCD;
    }
    for(int i = 0; i < 8; i++)
    {
        indicesFaces[328 + 4*i] = 139;
        indicesFaces[328 + 4*i + 1] = 64 + i + 16;
        indicesFaces[328 + 4*i + 2] = 64 + i + 16 + 1;
        indicesFaces[328 + 4*i + 3] = 0xABCD;
    }
    indicesFaces[360] = 136;
    indicesFaces[361] = 64;
    indicesFaces[362] = 137;
    indicesFaces[363] = 0xABCD;
    indicesFaces[364] = 137;
    indicesFaces[365] = 72;
    indicesFaces[366] = 138;
    indicesFaces[367] = 0xABCD;
    indicesFaces[368] = 138;
    indicesFaces[369] = 80;
    indicesFaces[370] = 139;
    indicesFaces[371] = 0xABCD;
    indicesFaces[372] = 139;
    indicesFaces[373] = 88;
    indicesFaces[374] = 136;
    indicesFaces[375] = 0xABCD;




    static GLushort indicesLines[504];
//connectors
    for(int i = 0; i <= 8; i++){
        indicesLines[2*i] = 64+i+24;
        indicesLines[2*i+1] = 136;
    }
    indicesLines[16] = 64;
    for(int i = 0; i <= 8; i++){
        indicesLines[18+2*i] = 64+i;
        indicesLines[18+2*i+1] = 137;
    }
    for(int i = 0; i <= 8; i++){
        indicesLines[36+2*i] = 64+i+8;
        indicesLines[36+2*i+1] = 138;
    }
    for(int i = 0; i <= 8; i++){
        indicesLines[54+2*i] = 64+i+16;
        indicesLines[54+2*i+1] = 139;
    }
    //endcap on round side
    for(int i = 0; i < 32; i++)
    {
        indicesLines[72 + 2*i] = i;
        indicesLines[72 + 2*i + 1] = i+64;
    }
    for(int i = 0; i < 32; i++)
    {
        indicesLines[136 + 2*i] = i;
        indicesLines[136 + 2*i + 1] = i+1;
    }
    indicesLines[199] = 0;
    for(int i = 0; i < 32; i++)
    {
        indicesLines[200 + 2*i] = i;
        indicesLines[200 + 2*i + 1] = i + 32;
    }
    for(int i = 0; i < 32; i++)
    {
        indicesLines[264 + 2*i] = i + 32;
        indicesLines[264 + 2*i + 1] = i + 33;
    }
    indicesLines[327] = 32;
    for(int i = 0; i < 32; i++)
    {
        indicesLines[328 + 2*i] = i + 32;
        indicesLines[328 + 2*i + 1] = i + 96;
    }
    for(int i = 0; i < 32; i++)
    {
        indicesLines[392 + 2*i] = i + 96;
        indicesLines[392 + 2*i + 1] = i + 96 + 1;
    }
    indicesLines[455] = 96;
    //endcap on rect side;
    indicesLines[456] = 128;
    indicesLines[457] = 129;
    indicesLines[458] = 129;
    indicesLines[459] = 130;
    indicesLines[460] = 130;
    indicesLines[461] = 131;
    indicesLines[462] = 131;
    indicesLines[463] = 128;

    indicesLines[464] = 132;
    indicesLines[465] = 133;
    indicesLines[466] = 133;
    indicesLines[467] = 134;
    indicesLines[468] = 134;
    indicesLines[469] = 135;
    indicesLines[470] = 135;
    indicesLines[471] = 132;

    indicesLines[472] = 136;
    indicesLines[473] = 137;
    indicesLines[474] = 137;
    indicesLines[475] = 138;
    indicesLines[476] = 138;
    indicesLines[477] = 139;
    indicesLines[478] = 139;
    indicesLines[479] = 136;

    indicesLines[480] = 140;
    indicesLines[481] = 141;
    indicesLines[482] = 141;
    indicesLines[483] = 142;
    indicesLines[484] = 142;
    indicesLines[485] = 143;
    indicesLines[486] = 143;
    indicesLines[487] = 140;


    indicesLines[488] = 128;
    indicesLines[489] = 136;
    indicesLines[490] = 129;
    indicesLines[491] = 137;
    indicesLines[492] = 130;
    indicesLines[493] = 138;
    indicesLines[494] = 131;
    indicesLines[495] = 139;

    indicesLines[496] = 132;
    indicesLines[497] = 140;
    indicesLines[498] = 133;
    indicesLines[499] = 141;
    indicesLines[500] = 134;
    indicesLines[501] = 142;
    indicesLines[502] = 135;
    indicesLines[503] = 143;



    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));

    //calculate flanges
    flange_rect->wizardParams.insert("Position x", (position.x()));
    flange_rect->wizardParams.insert("Position y", (position.y()));
    flange_rect->wizardParams.insert("Position z", (position.z()));
    flange_rect->wizardParams.insert("Angle x", (angle_x));
    flange_rect->wizardParams.insert("Angle y", (angle_y));
    flange_rect->wizardParams.insert("Angle z", (angle_z+180));
    flange_rect->wizardParams.insert("l", (fe));
    flange_rect->wizardParams.insert("b", (b + 2 * ff));
    flange_rect->wizardParams.insert("a", (a + 2 * ff));
    flange_rect->wizardParams.insert("s", (ff));
    flange_rect->processWizardInput();
    flange_rect->calculate();

    QVector3D position_fr = position + matrix_rotation *  QVector3D(l, b/2 - e - d/2, a/2 - f - d/2 );
    flange_round->wizardParams.insert("Position x", (position_fr.x()));
    flange_round->wizardParams.insert("Position y", (position_fr.y()));
    flange_round->wizardParams.insert("Position z", (position_fr.z()));
    flange_round->wizardParams.insert("Angle x", (angle_x));
    flange_round->wizardParams.insert("Angle y", (angle_y));
    flange_round->wizardParams.insert("Angle z", (angle_z+180));
    flange_round->wizardParams.insert("l", (fe));
    flange_round->wizardParams.insert("d", (d + 2 * ff));
    flange_round->wizardParams.insert("s", (ff));
    flange_round->processWizardInput();
    flange_round->calculate();

    this->snap_vertices.append(rectside[0][0][0]);
    this->snap_vertices.append(rectside[0][0][1]);
    this->snap_vertices.append(rectside[0][0][2]);
    this->snap_vertices.append(rectside[0][0][3]);


    this->snap_vertices.append(roundside[0][0][0]);
    this->snap_vertices.append(roundside[0][0][7]);
    this->snap_vertices.append(roundside[0][0][15]);
    this->snap_vertices.append(roundside[0][0][23]);
}

void CAD_air_ductTransitionRectRound::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    u = wizardParams.value("u").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    d = wizardParams.value("d").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

//void CAD_air_ductTransitionRectRound::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    int count_k = 32;
//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush);

//        //front face round
//        glwidget->glBegin(GL_QUADS);
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());

//        //outer faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());


//        //inner faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());


//        //inner faces rect
//        glwidget->glBegin(GL_QUADS);
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());

//        //outer faces rect
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());

//        //front faces rect
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glEnd();

//        //triangle rect-round-transition
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        for(int k = 0; k < 8; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        for(int k = 8; k < 16; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        for(int k = 16; k < 24; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        for(int k = 24; k < 32; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_QUADS);

//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(),   (GLfloat)roundside[1][0][0].y(),   (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][31].x(),   (GLfloat)roundside[1][0][31].y(),   (GLfloat)roundside[1][0][31].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][8].x(),   (GLfloat)roundside[1][0][8].y(),   (GLfloat)roundside[1][0][8].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][7].x(),   (GLfloat)roundside[1][0][7].y(),   (GLfloat)roundside[1][0][7].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][16].x(),   (GLfloat)roundside[1][0][16].y(),   (GLfloat)roundside[1][0][16].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][15].x(),   (GLfloat)roundside[1][0][15].y(),   (GLfloat)roundside[1][0][15].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][24].x(),   (GLfloat)roundside[1][0][24].y(),   (GLfloat)roundside[1][0][24].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][23].x(),   (GLfloat)roundside[1][0][23].y(),   (GLfloat)roundside[1][0][23].z());

//        glwidget->glEnd();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen);
//        glwidget->glLineWidth(1.0);

//        //        //front face round
//        //        for(int k = 0; k < count_k - 1; k++)
//        //        {
//        //            glBegin(GL_LINE_LOOP);
//        //            glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//        //            glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//        //            glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//        //            glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());

//        //        }
//        //        glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        //        glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        //        glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        //        glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
//        //        glEnd();

//        //outer faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glBegin(GL_LINE_LOOP);
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//            glwidget->glEnd();
//        }
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
//        glwidget->glEnd();

//        //inner faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glBegin(GL_LINE_LOOP);
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//            glwidget->glEnd();
//        }
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        glwidget->glEnd();

//        //inner faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();


//        //outer faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();

//        //inner faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();

//        //rect-round transition

//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 8; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 8; k < 16; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_LINES);
//        for(int k = 16; k < 24; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_LINES);
//        for(int k = 24; k < 32; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//    }
//}

void CAD_air_ductTransitionRectRound::paint(GLWidget *glwidget)
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
