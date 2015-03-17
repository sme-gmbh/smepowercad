/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "cad_air_ducttransitionrectrect.h"
#include "itemdb.h"

CAD_air_ductTransitionRectRect::CAD_air_ductTransitionRectRect() : CADitem(CADitemTypes::Air_DuctTransitionRectRect)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("a2", 300.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("b2", 200.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("e",    0.0);
    wizardParams.insert("f",    0.0);
    wizardParams.insert("u",   50.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",   10.0);

    transition_duct = new CAD_basic_duct();
    endcap_left_duct = new CAD_basic_duct();
    endcap_right_duct = new CAD_basic_duct();
    flange_left_duct = new CAD_basic_duct();
    flange_right_duct = new CAD_basic_duct();
    subItems.append(transition_duct);
    subItems.append(endcap_left_duct);
    subItems.append(endcap_right_duct);
    subItems.append(flange_left_duct);
    subItems.append(flange_right_duct);

    processWizardInput();
    calculate();
}

CAD_air_ductTransitionRectRect::~CAD_air_ductTransitionRectRect()
{
}

QList<CADitemTypes::ItemType> CAD_air_ductTransitionRectRect::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
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

QImage CAD_air_ductTransitionRectRect::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTransitionRectRect::iconPath()
{
    return ":/icons/cad_air/cad_air_ducttransitionrectrect.svg";
}

QString CAD_air_ductTransitionRectRect::domain()
{
    return "Air";
}

QString CAD_air_ductTransitionRectRect::description()
{
    return "Air|Duct transition rect/rect";
}

void CAD_air_ductTransitionRectRect::calculate()
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

//    pos_bot_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;
//    pos_bot_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_bot_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0, -1.0) * c  + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_bot_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -0.0, 0.0) * b + QVector3D(0.0, 0.0, -1.0) * a + position;

//    pos_top_1 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * b + QVector3D(0.0, 0.0, -0.0) * a + position;
//    pos_top_2 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0, -1.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_top_3 = QVector3D( 0.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * d + QVector3D(0.0, 0.5,  0.0) * e + QVector3D(0.0, 0.0,  0.0) * c + QVector3D(0.0, 0.0, 0.5) * f + position;
//    pos_top_4 = QVector3D(-1.0, 0.0, 0.0) * l + QVector3D(0.0,  0.0, 0.0) * b + QVector3D(0.0, 0.0,  0.0) * a + position;

    // Left end duct
    QVector3D position_led = position + QVector3D(0.0, 0.0, 0.0);
    // Right end duct
    QVector3D position_red = position + matrix_rotation * QVector3D(l, (b - b2) / 2 + e, (a - a2) / 2 + f);
    // Left flange duct
    QVector3D position_lfd = position + QVector3D(0, 0.0, 0.0);
    // Right flange duct
    QVector3D position_rfd = position + matrix_rotation * QVector3D(l, (b - b2) / 2 + e, (a - a2) / 2 + f);




    endcap_left_duct->wizardParams.insert("Position x", (position_led.x()));
    endcap_left_duct->wizardParams.insert("Position y", (position_led.y()));
    endcap_left_duct->wizardParams.insert("Position z", (position_led.z()));
    endcap_left_duct->wizardParams.insert("Angle x", (angle_x));
    endcap_left_duct->wizardParams.insert("Angle y", (angle_y));
    endcap_left_duct->wizardParams.insert("Angle z", (angle_z));
    endcap_left_duct->wizardParams.insert("l", (endcap));
    endcap_left_duct->wizardParams.insert("b", (b));
    endcap_left_duct->wizardParams.insert("a", (a));
    endcap_left_duct->wizardParams.insert("s", (s));
    endcap_left_duct->processWizardInput();
    endcap_left_duct->calculate();


    endcap_right_duct->wizardParams.insert("Position x", (position_red.x()));//position.x()+l));
    endcap_right_duct->wizardParams.insert("Position y", (position_red.y()));//position.y()+(b-d)/2+e));
    endcap_right_duct->wizardParams.insert("Position z", (position_red.z()));//position.z()+(a-c)/2+f));
    endcap_right_duct->wizardParams.insert("Angle x", (angle_x));
    endcap_right_duct->wizardParams.insert("Angle y", (angle_y));
    endcap_right_duct->wizardParams.insert("Angle z", (angle_z+180));
    endcap_right_duct->wizardParams.insert("l", (endcap));
    endcap_right_duct->wizardParams.insert("b", (b2));
    endcap_right_duct->wizardParams.insert("a", (a2));
    endcap_right_duct->wizardParams.insert("s", (s));
    endcap_right_duct->processWizardInput();
    endcap_right_duct->calculate();

    flange_left_duct->wizardParams.insert("Position x", (position_lfd.x()));
    flange_left_duct->wizardParams.insert("Position y", (position_lfd.y()));
    flange_left_duct->wizardParams.insert("Position z", (position_lfd.z()));
    flange_left_duct->wizardParams.insert("Angle x", (angle_x));
    flange_left_duct->wizardParams.insert("Angle y", (angle_y));
    flange_left_duct->wizardParams.insert("Angle z", (angle_z));
    flange_left_duct->wizardParams.insert("l", (fe));
    flange_left_duct->wizardParams.insert("b", (b+2*ff));
    flange_left_duct->wizardParams.insert("a", (a+2*ff));
    flange_left_duct->wizardParams.insert("s", (ff));
    flange_left_duct->processWizardInput();
    flange_left_duct->calculate();

    flange_right_duct->wizardParams.insert("Position x", (position_rfd.x()));//position.x()+l));
    flange_right_duct->wizardParams.insert("Position y", (position_rfd.y()));//position.y()+(b-d)/2+e));
    flange_right_duct->wizardParams.insert("Position z", (position_rfd.z()));//position.z()+(a-c)/2+f));
    flange_right_duct->wizardParams.insert("Angle x", (angle_x));
    flange_right_duct->wizardParams.insert("Angle y", (angle_y));
    flange_right_duct->wizardParams.insert("Angle z", (angle_z+180));
    flange_right_duct->wizardParams.insert("l", (fe));
    flange_right_duct->wizardParams.insert("b", (b2+2*ff));
    flange_right_duct->wizardParams.insert("a", (a2+2*ff));
    flange_right_duct->wizardParams.insert("s", (ff));
    flange_right_duct->processWizardInput();
    flange_right_duct->calculate();

    transition_duct->wizardParams.insert("Position x", (position.x()+endcap));
    transition_duct->wizardParams.insert("Position y", (position.y()));
    transition_duct->wizardParams.insert("Position z", (position.z()));
    transition_duct->wizardParams.insert("Angle x", (angle_x));
    transition_duct->wizardParams.insert("Angle y", (angle_y));
    transition_duct->wizardParams.insert("Angle z", (angle_z));
    transition_duct->wizardParams.insert("l", (l -2*endcap));
    transition_duct->wizardParams.insert("b", (b));
    transition_duct->wizardParams.insert("a", (a));
    transition_duct->wizardParams.insert("s", (s));
    transition_duct->processWizardInput();
    transition_duct->calculate();


    QVector3D vertices[] = {
        //set Outer Points
        endcap_left_duct->pos_bot_1,
        endcap_right_duct->pos_bot_4,
        endcap_right_duct->pos_bot_1,
        endcap_left_duct->pos_bot_4,
        endcap_left_duct->pos_top_1,
        endcap_right_duct->pos_top_4,
        endcap_right_duct->pos_top_1,
        endcap_left_duct->pos_top_4,
        //set inner Points
        endcap_right_duct->inner_pos_bot_4,
        endcap_left_duct->inner_pos_bot_1,
        endcap_left_duct->inner_pos_bot_4,
        endcap_right_duct->inner_pos_bot_1,
        endcap_right_duct->inner_pos_top_4,
        endcap_left_duct->inner_pos_top_1,
        endcap_left_duct->inner_pos_top_4,
        endcap_right_duct->inner_pos_top_1,
        endcap_left_duct->inner_pos_top_4
    };
//    //set Outer Points
//    transition_duct->pos_bot_1 = endcap_left_duct->pos_bot_1;
//    transition_duct->pos_bot_2 = endcap_right_duct->pos_bot_4;
//    transition_duct->pos_bot_3 = endcap_right_duct->pos_bot_1;
//    transition_duct->pos_bot_4 = endcap_left_duct->pos_bot_4;

//    transition_duct->pos_top_1 = endcap_left_duct->pos_top_1;
//    transition_duct->pos_top_2 = endcap_right_duct->pos_top_4;
//    transition_duct->pos_top_3 = endcap_right_duct->pos_top_1;
//    transition_duct->pos_top_4 = endcap_left_duct->pos_top_4;
//    //set Inner Points
//    transition_duct->inner_pos_bot_1 = endcap_left_duct->inner_pos_bot_1;
//    transition_duct->inner_pos_bot_2 = endcap_right_duct->inner_pos_bot_4;
//    transition_duct->inner_pos_bot_3 = endcap_right_duct->inner_pos_bot_1;
//    transition_duct->inner_pos_bot_4 = endcap_left_duct->inner_pos_bot_4;

//    transition_duct->inner_pos_top_1 = endcap_left_duct->inner_pos_top_1;
//    transition_duct->inner_pos_top_2 = endcap_right_duct->inner_pos_top_4;
//    transition_duct->inner_pos_top_3 = endcap_right_duct->inner_pos_top_1;
//    transition_duct->inner_pos_top_4 = endcap_left_duct->inner_pos_top_4;

    transition_duct->arrayBufVertices.bind();
    transition_duct->arrayBufVertices.allocate(vertices, sizeof(vertices));


    boundingBox.enterVertex(flange_left_duct->pos_bot_1);
    boundingBox.enterVertex(flange_left_duct->pos_top_1);
    boundingBox.enterVertex(flange_left_duct->pos_bot_2);
    boundingBox.enterVertex(flange_left_duct->pos_top_2);
    boundingBox.enterVertex(flange_left_duct->pos_bot_3);
    boundingBox.enterVertex(flange_left_duct->pos_top_3);
    boundingBox.enterVertex(flange_left_duct->pos_bot_4);
    boundingBox.enterVertex(flange_left_duct->pos_top_4);


    boundingBox.enterVertex(flange_right_duct->pos_bot_1);
    boundingBox.enterVertex(flange_right_duct->pos_top_1);
    boundingBox.enterVertex(flange_right_duct->pos_bot_2);
    boundingBox.enterVertex(flange_right_duct->pos_top_2);
    boundingBox.enterVertex(flange_right_duct->pos_bot_3);
    boundingBox.enterVertex(flange_right_duct->pos_top_3);
    boundingBox.enterVertex(flange_right_duct->pos_bot_4);
    boundingBox.enterVertex(flange_right_duct->pos_top_4);

    this->snap_vertices.append(endcap_left_duct->pos_bot_2);
    this->snap_vertices.append(endcap_left_duct->pos_bot_3);
    this->snap_vertices.append(endcap_left_duct->pos_top_2);
    this->snap_vertices.append(endcap_left_duct->pos_top_3);

    this->snap_vertices.append(endcap_right_duct->pos_bot_2);
    this->snap_vertices.append(endcap_right_duct->pos_bot_3);
    this->snap_vertices.append(endcap_right_duct->pos_top_2);
    this->snap_vertices.append(endcap_right_duct->pos_top_3);





    this->snap_basepoint = position;

//    this->snap_vertices.append(pos_bot_1);
//    this->snap_vertices.append(pos_bot_2);
//    this->snap_vertices.append(pos_bot_3);
//    this->snap_vertices.append(pos_bot_4);
//    this->snap_vertices.append(pos_top_1);
//    this->snap_vertices.append(pos_top_2);
//    this->snap_vertices.append(pos_top_3);
//    this->snap_vertices.append(pos_top_4);

    this->snap_flanges.append(position);
    this->snap_flanges.append(flange_right_duct->snap_flanges.at(0));

}

void CAD_air_ductTransitionRectRect::processWizardInput()
{

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();
    b2 = wizardParams.value("b2").toDouble();
    a2 = wizardParams.value("a2").toDouble();

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    endcap = wizardParams.value("u").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}
