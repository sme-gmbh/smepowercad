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

#ifndef RESTOREPOINT_H
#define RESTOREPOINT_H

#include "caditemtypes.h"
#include "layer.h"
#include "wizardparams.h"

class RestorePoint
{
public:
    enum RestoreType
    {
        Restore_Stoppoint,      // Restore point list processing stops if this type is encountered
        Restore_WizardParams,
        Restore_ItemDeletion,
        Restore_ItemCreation,
        Restore_ItemLayerChange
    };

    RestorePoint();
    RestorePoint(RestorePoint &restorePoint);
    RestorePoint(RestoreType type);
    RestorePoint(RestoreType type, Layer *layer, quint64 itemID, CADitemTypes::ItemType itemType, WizardParams wizardParamsBefore, WizardParams wizardParamsAfter);
    RestorePoint(RestoreType type, quint64 itemID, Layer *layerBefore, Layer *layerAfter);
    ~RestorePoint();

    Layer *layerBefore;
    Layer *layerAfter;
    CADitemTypes::ItemType itemType;
    quint64 itemID;
    WizardParams wizardParamsBefore;
    WizardParams wizardParamsAfter;

    RestoreType getType();

private:
    RestoreType restoreType;
};

#endif // RESTOREPOINT_H
