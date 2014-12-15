#include "cad_air_ductturn.h"

CAD_air_ductTurn::CAD_air_ductTurn() : CADitem(CADitem::Air_DuctTurn)
{
    this->description = "Air|Duct turn";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("r", QVariant::fromValue(100.0));
    wizardParams.insert("b", QVariant::fromValue(30.0));
    wizardParams.insert("b2", QVariant::fromValue(30.0));
    wizardParams.insert("a", QVariant::fromValue(20.0));

    wizardParams.insert("f", QVariant::fromValue(5.0));
    wizardParams.insert("e", QVariant::fromValue(5.0));

    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("fe", QVariant::fromValue(1.0));
    wizardParams.insert("alpha", QVariant::fromValue(90.0));

    flange_left_duct = new CAD_basic_duct();
    flange_right_duct = new CAD_basic_duct();
    this->subItems.append(flange_left_duct);
    this->subItems.append(flange_right_duct);

    processWizardInput();
    calculate();


}

CAD_air_ductTurn::~CAD_air_ductTurn()
{

}

QList<CADitem::ItemType> CAD_air_ductTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Duct);
    flangable_items.append(CADitem::Air_DuctEndPlate);
    flangable_items.append(CADitem::Air_DuctFireDamper);
    flangable_items.append(CADitem::Air_DuctTeeConnector);
    flangable_items.append(CADitem::Air_DuctTransition);
    flangable_items.append(CADitem::Air_DuctTransitionRectRound);
    flangable_items.append(CADitem::Air_DuctTurn);
    flangable_items.append(CADitem::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitem::Air_DuctYpiece);
    flangable_items.append(CADitem::Air_Filter);
    flangable_items.append(CADitem::Air_HeatExchangerAirAir);
    flangable_items.append(CADitem::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitem::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductTurn::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}


