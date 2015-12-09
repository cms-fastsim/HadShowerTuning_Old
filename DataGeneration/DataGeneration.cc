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
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "Randomize.hh"

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


int main(int argc,char** argv)
{

    if(argc<2){
	std::cout << "ERROR: bad arguments" << std::endl;
	std::cout << "USAGE: ./" << argv[0] << " outputFile.root                   # for default configuration" << std::endl;
	std::cout << "       ./" << argv[0] << " outputFile.root config.mac        # configure with geant4 macro" << std::endl;
	exit(1);
    }

    bool compressData = true;

    TFile * file = TFile::Open(argv[1],"RECREATE");

    G4RunManager* runManager = new G4RunManager;

    // the geometry
    hadshowertuning::DetectorConstructionASCI * detectorConstruction =new hadshowertuning::DetectorConstructionASCI();
    runManager->SetUserInitialization(detectorConstruction);

    // the physics list
    G4PhysListFactory factory;
    runManager->SetUserInitialization(factory.GetReferencePhysList("FTFP_BERT"));

    // set the properties of the primary particle
    runManager->SetUserAction(new hadshowertuning::PrimaryGeneratorAction());

    // event, tracking, stepping action
    TTree * tree = 0;
    if(!compressData)
	tree = new TTree("showers","showers");
    
    hadshowertuning::Data data;
    runManager->SetUserAction(new hadshowertuning::EventAction(file,tree,&data));
    runManager->SetUserAction(new hadshowertuning::TrackingAction(&data));
    std::map<const G4VPhysicalVolume*,int> volumeIdMap;
    runManager->SetUserAction(new hadshowertuning::SteppingAction(&data,&volumeIdMap));


    // initialise
    std::cout << "initializing..." << std::endl;
    runManager->Initialize();
    std::cout << "...initializing done" << std::endl;


    // define the tree
    if(tree){
	tree->Branch("hit_e",&data.hit_e);
	tree->Branch("hit_x",&data.hit_x);
	tree->Branch("hit_y",&data.hit_y);
	tree->Branch("hit_z",&data.hit_z);
	tree->Branch("hit_volumeId",&data.hit_volumeId);
	tree->Branch("hit_particleIndex",&data.hit_particleIndex);
	tree->Branch("hit_nSteps",&data.hit_nSteps);
	tree->Branch("particle_x",&data.particle_x);
	tree->Branch("particle_y",&data.particle_y);
	tree->Branch("particle_z",&data.particle_z);
	tree->Branch("particle_px",&data.particle_px);
	tree->Branch("particle_py",&data.particle_py);
	tree->Branch("particle_pz",&data.particle_pz);
	tree->Branch("particle_kinE",&data.particle_kinE);
	tree->Branch("particle_pdgId",&data.particle_pdgId);
	tree->Branch("particle_parentIndex",&data.particle_parentIndex);
    }
    // define the volume map
    volumeIdMap[detectorConstruction->getWorldVolume()] = 0;
    G4LogicalVolume * world = detectorConstruction->getWorldVolume()->GetLogicalVolume();
    for(int daughterIndex = 0 ; daughterIndex < world->GetNoDaughters();daughterIndex++)
    {
	volumeIdMap[world->GetDaughter(daughterIndex)] = daughterIndex+1;
    }

    // tree with geometry
    TTree * geomTree = new TTree("geometry","geometry");
    std::string name,material;
    double center_x,center_y,center_z,halfR_x,halfR_y,halfR_z;
    int id;
    geomTree->Branch("id",&id,"id/I");
    geomTree->Branch("name",&name);
    geomTree->Branch("material",&material);
    geomTree->Branch("center_x",&center_x,"center_x/D");
    geomTree->Branch("center_y",&center_y,"center_y/D");
    geomTree->Branch("center_z",&center_z,"center_z/D");
    geomTree->Branch("halfR_x",&halfR_x,"halfR_x/D");
    geomTree->Branch("halfR_y",&halfR_y,"halfR_y/D");
    geomTree->Branch("halfR_z",&halfR_z,"halfR_z/D");
  
    for( auto entry : volumeIdMap)
    {
	const G4VPhysicalVolume * physVol = entry.first;
	const G4LogicalVolume * logVol = physVol->GetLogicalVolume();
	const G4ThreeVector & center = physVol->GetTranslation();
	const G4Box * solid = dynamic_cast<G4Box *>(logVol->GetSolid());
	if(!solid)
	{
	    std::cout << "ERROR: geometry contains volumes that are not box-shaped. I cannot yet deal with that" << std::endl;
	    exit(1);
	}
	id = entry.second;
	name = physVol->GetName();
	material = logVol->GetMaterial()->GetName();
	center_x = center.x()/cm;
	center_y = center.y()/cm;
	center_z = center.z()/cm;
	halfR_x = solid->GetXHalfLength()/cm;
	halfR_y = solid->GetYHalfLength()/cm;
	halfR_z = solid->GetZHalfLength()/cm;
	geomTree->Fill();
    }
    geomTree->Write();

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    if(argc >= 4)
    {
	std::cout << "set seed to " << atoi(argv[3]) << std::endl;
	CLHEP::HepRandom::setTheSeed(atoi(argv[3]));
    }
    // if no extra arguments are provided, run a test run in default mode
    if(argc == 2)
    {
	int nEvents = 10;
	runManager->BeamOn(nEvents);
	std::cout << "WARNING: ran default configuration..." << std::endl;
	std::cout << "         run without command line arguments to print usage" << std::endl;
    }
    // else, 2nd argument is a geant4 macro
    // execut it
    else
    {
	G4UImanager* UI = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";
	G4String fileName = argv[2];
	UI->ApplyCommand(command+fileName); 
	std::cout << "INFO: configured with macro " << argv[2] << std::endl;
    }

    // clean-up
    delete runManager;
    if(tree)
	tree->Write();
    file->Close();

    
}
