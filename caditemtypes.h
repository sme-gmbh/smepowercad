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

#ifndef CADITEMTYPES_H
#define CADITEMTYPES_H

#include <QObject>
#include <QMetaEnum>

class CADitemTypes : public QObject
{
    Q_OBJECT
    Q_ENUMS(ItemType)

public:
    explicit CADitemTypes(QObject *parent = 0);

    enum ItemType
    {
        None,

        Air_Duct,                               // Kanal (eckig)
        Air_Pipe,                               // Rohr (rund)
        Air_DuctFireResistant,                  // Kanal (feuerbeständig)
        Air_DuctTurn,                           // Kanalbogen
        Air_PipeTurn,                           // Rohrbogen
        Air_PipeReducer,                        // Rohrreduzierung
        Air_DuctTeeConnector,                   // Kanal-T-Stück
        Air_DuctTransitionRectRect,                     // Kanalübergang (eckig)
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
        Air_PipeBranch,                         // Rohr-Abzweig
        Air_DuctBaffleSilencer,                 // Kanal-Kulissenschalldämpfer
        Air_Fan,                                // Ventilator
        Air_Humidifier,                         // Befeuchter
        Air_EmptyCabinet,                       // Leergerät
        Air_EquipmentFrame,                     // Geräterahmen
        Air_LinearDiffuser,                     // Linearauslass

        Arch_Foundation,                        // Fundament
        Arch_BoredPile,                         // Bohrpfahl
        Arch_LevelSlab,                         // Decke
        Arch_Wall_loadBearing,                  // Tragende Wand
        Arch_Wall_nonLoadBearing,               // Nicht tragende Wand
        Arch_Grating,                           // Gitterrost
        Arch_BlockOut,
        Arch_Support,                           // Stütze
        Arch_Beam,                              // Unterzug
        Arch_Door,
        Arch_Window,

        Basic_Arc,                              // Basisgeometrie: Bogen
        Basic_Box,                              // Basisgeometrie: Box
        Basic_Circle,                           // Basisgeometrie: Kreis
        Basic_Cylinder,                         // Basisgeometrie: Zylinder
        Basic_Duct,                             // Basisgeometrie: Kanal
        Basic_Face,                             // Basisgeometrie: Fläche
        Basic_Flume,                            // Basisgeometrie: Rinne
        Basic_Gauge,                            // Basisgeometrie: Anzeigegerät
        Basic_Gauge90Degree,                    // Basisgeometrie: Anzeigegerät mit senkrechtem Fühler
        Basic_Hemisphere,                       // Basisgeometrie: Halbkugel
        Basic_Line,                             // Basisgeometrie: Linie
        Basic_Pipe,                             // Basisgeometrie: Rohr
        Basic_PipeReducer,
        Basic_Plane,                            // Basisgeometrie: Ebene
        Basic_Point,                            // Basisgeometrie: Punkt
        Basic_Polyline,                         // Basisgeometrie: Polylinie
        Basic_Sphere,                           // Basisgeometrie: Kugel
        Basic_Turn,                             // Basisgeometrie: Rohrbogen
        Basic_TorisphericalHeadDIN28011,        // Klöpperboden (nach DIN28011)
        Basic_TorisphericalHeadDeepDishedDIN28013,  // Korbbogenboden (nach DIN28013)
        Basic_Torus,                            // Torus (Donut)
        Basic_Triangle,                         //Dreieck

