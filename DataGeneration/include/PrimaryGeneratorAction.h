/*!
 * define the primary
 */

#ifndef HADSHOWERTUNING_PRIMARYGENERATORACTION_H_
#define HADSHOWERTUNING_PRIMARYGENERATORACTION_H_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"

namespace hadshowertuning {

    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {
	public:
	
	PrimaryGeneratorAction() 
	    : G4VUserPrimaryGeneratorAction()
	    , fParticleGun(0)
	{
	    G4int n_particle = 1;
	    fParticleGun = new G4ParticleGun(n_particle);
	    
	    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	    G4String particleName;
	    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="pi+"));
	    fParticleGun->SetParticleEnergy(10.0*GeV);
	    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
	}
	
	~PrimaryGeneratorAction(){
	    delete fParticleGun;
	}
	
	virtual void GeneratePrimaries(G4Event* anEvent){
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	}
	
	private:
	
	G4ParticleGun* fParticleGun;
    };
}

#endif
