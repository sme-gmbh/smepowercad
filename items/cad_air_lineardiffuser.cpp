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

#include "cad_air_lineardiffuser.h"

CAD_air_lineardiffuser::CAD_air_lineardiffuser() : CADitem(CADitemTypes::Air_LinearDiffuser)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",    300.0);
    wizardParams.insert("a1",   100.0);
    wizardParams.insert("b",    200.0);
    wizardParams.insert("b1",   100.0);
    wizardParams.insert("l",   1000.0);
    wizardParams.insert("d",     50.0);
    wizardParams.insert("dist", 100.0);
    wizardParams.insert("n",    2);

    upper = new CAD_basic_box();
    lower = new CAD_basic_duct();
    this->subItems.append(lower);
    this->subItems.append(upper);

    processWizardInput();
    calculate();
}

CAD_air_lineardiffuser::~CAD_air_lineardiffuser()
{
//    foreach(CADitem* item, subItems)
//    {
//        delete item;
//    }
}

QList<CADitemTypes::ItemType> CAD_air_lineardiffuser::flangable_items(int flangeIndex)
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

QImage CAD_air_lineardiffuser::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_lineardiffuser::iconPath()
{
    return ":/icons/cad_air/cad_air_lineardiffuser.svg";
}

QString CAD_air_lineardiffuser::domain()
{
    return "Air";
}

QString CAD_air_lineardiffuser::description()
{
    return "Air|Linear Diffuser";
}

void CAD_air_lineardiffuser::calculate()
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

    QVector3D position_up = position + matrix_rotation * QVector3D(0.0, 0.0, a1 + a/2);
    upper->wizardParams.insert("Position x", position_up.x());
    upper->wizardParams.insert("Position y", position_up.y());
    upper->wizardParams.insert("Position z", position_up.z());
    upper->wizardParams.insert("Angle x", angle_x);
    upper->wizardParams.insert("Angle y", angle_y);
    upper->wizardParams.insert("Angle z", angle_z);
    upper->wizardParams.insert("l",  l);
    upper->wizardParams.insert("b", b);
    upper->wizardParams.insert("a",  a);
    upper->layer =  this->layer;
    upper->processWizardInput();
    upper->calculate();

    lower->wizardParams.insert("Position x", position.x());
    lower->wizardParams.insert("Position y", position.y());
    lower->wizardParams.insert("Position z", position.z());
    lower->wizardParams.insert("Angle x", angle_x);
    lower->wizardParams.insert("Angle y", angle_y);
    lower->wizardParams.insert("Angle z", angle_z);
    lower->wizardParams.insert("s",  0.05 * a);
    lower->wizardParams.insert("l", a1);
    lower->wizardParams.insert("b",  b1);
    lower->wizardParams.insert("a",  l);
    lower->layer =  this->layer;
    lower->processWizardInput();
    lower->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    lower->calculate();

    foreach(CADitem* item, subItems)
    {
        if(item == upper || item == lower)
            continue;
        delete (CAD_basic_box*)item;
    }
    subItems.clear();
    subItems.append(upper);
    subItems.append(lower);

    for(int i = -2; i <= 2; i++)
    {
        QVector3D pos = position + matrix_rotation * QVector3D(0.0, i*(b1-0.1*a)/5, a1/4);
        CAD_basic_box* box = new CAD_basic_box();
        this->subItems.append(box);
        box->wizardParams.insert("Position x", pos.x());
        box->wizardParams.insert("Position y", pos.y());
        box->wizardParams.insert("Position z", pos.z());
        box->wizardParams.insert("Angle x", angle_x);
        box->wizardParams.insert("Angle y", angle_y);
        box->wizardParams.insert("Angle z", angle_z);

        box->wizardParams.insert("l", l - 0.1*a);
        box->wizardParams.insert("b", b1/15);
        box->wizardParams.insert("a", a1/2);
        box->layer = this->layer;
        box->processWizardInput();
        box->calculate();
    }
    for(int i = 0; i < n; i++)
    {
        QVector3D pos = position + matrix_rotation * QVector3D(i * dist - (n-1)* dist/2, -b/2, a1 + a/2);
        CAD_basic_circle* p = new CAD_basic_circle();
        this->subItems.append(p);
        p->wizardParams.insert("Center x", pos.x());
        p->wizardParams.insert("Center y", pos.y());
        p->wizardParams.insert("Center z", pos.z());
        p->wizardParams.insert("Angle x", angle_x);
        p->wizardParams.insert("Angle y", angle_y);
        p->wizardParams.insert("Angle z", angle_z);

        p->wizardParams.insert("r", d / 2);
        p->layer = this->layer;
        p->processWizardInput();
        p->rotateAroundAxis(90.0, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
        p->calculate();
        this->snap_flanges.append(pos);
    }

    this->boundingBox.enterVertices(upper->boundingBox.getVertices());
    this->boundingBox.enterVertices(lower->boundingBox.getVertices());


}

void CAD_air_lineardiffuser::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    l = wizardParams.value("l").toDouble();
    a = wizardParams.value("a").toDouble();
    a1 = wizardParams.value("a1").toDouble();
    b = wizardParams.value("b").toDouble();
    b1 = wizardParams.value("b1").toDouble();
    n = wizardParams.value("n").toDouble();
    dist = wizardParams.value("dist").toDouble();
    d = wizardParams.value("d").toDouble();
}

QMatrix4x4 CAD_air_lineardiffuser::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else if(num >= 2 && num <= n + 1)
    {

        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
