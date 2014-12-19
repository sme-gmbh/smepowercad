#include "caditem.h"
#include "layer.h"

CADitem::CADitem(ItemType type)
{
    this->type = type;
    this->layer = NULL;
    this->highlight = false;
    this->selected = false;
    this->index = 0;
    this->color_brush = Qt::transparent;
    this->color_pen = Qt::transparent;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
    position = QVector3D(0.0, 0.0, 0.0);
    matrix_rotation.setToIdentity();
}

QColor CADitem::getColorPen()
{
    QColor color_pen_tmp = this->color_pen;

    if (color_pen_tmp == Qt::transparent)   // BYLAYER
    {
        color_pen_tmp = layer->pen.color();
    }
    else if (color_pen_tmp.value() < 50)
        color_pen_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_pen_tmp.value() > 127)
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() - 100);
        }
        else
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() + 100);
        }
    }

    return color_pen_tmp;
}

QColor CADitem::getColorBrush()
{
    QColor color_brush_tmp = this->color_brush;

    if (color_brush_tmp == Qt::transparent)   // BYLAYER
    {
        color_brush_tmp = layer->brush.color();
    }
    else if (color_brush_tmp.value() < 50)
        color_brush_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_brush_tmp.value() > 127)
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() - 100);
        }
        else
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() + 100);
        }
    }

    return color_brush_tmp;
}

void CADitem::setLayer(Layer *layer)
{
    this->layer = layer;
    this->setLayer_processItems(this->subItems);
}

void CADitem::setLayer_processItems(QList<CADitem *> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->layer = this->layer;
        this->setLayer_processItems(item->subItems);
    }
}

void CADitem::setID(quint64 id)
{
    this->id = id;
    this->setID_processItems(this->subItems);
}

void CADitem::setID_processItems(QList<CADitem*> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->id = this->id;
        this->setID_processItems(item->subItems);
    }
}

void CADitem::serialOut(QByteArray* out)
{
    QList<QString> keys = wizardParams.keys();

    *out += "Item" + QString().setNum(this->type).toUtf8() + ";" + layer->name.toUtf8() + ";" + QString().setNum(id) + ";";

    foreach (QString key, keys)
    {
        *out += key + ":";
        QVariant value = wizardParams.value(key);
        if (value.type() == QVariant::Double)
            *out += value.toString();
        *out += ";";
    }

    *out += "\n";
}

bool CADitem::serialIn(QByteArray* in)
{

}

CADitem::ItemType CADitem::getType()
{
    return type;
}

