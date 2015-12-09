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

void TrackingAction::storeParticle(G4Track * aTrack,int parentIndex)
{
    // get user information
    TrackInformation * info = (TrackInformation*)aTrack->GetUserInformation();
    if(!info){
	info = new TrackInformation();
	aTrack->SetUserInformation(info);
    }

    // check that particle was not stored yet
    if(info->particleIndex() >= 0){
	std::cout << "ERROR: attempt to store particle that is stored already" << std::endl;
	exit(1);
    }

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

    // update trackinformation
    info->setFirstInBranchIndex(particleIndex);
    info->setParticleIndex(particleIndex);
    info->setParentIndex(parentIndex); 
}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    // cheat away the constness of aTrack
    G4Track * myTrack = fpTrackingManager->GetTrack();
    if(myTrack != aTrack){
	std::cout << "Miljaarde, miljaarde" << std::endl;
    }

    // when a new event starts, reset, and store primary
    if(myTrack->GetTrackID()==1)
    {
	fFoundShowerStart = false;
	storeParticle(myTrack,-1);
    }
    
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // cheat away the constness of aTrack
    G4Track * myTrack = fpTrackingManager->GetTrack();
    if(myTrack != aTrack){
	std::cout << "Miljaarde, miljaarde" << std::endl;
    }

    // get the track info
    TrackInformation * myTrackInfo = (TrackInformation*)myTrack->GetUserInformation();
    
    // find secondaries
    G4TrackVector * secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries || secondaries->size()==0)
    {
	return;
    }

    for(auto secondary : *secondaries)
    {

	// initiate info
	TrackInformation * secondaryInfo = new TrackInformation();
	// inherrit some stuff from parent particle
	secondaryInfo->setParentIndex(myTrackInfo->particleIndex());  // -1 if parent not stored  
	secondaryInfo->setFirstInBranchIndex(myTrackInfo->firstInBranchIndex());
	secondaryInfo->setInEMShower(myTrackInfo->inEMShower());
	secondaryInfo->setInHadronicShower(myTrackInfo->inHadronicShower());
	secondary->SetUserInformation(secondaryInfo);

	// the process
	const G4VProcess * process = secondary->GetCreatorProcess();
	
        // store particles from the first inelastic hadronic interaction
	if( myTrack->GetTrackID() == 1   // to avoid weird cases: showers must be initiated by the original particle
	    && process
	    && process->GetProcessType() == fHadronic
	    && dynamic_cast<const G4HadronInelasticProcess *>(process))
	{
	    // store secondaries
	    storeParticle(secondary,myTrackInfo->particleIndex());
	    secondaryInfo->setInHadronicShower(true);
	}

	// don't bother with anything that is not in the hadronic shower
	if(!secondaryInfo->inHadronicShower())
	    continue;

	// store photons and electrons that initiate EM sub-showers
	bool goodPhoton = 
	    secondary->GetDynamicParticle()->GetPDGcode() == 22
	    && !myTrackInfo->inEMShower();
	bool goodElectron =
	    abs(secondary->GetDynamicParticle()->GetPDGcode()) == 11
	    && (    !myTrackInfo->inEMShower()
		    || (    myTrackInfo->initiatesEMShower() 
			    && myTrack->GetDynamicParticle()->GetPDGcode() == 22
			)
		);
	if ( goodPhoton || goodElectron )
	{
	    // store parent track if not yet done
	    if(myTrackInfo->particleIndex() < 0)
	    {
		storeParticle(myTrack,myTrackInfo->parentIndex());
	    }
	    // store photon / electron if not yet done
	    if(secondaryInfo->particleIndex() < 0)
	    {
		storeParticle(secondary,myTrackInfo->particleIndex());
	    }
	    // this particle is part of an EM shower
	    secondaryInfo->setInEMShower();
	    // and it might have initiated an EM shower
	    if(!myTrackInfo->inEMShower())  
	    {
		secondaryInfo->setInitiatesEMShower();
	    }
	}

	// store all pi0 and eta mesons
	if(secondary->GetDynamicParticle()->GetPDGcode() == 221 // eta
	   || secondary->GetDynamicParticle()->GetPDGcode() == 111) // pi0
	{
	    // store photon / electron if not yet done
	    if(secondaryInfo->particleIndex() < 0)
	    {
		storeParticle(secondary,myTrackInfo->particleIndex());
	    }
	}
	    

	// store all dauthers of eta and pi0 mesons
	if(myTrack->GetDynamicParticle()->GetPDGcode() == 221
	   || myTrack->GetDynamicParticle()->GetPDGcode() == 221)
	{
	    if(secondaryInfo->particleIndex() < 0)
	    {
		storeParticle(secondary,myTrackInfo->particleIndex());
		
	    }
	}
    }
}


    
