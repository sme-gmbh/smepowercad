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



