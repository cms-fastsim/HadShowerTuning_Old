#ifndef HADSHOWER_DATA
#define HADSHOWER_DATA

#include "vector"
#include "RtypesCore.h"

namespace hadshowertuning{

  class Data {
	
  public:

    Data(){
      this->clear();
    }
    
    std::vector<float> hit_energy;
    std::vector<float> hit_x;
    std::vector<float> hit_y;
    std::vector<float> hit_z;
    std::vector<int> hit_volume_id;
    std::vector<int> hit_particleIndex;
    Float_t showerStart_x,showerStart_y,showerStart_z;
    std::vector<float> particle_x;
    std::vector<float> particle_y;
    std::vector<float> particle_z;
    std::vector<float> particle_px;
    std::vector<float> particle_py;
    std::vector<float> particle_pz;
    std::vector<float> particle_kinE;
    std::vector<int> particle_pdgId;
    std::vector<int> particle_parentIndex;
    
    void clear(){
      hit_energy.clear();
      hit_x.clear();
      hit_y.clear();
      hit_z.clear();
      hit_volume_id.clear();
      hit_particleIndex.clear();
      showerStart_x = -1.;
      showerStart_y = -1.;
      showerStart_z = -1.;
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
