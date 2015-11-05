# HadShowerTuning

## Installation

See [INSTALL.md](INSTALL.md)

## Tuning datasets

A first ROOT tree with tuning data can be found here:

Properties of primary particle:
   * type: pi+
   * kinetic energy: 10 GeV
   * origin: (0,0,0)
   * theta = 0 (theta: opening angle between postive z-axis and particle direction) 

Geometry: flat, idealised barrel calorimeter

/afs/cern.ch/work/l/lveldere/public/showers_pi_kinE10_eta0.root

## Generation of tuning data

Tuning data is generated with the help of geant4.
A simplified detector geometry is used:
each calorimeter layer is represented by a 
flat, homogeneous, square-shaped layer, 
of proper thichness and material.
No magnetic field is applied.
The volume around the calorimeter is filled with air.

Read more in HadShowerTuning/DataGeneration/README.md (under construction)

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
hit_energy        # vector<float>
hit_x             # vector<float>
hit_y             # vector<float>
hit_z             # vector<float>
hit_volume_id     # vector<int>  
hit_particleIndex # vector<int>

# hit_volume_id :
# Each calorimeter layer is given an id number.
# The self-explenatory name of the corresponding layer can be looked up in the auxiliary tree name volumeIdMap

# hit_particleIndex:
# the primary particle and certain particles inside the shower are stored (see below)
# each hit i is associated to the particle with index hit_particleIndex[i] 
# a hit associated to a particle is produced by the particle or by a particle further downstream

# location of the first inelastic hadronic interaction
showerStart_x   # float
showerStart_y   # float
showerStart_z   # float


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










