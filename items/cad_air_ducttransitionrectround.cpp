#include "cad_air_ducttransitionrectround.h"
#include "glwidget.h"

CAD_air_ductTransitionRectRound::CAD_air_ductTransitionRectRound() : CADitem(CADitem::Air_DuctTransitionRectRound)
{
    this->flange_rect = new CAD_basic_duct();
    this->flange_round =new CAD_basic_pipe();
    this->subItems.append(flange_rect);
    this->subItems.append(flange_round);
    this->description = "Air|Duct transition rect/round";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("e", QVariant::fromValue(0.0));
    wizardParams.insert("f", QVariant::fromValue(0.0));
    wizardParams.insert("u", QVariant::fromValue(5.0));
    wizardParams.insert("d", QVariant::fromValue(20.0));

    processWizardInput();
    calculate();
}

CAD_air_ductTransitionRectRound::~CAD_air_ductTransitionRectRound()
{

}

QList<CADitem::ItemType> CAD_air_ductTransitionRectRound::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Pipe);
    flangable_items.append(CADitem::Air_PipeEndCap);
    flangable_items.append(CADitem::Air_PipeFireDamper);
    flangable_items.append(CADitem::Air_PipeReducer);
    flangable_items.append(CADitem::Air_PipeSilencer);
    flangable_items.append(CADitem::Air_PipeTeeConnector);
    flangable_items.append(CADitem::Air_PipeTurn);
    flangable_items.append(CADitem::Air_PipeVolumetricFlowController);
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
                roundside[i][j][k] = position + matrix_rotation * (matrix_turn * rad + QVector3D(l - i * u, b/2 - e - d/2, a/2 - f - d/2 ));
                boundingBox.enterVertex(roundside[i][j][k]);
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
            rectside[i][j][0] = position + matrix_rotation * QVector3D(i * u,  b/2 - j * s, -a/2 + j * s);
            rectside[i][j][1] = position + matrix_rotation * QVector3D(i * u,  b/2 - j * s,  a/2 - j * s);
            rectside[i][j][2] = position + matrix_rotation * QVector3D(i * u, -b/2 + j * s,  a/2 - j * s);
            rectside[i][j][3] = position + matrix_rotation * QVector3D(i * u, -b/2 + j * s, -a/2 + j * s);
            boundingBox.enterVertex(rectside[i][j][0]);
            boundingBox.enterVertex(rectside[i][j][1]);
            boundingBox.enterVertex(rectside[i][j][2]);
            boundingBox.enterVertex(rectside[i][j][3]);
        }
    }

    //calculate flanges
    flange_rect->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    flange_rect->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    flange_rect->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    flange_rect->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_rect->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_rect->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    flange_rect->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_rect->wizardParams.insert("b", QVariant::fromValue(b + 2 * ff));
    flange_rect->wizardParams.insert("a", QVariant::fromValue(a + 2 * ff));
    flange_rect->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_rect->processWizardInput();
    flange_rect->calculate();

    QVector3D position_fr = position + matrix_rotation *  QVector3D(l, b/2 - e - d/2, a/2 - f - d/2 );
    flange_round->wizardParams.insert("Position x", QVariant::fromValue(position_fr.x()));
    flange_round->wizardParams.insert("Position y", QVariant::fromValue(position_fr.y()));
    flange_round->wizardParams.insert("Position z", QVariant::fromValue(position_fr.z()));
    flange_round->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_round->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_round->wizardParams.insert("Angle z", QVariant::fromValue(angle_z+180));
    flange_round->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_round->wizardParams.insert("d", QVariant::fromValue(d + 2 * ff));
    flange_round->wizardParams.insert("s", QVariant::fromValue(ff));
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

void CAD_air_ductTransitionRectRound::paint(GLWidget *glwidget)
{
    QColor color_pen = getColorPen();
    QColor color_brush = getColorBrush();

    int count_k = 32;
    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush);

        //front face round
        glwidget->glBegin(GL_QUADS);
        for(int k = 0; k < count_k - 1; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
        }
        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());

        //outer faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
        }
        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());


        //inner faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
        }
        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());


        //inner faces rect
        glwidget->glBegin(GL_QUADS);
        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());

        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());

        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());

        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());

        //outer faces rect
        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());

        //front faces rect
        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
        glwidget->glEnd();

        //triangle rect-round-transition
        glwidget->glBegin(GL_TRIANGLE_FAN);
        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
        for(int k = 0; k < 8; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();
        glwidget->glBegin(GL_TRIANGLE_FAN);
        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
        for(int k = 8; k < 16; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();
        glwidget->glBegin(GL_TRIANGLE_FAN);
        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
        for(int k = 16; k < 24; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_TRIANGLE_FAN);
        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
        for(int k = 24; k < 32; k++)
        {
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
        }
        glwidget->glEnd();

        glwidget->glBegin(GL_QUADS);

        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(),   (GLfloat)roundside[1][0][0].y(),   (GLfloat)roundside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][31].x(),   (GLfloat)roundside[1][0][31].y(),   (GLfloat)roundside[1][0][31].z());

        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][8].x(),   (GLfloat)roundside[1][0][8].y(),   (GLfloat)roundside[1][0][8].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][7].x(),   (GLfloat)roundside[1][0][7].y(),   (GLfloat)roundside[1][0][7].z());

        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][16].x(),   (GLfloat)roundside[1][0][16].y(),   (GLfloat)roundside[1][0][16].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][15].x(),   (GLfloat)roundside[1][0][15].y(),   (GLfloat)roundside[1][0][15].z());

        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][24].x(),   (GLfloat)roundside[1][0][24].y(),   (GLfloat)roundside[1][0][24].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][23].x(),   (GLfloat)roundside[1][0][23].y(),   (GLfloat)roundside[1][0][23].z());

        glwidget->glEnd();
    }

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen);
        glwidget->glLineWidth(1.0);

        //        //front face round
        //        for(int k = 0; k < count_k - 1; k++)
        //        {
        //            glBegin(GL_LINE_LOOP);
        //            glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
        //            glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
        //            glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
        //            glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());

        //        }
        //        glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
        //        glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
        //        glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
        //        glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
        //        glEnd();

        //outer faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glwidget->glBegin(GL_LINE_LOOP);
            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
            glwidget->glEnd();
        }
        glwidget->glBegin(GL_LINE_LOOP);
        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
        glwidget->glEnd();

        //inner faces round
        for(int k = 0; k < count_k - 1; k++)
        {
            glwidget->glBegin(GL_LINE_LOOP);
            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
            glwidget->glEnd();
        }
        glwidget->glBegin(GL_LINE_LOOP);
        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
        glwidget->glEnd();

        //inner faces rect
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
        }
        glwidget->glEnd();


        //outer faces rect
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
        }
        glwidget->glEnd();

        //inner faces rect
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINE_LOOP);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINES);
        for(int k = 0; k < 4; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
        }
        glwidget->glEnd();

        //rect-round transition

        glwidget->glBegin(GL_LINES);
        for(int k = 0; k < 8; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();
        glwidget->glBegin(GL_LINES);
        for(int k = 8; k < 16; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();

        glwidget->glBegin(GL_LINES);
        for(int k = 16; k < 24; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();

        glwidget->glBegin(GL_LINES);
        for(int k = 24; k < 32; k++)
        {
            glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

        }
        glwidget->glEnd();
    }
}
