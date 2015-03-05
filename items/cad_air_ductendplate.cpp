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

#include "cad_air_ductendplate.h"
#include "itemdb.h"

CAD_air_ductEndPlate::CAD_air_ductEndPlate() : CADitem(CADitemTypes::Air_DuctEndPlate)
{
    this->plate = new CAD_basic_box();
    this->flange = new CAD_basic_duct();
    this->duct = new CAD_basic_duct;
    this->subItems.append(plate);
    this->subItems.append(flange);
    this->subItems.append(duct);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 200.0);
    wizardParams.insert("b", 300.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);
    wizardParams.insert("s",  10.0);



    processWizardInput();
    calculate();
}

CAD_air_ductEndPlate::~CAD_air_ductEndPlate()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductEndPlate::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    return flangable_items;
}

QImage CAD_air_ductEndPlate::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductEndPlate::iconPath()
{
    return ":/icons/cad_air/cad_air_ductendplate.svg";
}

QString CAD_air_ductEndPlate::domain()
{
    return "Air";
}

QString CAD_air_ductEndPlate::description()
{
    return "Air|Duct end plate";
}

void CAD_air_ductEndPlate::calculate()
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

    duct->wizardParams.insert("Position x", (position.x()));
    duct->wizardParams.insert("Position y", (position.y()));
    duct->wizardParams.insert("Position z", (position.z()));
    duct->wizardParams.insert("Angle x", (angle_x));
    duct->wizardParams.insert("Angle y", (angle_y));
    duct->wizardParams.insert("Angle z", (angle_z));
    duct->wizardParams.insert("l", (l/2));
    duct->wizardParams.insert("b", (b));
    duct->wizardParams.insert("a", (a));
    duct->wizardParams.insert("s", (s));
    duct->processWizardInput();
    duct->calculate();

    flange->wizardParams.insert("Position x", (position.x()));
    flange->wizardParams.insert("Position y", (position.y()));
    flange->wizardParams.insert("Position z", (position.z()));
    flange->wizardParams.insert("Angle x", (angle_x));
    flange->wizardParams.insert("Angle y", (angle_y));
    flange->wizardParams.insert("Angle z", (angle_z));
    flange->wizardParams.insert("l", (fe));
    flange->wizardParams.insert("b", (b + 2 * ff));
    flange->wizardParams.insert("a", (a + 2 * ff));
    flange->wizardParams.insert("s", (ff));
    flange->processWizardInput();
    flange->calculate();

    QVector3D position_p = position + matrix_rotation * QVector3D(3 * l / 4,0.0, 0.0);
    plate->wizardParams.insert("Position x", (position_p.x()));
    plate->wizardParams.insert("Position y", (position_p.y()));
    plate->wizardParams.insert("Position z", (position_p.z()));
    plate->wizardParams.insert("Angle x", (angle_x));
    plate->wizardParams.insert("Angle y", (angle_y));
    plate->wizardParams.insert("Angle z", (angle_z));
    plate->wizardParams.insert("l", (l/2));
    plate->wizardParams.insert("b", (b));
    plate->wizardParams.insert("a", (a));
    plate->processWizardInput();
    plate->calculate();

    boundingBox.enterVertex(flange->pos_bot_1);
    boundingBox.enterVertex(flange->pos_bot_2);
    boundingBox.enterVertex(flange->pos_bot_3);
    boundingBox.enterVertex(flange->pos_bot_4);
    boundingBox.enterVertex(flange->pos_top_1);
    boundingBox.enterVertex(flange->pos_top_2);
    boundingBox.enterVertex(flange->pos_top_3);
    boundingBox.enterVertex(flange->pos_top_4);

    boundingBox.enterVertex(plate->pos_bot_1);
    boundingBox.enterVertex(plate->pos_bot_2);
    boundingBox.enterVertex(plate->pos_bot_3);
    boundingBox.enterVertex(plate->pos_bot_4);
    boundingBox.enterVertex(plate->pos_top_1);
    boundingBox.enterVertex(plate->pos_top_2);
    boundingBox.enterVertex(plate->pos_top_3);
    boundingBox.enterVertex(plate->pos_top_4);

    snap_vertices.append(duct->pos_bot_2);
    snap_vertices.append(duct->pos_bot_3);
    snap_vertices.append(duct->pos_top_2);
    snap_vertices.append(duct->pos_top_3);
    snap_vertices.append(plate->pos_top_2);
    snap_vertices.append(plate->pos_top_3);
    snap_vertices.append(plate->pos_bot_2);
    snap_vertices.append(plate->pos_bot_3);
}


void CAD_air_ductEndPlate::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();

    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    s = wizardParams.value("s").toDouble();
}
