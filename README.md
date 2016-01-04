# HadShowerTuning

## Installation

See [INSTALL.md](INSTALL.md)

Note: analysis of tuing data does not require to install anything specifically (yet).
You just need ROOT.

## Analayis of tuning datasets

See [DataAnalysis/README.md](DataAnalysis/README.md)

## Tuning datasets

Barrel geometry ([DataGeneration/geometry_barrel.txt](DataGeneration/geometry_barrel.txt))
```
# pdgId = 211, theta = 0 energy in [0.5,150] GeV
/afs/cern.ch/work/l/lveldere/public/showers_V3/*.root
```

more to come

## Generation of tuning data

Tuning data is generated with the help of geant4.
A simplified detector geometry is used:
each calorimeter layer is represented by a 
flat, homogeneous, square-shaped layer, 
of proper thichness and material.
No magnetic field is applied.
The volume around the calorimeter is filled with air.

Read more in HadShowerTuning/DataGeneration/README.md (under construction)

## Understanding geometry files

geometries are defined in txt files, e.g.
[DataGeneration/geometry_barrel.txt](DataGeneration/geometry_barrel.txt)

A small extract illustrates how to read these files
```
// the most inner scintilator layer HB has a thickness of 0.9 cm,
// it is made of vinyltoluene, 
// and it's inner side is 177.7 cm away from the center of the detector

// we name this layer 'hb_sci_0'
// we  model this with a simple BOX shape
// along the x and y direction, the box half-width is 360 cm
// along the z-direction, the box has a half width of 0,45 cm  (0.9 cm / 2)
// the box is made of vinyltoluene 
:VOLU hb_sci_0 BOX 360.0*cm 360.0*cm 0.45*cm G4_PLASTIC_SC_VINYLTOLUENE 

// we place the center of the box at x=0, y=0, z=178.15 cm
// (178.15 cm = 177.7 + 0.9/2 cm)
:PLACE hb_sci_0 1 world R0 0.*cm 0.*cm 178.15*cm
```

## Tuning data format

The tuning data is stored in ROOT trees.

All distance observables are given in units of cm.
All energy and momentum measures are given in units of GeV.
The coordinate system is chosen such that the detector layers
are centered around, and perpendicular to, the z-axis.
The z-coordinate of each layer corresponds to the r-coordinate of the corresponding layer in the actual CMS detector.

```
# tree name: showers
# each tree entry corresponds to one shower

# Basic properties of energy depositions
hit_e             # vector<float>
hit_x             # vector<float>
hit_y             # vector<float>
hit_z             # vector<float>
hit_volumeId      # vector<int>  
hit_particleIndex # vector<int>

# hit_volumeId :
# Each calorimeter layer is given an id number.
# The self-explenatory name of the corresponding layer can be looked up in the auxiliary tree name volumeIdMap

# hit_particleIndex:
# the primary particle and certain particles inside the shower are stored (see below)
# each hit i is associated to the particle with index hit_particleIndex[i] 
# a hit associated to a particle is produced by the particle or by a particle further downstream

# a selection of particle is stored in the particle list
# the primary particle is always stored with index 0
# all secondaries of the first inelastic interaction are stored
# all electrons that initiate EM showers are stored
# the parents of those electrons are stored (mostly photons)
# the parents of those photons are stored (mostly pi0s)
particle_x           # vector<float>
particle_y           # vector<float>
particle_z           # vector<float>
particle_px          # vector<float>
particle_py          # vector<float>
particle_pz          # vector<float>
particle_kinE        # vector<float>
particle_parentIndex # vector<int>
```

### how to find the shower start?

The datas format is defined such that the first particle in each shower is the primary,
and the next few particles are the products of the first inelastic hadronic interaction of the primary.
Therefore, the position of the shower start can be found as follows:

```
if len(paritcles_x) > 1:  # check that there was a first inelastic hadronic interaction
    showerStart_x = particle_x[1]
    showerStart_y = particle_x[1]
    showerStart_z = particle_x[1]
```

## Compressed tuning data

### Location

/afs/cern.ch/work/l/lveldere/public/hadshowertuningdata_compressed_V2.tar

Untar to find 418 root files.
Each root file contains data for 500 showers.
The primaries are all charged pions.
The energy of the primaries can be extracted from the file name.

### Format

Each root file contains directory per shower.

Each directory contains 
   * finely binned longitudinal energy profiles (TH1F) for
      * longEProf_fineBin_mip: primary and secondaries outside the shower
      * longEProf_fineBin_pi0_1: shower from hardest pi0 inside the hadronic shower
      * longEProf_fineBin_pi0_2: shower from 2nd hardest pi0 inside the hadronic shower
      * longEProf_fineBin_had: anything else
   * longitudinal energy profiles (TH1F), one bin per detector layer 
      * longEProf_perLayer_* (see explanation above)
   * showerStart: position of shower start (TVector3)
   * primary_atEndVertex_position : position of the primary where it dies in geant4 (usualy the shower start, but low energy particles may loose all energy before the shower starts) (TVector3)
   * primary_atEndVertex_momentum : momentum of the primary where it stops existing before it dies in geant4 (TVector3)
   * primary_atEndVertex_kinE : first element of this vector is the kinematic energy of the particle before it dies in geant4 (TVectorT<double>)
   * vertex_pi0_1: shower from hardest pi0 (TVector3)
   * vector_pi0_2: shower from 2nd hardest pi0 (TVector3)
    






