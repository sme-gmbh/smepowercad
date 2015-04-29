#ifndef RESTOREPOINT_H
#define RESTOREPOINT_H

#include "wizardparams.h"
#include "caditemtypes.h"
#include "layer.h"

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
    ~RestorePoint();

    Layer* layer;
    CADitemTypes::ItemType itemType;
    quint64 itemID;
    WizardParams wizardParamsBefore;
    WizardParams wizardParamsAfter;

    RestoreType getType();

private:
    RestoreType restoreType;
};

#endif // RESTOREPOINT_H
