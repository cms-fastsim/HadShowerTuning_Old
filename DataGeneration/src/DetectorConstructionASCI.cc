#include "DetectorConstructionASCI.h"

#include "G4SystemOfUnits.hh"
#include "G4tgbVolumeMgr.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"

using namespace hadshowertuning;

DetectorConstructionASCI::DetectorConstructionASCI(std::string geometryFile) 
 : G4VUserDetectorConstruction()
 , geometryFile_(geometryFile)
{}

G4VPhysicalVolume * DetectorConstructionASCI::Construct()
{
    G4String filename = geometryFile_;
    G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
    volmgr->AddTextFile(filename);
    fWorld = volmgr->ReadAndConstructDetector();
    return fWorld;
}
