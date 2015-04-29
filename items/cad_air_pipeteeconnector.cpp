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

#include "cad_air_pipeteeconnector.h"
#include "itemdb.h"

CAD_air_pipeTeeConnector::CAD_air_pipeTeeConnector() : CADitem(CADitemTypes::Air_PipeTeeConnector)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    processWizardInput();
    calculate();
}

CAD_air_pipeTeeConnector::~CAD_air_pipeTeeConnector()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeTeeConnector::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
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

QImage CAD_air_pipeTeeConnector::wizardImage()
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

QString CAD_air_pipeTeeConnector::iconPath()
{
    return ":/icons/cad_air/cad_air_pipeteeconnector.svg";
}

QString CAD_air_pipeTeeConnector::domain()
{
    return "Air";
}

QString CAD_air_pipeTeeConnector::description()
{
    return "Air|Pipe T-Connector";
}

void CAD_air_pipeTeeConnector::calculate()
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
}

void CAD_air_pipeTeeConnector::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}

QMatrix4x4 CAD_air_pipeTeeConnector::rotationOfFlange(quint8 num)
{

        return matrix_rotation;
}
