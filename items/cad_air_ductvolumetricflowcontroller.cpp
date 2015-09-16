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

#include "cad_air_ductvolumetricflowcontroller.h"

CAD_air_ductVolumetricFlowController::CAD_air_ductVolumetricFlowController() : CADitem(CADitemTypes::Air_DuctVolumetricFlowController)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",    1.0);

    this->duct = new CAD_air_duct();
    this->flap = new CAD_basic_box();
    this->function = new CAD_basic_box();
    this->subItems.append(duct);
    this->subItems.append(flap);
    this->subItems.append(function);

    processWizardInput();
    calculate();
}

CAD_air_ductVolumetricFlowController::~CAD_air_ductVolumetricFlowController()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductVolumetricFlowController::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRect);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductVolumetricFlowController::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductVolumetricFlowController::iconPath()
{
    return ":/icons/cad_air/cad_air_ductvolumetricflowcontroller.svg";
}

QString CAD_air_ductVolumetricFlowController::domain()
{
    return "Air";
}

QString CAD_air_ductVolumetricFlowController::description()
{
    return "Air|Duct volumetric flow controller";
}

void CAD_air_ductVolumetricFlowController::calculate()
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

    duct->wizardParams.insert("Position x", (position.x()));
    duct->wizardParams.insert("Position y", (position.y()));
    duct->wizardParams.insert("Position z", (position.z()));
    duct->wizardParams.insert("Angle x", (angle_x));
    duct->wizardParams.insert("Angle y", (angle_y));
    duct->wizardParams.insert("Angle z", (angle_z));
    duct->wizardParams.insert("s", (s));
    duct->wizardParams.insert("l", (l));
    duct->wizardParams.insert("b", (b));
    duct->wizardParams.insert("a", (a));
    duct->wizardParams.insert("ff", (ff));
    duct->wizardParams.insert("fe", (fe));
    duct->processWizardInput();
    duct->calculate();

    QVector3D position_f = position + matrix_rotation * QVector3D(l / 2, 0.6 * b, 0.0);
    function->wizardParams.insert("Position x", (position_f.x()));
    function->wizardParams.insert("Position y", (position_f.y()));
    function->wizardParams.insert("Position z", (position_f.z()));
    function->wizardParams.insert("Angle x", (angle_x));
    function->wizardParams.insert("Angle y", (angle_y));
    function->wizardParams.insert("Angle z", (angle_z));
    function->wizardParams.insert("l", (0.3 * l));
    function->wizardParams.insert("b", (0.2 * b));
    function->wizardParams.insert("a", (0.5 * a));
    function->processWizardInput();
    function->calculate();

    QVector3D position_fl = position + matrix_rotation * QVector3D(l / 2, 0.0, 0.0);
    flap->wizardParams.insert("Position x", (position_fl.x()));
    flap->wizardParams.insert("Position y", (position_fl.y()));
    flap->wizardParams.insert("Position z", (position_fl.z()));
    flap->wizardParams.insert("Angle x", (angle_x));
    flap->wizardParams.insert("Angle y", (angle_y));
    flap->wizardParams.insert("Angle z", (angle_z));
    flap->wizardParams.insert("l", (a));
    flap->wizardParams.insert("b", (b - 2 * s));
    flap->wizardParams.insert("a", (0.1 * a));
    flap->rotateAroundAxis(45.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    flap->processWizardInput();
    flap->calculate();

    this->snap_flanges = this->duct->snap_flanges;
    this->snap_vertices = this->duct->snap_vertices;

    foreach(CADitem* item, subItems)
    {
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    }
}

void CAD_air_ductVolumetricFlowController::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();

}

QMatrix4x4 CAD_air_ductVolumetricFlowController::rotationOfFlange(quint8 num)
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
