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

#include "cad_air_pipevolumetricflowcontroller.h"
#include "itemdb.h"

CAD_air_pipeVolumetricFlowController::CAD_air_pipeVolumetricFlowController() : CADitem(CADitemTypes::Air_PipeVolumetricFlowController)
{
    mainPipe = new CAD_basic_pipe();
    function = new CAD_basic_box();
    handle = new CAD_basic_box();
    controller = new CAD_basic_pipe();
    this->subItems.append(mainPipe);
    this->subItems.append(function);
    this->subItems.append(handle);
    this->subItems.append(controller);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 300.0);
    wizardParams.insert("d", 300.0);
    wizardParams.insert("s", 5.0);

    processWizardInput();
    calculate();
}

CAD_air_pipeVolumetricFlowController::~CAD_air_pipeVolumetricFlowController()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeVolumetricFlowController::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Pipe);
    flangable_items.append(CADitemTypes::Air_PipeBranch);
    flangable_items.append(CADitemTypes::Air_PipeSilencer);
    flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_PipeFireDamper);
    flangable_items.append(CADitemTypes::Air_PipeEndCap);
    flangable_items.append(CADitemTypes::Air_PipeReducer);
    flangable_items.append(CADitemTypes::Air_PipeTurn);
    return flangable_items;
}

QImage CAD_air_pipeVolumetricFlowController::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_pipeVolumetricFlowController::iconPath()
{
    return ":/icons/cad_air/cad_air_pipevolumetricflowcontroller.svg";
}

QString CAD_air_pipeVolumetricFlowController::domain()
{
    return "Air";
}

QString CAD_air_pipeVolumetricFlowController::description()
{
    return "Air|Pipe volumetric flow controller";
}

void CAD_air_pipeVolumetricFlowController::calculate()
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

    mainPipe->wizardParams.insert("Position x", position.x());
    mainPipe->wizardParams.insert("Position y", position.y());
    mainPipe->wizardParams.insert("Position z", position.z());
    mainPipe->wizardParams.insert("Angle x", angle_x);
    mainPipe->wizardParams.insert("Angle y", angle_y);
    mainPipe->wizardParams.insert("Angle z", angle_z);
    mainPipe->wizardParams.insert("l", l);
    mainPipe->wizardParams.insert("d", d);
    mainPipe->wizardParams.insert("s", s);
    mainPipe->layer = this->layer;
    mainPipe->processWizardInput();
    mainPipe->calculate();

    QVector3D position_func = matrix_rotation * QVector3D(l/2, d/2 + 40.0, 0.0) + position;
    function->wizardParams.insert("Position x", position_func.x());
    function->wizardParams.insert("Position y", position_func.y());
    function->wizardParams.insert("Position z", position_func.z());
    function->wizardParams.insert("Angle x", angle_x);
    function->wizardParams.insert("Angle y", angle_y);
    function->wizardParams.insert("Angle z", angle_z);
    function->wizardParams.insert("l", 182.0);
    function->wizardParams.insert("b", 60.0);
    function->wizardParams.insert("a", 174.0);
    function->layer = this->layer;
    function->processWizardInput();
    function->calculate();

    QVector3D position_hand = matrix_rotation * QVector3D(l/2, d/2 + 95.0, 0.0) + position;
    handle->wizardParams.insert("Position x", position_hand.x());
    handle->wizardParams.insert("Position y", position_hand.y());
    handle->wizardParams.insert("Position z", position_hand.z());
    handle->wizardParams.insert("Angle x", angle_x);
    handle->wizardParams.insert("Angle y", angle_y);
    handle->wizardParams.insert("Angle z", angle_z);
    handle->wizardParams.insert("l", 10.0);
    handle->wizardParams.insert("b", 50.0);
    handle->wizardParams.insert("a", 10.0);
    handle->layer = this->layer;
    handle->processWizardInput();
    handle->calculate();

    QVector3D position_cont = matrix_rotation * QVector3D(l/2, 0.0, 0.0) + position;
    controller->wizardParams.insert("Position x", position_cont.x());
    controller->wizardParams.insert("Position y", position_cont.y());
    controller->wizardParams.insert("Position z", position_cont.z());
    controller->wizardParams.insert("Angle x", angle_x);
    controller->wizardParams.insert("Angle y", angle_y + 45.0);
    controller->wizardParams.insert("Angle z", angle_z);
    controller->wizardParams.insert("l", 1.0);
    controller->wizardParams.insert("d", d - s);
    controller->wizardParams.insert("s", (d - s)/2);
    controller->layer = this->layer;
    controller->processWizardInput();
    controller->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));

    this->boundingBox.enterVertices(mainPipe->boundingBox.getVertices());
    this->boundingBox.enterVertices(handle->boundingBox.getVertices());
    this->boundingBox.enterVertices(function->boundingBox.getVertices());




}

void CAD_air_pipeVolumetricFlowController::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();
    d = wizardParams.value("d").toDouble();

}

QMatrix4x4 CAD_air_pipeVolumetricFlowController::rotationOfFlange(quint8 num)
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