void CAD_air_ductTurn::calculate()
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
    matrix_rotation.rotate(angle_y+180, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    flange_left_duct->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    flange_left_duct->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    flange_left_duct->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    flange_left_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_left_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_left_duct->wizardParams.insert("Angle z", QVariant::fromValue(-angle_z));
    flange_left_duct->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_left_duct->wizardParams.insert("b", QVariant::fromValue(b+2*ff));
    flange_left_duct->wizardParams.insert("a", QVariant::fromValue(a+2*ff));
    flange_left_duct->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_left_duct->processWizardInput();
    flange_left_duct->calculate();

    // Vertex data
    int a;
    int b;

    //    int count_a = 11;
    //    int count_b = 21;

    // s iteration
    QMatrix4x4 matrix_turn;
    QVector3D position_rfd;
    for (int w = 0; w <= 1; w++)
    {
        // Turn angle iteration
        a = 0;
        b = 0;
        //add small endcap
        if(w == 0)
        {
            QVector3D lp1 = this->matrix_rotation * QVector3D(0.0, this->b / 2, this->a / 2) + position;
            this->vertices[w][a][b] = lp1;
            this->boundingBox.enterVertex(lp1);
            this->snap_vertices.append(lp1);
            b++;
            QVector3D lp2 = this->matrix_rotation * QVector3D(0.0, this->b / 2, -this->a / 2) + position;
            this->vertices[w][a][b] = lp2;
            this->boundingBox.enterVertex(lp2);
            this->snap_vertices.append(lp2);
            b++;
            QVector3D lp3 = this->matrix_rotation * QVector3D(0.0, -this->b / 2, -this->a / 2) + position;
            this->vertices[w][a][b] = lp3;
            this->boundingBox.enterVertex(lp3);
            this->snap_vertices.append(lp3);
            b++;
            QVector3D lp4 = this->matrix_rotation * QVector3D(0.0, -this->b/2, this->a / 2) + position;
            this->vertices[w][a][b] = lp4;
            this->boundingBox.enterVertex(lp4);
            this->snap_vertices.append(lp4);
            b++;
            QVector3D lp5 = this->matrix_rotation * QVector3D(0.0, this->b / 2, this->a / 2) + position;
            this->vertices[w][a][b] = lp5;
            b++;
        }
        else
        {
            QVector3D lp1 = this->matrix_rotation * QVector3D(0.0, this->b / 2 - this->s,  this->a / 2 - this->s) + position;
            this->vertices[w][a][b] = lp1;
            b++;
            QVector3D lp2 = this->matrix_rotation * QVector3D(0.0, this->b / 2 - this->s, -this->a / 2 + this->s) + position;
            this->vertices[w][a][b] = lp2;
            b++;
            QVector3D lp3 = this->matrix_rotation * QVector3D(0.0, -this->b / 2 + this->s, -this->a / 2  + this->s) + position;
            this->vertices[w][a][b] = lp3;
            b++;
            QVector3D lp4 = this->matrix_rotation * QVector3D(0.0, -this->b / 2 + this->s,  this->a / 2 - this->s) + position;
            this->vertices[w][a][b] = lp4;
            b++;
            QVector3D lp5 = this->matrix_rotation * QVector3D(0.0, this->b / 2 -this->s,  this->a / 2 - this->s) + position;
            this->vertices[w][a][b] = lp5;
            b++;
        }
        b = 0;
        a++;
        //add turn
        for (qreal i=0.0; i < 1.01; i += 0.10)    // 10 edges (11 vertices)
        {
            qreal angle_turn = this->alpha * i;

            matrix_turn.setToIdentity();
            matrix_turn.rotate(angle_turn, 0.0, 0.0, 1.0);

            // Pipe angle iteration

            if(w == 0)
            {
                // Width2 >= Width1 && draw outer
                if(this->b2 >= this->b)
                {
                    QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b, this->a / 2)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp1;
                    b++;
                    QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b, -this->a / 2)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp2;
                    b++;
                    QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, -this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp3;
                    b++;
                    QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp4;
                    b++;
                    QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b, this->a / 2)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp5;
                    b++;
                }
                // Width2 < Width1 && draw outer
                else
                {
                    QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2, this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2 + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp1;
                    b++;
                    QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2, -this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2 + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp2;
                    b++;
                    QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, -this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp3;
                    b++;
                    QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp4;
                    b++;
                    QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2, this->a / 2)
                                                       + QVector3D(-this->f, - this->r - this->b / 2  + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp5;
                    b++;
                }
            }
            else
            {
                // Width2 >= Width1 && draw inner
                if(this->b2 >= this->b)
                {
                    QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp1;
                    b++;
                    QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s, -this->a / 2 + this->s)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp2;
                    b++;
                    QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s                , -this->a / 2 + this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp3;
                    b++;
                    QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s                ,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp4;
                    b++;
                    QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f + this->b2 - this->b, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp5;
                    b++;
                }
                // Width2 < Width1 && draw inner
                else
                {
                    QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2 - this->s,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2 + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp1;
                    b++;
                    QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2 - this->s, -this->a / 2 + this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2 + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp2;
                    b++;
                    QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s                , -this->a / 2 + this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp3;
                    b++;
                    QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s                ,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2, 0.0)) + position;
                    this->vertices[w][a][b] = lp4;
                    b++;
                    QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b2 - this->s,  this->a / 2 - this->s)
                                                       + QVector3D(-this->f, - this->r - this->b / 2  + this->b - this->b2, 0.0)) + position;
                    this->vertices[w][a][b] = lp5;
                    b++;

                }
            }
            b = 0;
            a++;
        }
        //add big endcap
        qreal angle_turn = this->alpha;

        matrix_turn.setToIdentity();
        //matrix_turn.translate(this->radius, 0.0, 0.0);
        matrix_turn.rotate(angle_turn, 0.0, 0.0, 1.0);
        qreal angle_turn_rad = angle_turn / 180 * PI;

        if(w == 0)
        {


            QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b + (this->b2 - this->b), this->a / 2)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                                     + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp1;
            this->boundingBox.enterVertex(lp1);
            this->snap_vertices.append(lp1);
            b++;
            QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b + (this->b2 - this->b), -this->a / 2)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                                     + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp2;
            this->boundingBox.enterVertex(lp2);
            this->snap_vertices.append(lp2);
            b++;
            QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, -this->a / 2)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                                     + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp3;
            this->boundingBox.enterVertex(lp3);
            this->snap_vertices.append(lp3);
            b++;
            QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r, this->a / 2)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                                     + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp4;
            this->boundingBox.enterVertex(lp4);
            this->snap_vertices.append(lp4);
            b++;
            QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b + (this->b2 - this->b), this->a / 2)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                                     + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp5;
            b++;

        }
        else
        {
            QVector3D lp1 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s,  this->a / 2 - this->s)
                                               + QVector3D(-this->f + (this->b2 - this->b), - this->r - this->b / 2, 0.0)
                                               + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp1;
            b++;
            QVector3D lp2 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s, -this->a / 2 + this->s)
                                               + QVector3D(-this->f + (this->b2 - this->b), - this->r - this->b / 2, 0.0)
                                               + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp2;
            b++;
            QVector3D lp3 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s, -this->a / 2 + this->s)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                               + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp3;
            b++;
            QVector3D lp4 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->s,  this->a / 2 - this->s)
                                               + QVector3D(-this->f, - this->r - this->b / 2, 0.0)
                                               + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp4;
            b++;
            QVector3D lp5 = this->matrix_rotation * (matrix_turn * QVector3D(0.0, this->r + this->b - this->s,  this->a / 2 - this->s)
                                               + QVector3D(-this->f + (this->b2 - this->b), - this->r - this->b / 2, 0.0)
                                               + QVector3D(-cos(angle_turn_rad) * this->e, -sin(angle_turn_rad) * this->e, 0.0)) + position;
            this->vertices[w][a][b] = lp5;
            b++;
            this->snap_flanges.append((lp1 + lp3 ) / 2);
            position_rfd = (lp1 + lp3 ) / 2;
        }
        b = 0;
        a++;
        a++;
    }


    flange_right_duct->wizardParams.insert("Position x", QVariant::fromValue(position_rfd.x()));
    flange_right_duct->wizardParams.insert("Position y", QVariant::fromValue(position_rfd.y()));
    flange_right_duct->wizardParams.insert("Position z", QVariant::fromValue(position_rfd.z()));
    flange_right_duct->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    flange_right_duct->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    flange_right_duct->wizardParams.insert("Angle z", QVariant::fromValue(-angle_z-alpha+180));
    flange_right_duct->wizardParams.insert("l", QVariant::fromValue(fe));
    flange_right_duct->wizardParams.insert("b", QVariant::fromValue(b2+2*ff));
    flange_right_duct->wizardParams.insert("a", QVariant::fromValue(this->a+2*ff));
    flange_right_duct->wizardParams.insert("s", QVariant::fromValue(ff));
    flange_right_duct->processWizardInput();
    flange_right_duct->calculate();

    boundingBox.enterVertex(flange_left_duct->pos_bot_1);
    boundingBox.enterVertex(flange_left_duct->pos_bot_2);
    boundingBox.enterVertex(flange_left_duct->pos_bot_3);
    boundingBox.enterVertex(flange_left_duct->pos_bot_4);
    boundingBox.enterVertex(flange_left_duct->pos_top_1);
    boundingBox.enterVertex(flange_left_duct->pos_top_2);
    boundingBox.enterVertex(flange_left_duct->pos_top_3);
    boundingBox.enterVertex(flange_left_duct->pos_top_4);


    boundingBox.enterVertex(flange_right_duct->pos_bot_1);
    boundingBox.enterVertex(flange_right_duct->pos_bot_2);
    boundingBox.enterVertex(flange_right_duct->pos_bot_3);
    boundingBox.enterVertex(flange_right_duct->pos_bot_4);
    boundingBox.enterVertex(flange_right_duct->pos_top_1);
    boundingBox.enterVertex(flange_right_duct->pos_top_2);
    boundingBox.enterVertex(flange_right_duct->pos_top_3);
    boundingBox.enterVertex(flange_right_duct->pos_top_4);


}



void CAD_air_ductTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();

    r = wizardParams.value("r").toDouble();
    b = wizardParams.value("b").toDouble();
    b2 = wizardParams.value("b2").toDouble();
    a = wizardParams.value("a").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    f = wizardParams.value("f").toDouble();
    e = wizardParams.value("e").toDouble();

    if(abs(alpha - 90) > 10E-8 && abs(b - b2) > 10E-8)
        qDebug() << "Different width for input and output is only acceptable if angle = 90°";

}
