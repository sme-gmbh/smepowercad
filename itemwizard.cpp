#include "itemwizard.h"
#include "ui_itemwizard.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
    connect(this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
}

ItemWizard::~ItemWizard()
{
    delete ui;
}

void ItemWizard::showWizard(CADitem *item)
{
    if (item == NULL)
    {
        qDebug("CADitem is NULL");
        return;
    }

    // Do not show an empty wizard
    if(item->wizardParams.isEmpty())
    {
        item->calculate();
        emit signal_sceneRepaintNeeded();
        return;
    }

    currentItem = item;
    QMap<QString, QVariant>::iterator it;
    for (it = item->wizardParams.begin(); it != item->wizardParams.end(); it++)
    {
        QWidget *wdg;
        switch (it.value().type())
        {
        case QVariant::String:
            wdg = new QLineEdit(it.value().toString(), this);
            break;
        case QVariant::Int:
            wdg = new QSpinBox(this);
            ((QSpinBox*)wdg)->setMaximum(INT_MAX);
            ((QSpinBox*)wdg)->setMinimum(INT_MIN);
            ((QSpinBox*)wdg)->setValue(it.value().toInt());
            break;
        case QVariant::Double:
            wdg = new QDoubleSpinBox(this);
            ((QDoubleSpinBox*)wdg)->setMaximum(10e+20);
            ((QDoubleSpinBox*)wdg)->setMinimum(-10e+20);
            ((QDoubleSpinBox*)wdg)->setValue(it.value().toDouble());
            break;
        default:
            break;
        }
        wdg->setObjectName(it.key());


        ui->formLayout->addRow(it.key(), wdg);
    }

    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));

    this->show();
}

void ItemWizard::on_buttonBox_accepted()
{
//    this->save();
    this->accept();
}

void ItemWizard::on_buttonBox_rejected()
{
    currentItem->calculate();
    this->reject();
}

void ItemWizard::slot_rejected()
{
    this->deleteWdgs(ui->formLayout);
}

void ItemWizard::slot_accepted()
{
    this->save();
}

void ItemWizard::save()
{
    QMap<QString, QVariant> map;
    QVariant val;
    QWidget *wdg;
    for (int r = 0; r < ui->formLayout->rowCount(); r++)
    {
        wdg = ui->formLayout->itemAt(r, QFormLayout::FieldRole)->widget();

        switch (currentItem->wizardParams.value(wdg->objectName()).type())
        {
        case QVariant::String:
            val = QVariant::fromValue(((QLineEdit*)wdg)->text());
            break;
        case QVariant::Int:
            val = QVariant::fromValue(((QSpinBox*)wdg)->value());
            break;
        case QVariant::Double:
            val = QVariant::fromValue(((QDoubleSpinBox*)wdg)->value());
            break;
        default:
            break;
        }

        map.insert(wdg->objectName(), val);
    }

    this->deleteWdgs(ui->formLayout);


    currentItem->wizardParams = map;
    currentItem->processWizardInput();
    currentItem->calculate();
    emit signal_sceneRepaintNeeded();
}

