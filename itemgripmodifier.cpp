#include "itemgripmodifier.h"
#include "ui_itemgripmodifier.h"

#include <QToolButton>

ItemGripModifier::ItemGripModifier(ItemDB *itemDB, ItemWizard *itemWizard, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemGripModifier)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->hide();
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->item = NULL;
    this->activeGrip = Grip_None;

    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
}

ItemGripModifier::~ItemGripModifier()
{
    delete ui;
}

void ItemGripModifier::setItem(CADitem *item)
{
    this->item = item;
}

CADitem *ItemGripModifier::getItem()
{
    return this->item;
}

void ItemGripModifier::activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos)
{
    this->scenePos = scenePos;
    this->activeGrip = gripType;

    switch (gripType)
    {
    case Grip_Append:
    {
        showAppendBox();
        break;
    }
    case Grip_Copy:
    {
        break;
    }
    case Grip_CopyMulti:
    {
        showCopyMultiBox();
        break;
    }
    case Grip_Length:
    {
        break;
    }
    case Grip_Move:
    {
        break;
    }
    case Grip_Rotate_X:
    {
        break;
    }
    case Grip_Rotate_Y:
    {
        break;
    }
    case Grip_Rotate_Z:
    {
        break;
    }
    }
}

void ItemGripModifier::finishGrip()
{
    this->item = NULL;
    this->activeGrip = Grip_None;
    hide();
    deleteWdgs(ui->gridLayout);
}

ItemGripModifier::ItemGripType ItemGripModifier::getActiveGrip()
{
    return this->activeGrip;
}

void ItemGripModifier::slot_rejected()
{
    this->item = NULL;
    this->hide();
    deleteWdgs(ui->gridLayout);
}

void ItemGripModifier::slot_button_clicked()
{
    QToolButton* button = (QToolButton*)this->sender();
    CADitem::ItemType type = (CADitem::ItemType)button->property("ItemType").toInt();
    int flangeIndex = this->item->snap_flanges.indexOf(this->scenePos) + 1;

    CADitem* newItem = itemDB->drawItem(this->item->layerName, type);

    newItem->wizardParams.insert("Position x", QVariant::fromValue(scenePos.x()));
    newItem->wizardParams.insert("Position y", QVariant::fromValue(scenePos.y()));
    newItem->wizardParams.insert("Position z", QVariant::fromValue(scenePos.z()));
    // tbd: add flange angles
    newItem->wizardParams.insert("Angle x", QVariant::fromValue(this->item->angle_x));
    newItem->wizardParams.insert("Angle y", QVariant::fromValue(this->item->angle_y));
    newItem->wizardParams.insert("Angle z", QVariant::fromValue(this->item->angle_z));

    // Copy parameters that are parameters of the flange, but ony if they exist in the both items
    foreach (QString key, newItem->wizardParams.keys())
    {
        if ((key == "a") || (key == "b") || (key == "s") || (key == "fe") || (key == "ff") || (key == "g") || (key == "h"))
        {
            QString sourceKey = key;
            int flangeIndexOffset = 0;

            // Reverse search of indexed parameter in source flange parameters
            do
            {
                if ((flangeIndex + flangeIndexOffset) > 1)
                {
                    sourceKey = key + QString().setNum(flangeIndex + flangeIndexOffset);
                }
                else
                {
                    sourceKey = key;
                    break;
                }
                flangeIndexOffset--;
            }while(!this->item->wizardParams.keys().contains(sourceKey));


            if (this->item->wizardParams.keys().contains(sourceKey))
                newItem->wizardParams.insert(key, this->item->wizardParams.value(sourceKey));
        }
    }

    itemWizard->showWizard(newItem);

    finishGrip();
}

void ItemGripModifier::slot_button_copyMulty()
{
    CADitem* item = this->item;
    CADitem* newItem;
    qreal deltaX;
    qreal deltaY;
    qreal deltaZ;

    for (int x=0; x < copyMulti_spinBox_countX->value(); x++)
    {
        deltaX = (qreal)x * copyMulti_doubleSpinBox_distanceX->value();
        for (int y=0; y < copyMulti_spinBox_countY->value(); y++)
        {
            deltaY = (qreal)y * copyMulti_doubleSpinBox_distanceY->value();
            for (int z=0; z < copyMulti_spinBox_countZ->value(); z++)
            {
                // Do not make an in place copy of the source item
                if ((x == 0) && (y == 0) && (z == 0))
                    continue;

                deltaZ = (qreal)z * copyMulti_doubleSpinBox_distanceZ->value();
                QVector3D pos = item->position + QVector3D(deltaX, deltaY, deltaZ);

                // Copy Item
                newItem = this->itemDB->drawItem(item->layerName, item->getType());
                newItem->wizardParams = item->wizardParams;
                newItem->wizardParams.insert("Position x", QVariant::fromValue(pos.x()));
                newItem->wizardParams.insert("Position y", QVariant::fromValue(pos.y()));
                newItem->wizardParams.insert("Position z", QVariant::fromValue(pos.z()));
                newItem->processWizardInput();
                newItem->calculate();
            }
        }
    }

    finishGrip();
    emit signal_sceneRepaintNeeded();
}

