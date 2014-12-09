#include "cad_air_pipe.h"

CAD_air_pipe::CAD_air_pipe() : CADitem(CADitem::Air_Pipe)
{
    pipe = new CAD_basic_pipe();
    subItems.append(pipe);
    this->description = "Air|Pipe";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    wizardParams.insert("d", QVariant::fromValue(20.0));
    wizardParams.insert("s", QVariant::fromValue(0.5));
    wizardParams.insert("l", QVariant::fromValue(100.0));

    processWizardInput();
    calculate();
}

CAD_air_pipe::~CAD_air_pipe()
{

}

QList<CADitem::ItemType> CAD_air_pipe::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Pipe);
    flangable_items.append(CADitem::Air_PipeEndCap);
    flangable_items.append(CADitem::Air_PipeFireDamper);
    flangable_items.append(CADitem::Air_PipeReducer);
    flangable_items.append(CADitem::Air_PipeSilencer);
    flangable_items.append(CADitem::Air_PipeTeeConnector);
    flangable_items.append(CADitem::Air_PipeTurn);
    flangable_items.append(CADitem::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_pipe::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
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

    pipe->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    pipe->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    pipe->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    pipe->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    pipe->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    pipe->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    pipe->wizardParams.insert("l", QVariant::fromValue(l));
    pipe->wizardParams.insert("d", QVariant::fromValue(d));
    pipe->wizardParams.insert("s", QVariant::fromValue(s));
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