void ItemWizard::deleteWdgs(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        if (item->layout()) {
            deleteWdgs(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    this->layout()->removeItem(ui->formLayout);
    ui->formLayout->deleteLater();
    ui->formLayout = new QFormLayout;
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->formLayout);

    qDebug() << ui->formLayout->rowCount();
}

QImage ItemWizard::wizardImage(CADitem *item)
{
    QImage image;
//    image = QImage(200, 100, QImage::Format_RGB32);
//    image.fill(Qt::white);

    switch (item->getType())
    {
    case CADitem::Air_CanvasFlange:
    {
        image = CAD_air_canvasFlange::wizardImage();
        break;
    }
    case CADitem::Air_Duct:
    {
        image = CAD_air_duct::wizardImage();
        break;
    }
    case CADitem::Air_DuctBaffleSilencer:
    {
        image = CAD_air_ductBaffleSilencer::wizardImage();
        break;
    }
    case CADitem::Air_DuctEndPlate:
    {
        image = CAD_air_ductEndPlate::wizardImage();
        break;
    }
    case CADitem::Air_DuctFireDamper:
    {
        image = CAD_air_ductFireDamper::wizardImage();
        break;
    }
    case CADitem::Air_DuctFireResistant:
    {
        image = CAD_air_ductFireResistant::wizardImage();
        break;
    }
    case CADitem::Air_DuctTeeConnector:
    {
        image = CAD_air_ductTeeConnector::wizardImage();
        break;
    }
    case CADitem::Air_DuctTransition:
    {
        image = CAD_air_ductTransition::wizardImage();
        break;
    }
    case CADitem::Air_DuctTransitionRectRound:
    {
        image = CAD_air_ductTransitionRectRound ::wizardImage();
        break;
    }
    case CADitem::Air_DuctTurn:
    {
        image = CAD_air_ductTurn::wizardImage();
        break;
    }
    case CADitem::Air_DuctVolumetricFlowController:
    {
        image = CAD_air_ductVolumetricFlowController::wizardImage();
        break;
    }
    case CADitem::Air_DuctYpiece:
    {
        image = CAD_air_ductYpiece::wizardImage();
        break;
    }
    case CADitem::Air_EmptyCabinet:
    {
        image = CAD_air_emptyCabinet::wizardImage();
        break;
    }
    case CADitem::Air_EquipmentFrame:
    {
        image = CAD_air_equipmentFrame::wizardImage();
        break;
    }
    case CADitem::Air_Fan:
    {
        image = CAD_air_fan::wizardImage();
        break;
    }
    case CADitem::Air_Filter:
    {
        image = CAD_air_filter::wizardImage();
        break;
    }
    case CADitem::Air_HeatExchangerAirAir:
    {
        image = CAD_air_heatExchangerAirAir::wizardImage();
        break;
    }
    case CADitem::Air_HeatExchangerWaterAir:
    {image = CAD_air_heatExchangerWaterAir::wizardImage();
        break;
    }
    case CADitem::Air_Humidifier:
    {
        image = CAD_air_humidifier::wizardImage();
        break;
    }
    case CADitem::Air_MultiLeafDamper:
    {
        image = CAD_air_multiLeafDamper::wizardImage();
        break;
    }
    case CADitem::Air_Pipe:
    {
        image = CAD_air_pipe::wizardImage();
        break;
    }
    case CADitem::Air_PipeEndCap:
    {
        image = CAD_air_pipeEndCap::wizardImage();
        break;
    }
    case CADitem::Air_PipeFireDamper:
    {
        image = CAD_air_pipeFireDamper::wizardImage();
        break;
    }
    case CADitem::Air_PipeReducer:
    {
        image = CAD_air_pipeReducer::wizardImage();
        break;
    }
    case CADitem::Air_PipeSilencer:
    {
        image = CAD_air_pipeSilencer::wizardImage();
        break;
    }
    case CADitem::Air_PipeTeeConnector:
    {
        image = CAD_air_pipeTeeConnector::wizardImage();
        break;
    }
    case CADitem::Air_PipeTurn:
    {
        image = CAD_air_pipeTurn::wizardImage();
        break;
    }
    case CADitem::Air_PipeVolumetricFlowController:
    {
        image = CAD_air_pipeVolumetricFlowController::wizardImage();
        break;
    }
    case CADitem::Air_PressureReliefDamper:
    {
        image = CAD_air_pressureReliefDamper::wizardImage();
        break;
    }
    case CADitem::Air_ThrottleValve:
    {
        image = CAD_air_throttleValve::wizardImage();
        break;
    }

    case CADitem::Arch_Beam:
    {
        image = CAD_arch_beam::wizardImage();
        break;
    }
    case CADitem::Arch_BlockOut:
    {
        image = CAD_arch_blockOut::wizardImage();
        break;
    }
    case CADitem::Arch_BoredPile:
    {
        image = CAD_arch_boredPile::wizardImage();
        break;
    }
    case CADitem::Arch_Door:
    {
        image = CAD_arch_door::wizardImage();
        break;
    }
    case CADitem::Arch_Foundation:
    {
        image = CAD_arch_foundation::wizardImage();
        break;
    }
    case CADitem::Arch_Grating:
    {
        image = CAD_arch_grating::wizardImage();
        break;
    }
    case CADitem::Arch_LevelSlab:
    {
        image = CAD_arch_levelSlab::wizardImage();
        break;
    }
    case CADitem::Arch_Support:
    {
        image = CAD_arch_support::wizardImage();
        break;
    }
    case CADitem::Arch_Wall_loadBearing:
    {
        image = CAD_arch_wall_loadBearing::wizardImage();
        break;
    }
    case CADitem::Arch_Wall_nonLoadBearing:
    {
        image = CAD_arch_wall_nonLoadBearing::wizardImage();
        break;
    }
    case CADitem::Arch_Window:
    {
        image = CAD_arch_window::wizardImage();
        break;
    }
    case CADitem::Basic_Arc:
    {
        image = CAD_basic_arc::wizardImage();
        break;
    }
    case CADitem::Basic_Box:
    {
        image = CAD_basic_box::wizardImage();
        break;
    }
    case CADitem::Basic_Circle:
    {
        image = CAD_basic_circle::wizardImage();
        break;
    }
    case CADitem::Basic_Cylinder:
    {
        image = CAD_basic_cylinder::wizardImage();
        break;
    }
    case CADitem::Basic_Duct:
    {
        image = CAD_basic_duct::wizardImage();
        break;
    }
    case CADitem::Basic_Face:
    {
        image = CAD_basic_face::wizardImage();
        break;
    }
    case CADitem::Basic_Line:
    {
        image = CAD_basic_line::wizardImage();
        break;
    }
    case CADitem::Basic_Pipe:
    {
        image = CAD_basic_pipe::wizardImage();
        break;
    }
    case CADitem::Basic_Plane:
    {
        image = CAD_basic_plane::wizardImage();
        break;
    }
    case CADitem::Basic_Point:
    {
        image = CAD_basic_point::wizardImage();
        break;
    }
    case CADitem::Basic_Polyline:
    {
        image = CAD_basic_polyline::wizardImage();
        break;
    }
    case CADitem::Basic_Sphere:
    {
        image = CAD_basic_sphere::wizardImage();
        break;
    }
    case CADitem::Basic_Turn:
    {
        image = CAD_basic_turn::wizardImage();
        break;
    }

    case CADitem::Electrical_Cabinet:
    {
        image = CAD_electrical_cabinet::wizardImage();
        break;
    }
    case CADitem::Electrical_CableTray:
    {
        image = CAD_electrical_cableTray::wizardImage();
        break;
    }

    case CADitem::HeatCool_Adjustvalve:
    {
        image = CAD_heatcool_adjustvalve::wizardImage();
        break;
    }
    case CADitem::HeatCool_BallValve:
    {
        image = CAD_heatcool_ballValve::wizardImage();
        break;
    }
    case CADitem::HeatCool_Boiler:
    {
        image = CAD_heatcool_boiler::wizardImage();
        break;
    }
    case CADitem::HeatCool_ButterflyValve:
    {
        image = CAD_heatcool_butterflyValve::wizardImage();
        break;
    }
    case CADitem::HeatCool_Chiller:
    {
        image = CAD_heatcool_chiller::wizardImage();
        break;
    }
    case CADitem::HeatCool_Controlvalve:
    {
        image = CAD_heatcool_controlvalve::wizardImage();
        break;
    }
    case CADitem::HeatCool_CoolingTower:
    {
        image = CAD_heatcool_coolingTower::wizardImage();
        break;
    }
    case CADitem::HeatCool_ExpansionChamber:
    {
        image = CAD_heatcool_expansionChamber::wizardImage();
        break;
    }
    case CADitem::HeatCool_Filter:
    {
        image = CAD_heatcool_filter::wizardImage();
        break;
    }
    case CADitem::HeatCool_Flange:
    {
        image = CAD_heatcool_flange::wizardImage();
        break;
    }
    case CADitem::HeatCool_Flowmeter:
    {
        image = CAD_heatcool_flowmeter::wizardImage();
        break;
    }
    case CADitem::HeatCool_HeatExchanger:
    {
        image = CAD_heatcool_heatExchanger::wizardImage();
        break;
    }
    case CADitem::HeatCool_Pipe:
    {
        image = CAD_heatcool_pipe::wizardImage();
        break;
    }
    case CADitem::HeatCool_PipeEndCap:
    {
        image = CAD_heatcool_pipeEndCap::wizardImage();
        break;
    }
    case CADitem::HeatCool_PipeReducer:
    {
        image = CAD_heatcool_pipeReducer::wizardImage();
        break;
    }
    case CADitem::HeatCool_PipeTeeConnector:
    {
        image = CAD_heatcool_pipeTeeConnector::wizardImage();
        break;
    }
    case CADitem::HeatCool_PipeTurn:
    {
        image = CAD_heatcool_pipeTurn::wizardImage();
        break;
    }
    case CADitem::HeatCool_Pump:
    {
        image = CAD_heatcool_pump::wizardImage();
        break;
    }
    case CADitem::HeatCool_Radiator:
    {
        image = CAD_heatcool_radiator::wizardImage();
        break;
    }
    case CADitem::HeatCool_SafetyValve:
    {
        image = CAD_heatcool_safetyValve::wizardImage();
        break;
    }
    case CADitem::HeatCool_Sensor:
    {
        image = CAD_heatcool_sensor::wizardImage();
        break;
    }
    case CADitem::HeatCool_StorageBoiler:
    {
        image = CAD_heatcool_storageBoiler::wizardImage();
        break;
    }
    case CADitem::HeatCool_WaterHeater:
    {
        image = CAD_heatcool_waterHeater::wizardImage();
        break;
    }

    case CADitem::Sanitary_ElectricWaterHeater:
    {
        image = CAD_sanitary_electricWaterHeater::wizardImage();
        break;
    }
    case CADitem::Sanitary_EmergencyEyeShower:
    {
        image = CAD_sanitary_emergencyEyeShower::wizardImage();
        break;
    }
    case CADitem::Sanitary_EmergencyShower:
    {
        image = CAD_sanitary_emergencyShower::wizardImage();
        break;
    }
    case CADitem::Sanitary_Flange:
    {
        image = CAD_sanitary_flange::wizardImage();
        break;
    }
    case CADitem::Sanitary_LiftingUnit:
    {
        image = CAD_sanitary_liftingUnit::wizardImage();
        break;
    }
    case CADitem::Sanitary_Pipe:
    {
        image = CAD_sanitary_pipe::wizardImage();
        break;
    }
    case CADitem::Sanitary_PipeEndCap:
    {
        image = CAD_sanitary_pipeEndCap::wizardImage();
        break;
    }
    case CADitem::Sanitary_PipeReducer:
    {
        image = CAD_sanitary_pipeReducer::wizardImage();
        break;
    }
    case CADitem::Sanitary_PipeTeeConnector:
    {
        image = CAD_sanitary_pipeTeeConnector::wizardImage();
        break;
    }
    case CADitem::Sanitary_PipeTurn:
    {
        image = CAD_sanitary_pipeTurn::wizardImage();
        break;
    }
    case CADitem::Sanitary_Shower:
    {
        image = CAD_sanitary_shower::wizardImage();
        break;
    }
    case CADitem::Sanitary_Sink:
    {
        image = CAD_sanitary_sink::wizardImage();
        break;
    }
    case CADitem::Sanitary_WashBasin:
    {
        image = CAD_sanitary_washBasin::wizardImage();
        break;
    }

    default:
        break;
    }

    return image.scaledToWidth(400, Qt::SmoothTransformation);
}