void ItemGripModifier::deleteWdgs(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            deleteWdgs(item->layout());
            delete item->layout();
        }
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }

    this->layout()->removeItem(ui->gridLayout);
    ui->gridLayout->deleteLater();
    ui->gridLayout = new QGridLayout(this);
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->gridLayout);
}

void ItemGripModifier::showAppendBox()
{
    QList<CADitem::ItemType> flangable_items;

    switch (item->getType())
    {
    case CADitem::Air_CanvasFlange:
    {
        flangable_items = CAD_air_canvasFlange::flangable_items();
        break;
    }
    case CADitem::Air_Duct:
    {
        flangable_items = CAD_air_duct::flangable_items();
        break;
    }
    case CADitem::Air_DuctBaffleSilencer:
    {
        flangable_items = CAD_air_ductBaffleSilencer::flangable_items();
        break;
    }
    case CADitem::Air_DuctEndPlate:
    {
        flangable_items = CAD_air_ductEndPlate::flangable_items();
        break;
    }
    case CADitem::Air_DuctFireDamper:
    {
        flangable_items = CAD_air_ductFireDamper::flangable_items();
        break;
    }
    case CADitem::Air_DuctFireResistant:
    {
        flangable_items = CAD_air_ductFireResistant::flangable_items();
        break;
    }
    case CADitem::Air_DuctTeeConnector:
    {
        flangable_items = CAD_air_ductTeeConnector::flangable_items();
        break;
    }
    case CADitem::Air_DuctTransition:
    {
        flangable_items = CAD_air_ductTransition::flangable_items();
        break;
    }
    case CADitem::Air_DuctTransitionRectRound:
    {
        flangable_items = CAD_air_ductTransitionRectRound::flangable_items();
        break;
    }
    case CADitem::Air_DuctTurn:
    {
        flangable_items = CAD_air_ductTurn::flangable_items();
        break;
    }
    case CADitem::Air_DuctVolumetricFlowController:
    {
        flangable_items = CAD_air_ductVolumetricFlowController::flangable_items();
        break;
    }
    case CADitem::Air_DuctYpiece:
    {
        flangable_items = CAD_air_ductYpiece::flangable_items();
        break;
    }
    case CADitem::Air_EmptyCabinet:
    {
        flangable_items = CAD_air_emptyCabinet::flangable_items();
        break;
    }
    case CADitem::Air_EquipmentFrame:
    {
        flangable_items = CAD_air_equipmentFrame::flangable_items();
        break;
    }
    case CADitem::Air_Fan:
    {
        flangable_items = CAD_air_fan::flangable_items();
        break;
    }
    case CADitem::Air_Filter:
    {
        flangable_items = CAD_air_filter::flangable_items();
        break;
    }
    case CADitem::Air_HeatExchangerAirAir:
    {
        flangable_items = CAD_air_heatExchangerAirAir::flangable_items();
        break;
    }
    case CADitem::Air_HeatExchangerWaterAir:
    {
        flangable_items = CAD_air_heatExchangerWaterAir::flangable_items();
        break;
    }
    case CADitem::Air_Humidifier:
    {
        flangable_items = CAD_air_humidifier::flangable_items();
        break;
    }
    case CADitem::Air_MultiLeafDamper:
    {
        flangable_items = CAD_air_multiLeafDamper::flangable_items();
        break;
    }
    case CADitem::Air_Pipe:
    {
        flangable_items = CAD_air_pipe::flangable_items();
        break;
    }
    case CADitem::Air_PipeEndCap:
    {
        flangable_items = CAD_air_pipeEndCap::flangable_items();
        break;
    }
    case CADitem::Air_PipeFireDamper:
    {
        flangable_items = CAD_air_pipeFireDamper::flangable_items();
        break;
    }
    case CADitem::Air_PipeReducer:
    {
        flangable_items = CAD_air_pipeReducer::flangable_items();
        break;
    }
    case CADitem::Air_PipeSilencer:
    {
        flangable_items = CAD_air_pipeSilencer::flangable_items();
        break;
    }
    case CADitem::Air_PipeTeeConnector:
    {
        flangable_items = CAD_air_pipeTeeConnector::flangable_items();
        break;
    }
    case CADitem::Air_PipeTurn:
    {
        flangable_items = CAD_air_pipeTurn::flangable_items();
        break;
    }
    case CADitem::Air_PipeVolumetricFlowController:
    {
        flangable_items = CAD_air_pipeVolumetricFlowController::flangable_items();
        break;
    }
    case CADitem::Air_PressureReliefDamper:
    {
        flangable_items = CAD_air_pressureReliefDamper::flangable_items();
        break;
    }
    case CADitem::Air_ThrottleValve:
    {
        flangable_items = CAD_air_throttleValve::flangable_items();
        break;
    }

    case CADitem::Arch_Beam:
    {
        flangable_items = CAD_arch_beam::flangable_items();
        break;
    }
    case CADitem::Arch_BlockOut:
    {
        flangable_items = CAD_arch_blockOut::flangable_items();
        break;
    }
    case CADitem::Arch_BoredPile:
    {
//        flangable_items = CAD_arch_::flangable_items();
        break;
    }
    case CADitem::Arch_Door:
    {
        flangable_items = CAD_arch_door::flangable_items();
        break;
    }
    case CADitem::Arch_Foundation:
    {
//        flangable_items = CAD_arch_::flangable_items();
        break;
    }
    case CADitem::Arch_Grating:
    {
//        flangable_items = CAD_arch_::flangable_items();
        break;
    }
    case CADitem::Arch_LevelSlab:
    {
        flangable_items = CAD_arch_levelSlab::flangable_items();
        break;
    }
    case CADitem::Arch_Support:
    {
        flangable_items = CAD_arch_support::flangable_items();
        break;
    }
    case CADitem::Arch_Wall_loadBearing:
    {
        flangable_items = CAD_arch_wall_loadBearing::flangable_items();
        break;
    }
    case CADitem::Arch_Wall_nonLoadBearing:
    {
        flangable_items = CAD_arch_wall_nonLoadBearing::flangable_items();
        break;
    }
    case CADitem::Arch_Window:
    {
        flangable_items = CAD_arch_window::flangable_items();
        break;
    }

    case CADitem::Basic_Arc:
    {
        flangable_items = CAD_basic_arc::flangable_items();
        break;
    }
    case CADitem::Basic_Box:
    {
        flangable_items = CAD_basic_box::flangable_items();
        break;
    }
    case CADitem::Basic_Circle:
    {
        flangable_items = CAD_basic_circle::flangable_items();
        break;
    }
    case CADitem::Basic_Cylinder:
    {
        flangable_items = CAD_basic_cylinder::flangable_items();
        break;
    }
    case CADitem::Basic_Duct:
    {
        flangable_items = CAD_basic_duct::flangable_items();
        break;
    }
    case CADitem::Basic_Face:
    {
//        flangable_items = CAD_basic_::flangable_items();
        break;
    }
    case CADitem::Basic_Line:
    {
        flangable_items = CAD_basic_line::flangable_items();
        break;
    }
    case CADitem::Basic_Pipe:
    {
        flangable_items = CAD_basic_pipe::flangable_items();
        break;
    }
    case CADitem::Basic_Plane:
    {
        flangable_items = CAD_basic_plane::flangable_items();
        break;
    }
    case CADitem::Basic_Point:
    {
        flangable_items = CAD_basic_point::flangable_items();
        break;
    }
    case CADitem::Basic_Polyline:
    {
        flangable_items = CAD_basic_polyline::flangable_items();
        break;
    }
    case CADitem::Basic_Sphere:
    {
        flangable_items = CAD_basic_sphere::flangable_items();
        break;
    }
    case CADitem::Basic_Turn:
    {
        flangable_items = CAD_basic_turn::flangable_items();
        break;
    }

    case CADitem::Electrical_Cabinet:
    {
        flangable_items = CAD_electrical_cabinet::flangable_items();
        break;
    }
    case CADitem::Electrical_CableTray:
    {
        flangable_items = CAD_electrical_cableTray::flangable_items();
        break;
    }

    case CADitem::HeatCool_Adjustvalve:
    {
        flangable_items = CAD_heatcool_adjustvalve::flangable_items();
        break;
    }
    case CADitem::HeatCool_BallValve:
    {
        flangable_items = CAD_heatcool_ballValve::flangable_items();
        break;
    }
    case CADitem::HeatCool_Boiler:
    {
        flangable_items = CAD_heatcool_boiler::flangable_items();
        break;
    }
    case CADitem::HeatCool_ButterflyValve:
    {
        flangable_items = CAD_heatcool_butterflyValve::flangable_items();
        break;
    }
    case CADitem::HeatCool_Chiller:
    {
        flangable_items = CAD_heatcool_chiller::flangable_items();
        break;
    }
    case CADitem::HeatCool_Controlvalve:
    {
        flangable_items = CAD_heatcool_controlvalve::flangable_items();
        break;
    }
    case CADitem::HeatCool_CoolingTower:
    {
        flangable_items = CAD_heatcool_coolingTower::flangable_items();
        break;
    }
    case CADitem::HeatCool_ExpansionChamber:
    {
        flangable_items = CAD_heatcool_expansionChamber::flangable_items();
        break;
    }
    case CADitem::HeatCool_Filter:
    {
        flangable_items = CAD_heatcool_filter::flangable_items();
        break;
    }
    case CADitem::HeatCool_Flange:
    {
        flangable_items = CAD_heatcool_flange::flangable_items();
        break;
    }
    case CADitem::HeatCool_Flowmeter:
    {
        flangable_items = CAD_heatcool_flowmeter::flangable_items();
        break;
    }
    case CADitem::HeatCool_HeatExchanger:
    {
        flangable_items = CAD_heatcool_heatExchanger::flangable_items();
        break;
    }
    case CADitem::HeatCool_Pipe:
    {
        flangable_items = CAD_heatcool_pipe::flangable_items();
        break;
    }
    case CADitem::HeatCool_PipeEndCap:
    {
        flangable_items = CAD_heatcool_pipeEndCap::flangable_items();
        break;
    }
    case CADitem::HeatCool_PipeReducer:
    {
        flangable_items = CAD_heatcool_pipeReducer::flangable_items();
        break;
    }
    case CADitem::HeatCool_PipeTeeConnector:
    {
        flangable_items = CAD_heatcool_pipeTeeConnector::flangable_items();
        break;
    }
    case CADitem::HeatCool_PipeTurn:
    {
        flangable_items = CAD_heatcool_pipeTurn::flangable_items();
        break;
    }
    case CADitem::HeatCool_Pump:
    {
        flangable_items = CAD_heatcool_pump::flangable_items();
        break;
    }
    case CADitem::HeatCool_Radiator:
    {
        flangable_items = CAD_heatcool_radiator::flangable_items();
        break;
    }
    case CADitem::HeatCool_SafetyValve:
    {
        flangable_items = CAD_heatcool_safetyValve::flangable_items();
        break;
    }
    case CADitem::HeatCool_Sensor:
    {
        flangable_items = CAD_heatcool_sensor::flangable_items();
        break;
    }
    case CADitem::HeatCool_StorageBoiler:
    {
        flangable_items = CAD_heatcool_storageBoiler::flangable_items();
        break;
    }
    case CADitem::HeatCool_WaterHeater:
    {
        flangable_items = CAD_heatcool_waterHeater::flangable_items();
        break;
    }

    case CADitem::Sanitary_ElectricWaterHeater:
    {
        flangable_items = CAD_sanitary_electricWaterHeater::flangable_items();
        break;
    }
    case CADitem::Sanitary_EmergencyEyeShower:
    {
        flangable_items = CAD_sanitary_emergencyEyeShower::flangable_items();
        break;
    }
    case CADitem::Sanitary_EmergencyShower:
    {
        flangable_items = CAD_sanitary_emergencyShower::flangable_items();
        break;
    }
    case CADitem::Sanitary_Flange:
    {
        flangable_items = CAD_sanitary_flange::flangable_items();
        break;
    }
    case CADitem::Sanitary_LiftingUnit:
    {
        flangable_items = CAD_sanitary_liftingUnit::flangable_items();
        break;
    }
    case CADitem::Sanitary_Pipe:
    {
        flangable_items = CAD_sanitary_pipe::flangable_items();
        break;
    }
    case CADitem::Sanitary_PipeEndCap:
    {
        flangable_items = CAD_sanitary_pipeEndCap::flangable_items();
        break;
    }
    case CADitem::Sanitary_PipeReducer:
    {
        flangable_items = CAD_sanitary_pipeReducer::flangable_items();
        break;
    }
    case CADitem::Sanitary_PipeTeeConnector:
    {
        flangable_items = CAD_sanitary_pipeTeeConnector::flangable_items();
        break;
    }
    case CADitem::Sanitary_PipeTurn:
    {
        flangable_items = CAD_sanitary_pipeTurn::flangable_items();
        break;
    }
    case CADitem::Sanitary_Shower:
    {
        flangable_items = CAD_sanitary_shower::flangable_items();
        break;
    }
    case CADitem::Sanitary_Sink:
    {
        flangable_items = CAD_sanitary_sink::flangable_items();
        break;
    }
    case CADitem::Sanitary_WashBasin:
    {
        flangable_items = CAD_sanitary_washBasin::flangable_items();
        break;
    }

    default:
        break;
    }

    deleteWdgs(ui->gridLayout);
    ui->label->setText(tr("Choose new item"));

    int buttonCount = flangable_items.count();
    int columnCount = sqrt(buttonCount);
    int column = 0;
    int row = 0;

    foreach(CADitem::ItemType type, flangable_items)
    {
        QIcon icon = itemDB->getIconByItemType(type, QSize(64, 64));
        QToolButton* button = new QToolButton(this);
        button->setMinimumSize(74, 74);
        button->setMaximumSize(74, 74);
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        button->setIconSize(QSize(64, 64));
        button->setIcon(icon);
        button->setProperty("ItemType", QVariant((int)type));

        connect(button, SIGNAL(clicked()), this, SLOT(slot_button_clicked()));


        ui->gridLayout->addWidget(button, row, column);
        column++;
        if (column == columnCount)
        {
            column = 0;
            row++;
        }
    }

    this->move(QCursor::pos());
    this->show();
}

