#ifndef CADITEM_H
#define CADITEM_H

#include <QString>
#include <QColor>
#include <QList>
#include <QMap>
#include <QVector3D>
#include "math/m3dbox.h"

class CADitem
{
public:
    enum ItemType
    {
        Basic_Point,                            // Basisgeometrie: Punkt
        Basic_Line,                             // Basisgeometrie: Linie
        Basic_Polyline,                         // Basisgeometrie: Polylinie
        Basic_Circle,                           // Basisgeometrie: Kreis
        Basic_Arc,                              // Basisgeometrie: Bogen
        Basic_Face,                             // Basisgeometrie: Fläche
        Basic_Plane,                            // Basisgeometrie: Ebene
        Basic_Box,                              // Basisgeometrie: Box
        Basic_Cylinder,                         // Basisgeometrie: Zylinder
        Basic_Sphere,                           // Basisgeometrie: Kugel
        Arch_Foundation,                        // Fundament
        Arch_BoredPile,                         // Bohrpfahl
        Arch_LevelSlab,                         // Decke
        Arch_Wall_loadBearing,                  // Tagende Wand
        Arch_Wall_nonLoadBearing,               // Nicht tragende Wand
        Arch_Grating,                           // Gitterrost
        Arch_BlockOut,
        Arch_Support,                           // Stütze
        Arch_Beam,                              // Unterzug
        Arch_Door,
        Arch_Window,
        Air_Duct,                               // Kanal (eckig)
        Air_Pipe,                               // Rohr (rund)
        Air_DuctFireResistant,                  // Kanal (feuerbeständig)
        Air_DuctTurn,                           // Kanalbogen
        Air_PipeTurn,                           // Rohrbogen
        Air_PipeReducer,                        // Rohrreduzierung
        Air_PipeTeeConnector,                   // Rohr-T-Stück, auch mit mehr als 3 Anschlüssen
        Air_DuctTeeConnector,                   // Kanal-T-Stück
        Air_DuctTransition,                     // Kanalübergang (eckig)
        Air_DuctTransitionRectRound,            // Kanalübergang eckig/rund
        Air_DuctYpiece,                         // Kanal-Hosenstück
        Air_DuctEndPlate,                       // Kanalenddeckel (Boden)
        Air_PipeEndCap,                         // Rohr-Enddeckel
        Air_ThrottleValve,                      // Drossel-/ Absperrklappe (rund)
        Air_MultiLeafDamper,                    // Jalousieklappe (eckig)
        Air_PressureReliefDamper,               // Druckentlastungsklappe
        Air_PipeFireDamper,                     // Brandschutzklappe (rund)
        Air_DuctFireDamper,                     // Brandschutzklappe (eckig)
        Air_DuctVolumetricFlowController,       // Kanal-Volumenstromregler
        Air_PipeVolumetricFlowController,       // Rohr-Volumenstromregler
        Air_HeatExchangerWaterAir,              // Wärmetauscher Wasser/Luft
        Air_HeatExchangerAirAir,                // Wärmetauscher Luft/Luft
        Air_CanvasFlange,                       // Segeltuchstutzen
        Air_Filter,                             // Luftfilter
        Air_PipeSilencer,                       // Rohr-Schalldämpfer
        Air_DuctBaffleSilencer,                 // Kanal-Kulissenschalldämpfer
        Air_Fan,                                // Ventilator
        Air_Humidifier,                         // Befeuchter
        Air_EmptyCabinet,                       // Leergerät
        Air_EquipmentFrame,                     // Geräterahmen
        HeatCool_Pipe,                          // Rohr
        HeatCool_PipeTurn,                      // Rohrbogen
        HeatCool_PipeReducer,                   // Reduzierung
        HeatCool_PipeTeeConnector,              // T-Stück
        HeatCool_PipeEndCap,                    // Endkappe / Klöpperboden
        HeatCool_Flange,                        // Flansch
        HeatCool_Chiller,                       // Kältemaschine
        HeatCool_CoolingTower,                  // Kühlturm
        HeatCool_ExpansionChamber,              // Ausdehnungsgefäß
        HeatCool_Boiler,                        // Heizkessel
        HeatCool_WaterHeater,                   // Warmwasserboiler
        HeatCool_StorageBoiler,                 // Speicherkessel / Schichtspeicher
        HeatCool_HeatExchanger,                 // Wärmetauscher
        HeatCool_Radiator,                      // Heizkörper
        HeatCool_Pump,                          // Pumpe
        HeatCool_Filter,                        // Filter
        HeatCool_Controlvalve,                  // Regelventil
        HeatCool_Adjustvalve,                   // Regulierventil
        HeatCool_BallValve,                     // Kugelhahn
        HeatCool_ButterflyValve,                // Absperrklappe
        HeatCool_SafetyValve,                   // Sicherheitsventil
        HeatCool_Flowmeter,                     // Durchflussmesser
        HeatCool_Sensor,                        // Sensor
        Sprinkler_Pipe,                         // Sprinklerrohr
        Sprinkler_PipeTurn,                     // Rohrbogen
        Sprinkler_PipeReducer,                  // Reduzierung
        Sprinkler_PipeEndCap,                   // Rohr-Endkappe
        Sprinkler_Head,                         // Sprinklerkopf
        Sprinkler_Pump,                         // Sprinklerpumpe
        Sprinkler_Valve,                        // Ventil
        Sprinkler_Distribution,                 // Verteiler
        Sprinkler_TeeConnector,                 // T-Stück
        Sprinkler_ZoneCheck,                    // Zone-Check
        Sprinkler_WetAlarmValve,                // Nassalarmventil
        Sprinkler_CompressedAirWaterContainer,  // Druckluftwasserbehälter
        Sanitary_Pipe,                          // Rohr (allgemein)
        Sanitary_PipeTurn,                      // Rohrbogen
        Sanitary_PipeReducer,                   // Reduzierung
        Sanitary_PipeTeeConnector,              // T-Stück
        Sanitary_PipeEndCap,                    // Endkappe
        Sanitary_Flange,                        // Flansch
        Sanitary_ElectricWaterHeater,           // Elektrischer Wasserboiler
        Sanitary_WashBasin,                     // Waschbecken
        Sanitary_Sink,                          // Ausgussbecken
        Sanitary_Shower,                        // Dusche
        Sanitary_EmergencyShower,               // Notdusche
        Sanitary_EmergencyEyeShower,            // Augendusche
        Sanitary_LiftingUnit,                   // Hebeanlage
        Electrical_CableTray,                   // Kabeltrasse
        Electrical_Cabinet                      // Schaltschrank


    };

    CADitem(ItemType type);
    void calculate();

    // data types tbd.
    ItemType getType();
    QString layer;
    QColor color_pen;       // Transparent means "BYLAYER"
    QColor color_brush;       // Transparent means "BYLAYER"
    QMap<QString, QString> attributes;
    QVector3D position;
    M3dBox boundingBox;
    QList<CADitem*> subItems;

    // Object Snap
    QList<QVector3D> snap_center;
    QList<QVector3D> snap_vertices;

    // Highlighting
    bool highlight;
    quint32 index;

private:
    ItemType type;
};

#endif // CADITEM_H
