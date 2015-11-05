#include "TrackingAction.h"
#include "Data.h"
#include "TrackInformation.h"
#include "TVector3.h"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4ThreeVector.hh"
#include "G4TrackingManager.hh"
#include "G4SystemOfUnits.hh"

using namespace hadshowertuning;

// move all this shit to the stepping action... ???
// do something whenever a step produces secondaries...

TrackingAction::TrackingAction(Data * data)
  : fData(data)
  , fFoundShowerStart(false)
{}

TrackInformation * TrackingAction::storeParticle(G4Track * aTrack,int parentIndex)
{
    // index of the particle
    int particleIndex = fData->particle_px.size();
    // update the data connected to the tree
    const G4ThreeVector & position = aTrack->GetPosition();
    fData->particle_x.push_back(position.x()/cm);
    fData->particle_y.push_back(position.y()/cm);
    fData->particle_z.push_back(position.z()/cm);
    const G4ThreeVector momentum = aTrack->GetMomentum();
    fData->particle_px.push_back(momentum.x()/GeV);
    fData->particle_py.push_back(momentum.y()/GeV);
    fData->particle_pz.push_back(momentum.z()/GeV);
    fData->particle_pdgId.push_back(aTrack->GetDynamicParticle()->GetPDGcode());
    fData->particle_kinE.push_back(aTrack->GetKineticEnergy()/GeV);
    fData->particle_parentIndex.push_back(parentIndex);
    // update the track information
    TrackInformation * info = (TrackInformation*)(aTrack->GetUserInformation());
    info->setFirstInBranchIndex(particleIndex);
    info->setParticleIndex(particleIndex);
    info->setParentIndex(parentIndex);
    return info;
}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    // cheat away the constness
    G4Track * myTrack = fpTrackingManager->GetTrack();
    if(myTrack != aTrack){
	std::cout << "Miljaarde, miljaarde" << std::endl;
    }

    // when a new event starts, reset, and store primary
    if(myTrack->GetTrackID()==1)
    {
	fFoundShowerStart = false;
	myTrack->SetUserInformation(new TrackInformation());
	storeParticle(myTrack,-1);
    }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // get a non-const version of aTrack
    G4Track * myTrack = fpTrackingManager->GetTrack();
    if(myTrack != aTrack){
	std::cout << "Miljaarde, miljaarde" << std::endl;
    }

    // get the track info
    TrackInformation* myTrackInfo = (TrackInformation*)myTrack->GetUserInformation();
    
    // find secondaries
    G4TrackVector * secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries || secondaries->size()==0)
    {
	return;
    }

    /*
    bool heavyStuff = myTrack->GetDynamicParticle()->GetPDGcode() == 111 && myTrack->GetKineticEnergy() > (3*GeV);
    if(heavyStuff)
    {
	std::cout << "HS!!!" << std::endl;
	std::cout << myTrack->GetTrackID() << std::endl;
	std::cout << myTrack->GetDynamicParticle()->GetPDGcode() << std::endl;
	std::cout << myTrack->GetKineticEnergy()/GeV << std::endl;
    }
    */

    for(auto secondary : *secondaries)
    {

	// each track inherrits information from it's parent
	TrackInformation * secondaryInfo = new TrackInformation(myTrackInfo);
	secondary->SetUserInformation(secondaryInfo);
	// it is not stored yet
	secondaryInfo->setParticleIndex(-1);
	// and the parent might be stored
	secondaryInfo->setParentIndex(myTrackInfo->particleIndex());
	// and we determine later whether or not this particle initiates an EM sub-shower
	secondaryInfo->setInitiatesEMShower(false);

	// the process
	const G4VProcess * process = secondary->GetCreatorProcess();
	
        // store particles from the first inelastic hadronic interaction
	if( myTrack->GetTrackID() == 1   // showers must be initiated by the original particle
	    && process
	    && process->GetProcessType() == fHadronic
	    && dynamic_cast<const G4HadronInelasticProcess *>(process))
	{
	    // store the shower start
	    const G4ThreeVector & position = secondary->GetPosition();
	    fData->showerStart_x = position.x()/cm;
	    fData->showerStart_y = position.y()/cm;
	    fData->showerStart_z = position.z()/cm;

	    // store secondaries
	    storeParticle(secondary,myTrackInfo->particleIndex());
	    secondaryInfo->setInHadronicShower(true);
	}

	// store photons and electrons that initiate EM sub-showers
	bool goodPhoton = 
	    secondaryInfo->inHadronicShower()
	    && secondary->GetDynamicParticle()->GetPDGcode() == 22
	    && !myTrackInfo->isPartOfEMShower();
	bool goodElectron =
	    secondaryInfo->inHadronicShower()
	    && abs(secondary->GetDynamicParticle()->GetPDGcode()) == 11
	    && (!myTrackInfo->isPartOfEMShower()
		|| (myTrackInfo->initiatesEMShower() && myTrack->GetDynamicParticle()->GetPDGcode() == 22)
		);
	/*
	if(heavyStuff){
	    std::cout << goodPhoton << " " << goodElectron << std::endl;
	    std::cout << secondaryInfo->inHadronicShower() << std::endl;
	    std::cout << secondary->GetDynamicParticle()->GetPDGcode() << std::endl;
	    std::cout << !myTrackInfo->isPartOfEMShower() << std::endl;
	}
	*/
	if ( goodPhoton || goodElectron )
	{
	    // store parent track if not yet done
	    if(myTrackInfo->particleIndex() < 0)
	    {
		storeParticle(myTrack,myTrackInfo->parentIndex());
	    }
	    // store photon / electron if not yet done
	    if(secondaryInfo->particleIndex() < 0){
		storeParticle(secondary,myTrackInfo->particleIndex());
	    }
	    // this particle is part of an EM shower
	    secondaryInfo->setIsPartOfEMShower();
	    // and it might have initiated an EM shower
	    if(!myTrackInfo->isPartOfEMShower())  
	    {
		secondaryInfo->setInitiatesEMShower();
	    }
	}
    }
}


    
