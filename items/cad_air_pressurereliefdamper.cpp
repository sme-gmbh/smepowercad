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

#include "cad_air_pressurereliefdamper.h"
#include "itemdb.h"

CAD_air_pressureReliefDamper::CAD_air_pressureReliefDamper() : CADitem(CADitemTypes::Air_PressureReliefDamper)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1000.0);
    wizardParams.insert("b", 1000.0);
    wizardParams.insert("l",  200.0);
    wizardParams.insert("fe",   0.0);
    wizardParams.insert("ff",   0.0);
    wizardParams.insert("s",   20.0);

    duct = new CAD_basic_duct;
    flange = new CAD_basic_duct;
    this->subItems.append(duct);
    this->subItems.append(flange);

    processWizardInput();
    calculate();
}

CAD_air_pressureReliefDamper::~CAD_air_pressureReliefDamper()
{

}

QList<CADitemTypes::ItemType> CAD_air_pressureReliefDamper::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    return flangable_items;
}

QImage CAD_air_pressureReliefDamper::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_pressureReliefDamper::iconPath()
{
    return ":/icons/cad_air/cad_air_pressurereliefdamper.svg";
}

QString CAD_air_pressureReliefDamper::domain()
{
    return "Air";
}

QString CAD_air_pressureReliefDamper::description()
{
    return "Air|Pressure relief damper";
}

void CAD_air_pressureReliefDamper::calculate()
{

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
    this->snap_flanges.append(position);
    this->subItems.clear();
    this->subItems.append(duct);
    this->subItems.append(flange);

    duct->wizardParams.insert("Position x", (position.x()));
    duct->wizardParams.insert("Position y", (position.y()));
    duct->wizardParams.insert("Position z", (position.z()));
    duct->wizardParams.insert("Angle x", (angle_x));
    duct->wizardParams.insert("Angle y", (angle_y));
    duct->wizardParams.insert("Angle z", (angle_z));
    duct->wizardParams.insert("l", (l));
    duct->wizardParams.insert("b", (b));
    duct->wizardParams.insert("a", (a));
    duct->wizardParams.insert("s", (s));
    duct->layer = this->layer;
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
    flange->matrix_rotation = this->matrix_rotation;
    flange->layer = this->layer;
    flange->calculate();

    for(int i = 0; i < 10; i++)
    {
        CAD_basic_box *box = new CAD_basic_box();
        this->subItems.append(box);
        QVector3D position_box = position + matrix_rotation * QVector3D(l + 0.025 * (a - 2*s), 0.0, (a - 2*s) * (0.45 - i * 0.1));
        box->wizardParams.insert("Position x", position_box.x());
        box->wizardParams.insert("Position y", position_box.y());
        box->wizardParams.insert("Position z", position_box.z());
        box->wizardParams.insert("Angle x", angle_x);
        box->wizardParams.insert("Angle y", angle_y);
        box->wizardParams.insert("Angle z", angle_z);

        box->wizardParams.insert("l", 0.025 * (a - 2*s));
        box->wizardParams.insert("b", b - 2*s);
        box->wizardParams.insert("a", 0.1 * (a - 2*s));

        box->processWizardInput();
        box->rotateAroundAxis(-45.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
        box->layer = this->layer;
        box->calculate();
        this->boundingBox.enterVertices(box->boundingBox.getVertices());
    }

    this->boundingBox.enterVertices(duct->boundingBox.getVertices());
    this->boundingBox.enterVertices(flange->boundingBox.getVertices());
}

void CAD_air_pressureReliefDamper::processWizardInput()
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

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

QMatrix4x4 CAD_air_pressureReliefDamper::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
