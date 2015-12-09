#include "DetectorConstructionASCI.hh"

#include "G4SystemOfUnits.hh"
#include "G4tgbVolumeMgr.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"

using namespace hadshowertuning;

DetectorConstructionASCI::DetectorConstructionASCI() 
 : G4VUserDetectorConstruction()
{}

G4VPhysicalVolume * DetectorConstructionASCI::Construct(){

  G4String filename = "/nfs/dust/cms/user/lveldere/FastSimDev_Oct/g4fresh/HadShowerTuning/DataGeneration-build/geometry.txt";
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->AddTextFile(filename);
  fWorld = volmgr->ReadAndConstructDetector();
  return fWorld;
}