        Cleanroom_CeilingCornerPiece,
        Cleanroom_CeilingFilterFanUnit,
        Cleanroom_CeilingFrameFeedThrough,
        Cleanroom_CeilingFrame,
        Cleanroom_CeilingGrating,
        Cleanroom_CeilingJoiningKnot,
        Cleanroom_CeilingMaintenanceFlap,
        Cleanroom_CeilingMountingRails,
        Cleanroom_CeilingPanel,
        Cleanroom_CeilingSmokeExtractFlap,
        Cleanroom_CeilingSuspension,
        Cleanroom_CeilingTeeJoiningPiece,
        Cleanroom_CeilingVerticalLadder,
        Cleanroom_ControlButton,
        Cleanroom_ControlEmergencySwitch,
        Cleanroom_ControlLedTouchKey,
        Cleanroom_ControlRadarSensor,
        Cleanroom_ControlSwitch,
        Cleanroom_DoorFrame,
        Cleanroom_DoorSlidingDouble,
        Cleanroom_DoorSlidingSingle,
        Cleanroom_DoorSwingingDouble,
        Cleanroom_DoorSwingingSingle,
        Cleanroom_FloorStiffenerDiagonal,
        Cleanroom_FloorStiffenerHorizontal,
        Cleanroom_FloorGrating,
        Cleanroom_FloorPanel,
        Cleanroom_FloorPanelWithBushing,
        Cleanroom_FloorPanelWithTank,
        Cleanroom_FloorPanelPerforated,
        Cleanroom_FloorSupport,
        Cleanroom_TagElectricalGrounding,
        Cleanroom_TagLeakageDetector,
        Cleanroom_TagSmokeDetector,
        Cleanroom_TagFireDetector,
        Cleanroom_VacuumCleanerSocket,
        Cleanroom_WallStiffenerDiagonal,
        Cleanroom_WallMountingProfile,
        Cleanroom_WallOverflowGrate,
        Cleanroom_WallPanel,
        Cleanroom_WallPost,
        Cleanroom_WallSmokeExtractFlap,

        Electrical_BusbarEndFeederUnitDoubleSided,  // Stromschiene Einspeisung zweiseitig
        Electrical_BusbarEndFeederUnitSingleSided,  // Stromschiene Einspeisung einseitig
        Electrical_BusbarTapOffUnit,
        Electrical_BusbarWithoutTapoffPoints,   // Stromschiene
        Electrical_BusbarWithTapoffPoints1Row,  //
        Electrical_BusbarWithTapoffPoints2Row,  //
        Electrical_Cabinet,                     // Schaltschrank
        Electrical_CabinetWithDoorFrontAndBack, // Schaltschrank mit Türen vorne und hinten
        Electrical_CabinetWithDoorLeftAndRight, // Schaltschrank mit zwei Türen an der Vorderseite
        Electrical_CabinetWithoutDoor,          // Schaltschrank ohne Tür
        Electrical_Cabletray,                   // Kabeltrasse
        Electrical_CabletrayCross,              // Kabeltrasse Kreuzung
        Electrical_CabletrayReducer,            // Kabeltrasse Übergang
        Electrical_CabletrayTeeConnector,       // Kabeltrasse T-Stück
        Electrical_CabletrayTransition,         // Kabeltrasse Übergang (in z-Richtung)
        Electrical_CabletrayTurn,               // Kabeltrasse Ecke
        Electrical_CabletrayVerticalLadder,     // Kabeltrasse vertikale Leiter
        Electrical_EquipmentSwitchOrSocket,     // Schalter oder Steckdose
        Electrical_LuminaireEscapeLighting,     // Notausgangsleuchte
        Electrical_LuminaireRailMounted,
        Electrical_LuminaireRecessedMounted,    // Leuchte ecking, Unterputz
        Electrical_LuminaireSemicircular,       // Leutche rund, Aufputz
        Electrical_LuminaireSurfaceMounted,     // Leuchte eckig, Aufputz
        Electrical_MotorAsynchronous,           // Asynchronmotor

        Gas_ActivatedCarbonAdsorber,
        Gas_Compressor,
        Gas_DryerDesiccant,
        Gas_DryerRefrigerant,
        Gas_Filter,
        Gas_GaugePressure,
        Gas_Hose,
        Gas_LiquidSeparator,
        Gas_OilWaterSeparationSystem,
        Gas_Pipe,
        Gas_PipeTeeConnector,
        Gas_PipeTurn,
        Gas_PressureRegulator,
        Gas_Pump,
        Gas_QuicklockCoupling,
        Gas_SensorFlowrate,
        Gas_SensorMoisture,
        Gas_Tank,
        Gas_ValveBall,
        Gas_ValveDiaphragm,
        Gas_ValveNonReturn,

