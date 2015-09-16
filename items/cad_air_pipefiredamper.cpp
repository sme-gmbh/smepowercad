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

#include "cad_air_pipefiredamper.h"

CAD_air_pipeFireDamper::CAD_air_pipeFireDamper() : CADitem(CADitemTypes::Air_PipeFireDamper)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 1000.0);
    wizardParams.insert("l2", 700.0);
    wizardParams.insert("d",  200.0);
    wizardParams.insert("s",    1.0);

    this->main_pipe = new CAD_basic_pipe();
    this->function = new CAD_basic_box();
    this->subItems.append(main_pipe);
    this->subItems.append(function);

    processWizardInput();
    calculate();
}

CAD_air_pipeFireDamper::~CAD_air_pipeFireDamper()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeFireDamper::flangable_items(int flangeIndex)
{
    QList<CADitemTypes::ItemType> flangable_items;
    if(flangeIndex >= 2)
    {
        flangable_items.append(CADitemTypes::Air_Pipe);
        flangable_items.append(CADitemTypes::Air_PipeBranch);
        flangable_items.append(CADitemTypes::Air_PipeSilencer);
        flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
        flangable_items.append(CADitemTypes::Air_PipeFireDamper);
        flangable_items.append(CADitemTypes::Air_PipeEndCap);
        flangable_items.append(CADitemTypes::Air_PipeReducer);
        flangable_items.append(CADitemTypes::Air_PipeTurn);
    }
    return flangable_items;
}

QImage CAD_air_pipeFireDamper::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_pipeFireDamper::iconPath()
{
    return ":/icons/cad_air/cad_air_pipefiredamper.svg";
}

QString CAD_air_pipeFireDamper::domain()
{
    return "Air";
}

QString CAD_air_pipeFireDamper::description()
{
    return "Air|Pipe fire damper";
}

void CAD_air_pipeFireDamper::calculate()
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
    this->snap_flanges.append(position + matrix_rotation * QVector3D(-l2, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l-l2, 0.0, 0.0));

    QVector3D position_mp = position + matrix_rotation *  QVector3D(-l2, 0.0, 0.0);
    main_pipe->wizardParams.insert("Position x", (position_mp.x()));
    main_pipe->wizardParams.insert("Position y", (position_mp.y()));
    main_pipe->wizardParams.insert("Position z", (position_mp.z()));
    main_pipe->wizardParams.insert("Angle x", (angle_x));
    main_pipe->wizardParams.insert("Angle y", (angle_y));
    main_pipe->wizardParams.insert("Angle z", (angle_z));
    main_pipe->wizardParams.insert("l", (l));
    main_pipe->wizardParams.insert("d", (d));
    main_pipe->wizardParams.insert("s", (s));
    main_pipe->processWizardInput();
    main_pipe->calculate();

    QVector3D position_f = position + matrix_rotation * QVector3D(-l2 / 3, 0.6 * d, 0.0);
    function->wizardParams.insert("Position x", (position_f.x()));
    function->wizardParams.insert("Position y", (position_f.y()));
    function->wizardParams.insert("Position z", (position_f.z()));
    function->wizardParams.insert("Angle x", (angle_x));
    function->wizardParams.insert("Angle y", (angle_y));
    function->wizardParams.insert("Angle z", (angle_z));
    function->wizardParams.insert("l", (2 * l2 /3));
    function->wizardParams.insert("b", (0.2 * d));
    function->wizardParams.insert("a", (d));
    function->processWizardInput();
    function->calculate();

    this->boundingBox = main_pipe->boundingBox;
    foreach(QVector3D vertex, function->snap_vertices)
        this->boundingBox.enterVertex(vertex);



}

void CAD_air_pipeFireDamper::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    s = wizardParams.value("s").toDouble();


}

QMatrix4x4 CAD_air_pipeFireDamper::rotationOfFlange(quint8 num)
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
