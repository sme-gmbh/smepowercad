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

#include "restorepoint.h"

RestorePoint::RestorePoint()
{

}

RestorePoint::RestorePoint(RestorePoint &restorePoint)
{
    this->layer = restorePoint.layer;
    this->itemType = restorePoint.itemType;
    this->itemID = restorePoint.itemID;
    this->wizardParamsBefore = restorePoint.wizardParamsBefore;
    this->wizardParamsAfter = restorePoint.wizardParamsAfter;
    this->restoreType = restorePoint.restoreType;
}

RestorePoint::RestorePoint(RestoreType type)
{
    this->restoreType = type;
}

RestorePoint::RestorePoint(RestorePoint::RestoreType type, Layer* layer, quint64 itemID, CADitemTypes::ItemType itemType, WizardParams wizardParamsBefore, WizardParams wizardParamsAfter)
{
    this->layer = layer;
    this->restoreType = type;
    this->itemID = itemID;
    this->itemType = itemType;
    this->wizardParamsBefore = wizardParamsBefore;
    this->wizardParamsAfter = wizardParamsAfter;
}

RestorePoint::~RestorePoint()
{

}

RestorePoint::RestoreType RestorePoint::getType()
{
    return this->restoreType;
}



