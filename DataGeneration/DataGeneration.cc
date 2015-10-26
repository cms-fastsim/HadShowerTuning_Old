// root
#include "TFile.h"
#include "TTree.h"

// g4 classes
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4VVisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4LogicalVolume.hh"

// g4-related user classes
#include "DetectorConstructionASCI.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"

// other
#include "Data.h"

//
// class declaration
//

int main(int argc,char** argv){

  TFile * file = TFile::Open("test123.root","RECREATE");

  G4UIExecutive* ui = 0;
  if ( argc > 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4RunManager* runManager = new G4RunManager;

  hadshowertuning::DetectorConstructionASCI * detectorConstruction =new hadshowertuning::DetectorConstructionASCI();
  runManager->SetUserInitialization(detectorConstruction);

  // set the physics list
  G4PhysListFactory factory;
  runManager->SetUserInitialization(factory.GetReferencePhysList("FTFP_BERT"));

  // set the properties of the primary particle
  runManager->SetUserAction(new hadshowertuning::PrimaryGeneratorAction());

  // fill a tree at the end of each event
  TTree * tree = new TTree("showers","showers");
  hadshowertuning::Data data;
  tree->Branch("hit_energy",&data.hit_energy);
  tree->Branch("hit_x",&data.hit_x);
  tree->Branch("hit_y",&data.hit_y);
  tree->Branch("hit_z",&data.hit_z);
  tree->Branch("hit_volume_id",&data.hit_volume_id);
  tree->Branch("hit_pi0_index",&data.hit_pi0_index);
  tree->Branch("showerStart_x",&data.showerStart_x,"showerStart_x/F");
  tree->Branch("showerStart_y",&data.showerStart_y,"showerStart_y/F");
  tree->Branch("showerStart_z",&data.showerStart_z,"showerStart_z/F");
  tree->Branch("primary_px",&data.primary_px,"primary_px/F");
  tree->Branch("primary_py",&data.primary_py,"primary_py/F");
  tree->Branch("primary_pz",&data.primary_pz,"primary_pz/F");
  tree->Branch("primary_pdgId",&data.primary_pdgId,"primary_pdgId/I");
  tree->Branch("primary_kinE",&data.primary_kinE,"primary_kinE/F");
  tree->Branch("pi0_x",&data.pi0_x);
  tree->Branch("pi0_y",&data.pi0_y);
  tree->Branch("pi0_z",&data.pi0_z);
  tree->Branch("pi0_px",&data.pi0_px);
  tree->Branch("pi0_py",&data.pi0_py);
  tree->Branch("pi0_pz",&data.pi0_pz);
  tree->Branch("pi0_kinE",&data.pi0_kinE);
  runManager->SetUserAction(new hadshowertuning::EventAction(tree,&data));
  runManager->SetUserAction(new hadshowertuning::TrackingAction(&data));
  std::map<const G4VPhysicalVolume*,int> volumeIdMap;
  runManager->SetUserAction(new hadshowertuning::SteppingAction(&data,&volumeIdMap));
  
  // initialise
  runManager->Initialize();

  // map the physical volumes
  volumeIdMap[detectorConstruction->getWorldVolume()] = 0;
  G4LogicalVolume * world = detectorConstruction->getWorldVolume()->GetLogicalVolume();
  for(int daughterIndex = 0 ; daughterIndex < world->GetNoDaughters();daughterIndex++){
    volumeIdMap[world->GetDaughter(daughterIndex)] = daughterIndex+1;
  }
  std::cout << volumeIdMap.size() << std::endl;

  // tree with auxiliary information
  TTree * volumeIdTree = new TTree("volumeIdMap","volumeIdMap");
  std::string volume_name;
  int volume_id;
  volumeIdTree->Branch("volume_name",&volume_name);
  volumeIdTree->Branch("volume_id",&volume_id,"volume_id/I");
  for( auto entry : volumeIdMap){
    volume_name = entry.first->GetName();
    volume_id = entry.second;
    volumeIdTree->Fill();
  }
  volumeIdTree->Write();

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

   
  // run
  if(ui)
    ui->SessionStart();
   
  if(!ui){
    int nEvents = 10000;
    runManager->BeamOn(nEvents);
    std::cout << "123" << std::endl;
  }
  delete runManager;


  tree->Write();
  file->Close();
   
}