// Item switch template
/*
 *
 * switch (type)
    {
    case CADitem::Air_CanvasFlange:
    {
        break;
    }
    case CADitem::Air_Duct:
    {
        break;
    }
    case CADitem::Air_DuctBaffleSilencer:
    {
        break;
    }
    case CADitem::Air_DuctEndPlate:
    {
        break;
    }
    case CADitem::Air_DuctFireDamper:
    {
        break;
    }
    case CADitem::Air_DuctFireResistant:
    {
        break;
    }
    case CADitem::Air_DuctTeeConnector:
    {
        break;
    }
    case CADitem::Air_DuctTransition:
    {
        break;
    }
    case CADitem::Air_DuctTransitionRectRound:
    {
        break;
    }
    case CADitem::Air_DuctTurn:
    {
        break;
    }
    case CADitem::Air_DuctVolumetricFlowController:
    {
        break;
    }
    case CADitem::Air_DuctYpiece:
    {
        break;
    }
    case CADitem::Air_EmptyCabinet:
    {
        break;
    }
    case CADitem::Air_EquipmentFrame:
    {
        break;
    }
    case CADitem::Air_Fan:
    {
        break;
    }
    case CADitem::Air_Filter:
    {
        break;
    }
    case CADitem::Air_HeatExchangerAirAir:
    {
        break;
    }
    case CADitem::Air_HeatExchangerWaterAir:
    {
        break;
    }
    case CADitem::Air_Humidifier:
    {
        break;
    }
    case CADitem::Air_MultiLeafDamper:
    {
        break;
    }
    case CADitem::Air_Pipe:
    {
        break;
    }
    case CADitem::Air_PipeEndCap:
    {
        break;
    }
    case CADitem::Air_PipeFireDamper:
    {
        break;
    }
    case CADitem::Air_PipeFireDamper:
    {
        break;
    }
    case CADitem::Air_PipeReducer:
    {
        break;
    }
    case CADitem::Air_PipeSilencer:
    {
        break;
    }
    case CADitem::Air_PipeTeeConnector:
    {
        break;
    }
    case CADitem::Air_PipeTurn:
    {
        break;
    }
    case CADitem::Air_PipeVolumetricFlowController:
    {
        break;
    }
    case CADitem::Air_PressureReliefDamper:
    {
        break;
    }
    case CADitem::Air_ThrottleValve:
    {
        break;
    }

    case CADitem::Arch_Beam:
    {
        break;
    }
    case CADitem::Arch_BlockOut:
    {
        break;
    }
    case CADitem::Arch_BoredPile:
    {
        break;
    }
    case CADitem::Arch_Door:
    {
        break;
    }
    case CADitem::Arch_Foundation:
    {
        break;
    }
    case CADitem::Arch_Grating:
    {
        break;
    }
    case CADitem::Arch_LevelSlab:
    {
        break;
    }
    case CADitem::Arch_Support:
    {
        break;
    }
    case CADitem::Arch_Wall_loadBearing:
    {
        break;
    }
    case CADitem::Arch_Wall_nonLoadBearing:
    {
        break;
    }
    case CADitem::Arch_Window:
    {
        break;
    }
    case CADitem::Basic_Arc:
    {
        break;
    }
    case CADitem::Basic_Box:
    {
        break;
    }
    case CADitem::Basic_Circle:
    {
        break;
    }
    case CADitem::Basic_Cylinder:
    {
        break;
    }
    case CADitem::Basic_Duct:
    {
        break;
    }
    case CADitem::Basic_Face:
    {
        break;
    }
    case CADitem::Basic_Line:
    {
        break;
    }
    case CADitem::Basic_Pipe:
    {
        break;
    }
    case CADitem::Basic_Plane:
    {
        break;
    }
    case CADitem::Basic_Point:
    {
        break;
    }
    case CADitem::Basic_Polyline:
    {
        break;
    }
    case CADitem::Basic_Sphere:
    {
        break;
    }
    case CADitem::Basic_Turn:
    {
        break;
    }

    case CADitem::Electrical_Cabinet:
    {
        break;
    }
    case CADitem::Electrical_CableTray:
    {
        break;
    }

    case CADitem::HeatCool_Adjustvalve:
    {
        break;
    }
    case CADitem::HeatCool_BallValve:
    {
        break;
    }
    case CADitem::HeatCool_Boiler:
    {
        break;
    }
    case CADitem::HeatCool_ButterflyValve:
    {
        break;
    }
    case CADitem::HeatCool_Chiller:
    {
        break;
    }
    case CADitem::HeatCool_Controlvalve:
    {
        break;
    }
    case CADitem::HeatCool_CoolingTower:
    {
        break;
    }
    case CADitem::HeatCool_ExpansionChamber:
    {
        break;
    }
    case CADitem::HeatCool_Filter:
    {
        break;
    }
    case CADitem::HeatCool_Flange:
    {
        break;
    }
    case CADitem::HeatCool_Flowmeter:
    {
        break;
    }
    case CADitem::HeatCool_HeatExchanger:
    {
        break;
    }
    case CADitem::HeatCool_Pipe:
    {
        break;
    }
    case CADitem::HeatCool_PipeEndCap:
    {
        break;
    }
    case CADitem::HeatCool_PipeReducer:
    {
        break;
    }
    case CADitem::HeatCool_PipeTeeConnector:
    {
        break;
    }
    case CADitem::HeatCool_PipeTurn:
    {
        break;
    }
    case CADitem::HeatCool_Pump:
    {
        break;
    }
    case CADitem::HeatCool_Radiator:
    {
        break;
    }
    case CADitem::HeatCool_SafetyValve:
    {
        break;
    }
    case CADitem::HeatCool_Sensor:
    {
        break;
    }
    case CADitem::HeatCool_StorageBoiler:
    {
        break;
    }
    case CADitem::HeatCool_WaterHeater:
    {
        break;
    }

    case CADitem::Sanitary_ElectricWaterHeater:
    {
        break;
    }
    case CADitem::Sanitary_EmergencyEyeShower:
    {
        break;
    }
    case CADitem::Sanitary_EmergencyShower:
    {
        break;
    }
    case CADitem::Sanitary_Flange:
    {
        break;
    }
    case CADitem::Sanitary_LiftingUnit:
    {
        break;
    }
    case CADitem::Sanitary_Pipe:
    {
        break;
    }
    case CADitem::Sanitary_PipeEndCap:
    {
        break;
    }
    case CADitem::Sanitary_PipeReducer:
    {
        break;
    }
    case CADitem::Sanitary_PipeTeeConnector:
    {
        break;
    }
    case CADitem::Sanitary_PipeTurn:
    {
        break;
    }
    case CADitem::Sanitary_Shower:
    {
        break;
    }
    case CADitem::Sanitary_Sink:
    {
        break;
    }
    case CADitem::Sanitary_WashBasin:
    {
        break;
    }

    default:
        break;
    }
 */