void ItemGripModifier::showCopyMultiBox()
{
    deleteWdgs(ui->gridLayout);
    ui->label->setText(tr("Multi Copy"));

    copyMulti_spinBox_countX = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceX = new QDoubleSpinBox(this);
    copyMulti_spinBox_countY = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceY = new QDoubleSpinBox(this);
    copyMulti_spinBox_countZ = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceZ = new QDoubleSpinBox(this);

    copyMulti_doubleSpinBox_distanceX->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceX->setMaximum( 10e+20);
    copyMulti_doubleSpinBox_distanceY->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceY->setMaximum( 10e+20);
    copyMulti_doubleSpinBox_distanceZ->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceZ->setMaximum( 10e+20);
    copyMulti_spinBox_countX->setMinimum(1);
    copyMulti_spinBox_countX->setMaximum(INT_MAX);
    copyMulti_spinBox_countY->setMinimum(1);
    copyMulti_spinBox_countY->setMaximum(INT_MAX);
    copyMulti_spinBox_countZ->setMinimum(1);
    copyMulti_spinBox_countZ->setMaximum(INT_MAX);

    ui->gridLayout->addWidget(new QLabel(tr("Count X")),    0, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance X")), 1, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Count Y")),    2, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance Y")), 3, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Count Z")),    4, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance Z")), 5, 0);

    ui->gridLayout->addWidget(copyMulti_spinBox_countX,          0, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceX, 1, 1);
    ui->gridLayout->addWidget(copyMulti_spinBox_countY,          2, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceY, 3, 1);
    ui->gridLayout->addWidget(copyMulti_spinBox_countZ,          4, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceZ, 5, 1);

    QToolButton* button_cancel = new QToolButton(this);
    button_cancel->setText(tr("Cancel"));
    button_cancel->setFocusPolicy(Qt::NoFocus);
    button_cancel->setMinimumSize(150, 40);
    button_cancel->setMaximumSize(200, 40);
    button_cancel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(slot_rejected()));
    ui->gridLayout->addWidget(button_cancel, 6, 0);

    QToolButton* button_ok = new QToolButton(this);
    button_ok->setText(tr("Ok"));
    button_ok->setFocusPolicy(Qt::NoFocus);
    button_ok->setMinimumSize(150, 40);
    button_ok->setMaximumSize(200, 40);
    button_ok->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(button_ok, SIGNAL(clicked()), this, SLOT(slot_button_copyMulty()));
    ui->gridLayout->addWidget(button_ok, 6, 1);

    this->move(QCursor::pos());
    this->show();
}
