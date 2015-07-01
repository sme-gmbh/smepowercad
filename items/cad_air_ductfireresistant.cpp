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

#include "cad_air_ductfireresistant.h"
//#include "itemdb.h"

CAD_air_ductFireResistant::CAD_air_ductFireResistant() : CADitem(CADitemTypes::Air_DuctFireResistant)
{
    duct = new CAD_air_duct();
    this->subItems.append(duct);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("a",  300.0);
    wizardParams.insert("b",  200.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("fe",  40.0);
    wizardParams.insert("ff",  20.0);
    wizardParams.insert("s",   50.0);

    processWizardInput();
    calculate();
}

CAD_air_ductFireResistant::~CAD_air_ductFireResistant()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductFireResistant::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductFireResistant::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductFireResistant::iconPath()
{
    return ":/icons/cad_air/cad_air_ductfireresistant.svg";
}

QString CAD_air_ductFireResistant::domain()
{
    return "Air";
}

QString CAD_air_ductFireResistant::description()
{
    return "Air|Duct (fire resistant)";
}

void CAD_air_ductFireResistant::calculate()
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

    duct->wizardParams.insert("Position x", position.x());
    duct->wizardParams.insert("Position y", position.y());
    duct->wizardParams.insert("Position z", position.z());

    duct->wizardParams.insert("Angle x", angle_x);
    duct->wizardParams.insert("Angle y", angle_y);
    duct->wizardParams.insert("Angle z", angle_z);

    duct->wizardParams.insert("a",  a);
    duct->wizardParams.insert("b",  b);
    duct->wizardParams.insert("l",  l);
    duct->wizardParams.insert("fe",  fe);
    duct->wizardParams.insert("ff",  ff);
    duct->wizardParams.insert("s",   s);
    duct->layer = this->layer;
    duct->processWizardInput();
    duct->calculate();

    this->snap_flanges = duct->snap_flanges;
    this->snap_center = duct->snap_center;
    this->snap_vertices = duct->snap_vertices;
    this->boundingBox = duct->boundingBox;
}

void CAD_air_ductFireResistant::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    s = wizardParams.value("s").toDouble();

}

QMatrix4x4 CAD_air_ductFireResistant::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2)
    {
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
