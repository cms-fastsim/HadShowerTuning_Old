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

TrackingAction::TrackingAction(Data * data)
  : fData(data)
  , fFoundShowerStart(false)
{}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack){

  // when a new event starts, reset
  if(aTrack->GetTrackID()==1)
    fFoundShowerStart = false;
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack){

  // find the track's secondaries
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(!secondaries || secondaries->size()==0)
    return;

  // loop over the secondaryies
  for(auto & secondary : *secondaries){
    
    // if the track caries user information, pass it to the secondaries
    // in this way, tracks are made aware of their ancestors
    // here this is used to make tracks aware of whether or not they origin from a pi0
    // (see further down)
    if(aTrack->GetUserInformation()){
      secondary->SetUserInformation(new TrackInformation((TrackInformation*)aTrack->GetUserInformation()));
    }
 
    // only consider secondaries from hadronic inelastic interacions
    // note: secondaries of one track can originate from several processes
    const G4VProcess * process = secondary->GetCreatorProcess();
    if(!process)
      continue;
    if(!process->GetProcessType() == fHadronic)
      continue;
    if(!dynamic_cast<const G4HadronInelasticProcess *>(process))
      continue;
  
    // store the shower start
    if(!fFoundShowerStart){
      fFoundShowerStart = true;
      // the position
      const G4ThreeVector & position = secondary->GetPosition();
      fData->showerStart_x = position.x()/cm;
      fData->showerStart_y = position.y()/cm;
      fData->showerStart_z = position.z()/cm;
      // properties of the primary
      TVector3 primary;
      double restMass,phi,theta,p,kinE,energy;
      phi = aTrack->GetVertexMomentumDirection().phi();
      theta = aTrack->GetVertexMomentumDirection().theta();
      restMass = aTrack->GetDynamicParticle()->GetDefinition()->GetPDGMass();
      kinE = aTrack->GetVertexKineticEnergy();
      energy = kinE + restMass;
      p = sqrt(energy*energy - restMass*restMass);
      primary.SetMagThetaPhi(p,theta,phi);
      fData->primary_px = primary.Px()/GeV;
      fData->primary_py = primary.Py()/GeV;
      fData->primary_pz = primary.Pz()/GeV;
      fData->primary_kinE = kinE/GeV;
      fData->primary_pdgId =  aTrack->GetDynamicParticle()->GetPDGcode();
    }

  
    // store pi0s, make tracks aware they originate from pi0s
    if(secondary->GetDynamicParticle()->GetPDGcode() == 111){
      int pi0_index = fData->pi0_px.size();
      TrackInformation * info = new TrackInformation(pi0_index); // note: g4 takes care of deleting this
      secondary->SetUserInformation(info);
      const G4ThreeVector momentum = secondary->GetMomentum();
      const G4ThreeVector & position = secondary->GetPosition();
      fData->pi0_px.push_back(momentum.x()/GeV);
      fData->pi0_py.push_back(momentum.y()/GeV);
      fData->pi0_pz.push_back(momentum.z()/GeV);
      fData->pi0_x.push_back(position.x()/cm);
      fData->pi0_y.push_back(position.y()/cm);
      fData->pi0_z.push_back(position.z()/cm);
      fData->pi0_kinE.push_back(secondary->GetKineticEnergy()/GeV);
    }
  }
  
}
