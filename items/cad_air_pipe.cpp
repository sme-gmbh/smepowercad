#include "cad_air_pipe.h"
#include "itemdb.h"

CAD_air_pipe::CAD_air_pipe() : CADitem(CADitemTypes::Air_Pipe)
{
    pipe = new CAD_basic_pipe();
    subItems.append(pipe);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 1000.0);
    wizardParams.insert("d",  200.0);
    wizardParams.insert("s",   5.0);

    processWizardInput();
    calculate();
}

CAD_air_pipe::~CAD_air_pipe()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipe::flangable_items()
{
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

QImage CAD_air_pipe::wizardImage()
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

QString CAD_air_pipe::iconPath()
{
    return ":/icons/cad_air/cad_air_pipe.svg";
}

QString CAD_air_pipe::domain()
{
    return "Air";
}

QString CAD_air_pipe::description()
{
    return "Air|Pipe";
}

void CAD_air_pipe::calculate()
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

    pipe->wizardParams.insert("Position x", (position.x()));
    pipe->wizardParams.insert("Position y", (position.y()));
    pipe->wizardParams.insert("Position z", (position.z()));
    pipe->wizardParams.insert("Angle x", (angle_x));
    pipe->wizardParams.insert("Angle y", (angle_y));
    pipe->wizardParams.insert("Angle z", (angle_z));
    pipe->wizardParams.insert("l", (l));
    pipe->wizardParams.insert("d", (d));
    pipe->wizardParams.insert("s", (s));
    pipe->processWizardInput();
    pipe->calculate();

    this->snap_flanges.append(pipe->snap_flanges);
    this->snap_vertices.append(pipe->snap_vertices);
    this->boundingBox = pipe->boundingBox;
}

void CAD_air_pipe::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d = wizardParams.value("d").toDouble();
    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();

}