        HeatCool_32WayValve,                   // 3/2 Wegeventil
        HeatCool_Adjustvalve,                   // Regulierventil
        HeatCool_BallValve,                     // Kugelhahn
        HeatCool_Boiler,                        // Heizkessel
        HeatCool_ButterflyValveBolted,          // Absperrklappe mit Gewindeaugen
        HeatCool_ButterflyValveClamped,         // Absperrklappe eingeklemmt
        HeatCool_Chiller,                       // Kältemaschine
        HeatCool_Controlvalve,                  // Regelventil
        HeatCool_CoolingTower,                  // Kühlturm
        HeatCool_DirtArrester,                  // Schmutzfänger
        HeatCool_ExpansionChamber,              // Ausdehnungsgefäß
        HeatCool_Filter,                        // Filter
        HeatCool_Flange,                        // Flansch
        HeatCool_Flowmeter,                     // Durchflussmesser
        HeatCool_Gauge,                         // Anzeigegerät
        HeatCool_Gauge90Degree,                 // Anzeigegerät mit senkrechtem Fühler
        HeatCool_HeatexchangerSoldered,         // Wärmetauscher gelötet
        HeatCool_HeatexchangerBolted,           // Wärmetauscher geschraubt
        HeatCool_NonReturnFlap,                 // Rückschlagklappen
        HeatCool_NonReturnValve,                // Rückschlagarmatur
        HeatCool_Pipe,                          // Rohr
        HeatCool_PipeEndCap,                    // Endkappe / Klöpperboden
        HeatCool_PipeReducer,                   // Reduzierung
        HeatCool_PipeTeeConnector,              // T-Stück
        HeatCool_PipeTurn,                      // Rohrbogen
        HeatCool_PumpInline,                    // Inlinepumpe
        HeatCool_PumpNorm,                      // Normpumpe
        HeatCool_Radiator,                      // Heizkörper
        HeatCool_RadiatorCompact,               // Kompaktheizkörper
        HeatCool_RadiatorFlange,                // Anschlussstück für Ventilheizkörper
        HeatCool_RadiatorFlangeBent,            // Anschlussstück für Ventilheizkörper, gebogen
        HeatCool_RadiatorValve,                 // Ventilheizkörper
        HeatCool_SafetyValve,                   // Sicherheitsventil
        HeatCool_Sensor,                        // Sensor
        HeatCool_StorageBoiler,                 // Speicherkessel / Schichtspeicher
        HeatCool_Valve,                         // Armatur
        HeatCool_Valve90Degree,                 // Eckarmatur
        HeatCool_ValveHandwheel,                // Handrad zur Ventilsteuerung
        HeatCool_ValveHandwheelGear,            // Handrad mit Getriebe zur Ventilsteuerung
        HeatCool_ValveLever,                    // Hebel zur Ventilsteuerung
        HeatCool_ValveMotorRect,                // rechteckiger Motor zur Ventilsteuerung
        HeatCool_ValveMotorRound,               // zylindrischer Motor zur Ventilsteuerung
        HeatCool_WaterHeater,                   // Warmwasserboiler

        Sanitary_CleaningPiece,
        Sanitary_Pipe,                          // Rohr (allgemein)
        Sanitary_PipeEndCap,                    // Endkappe
        Sanitary_PipeReducer,                   // Reduzierung
        Sanitary_PipeTeeConnector,              // T-Stück
        Sanitary_PipeTurn,                      // Rohrbogen
        Sanitary_PipeYPiece,                    // Y-Stück
        Sanitary_Flange,                        // Flansch
        Sanitary_ElectricWaterHeater,           // Elektrischer Wasserboiler
        Sanitary_WashBasin,                     // Waschbecken
        Sanitary_Sink,                          // Ausgussbecken
        Sanitary_Shower,                        // Dusche
        Sanitary_EmergencyShower,               // Notdusche
        Sanitary_EmergencyEyeShower,            // Augendusche
        Sanitary_LiftingUnit,                   // Hebeanlage

        Sprinkler_CompressedAirWaterContainer,  // Druckluftwasserbehälter
        Sprinkler_Distribution,                 // Verteiler
        Sprinkler_Flange,                       // Flansch
        Sprinkler_Head,                         // Sprinklerkopf
        Sprinkler_Pipe,                         // Sprinklerrohr
        Sprinkler_PipeEndCap,                   // Rohr-Endkappe
        Sprinkler_PipeReducer,                  // Reduzierung
        Sprinkler_PipeTeeConnector,             // T-Stück
        Sprinkler_PipeTurn,                     // Rohrbogen
        Sprinkler_Pump,                         // Sprinklerpumpe
        Sprinkler_Valve,                        // Ventil
        Sprinkler_WetAlarmValve,                // Nassalarmventil
        Sprinkler_ZoneCheck,                    // Zone-Check

        LastItem
    };


    QString getEnumNameOfItemType(CADitemTypes::ItemType type);
    int countOfItemTypes();

signals:

public slots:

};

#endif // CADITEMTYPES_H
