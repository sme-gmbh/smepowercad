#include "restorepoint.h"

RestorePoint::RestorePoint()
{

}

RestorePoint::RestorePoint(RestorePoint &restorePoint)
{
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

RestorePoint::RestorePoint(RestorePoint::RestoreType type, quint64 itemID, WizardParams wizardParamsBefore, WizardParams wizardParamsAfter)
{
    this->restoreType = type;
    this->itemID = itemID;
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



