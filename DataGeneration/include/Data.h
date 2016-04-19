/*!
 *  data container used to pass information between geant4 user actions
 */

#ifndef HADSHOWER_DATA
#define HADSHOWER_DATA

#include "vector"
#include "string"
#include "RtypesCore.h"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

namespace hadshowertuning
{

    class Data 
    {
	
    public:
	
	Data()
	    {
		this->clear();
	    }

	// simhit information
	std::vector<float> hit_e;
	std::vector<float> hit_x;
	std::vector<float> hit_y;
	std::vector<float> hit_z;
	std::vector<int>   hit_volumeId;
	std::vector<int>   hit_particleIndex;

	// simtrack information
	// mother daughter relation is stored
	std::vector<float> particle_x;
	std::vector<float> particle_y;
	std::vector<float> particle_z;
	std::vector<float> particle_px;
	std::vector<float> particle_py;
	std::vector<float> particle_pz;
	std::vector<float> particle_kinE;
	std::vector<int>   particle_pdgId;
	std::vector<int>   particle_parentIndex;

	// store information of additional tracks or states of tracks
	std::vector<float>  extraParticle_x;
	std::vector<float>  extraParticle_y;
	std::vector<float>  extraParticle_z;
	std::vector<float>  extraParticle_px;
	std::vector<float>  extraParticle_py;
	std::vector<float>  extraParticle_pz;
	std::vector<float>  extraParticle_kinE;
	std::vector<int>    extraParticle_pdgId;
	std::vector<std::string> extraParticle_name;
	
	void clear()
	{
	    hit_e.clear();
	    hit_x.clear();
	    hit_y.clear();
	    hit_z.clear();
	    hit_volumeId.clear();
	    hit_particleIndex.clear();
	    
	    particle_x.clear();
	    particle_y.clear();
	    particle_z.clear();
	    particle_px.clear();
	    particle_py.clear();
	    particle_pz.clear();
	    particle_kinE.clear();
	    particle_pdgId.clear();
	    particle_parentIndex.clear();
	    
	    extraParticle_x.clear();
	    extraParticle_y.clear();
	    extraParticle_z.clear();
	    extraParticle_px.clear();
	    extraParticle_py.clear();
	    extraParticle_pz.clear();
	    extraParticle_kinE.clear();
	    extraParticle_name.clear();
	}

	void addExtraParticle(const G4ThreeVector & position,const G4ThreeVector & momentum,G4double kinE,int pdgId,std::string name)
	{
	    extraParticle_x.push_back(position.x()/cm);
	    extraParticle_y.push_back(position.y()/cm);
	    extraParticle_z.push_back(position.z()/cm);
	    extraParticle_px.push_back(momentum.x()/GeV);
	    extraParticle_py.push_back(momentum.y()/GeV);
	    extraParticle_pz.push_back(momentum.z()/GeV);
	    extraParticle_kinE.push_back(kinE/GeV);
	    extraParticle_pdgId.push_back(pdgId);
	    extraParticle_name.push_back(name);
	}
	
    };
}

#endif
