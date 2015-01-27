#include "cad_heatcool_pipereducer.h"
#include "itemdb.h"

CAD_heatcool_pipeReducer::CAD_heatcool_pipeReducer() : CADitem(CADitemTypes::HeatCool_PipeReducer)
{
    reducer = new CAD_basic_pipe();
    endcap_bottom = new CAD_basic_pipe();
    endcap_top = new CAD_basic_pipe();
    this->subItems.append(reducer);
    this->subItems.append(endcap_bottom);
    this->subItems.append(endcap_top);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l",   400.0);
    wizardParams.insert("l1",   50.0);
    wizardParams.insert("l2",   50.0);
    wizardParams.insert("d",   300.0);
    wizardParams.insert("do",  400.0);
    wizardParams.insert("d2",  200.0);
    wizardParams.insert("do2", 300.0);
    wizardParams.insert("f",     0.0);
    wizardParams.insert("s",    10.0);

    processWizardInput();
    calculate();
}

CAD_heatcool_pipeReducer::~CAD_heatcool_pipeReducer()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_pipeReducer::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Adjustvalve);
    flangable_items.append(CADitemTypes::HeatCool_BallValve);
    flangable_items.append(CADitemTypes::HeatCool_Boiler);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValve);
    flangable_items.append(CADitemTypes::HeatCool_Chiller);
    flangable_items.append(CADitemTypes::HeatCool_Controlvalve);
    flangable_items.append(CADitemTypes::HeatCool_CoolingTower);
    flangable_items.append(CADitemTypes::HeatCool_ExpansionChamber);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_Flange);
    flangable_items.append(CADitemTypes::HeatCool_Flowmeter);
    flangable_items.append(CADitemTypes::HeatCool_HeatExchanger);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_Pump);
    flangable_items.append(CADitemTypes::HeatCool_Radiator);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Sensor);
    flangable_items.append(CADitemTypes::HeatCool_StorageBoiler);
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_pipeReducer::wizardImage()
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

QString CAD_heatcool_pipeReducer::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_pipereducer.svg";
}

QString CAD_heatcool_pipeReducer::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_pipeReducer::description()
{
    return "Heat/Cool|Pipe reducer";
}

void CAD_heatcool_pipeReducer::calculate()
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

    reducer->wizardParams.insert("Position x", (position.x()));
    reducer->wizardParams.insert("Position y", (position.y()));
    reducer->wizardParams.insert("Position z", (position.z()));
    reducer->wizardParams.insert("Angle x", (angle_x));
    reducer->wizardParams.insert("Angle y", (angle_y));
    reducer->wizardParams.insert("Angle z", (angle_z));
    reducer->wizardParams.insert("l", (l));
    reducer->wizardParams.insert("d", (d));
    reducer->wizardParams.insert("s", (s));
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
    QVector3D vertices[64];
    int index = 0;

    for (qreal i=0.0; i < 1.0; i += 0.0625)    // 16 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;
        QVector3D pos_bot = position + matrix_rotation * QVector3D(l1, 0.0, 0.0);

        //outer bottom
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * d_o/2, cos(angle) * d_o/2);
        linePos += pos_bot;
        vertices[index] = linePos;
        index++;
        //inner bottom
        vertices[index] = linePos + (pos_bot - linePos).normalized() * ((d_o-d)/2 + s);
        index++;
        QVector3D pos_top = position + matrix_rotation * QVector3D(l - l2, f, 0.0);
        //outer top
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * d_o2/2, cos(angle) * d_o2/2);
        linePos += pos_top;
        vertices[index] = linePos;
        index++;
        //inner top
        vertices[index] = linePos + (pos_top - linePos).normalized() * ((d_o2-d2)/2 + s);
        index++;
    }

    reducer->arrayBufVertices.bind();
    reducer->arrayBufVertices.allocate(vertices, sizeof(vertices));

    endcap_bottom->wizardParams.insert("Position x", (position.x()));
    endcap_bottom->wizardParams.insert("Position y", (position.y()));
    endcap_bottom->wizardParams.insert("Position z", (position.z()));
    endcap_bottom->wizardParams.insert("Angle x", (angle_x));
    endcap_bottom->wizardParams.insert("Angle y", (angle_y));
    endcap_bottom->wizardParams.insert("Angle z", (angle_z));
    endcap_bottom->wizardParams.insert("l", (l1));
    endcap_bottom->wizardParams.insert("d", (d_o));
    endcap_bottom->wizardParams.insert("s", (d_o-d)/2 + s);
    endcap_bottom->layer = this->layer;
    endcap_bottom->processWizardInput();
    endcap_bottom->calculate();

    QVector3D position_top = position + matrix_rotation * QVector3D(l - l2, f, 0.0);
    endcap_top->wizardParams.insert("Position x", (position_top.x()));
    endcap_top->wizardParams.insert("Position y", (position_top.y()));
    endcap_top->wizardParams.insert("Position z", (position_top.z()));
    endcap_top->wizardParams.insert("Angle x", (angle_x));
    endcap_top->wizardParams.insert("Angle y", (angle_y));
    endcap_top->wizardParams.insert("Angle z", (angle_z));
    endcap_top->wizardParams.insert("l", (l2));
    endcap_top->wizardParams.insert("d", (d_o2));
    endcap_top->wizardParams.insert("s", (d_o2-d2)/2 + s);
    endcap_top->layer = this->layer;
    endcap_top->processWizardInput();
    endcap_top->calculate();

    this->snap_flanges.append(endcap_top->snap_flanges.at(1));

    this->boundingBox = reducer->boundingBox;
}

void CAD_heatcool_pipeReducer::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    d = wizardParams.value("d").toDouble();
    d_o = wizardParams.value("do").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    d_o2 = wizardParams.value("do2").toDouble();
    f = wizardParams.value("f").toDouble();
    s = wizardParams.value("s").toDouble();
}
