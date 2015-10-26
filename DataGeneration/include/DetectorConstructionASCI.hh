#ifndef HADSHOWERTUNING_DETECTORCONSTRUCTION_H_
#define HADSHOWERTUNING_DETECTORCONSTRUCTION_H_

#include "G4VUserDetectorConstruction.hh"


namespace hadshowertuning {

    class DetectorConstructionASCI : public G4VUserDetectorConstruction
    {
	public:
	
      DetectorConstructionASCI();
      virtual G4VPhysicalVolume * Construct();
      G4VPhysicalVolume * getWorldVolume() {return fWorld;}
      
    private:
      
      G4VPhysicalVolume * fWorld;
    };
}

#endif
