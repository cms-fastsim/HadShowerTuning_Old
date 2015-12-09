#ifndef HADSHOWER_DATA
#define HADSHOWER_DATA

#include "vector"
#include "RtypesCore.h"

namespace hadshowertuning
{

    class Data 
    {
	
    public:
	
	Data()
	    {
		this->clear();
	    }
    
	std::vector<float> hit_e;
	std::vector<float> hit_x;
	std::vector<float> hit_y;
	std::vector<float> hit_z;
	std::vector<int>   hit_volumeId;
	std::vector<int>   hit_particleIndex;
	std::vector<int>   hit_nSteps;
	std::vector<float> particle_x;
	std::vector<float> particle_y;
	std::vector<float> particle_z;
	std::vector<float> particle_px;
	std::vector<float> particle_py;
	std::vector<float> particle_pz;
	std::vector<float> particle_kinE;
	std::vector<int>   particle_pdgId;
	std::vector<int>   particle_parentIndex;
    
	void clear()
	    {
		hit_e.clear();
		hit_x.clear();
		hit_y.clear();
		hit_z.clear();
		hit_volumeId.clear();
		hit_particleIndex.clear();
		hit_nSteps.clear();
		particle_x.clear();
		particle_y.clear();
		particle_z.clear();
		particle_px.clear();
		particle_py.clear();
		particle_pz.clear();
		particle_kinE.clear();
		particle_pdgId.clear();
		particle_parentIndex.clear();
	    }
    };
}

#endif
