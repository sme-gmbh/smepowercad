/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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
    this->item = NULL;
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
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
    this->items.append(item);
}

void ItemGripModifier::setItems(QList<CADitem *> items)
{
    this->items = items;
}

void ItemGripModifier::setScenePosSource(QVector3D pos)
{
    this->scenePos = pos;
}

CADitem *ItemGripModifier::getItem()
{
    return this->item;
}

QList<CADitem*> ItemGripModifier::getItems()
{
    return this->items;
}

QString ItemGripModifier::getItemDescription()
{
    QString description;

    bool multiTypes = false;

    foreach (CADitem* item, this->items)
    {
        if (description.isEmpty())
            description = item->description();
        else
        {
            if (item->description() != description)
                multiTypes = true;
            break;
        }
    }

    if (multiTypes)
        description = QString().sprintf("%i different items", this->items.count());
    else if (this->items.count() > 1)
        description.prepend(QString().sprintf("%i of ", this->items.count()));


    return description;
}

QVector3D ItemGripModifier::getScenePosSource()
{
    return this->scenePos;
}

void ItemGripModifier::activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos)
{
    Q_UNUSED(mousePos);
    this->scenePos = scenePos;
    this->activeGrip = gripType;

    switch (gripType)
    {
    case Grip_None:
        break;
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
    case Grip_Rotate_aroundPoint:
    {
        showRotateAroundPointBox();
        break;
    }
    }
}

void ItemGripModifier::moveItemsTo(QVector3D new_scenePos)
{
    itemDB->setRestorePoint();
    foreach (CADitem* item, this->items)
    {
        QVector3D offset = this->scenePos - item->position;    // Offset between point of pickup and basepoint of picked object
        QVector3D newPos = new_scenePos - offset;
        WizardParams newParams;
        newParams.insert("Position x", ((qreal)newPos.x()));
        newParams.insert("Position y", ((qreal)newPos.y()));
        newParams.insert("Position z", ((qreal)newPos.z()));
        itemDB->modifyItem_withRestorePoint(item, newParams);
    }
    this->finishGrip();
}

void ItemGripModifier::copyItemsTo(QVector3D new_scenePos)
{
    itemDB->setRestorePoint();
    foreach (CADitem* item, this->items)
    {
        QVector3D offset = this->scenePos - item->position;    // Offset between point of pickup and basepoint of picked object
        QVector3D newPos = new_scenePos - offset;
        WizardParams newParams;
        newParams = item->wizardParams;
        newParams.insert("Position x", ((qreal)newPos.x()));
        newParams.insert("Position y", ((qreal)newPos.y()));
        newParams.insert("Position z", ((qreal)newPos.z()));
        CADitem* newItem = this->itemDB->drawItem_withRestorePoint(item->layer, item->getType(), newParams);
        newItem->processWizardInput();
        newItem->calculate();
    }
    this->finishGrip();
}

void ItemGripModifier::finishGrip()
{
    this->items.clear();
    this->item = NULL;
    this->activeGrip = Grip_None;
    hide();
    deleteWdgs();
}

ItemGripModifier::ItemGripType ItemGripModifier::getActiveGrip()
{
    return this->activeGrip;
}

void ItemGripModifier::slot_rejected()
{
    this->items.clear();
    this->item = NULL;
    this->hide();
    deleteWdgs();
}

void ItemGripModifier::slot_button_clicked()
{
    QToolButton* button = (QToolButton*)this->sender();
    CADitemTypes::ItemType type = (CADitemTypes::ItemType)button->property("ItemType").toInt();
    int flangeIndex = this->item->snap_flanges.indexOf(this->scenePos) + 1;

    itemDB->setRestorePoint();
    CADitem* newItem = itemDB->drawItem_withRestorePoint(this->item->layer, type, WizardParams());

    newItem->wizardParams.insert("Position x", ((qreal)scenePos.x()));
    newItem->wizardParams.insert("Position y", ((qreal)scenePos.y()));
    newItem->wizardParams.insert("Position z", ((qreal)scenePos.z()));

    //calculate angles from flange matrix:
    QMatrix4x4 matrix_rotation = item->rotationOfFlange(flangeIndex);
    QVector3D angles = MAngleCalculations().anglesFromMatrix(matrix_rotation);
    newItem->wizardParams.insert("Angle x", angles.x());
    newItem->wizardParams.insert("Angle y", angles.y());
    newItem->wizardParams.insert("Angle z", angles.z());

    // Copy parameters that are parameters of the flange, but ony if they exist in the both items
    foreach (QString key, newItem->wizardParams.keys())
    {
        if ((key == "a") || (key == "b") || (key == "d") || (key == "s") || (key == "fe") || (key == "ff") || (key == "g") || (key == "h") || (key == "iso"))
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

    itemWizard->showWizard(newItem, itemDB);
    itemDB->modifyItem_withRestorePoint(newItem, newItem->wizardParams);

    finishGrip();
}

void ItemGripModifier::slot_button_copyMulty()
{
    foreach (CADitem* item, this->items)
    {
        CADitem* newItem;
        qreal deltaX;
        qreal deltaY;
        qreal deltaZ;

        itemDB->setRestorePoint();
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
                    newItem = this->itemDB->drawItem_withRestorePoint(item->layer, item->getType(), item->wizardParams);
                    WizardParams newParams = item->wizardParams;
                    newParams.insert("Position x", ((qreal)pos.x()));
                    newParams.insert("Position y", ((qreal)pos.y()));
                    newParams.insert("Position z", ((qreal)pos.z()));
                    newItem->wizardParams.insert(newParams);
                    newItem->processWizardInput();
                    newItem->calculate();
                }
            }
        }
    }
    finishGrip();

    emit signal_sceneRepaintNeeded();
}

