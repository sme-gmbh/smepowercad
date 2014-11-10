#include "itemwizard.h"
#include "ui_itemwizard.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
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
    this->save();
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
    ((QVBoxLayout*)this->layout())->insertLayout(0, ui->formLayout);

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
        image = CAD_basic_pipe::wizardImage();
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
        image = CAD_basic_turn::wizardImage();
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

    return image.scaledToWidth(400, Qt::SmoothTransformation);
}
