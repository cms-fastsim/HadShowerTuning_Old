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
    std::vector<int> hit_pi0_index;
    Float_t showerStart_x,showerStart_y,showerStart_z;
    Float_t primary_px,primary_py,primary_pz,primary_kinE;
    Int_t primary_pdgId;
    std::vector<float> pi0_px;
    std::vector<float> pi0_py;
    std::vector<float> pi0_pz;
    std::vector<float> pi0_kinE;
    std::vector<float> pi0_x;
    std::vector<float> pi0_y;
    std::vector<float> pi0_z;
    
    void clear(){
      hit_energy.clear();
      hit_x.clear();
      hit_y.clear();
      hit_z.clear();
      hit_volume_id.clear();
      hit_pi0_index.clear();
      showerStart_x = -1.;
      showerStart_y = -1.;
      showerStart_z = -1.;
      primary_px = -1;
      primary_py = -1;
      primary_pz = -1;
      primary_kinE = - 1;
      primary_pdgId = -1;
      pi0_px.clear();
      pi0_py.clear();
      pi0_pz.clear();
      pi0_kinE.clear();
      pi0_x.clear();
      pi0_y.clear();
      pi0_z.clear();
    }
  };
}

#endif
