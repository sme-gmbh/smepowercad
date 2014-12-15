#include "cad_air_pipefiredamper.h"

CAD_air_pipeFireDamper::CAD_air_pipeFireDamper() : CADitem(CADitem::Air_PipeFireDamper)
{
    this->description = "Air|Pipe fire damper";

    this->main_pipe = new CAD_basic_pipe();
    this->function = new CAD_basic_box();
    this->subItems.append(main_pipe);
    this->subItems.append(function);

    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("d", QVariant::fromValue(20.0));
    wizardParams.insert("ff", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(100.0));
    wizardParams.insert("l2", QVariant::fromValue(70.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));

    processWizardInput();
    calculate();
}

CAD_air_pipeFireDamper::~CAD_air_pipeFireDamper()
{

}

QList<CADitem::ItemType> CAD_air_pipeFireDamper::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Pipe);
    flangable_items.append(CADitem::Air_PipeEndCap);
    flangable_items.append(CADitem::Air_PipeReducer);
    flangable_items.append(CADitem::Air_PipeSilencer);
    flangable_items.append(CADitem::Air_PipeTeeConnector);
    flangable_items.append(CADitem::Air_PipeTurn);
    flangable_items.append(CADitem::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_pipeFireDamper::wizardImage()
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
    main_pipe->wizardParams.insert("Position x", QVariant::fromValue(position_mp.x()));
    main_pipe->wizardParams.insert("Position y", QVariant::fromValue(position_mp.y()));
    main_pipe->wizardParams.insert("Position z", QVariant::fromValue(position_mp.z()));
    main_pipe->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    main_pipe->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    main_pipe->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    main_pipe->wizardParams.insert("l", QVariant::fromValue(l));
    main_pipe->wizardParams.insert("d", QVariant::fromValue(d));
    main_pipe->wizardParams.insert("s", QVariant::fromValue(s));
    main_pipe->processWizardInput();
    main_pipe->calculate();

    QVector3D position_f = position + matrix_rotation * QVector3D(-l2 / 3, -0.6 * d, 0.0);
    function->wizardParams.insert("Position x", QVariant::fromValue(position_f.x()));
    function->wizardParams.insert("Position y", QVariant::fromValue(position_f.y()));
    function->wizardParams.insert("Position z", QVariant::fromValue(position_f.z()));
    function->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    function->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    function->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    function->wizardParams.insert("Size x", QVariant::fromValue(2 * l2 /3));
    function->wizardParams.insert("Size y", QVariant::fromValue(0.2 * d));
    function->wizardParams.insert("Size z", QVariant::fromValue(d));
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
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    s = wizardParams.value("s").toDouble();


}
