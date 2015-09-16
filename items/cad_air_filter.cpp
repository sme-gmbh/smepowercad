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

#include "cad_air_filter.h"

CAD_air_filter::CAD_air_filter() : CADitem(CADitemTypes::Air_Filter)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 2000.0);
    wizardParams.insert("b", 2000.0);
    wizardParams.insert("l", 1500.0);
    wizardParams.insert("s",    1.0);

    duct = new CAD_basic_duct();
    subItems.append(duct);

    processWizardInput();
    calculate();
}

CAD_air_filter::~CAD_air_filter()
{

}

QList<CADitemTypes::ItemType> CAD_air_filter::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_EmptyCabinet);
    flangable_items.append(CADitemTypes::Air_CanvasFlange);
    flangable_items.append(CADitemTypes::Air_Humidifier);
    flangable_items.append(CADitemTypes::Air_Fan);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_DuctBaffleSilencer);
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRect);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_filter::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_filter::iconPath()
{
    return ":/icons/cad_air/cad_air_filter.svg";
}

QString CAD_air_filter::domain()
{
    return "Air";
}

QString CAD_air_filter::description()
{
    return "Air|Filter";
}

void CAD_air_filter::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_duct = position + matrix_rotation * QVector3D(0.0, 0.0, a/2.0);

    duct->wizardParams.insert("Position x", position_duct.x());
    duct->wizardParams.insert("Position y", position_duct.y());
    duct->wizardParams.insert("Position z", position_duct.z());
    duct->wizardParams.insert("Angle x", angle_x);
    duct->wizardParams.insert("Angle y", angle_y);
    duct->wizardParams.insert("Angle z", angle_z);
    duct->wizardParams.insert("l", l);
    duct->wizardParams.insert("b", b);
    duct->wizardParams.insert("a", a);
    duct->wizardParams.insert("s", s);
    duct->processWizardInput();
    duct->calculate();

    boundingBox.enterVertices(duct->boundingBox.getVertices());

    this->snap_vertices.append(duct->snap_vertices);
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
}

void CAD_air_filter::processWizardInput()
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
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_air_filter::rotationOfFlange(quint8 num)
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