void ItemGripModifier::slot_button_rotateAroundPoint()
{
    qreal centerX = rotate_doubleSpinBox_centerX->value();
    qreal centerY = rotate_doubleSpinBox_centerY->value();
    qreal centerZ = rotate_doubleSpinBox_centerZ->value();
    QVector3D center = QVector3D(centerX, centerY, centerZ);
    qreal angleX  = rotate_doubleSpinBox_angleX->value();
    qreal angleY  = rotate_doubleSpinBox_angleY->value();
    qreal angleZ  = rotate_doubleSpinBox_angleZ->value();

    itemDB->setRestorePoint();
    foreach (CADitem* item, this->items)
    {
        WizardParams newParams = item->rotateAroundPoint(center, angleX, angleY, angleZ);
        itemDB->modifyItem_withRestorePoint(item, newParams);
    }

    finishGrip();

    emit signal_sceneRepaintNeeded();
}

void ItemGripModifier::deleteWdgs()
{
    QLayoutItem *item;
    while (ui->gridLayout->count() > 0)
    {
        item = ui->gridLayout->takeAt(0);
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }

    this->layout()->removeItem(ui->gridLayout);
    delete ui->gridLayout;
    ui->gridLayout = new QGridLayout();
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->gridLayout);
}

void ItemGripModifier::showAppendBox()
{
    int flangeIndex = this->item->snap_flanges.indexOf(this->scenePos) + 1;
    QList<CADitemTypes::ItemType> flangable_items = item->flangable_items(flangeIndex);

    deleteWdgs();
    ui->label->setText(tr("Choose new item"));

    int buttonCount = flangable_items.count();
    int columnCount = sqrt(buttonCount);
    int column = 0;
    int row = 0;

    foreach(CADitemTypes::ItemType type, flangable_items)
    {
        QIcon icon = itemDB->getIconByItemType(type, QSize(64, 64));
        QToolButton* button = new QToolButton(this);
        button->setToolTip(itemDB->getItemDescriptionByItemType(type));
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
    deleteWdgs();
    ui->label->setText(tr("Multi Copy"));

    copyMulti_spinBox_countX = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceX = new QDoubleSpinBox(this);
    copyMulti_spinBox_countY = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceY = new QDoubleSpinBox(this);
    copyMulti_spinBox_countZ = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceZ = new QDoubleSpinBox(this);

    copyMulti_doubleSpinBox_distanceX->setMinimum(-1e+20);
    copyMulti_doubleSpinBox_distanceX->setMaximum( 1e+20);
    copyMulti_doubleSpinBox_distanceY->setMinimum(-1e+20);
    copyMulti_doubleSpinBox_distanceY->setMaximum( 1e+20);
    copyMulti_doubleSpinBox_distanceZ->setMinimum(-1e+20);
    copyMulti_doubleSpinBox_distanceZ->setMaximum( 1e+20);
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

void ItemGripModifier::showRotateAroundPointBox()
{
    deleteWdgs();
    ui->label->setText(tr("Rotate around point"));

    rotate_doubleSpinBox_centerX = new QDoubleSpinBox(this);
    rotate_doubleSpinBox_centerY = new QDoubleSpinBox(this);
    rotate_doubleSpinBox_centerZ = new QDoubleSpinBox(this);
    rotate_doubleSpinBox_angleX = new QDoubleSpinBox(this);
    rotate_doubleSpinBox_angleY = new QDoubleSpinBox(this);
    rotate_doubleSpinBox_angleZ = new QDoubleSpinBox(this);

    rotate_doubleSpinBox_centerX->setMinimum(-1e+20);
    rotate_doubleSpinBox_centerY->setMinimum(-1e+20);
    rotate_doubleSpinBox_centerZ->setMinimum(-1e+20);
    rotate_doubleSpinBox_angleX->setMinimum(-1e+20);
    rotate_doubleSpinBox_angleY->setMinimum(-1e+20);
    rotate_doubleSpinBox_angleZ->setMinimum(-1e+20);

    rotate_doubleSpinBox_centerX->setMaximum(1e+20);
    rotate_doubleSpinBox_centerY->setMaximum(1e+20);
    rotate_doubleSpinBox_centerZ->setMaximum(1e+20);
    rotate_doubleSpinBox_angleX->setMaximum(1e+20);
    rotate_doubleSpinBox_angleY->setMaximum(1e+20);
    rotate_doubleSpinBox_angleZ->setMaximum(1e+20);

    rotate_doubleSpinBox_centerX->setValue(this->scenePos.x());
    rotate_doubleSpinBox_centerY->setValue(this->scenePos.y());
    rotate_doubleSpinBox_centerZ->setValue(this->scenePos.z());

    ui->gridLayout->addWidget(new QLabel(tr("Center X")), 0, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Center Y")), 1, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Center Z")), 2, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Angle X")),  3, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Angle Y")),  4, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Angle Z")),  5, 0);

    ui->gridLayout->addWidget(rotate_doubleSpinBox_centerX, 0, 1);
    ui->gridLayout->addWidget(rotate_doubleSpinBox_centerY, 1, 1);
    ui->gridLayout->addWidget(rotate_doubleSpinBox_centerZ, 2, 1);
    ui->gridLayout->addWidget(rotate_doubleSpinBox_angleX,  3, 1);
    ui->gridLayout->addWidget(rotate_doubleSpinBox_angleY,  4, 1);
    ui->gridLayout->addWidget(rotate_doubleSpinBox_angleZ,  5, 1);

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
    connect(button_ok, SIGNAL(clicked()), this, SLOT(slot_button_rotateAroundPoint()));
    ui->gridLayout->addWidget(button_ok, 6, 1);

    this->move(QCursor::pos());
    this->show();
}
