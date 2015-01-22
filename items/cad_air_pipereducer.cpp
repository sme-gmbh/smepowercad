#include "cad_air_pipereducer.h"
#include "itemdb.h"

CAD_air_pipeReducer::CAD_air_pipeReducer() : CADitem(CADitemTypes::Air_PipeReducer)
{
    this->reducer = new CAD_basic_pipe();
    this->subItems.append(reducer);
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    wizardParams.insert("d", QVariant::fromValue(30.0));
    wizardParams.insert("d2", QVariant::fromValue(20.0));
    wizardParams.insert("s", QVariant::fromValue(1.0));
    wizardParams.insert("l", QVariant::fromValue(40.0));



    processWizardInput();
    calculate();
}

CAD_air_pipeReducer::~CAD_air_pipeReducer()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeReducer::flangable_items()
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

QImage CAD_air_pipeReducer::wizardImage()
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

QString CAD_air_pipeReducer::iconPath()
{
    return ":/icons/cad_air/cad_air_pipereducer.svg";
}

QString CAD_air_pipeReducer::domain()
{
    return "Air";
}

QString CAD_air_pipeReducer::description()
{
    return "Air|Pipe reducer";
}

void CAD_air_pipeReducer::calculate()
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
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));

    reducer->wizardParams.insert("Position x", QVariant::fromValue(position.x()));
    reducer->wizardParams.insert("Position y", QVariant::fromValue(position.y()));
    reducer->wizardParams.insert("Position z", QVariant::fromValue(position.z()));
    reducer->wizardParams.insert("Angle x", QVariant::fromValue(angle_x));
    reducer->wizardParams.insert("Angle y", QVariant::fromValue(angle_y));
    reducer->wizardParams.insert("Angle z", QVariant::fromValue(angle_z));
    reducer->wizardParams.insert("l", QVariant::fromValue(l));
    reducer->wizardParams.insert("d", QVariant::fromValue(d));
    reducer->wizardParams.insert("s", QVariant::fromValue(s));
    reducer->layer = this->layer;
    reducer->processWizardInput();
    reducer->calculate();

    reducer->snap_flanges.clear();
    reducer->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));

    qreal radius = d/2;
    qreal radius2 = d2/2;
    reducer->vertices_inner_bottom.clear();
    reducer->vertices_inner_top.clear();
    reducer->vertices_outer_top.clear();
    reducer->vertices_outer_bottom.clear();
    QVector3D vertices[200];
    int index = 0;

    for (qreal i=0.0; i < 1.0; i += 0.02)    // 50 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;

        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += position;
        vertices[index] = linePos + (position - linePos).normalized() * s;
        index++;
        //        reducer->vertices_outer_bottom.append(linePos);
        vertices[index] = linePos;
        index++;
        //        reducer->vertices_inner_bottom.append(linePos + (position - linePos).normalized() * s);
        QVector3D pos_top = position + matrix_rotation * QVector3D(l, 0.0, 0.0);
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius2, cos(angle) * radius2);
        linePos += pos_top;
        vertices[index] = linePos;
        index++;
        //        reducer->vertices_outer_top.append(linePos);
        vertices[index] = linePos + (pos_top - linePos).normalized() * s;
        index++;
        //        reducer->vertices_inner_top.append(linePos + (pos_top - linePos).normalized() * s);
    }

    reducer->arrayBufVertices.bind();
    reducer->arrayBufVertices.allocate(vertices, sizeof(vertices));
    this->boundingBox = reducer->boundingBox;
}

void CAD_air_pipeReducer::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    d = wizardParams.value("d").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    l = wizardParams.value("l").toDouble();

}


