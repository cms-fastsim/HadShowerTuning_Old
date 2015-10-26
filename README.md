# HadShowerTuning

## Generation of tuning data

Tuning data is generated with the help of geant4.
A simplified detector geometry is used:
each calorimeter layer is represented by a 
flat, homogeneous, square-shaped layer, 
of proper thichness and material.
No magnetic field is applied.
The volume around the calorimeter is filled with air.

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
hit_energy      # vector<float>
hit_x           # vector<float>
hit_y           # vector<float>
hit_z           # vector<float>

# Each calorimeter layer is given an id number.
# The self-explenatory name of the corresponding layer can be looked up in the auxiliary tree name volumeIdMap
hit_volume_id   # vector<int>  

# each pi0 produced in hadronic inelastic interactions in showers are stored in the tree (see further)
# when a hit is associated to such a pi0, the index of the pi0 is stored
# when a hit is not associated, the value -1 is stored
hit_pi0_index   # vector<int>

# location of the first inelastic hadronic interaction
showerStart_x   # float
showerStart_y   # float
showerStart_z   # float

# type, momentum, and kinematic energy of the primary at the origin
# note: properties might be altered between origin and shower start
primary_px      # float
primary_py      # float
primary_pz      # float
primary_pdgId   # int
primary_kinE    # float

# each pi0 produced in hadronic interactions in showers are stored in the tree
# the stored properties are position, momentum and kinetic energy at the point of interaction
pi0_x           # vector<float>
pi0_y           # vector<float>
pi0_z           # vector<float>
pi0_px          # vector<float>
pi0_py          # vector<float>
pi0_pz          # vector<float>
pi0_kinE        # vector<float>
```

## Tuning data location

A first ROOT tree with tuning data can be found here:

Properties of primary particle:
   * type: pi+
   * kinetic energy: 10 GeV
   * origin: (0,0,0)
   * theta = 0 (theta: opening angle between postive z-axis and particle direction) 

Geometry: flat, idealised barrel calorimeter

/afs/cern.ch/work/l/lveldere/public/showers_pi_kinE10_eta0.root









