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

#include "cad_air_canvasflange.h"
//#include "itemdb.h"

CAD_air_canvasFlange::CAD_air_canvasFlange() : CADitem(CADitemTypes::Air_CanvasFlange)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 200.0);
    wizardParams.insert("b", 300.0);
    wizardParams.insert("l", 50.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);
    wizardParams.insert("s",  1.0);

    this->flange_left = new CAD_basic_duct();
    this->flange_right = new CAD_basic_duct();
    this->canvas1 = new CAD_basic_duct();
    this->canvas2 = new CAD_basic_duct();
    this->canvas3 = new CAD_basic_duct();

    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(canvas1);
    this->subItems.append(canvas2);
    this->subItems.append(canvas3);

    processWizardInput();
    calculate();
}

CAD_air_canvasFlange::~CAD_air_canvasFlange()
{

}

QList<CADitemTypes::ItemType> CAD_air_canvasFlange::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_CanvasFlange);
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctBaffleSilencer);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRect);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Fan);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_Humidifier);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_canvasFlange::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_canvasFlange::iconPath()
{
    return ":/icons/cad_air/cad_air_canvasflange.svg";
}

QString CAD_air_canvasFlange::domain()
{
    return "Air";
}

QString CAD_air_canvasFlange::description()
{
    return "Air|Canvas flange";
}

void CAD_air_canvasFlange::calculate()
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

    flange_left->wizardParams.insert("Position x", (position.x()));
    flange_left->wizardParams.insert("Position y", (position.y()));
    flange_left->wizardParams.insert("Position z", (position.z()));
    flange_left->wizardParams.insert("Angle x", (angle_x));
    flange_left->wizardParams.insert("Angle y", (angle_y));
    flange_left->wizardParams.insert("Angle z", (angle_z));
    flange_left->wizardParams.insert("l", (fe));
    flange_left->wizardParams.insert("b", (b+2*ff));
    flange_left->wizardParams.insert("a", (a+2*ff));
    flange_left->wizardParams.insert("s", (ff + s));
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_fr = position + matrix_rotation * QVector3D(l - fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", (position_fr.x()));
    flange_right->wizardParams.insert("Position y", (position_fr.y()));
    flange_right->wizardParams.insert("Position z", (position_fr.z()));
    flange_right->wizardParams.insert("Angle x", (angle_x));
    flange_right->wizardParams.insert("Angle y", (angle_y));
    flange_right->wizardParams.insert("Angle z", (angle_z));
    flange_right->wizardParams.insert("l", (fe));
    flange_right->wizardParams.insert("b", (b + 2 * ff));
    flange_right->wizardParams.insert("a", (a + 2 * ff));
    flange_right->wizardParams.insert("s", (ff + s));
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_ca1 = position + matrix_rotation * QVector3D(fe, 0.0, 0.0);
    canvas1->wizardParams.insert("Position x", (position_ca1.x()));
    canvas1->wizardParams.insert("Position y", (position_ca1.y()));
    canvas1->wizardParams.insert("Position z", (position_ca1.z()));
    canvas1->wizardParams.insert("Angle x", (angle_x));
    canvas1->wizardParams.insert("Angle y", (angle_y));
    canvas1->wizardParams.insert("Angle z", (angle_z));
    canvas1->wizardParams.insert("l", ((l - 2 * fe) / 3));
    canvas1->wizardParams.insert("b", (b));
    canvas1->wizardParams.insert("a", (a));
    canvas1->wizardParams.insert("s", (s));
    canvas1->layer = this->layer;
    canvas1->processWizardInput();
    canvas1->calculate();

    QVector3D position_ca2 = position + matrix_rotation * QVector3D((fe + l) / 3, 0.0, 0.0);
    canvas2->wizardParams.insert("Position x", (position_ca2.x()));
    canvas2->wizardParams.insert("Position y", (position_ca2.y()));
    canvas2->wizardParams.insert("Position z", (position_ca2.z()));
    canvas2->wizardParams.insert("Angle x", (angle_x));
    canvas2->wizardParams.insert("Angle y", (angle_y));
    canvas2->wizardParams.insert("Angle z", (angle_z));
    canvas2->wizardParams.insert("l", ((l - 2 * fe) / 3));
    canvas2->wizardParams.insert("b", (b - 2 * s));
    canvas2->wizardParams.insert("a", (a - 2 * s));
    canvas2->wizardParams.insert("s", (s));
    canvas2->layer = this->layer;
    canvas2->processWizardInput();
    canvas2->calculate();

    QVector3D position_ca3 = position + matrix_rotation * QVector3D((2 * l - fe) / 3, 0.0, 0.0);
    canvas3->wizardParams.insert("Position x", (position_ca3.x()));
    canvas3->wizardParams.insert("Position y", (position_ca3.y()));
    canvas3->wizardParams.insert("Position z", (position_ca3.z()));
    canvas3->wizardParams.insert("Angle x", (angle_x));
    canvas3->wizardParams.insert("Angle y", (angle_y));
    canvas3->wizardParams.insert("Angle z", (angle_z));
    canvas3->wizardParams.insert("l", ((l - 2 * fe) / 3));
    canvas3->wizardParams.insert("b", (b));
    canvas3->wizardParams.insert("a", (a));
    canvas3->wizardParams.insert("s", (s));
    canvas3->layer = this->layer;
    canvas3->processWizardInput();
    canvas3->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0,0.0));

    boundingBox.enterVertices(flange_left->boundingBox.getVertices());
    boundingBox.enterVertices(flange_right->boundingBox.getVertices());
}

void CAD_air_canvasFlange::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    this->b = wizardParams.value("b").toDouble();
    this->a = wizardParams.value("a").toDouble();
    this->l = wizardParams.value("l").toDouble();
    this->ff = wizardParams.value("ff").toDouble();
    this->fe = wizardParams.value("fe").toDouble();
    this->s = wizardParams.value("s").toDouble();

}

QMatrix4x4 CAD_air_canvasFlange::rotationOfFlange(quint8 num)
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
