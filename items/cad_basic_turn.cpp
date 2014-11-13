#include "cad_basic_turn.h"

CAD_basic_turn::CAD_basic_turn() : CADitem(CADitem::Basic_Turn)
{
    this->description = "Basic|Turn";
    radius_pipe = 5.0;
    radius_turn = 20.0;
    angle_turn = 90.0;

    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("Wall thickness", QVariant::fromValue(2.0));
    wizardParams.insert("Turn radius", QVariant::fromValue(20.0));
    wizardParams.insert("Turn angle", QVariant::fromValue(90.0));
    wizardParams.insert("Outer diameter", QVariant::fromValue(10.0));

    processWizardInput();
    calculate();
}

CAD_basic_turn::~CAD_basic_turn()
{

}

QList<CADitem::ItemType> CAD_basic_turn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Basic_Turn);
    flangable_items.append(CADitem::Basic_Pipe);
    return flangable_items;
}

QImage CAD_basic_turn::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
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

    // Wall thickness iteration
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
            matrix_turn.rotate(angle_turn, 0.0, 0.0, 1.0);

            // Pipe angle iteration

            for (qreal j=0.0; j < 1.01; j += 0.05)    // 20 edges (21 vertices)
            {
                qreal angle_pipe = 2 * PI * j;
                QVector3D linePos;

                if (w == 0)
                    linePos = this->matrix_rotation * matrix_turn * QVector3D(-this->radius_turn + sin(angle_pipe) * (this->radius_pipe), 0.0, cos(angle_pipe) * (this->radius_pipe));
                else
                    linePos = this->matrix_rotation * matrix_turn * QVector3D(-this->radius_turn + sin(angle_pipe) * (this->radius_pipe - this->wallThickness), 0.0, cos(angle_pipe) * (this->radius_pipe - this->wallThickness));
                linePos += this->position;

                this->vertices[w][a][b] = linePos;
                this->boundingBox.enterVertex(linePos);
                b++;
            }
            b = 0;
            a++;
        }
    }

    QVector3D endOfTurn = this->position + (matrix_rotation * matrix_turn * QVector3D(-this->radius_turn, 0.0, 0.0));
    this->snap_vertices.append(endOfTurn);
    this->snap_flanges.append(endOfTurn);
}

void CAD_basic_turn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    wallThickness = wizardParams.value("Wall thickness").toDouble();
    radius_turn = wizardParams.value("Turn radius").toDouble();
    angle_turn = wizardParams.value("Turn angle").toDouble();
    radius_pipe = wizardParams.value("Outer diameter").toDouble() / 2.0;

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    //    direction =  matrix_rotation * QVector3D(0.0, 0.0, 1.0) * length;
}
