/*!
 *   shoot single particles on a target
 *   geometry of target defined with txt file   cmd line argument: geometry=relative/path/to/geometry.txt   (obligatory)
 *   store data in outputfile                   cmd line argument: oFile=relative/path/to/oFile.root        (obligatory)
 *   steer geant4 with macro                    cmd line argument: g4Macro=relative/path/to/macro.mac       (obligatory)
 *   standard output consists of one directory per shower in output file with
 *         - set of energy profiles
 *         - set of particle states (position, momentum, kinematic energy, PDG id)
 *   optional one can store in addition a detailed record of hits and tracks in TTree format   
 *                                              cmd line argument: makeTree                                 (optional)
 *   optional: set the seed for the random number generator
 *                                              cmd line argument: seed=N                                   (optional, default=1)
 */

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
#include "DetectorConstructionASCI.h"
#include "PrimaryGeneratorAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"

// other
#include "Data.h"

int main(int argc,char** argv)
{

    using namespace std;
    
    // parse command line arguments
    map<string,string> options;
    for(int i = 1;i<argc;i++)
    {
	string arg = argv[i];
	size_t pos = arg.find('=');
	string key = "";
	string value = "";
	if(pos != string::npos)
	{
	    key = arg.substr(0,pos);
	    value = arg.substr(pos+1,arg.size()-pos-1);
	}
	else
	{
	    key = arg;
	}
	options[key] = value;
    }
    // read obligatory arguments
    string opt_ofile(options.at("oFile"));
    string opt_geometry(options.at("geometry")); 
    string opt_g4macro(options.at("g4Macro")); 
    // read optional flags
    bool opt_makeTree = options.find("makeTree") != options.end();
    // read other optional arguments
    auto it = options.find("seed");
    int opt_seed(it == options.end() ? 1 : atoi(options["seed"].c_str()));

    // create main g4 object
    G4RunManager* runManager = new G4RunManager;
    
    // set the geometry
    hadshowertuning::DetectorConstructionASCI * detectorConstruction =new hadshowertuning::DetectorConstructionASCI(opt_geometry);
    runManager->SetUserInitialization(detectorConstruction);

    // set the physics list
    G4PhysListFactory factory;
    runManager->SetUserInitialization(factory.GetReferencePhysList("FTFP_BERT"));

    // set the properties of the primary particle
    runManager->SetUserAction(new hadshowertuning::PrimaryGeneratorAction());

    // define how to gather and store data
    TFile * file = TFile::Open(opt_ofile.c_str(),"RECREATE");
    hadshowertuning::Data data;
    runManager->SetUserAction(new hadshowertuning::EventAction(file,&data,opt_makeTree)); // takes care of storing relevant data
    runManager->SetUserAction(new hadshowertuning::TrackingAction(&data));                // gathers simTrack-level data
    std::map<const G4VPhysicalVolume*,int> volumeIdMap;
    runManager->SetUserAction(new hadshowertuning::SteppingAction(&data,&volumeIdMap));   // gather simStep-level data

    // initialise
    runManager->Initialize();
    
    // store the geometry in the output file in TTree format
    volumeIdMap[detectorConstruction->getWorldVolume()] = 0;
    G4LogicalVolume * world = detectorConstruction->getWorldVolume()->GetLogicalVolume();
    for(int d = 0 ; d < world->GetNoDaughters();d++)
    {
	auto daughter = world->GetDaughter(d);
	volumeIdMap[daughter] = d+1;
    }
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

    // set random seed
    std::cout << "set seed to " << opt_seed << std::endl;
    CLHEP::HepRandom::setTheSeed(opt_seed);

    // run geant4
    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = opt_g4macro;
    UI->ApplyCommand(command+fileName); 
    std::cout << "INFO: configured with macro " << fileName << std::endl;
    
    // clean-up
    delete runManager;
    file->Close();
    
}
