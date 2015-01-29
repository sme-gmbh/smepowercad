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

#include "cad_air_heatexchangerwaterair.h"
#include "itemdb.h"

CAD_air_heatExchangerWaterAir::CAD_air_heatExchangerWaterAir() : CADitem(CADitemTypes::Air_HeatExchangerWaterAir)
{
    this->duct = new CAD_air_duct();
    this->pipe3 = new CAD_basic_pipe();
    this->pipe4 = new CAD_basic_pipe();
    this->exchanger = new CAD_basic_box();
    this->subItems.append(duct);
    this->subItems.append(pipe3);
    this->subItems.append(pipe4);
    this->subItems.append(exchanger);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1000.0);
    wizardParams.insert("b", 1000.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("l3", 300.0);
    wizardParams.insert("l4", 700.0);
    wizardParams.insert("d",  200.0);
    wizardParams.insert("j",  400.0);
    wizardParams.insert("m3", 200.0);
    wizardParams.insert("m4", 400.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",   10.0);

    processWizardInput();
    calculate();
}

CAD_air_heatExchangerWaterAir::~CAD_air_heatExchangerWaterAir()
{

}

QList<CADitemTypes::ItemType> CAD_air_heatExchangerWaterAir::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_heatExchangerWaterAir::wizardImage()
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

QString CAD_air_heatExchangerWaterAir::iconPath()
{
    return ":/icons/cad_air/cad_air_heatexchangerwaterair.svg";
}

QString CAD_air_heatExchangerWaterAir::domain()
{
    return "Air";
}

QString CAD_air_heatExchangerWaterAir::description()
{
    return "Air|Heat exchanger water/air";
}

void CAD_air_heatExchangerWaterAir::calculate()
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

    this->duct->wizardParams.insert("Position x", (position.x()));
    this->duct->wizardParams.insert("Position y", (position.y()));
    this->duct->wizardParams.insert("Position z", (position.z()));
    this->duct->wizardParams.insert("Angle x", (angle_x));
    this->duct->wizardParams.insert("Angle y", (angle_y));
    this->duct->wizardParams.insert("Angle z", (angle_z));
    this->duct->wizardParams.insert("s", (s));
    this->duct->wizardParams.insert("l", (l));
    this->duct->wizardParams.insert("b", (b));
    this->duct->wizardParams.insert("a", (a));
    this->duct->wizardParams.insert("ff", (ff));
    this->duct->wizardParams.insert("fe", (fe));

    this->duct->processWizardInput();
    this->duct->calculate();

    QVector3D position_p3 = position + matrix_rotation *  QVector3D(l3, -b/2, -a/2 + m3);
    pipe3->wizardParams.insert("Position x", (position_p3.x()));
    pipe3->wizardParams.insert("Position y", (position_p3.y()));
    pipe3->wizardParams.insert("Position z", (position_p3.z()));
    pipe3->wizardParams.insert("Angle x", (angle_x));
    pipe3->wizardParams.insert("Angle y", (angle_y));
    pipe3->wizardParams.insert("Angle z", (angle_z-90));
    pipe3->wizardParams.insert("l", (j));
    pipe3->wizardParams.insert("d", (d));
    pipe3->wizardParams.insert("s", (s));

    pipe3->processWizardInput();
    pipe3->calculate();

    QVector3D position_p4 = position + matrix_rotation *  QVector3D(l4, -b/2, -a/2 + m4);
    pipe4->wizardParams.insert("Position x", (position_p4.x()));
    pipe4->wizardParams.insert("Position y", (position_p4.y()));
    pipe4->wizardParams.insert("Position z", (position_p4.z()));
    pipe4->wizardParams.insert("Angle x", (angle_x));
    pipe4->wizardParams.insert("Angle y", (angle_y));
    pipe4->wizardParams.insert("Angle z", (angle_z-90));
    pipe4->wizardParams.insert("l", (j));
    pipe4->wizardParams.insert("d", (d));
    pipe4->wizardParams.insert("s", (s));

    pipe4->processWizardInput();
    pipe4->calculate();

    QVector3D position_p = position + matrix_rotation * QVector3D(l / 2, 0.0, 0.0);
    exchanger->wizardParams.insert("Position x", (position_p.x()));
    exchanger->wizardParams.insert("Position y", (position_p.y()));
    exchanger->wizardParams.insert("Position z", (position_p.z()));
    exchanger->wizardParams.insert("Angle x", (angle_x));
    exchanger->wizardParams.insert("Angle y", (angle_y));
    exchanger->wizardParams.insert("Angle z", (angle_z));
    exchanger->wizardParams.insert("Size x", (0.8 * l));
    exchanger->wizardParams.insert("Size y", (0.8 * b));
    exchanger->wizardParams.insert("Size z", (0.8 * a));
    exchanger->processWizardInput();
    exchanger->calculate();

    this->snap_flanges.append(duct->snap_flanges);
    this->snap_flanges.append(pipe3->snap_flanges.at(0));
    this->snap_flanges.append(pipe4->snap_flanges.at(0));

    this->boundingBox = duct->boundingBox;
    this->boundingBox.enterVertex(pipe3->snap_flanges.at(0));
    this->boundingBox.enterVertex(pipe4->snap_flanges.at(0));


}

void CAD_air_heatExchangerWaterAir::processWizardInput()
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
    l = wizardParams.value("l").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    l4 = wizardParams.value("l4").toDouble();
    d = wizardParams.value("d").toDouble();
    j = wizardParams.value("j").toDouble();
    m3 = wizardParams.value("m3").toDouble();
    m4 = wizardParams.value("m4").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();

}
