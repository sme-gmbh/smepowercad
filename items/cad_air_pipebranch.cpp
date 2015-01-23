#include "cad_air_pipebranch.h"
#include "itemdb.h"

CAD_air_pipeBranch::CAD_air_pipeBranch() : CADitem(CADitemTypes::Air_PipeBranch)
{
    wizardParams.insert("Position x", (0.0));
    wizardParams.insert("Position y", (0.0));
    wizardParams.insert("Position z", (0.0));
    wizardParams.insert("Angle x", (0.0));
    wizardParams.insert("Angle y", (0.0));
    wizardParams.insert("Angle z", (0.0));

    wizardParams.insert("d", (20.0));
    wizardParams.insert("d2", (20.0));
    wizardParams.insert("s", (1.0));
    wizardParams.insert("l", (100.0));
    wizardParams.insert("l1", (50));
    wizardParams.insert("l2", (50.0));
    wizardParams.insert("alpha", (30.0));

    this->pipe = new CAD_basic_pipe();
    this->branch = new CAD_basic_pipe();
    this->subItems.append(pipe);
    this->subItems.append(branch);

    processWizardInput();
    calculate();
}

CAD_air_pipeBranch::~CAD_air_pipeBranch()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeBranch::flangable_items()
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

QImage CAD_air_pipeBranch::wizardImage()
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

QString CAD_air_pipeBranch::iconPath()
{
    return ":/icons/cad_air/cad_air_pipebranch.svg";
}

QString CAD_air_pipeBranch::domain()
{
    return "Air";
}

QString CAD_air_pipeBranch::description()
{
    return "Air|Pipe branch";
}

void CAD_air_pipeBranch::calculate()
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

    QVector3D position_br = position + matrix_rotation * QVector3D(l1, 0.0, 0.0);
    branch->wizardParams.insert("Position x", (position_br.x()));
    branch->wizardParams.insert("Position y", (position_br.y()));
    branch->wizardParams.insert("Position z", (position_br.z()));
    branch->wizardParams.insert("Angle x", (angle_x));
    branch->wizardParams.insert("Angle y", (angle_y));
    branch->wizardParams.insert("Angle z", (angle_z+alpha));
    branch->wizardParams.insert("l", (l2));
    branch->wizardParams.insert("d", (d2));
    branch->wizardParams.insert("s", (s));
    branch->processWizardInput();
    branch->calculate();

    this->snap_flanges.append(pipe->snap_flanges);
    this->snap_flanges.append(branch->snap_flanges.at(0));
    this->snap_vertices.append(pipe->snap_vertices);

    this->boundingBox = pipe->boundingBox;
    this->boundingBox.enterVertex(position + matrix_rotation * QVector3D(sin(alpha) * l2 + cos(alpha) * d2, cos(alpha) * l2 - sin(alpha) * d2, 0.0));
}

void CAD_air_pipeBranch::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d = wizardParams.value("d").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
}
