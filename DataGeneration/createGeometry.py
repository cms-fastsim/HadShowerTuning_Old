#!/usr/bin/env python

filename = "geometry.txt"
worldR = 800

# define material not availabel in the geant4 NIST database
extraMaterials = """
:ELEM W W 74 183.84
:ELEM Pb Pb 82 207.2
:ELEM O O 8 15.999
:MIXT_BY_NATOMS lead-tungstate 8.3*g/cm3 3
   W 1
   Pb 1
   O 4
"""

# extra definitions
extraDefinitions = """
// a zero ration
:ROTM R0 0. 0. 0.

// world volume
:VOLU world BOX {halfR}*cm {halfR}.*cm {halfR}.*cm G4_AIR
""".format(halfR = worldR/2)

# a container to store information of 
class Layer:
    # arguments:
    # - name: layer name
    # - inner_z: inner_z coordinate 
    # - dz: thickness
    # - material: material name
    def __init__(self,name,inner_z,dz,material):
        self.name = name
        self.inner_z = inner_z
        self.dz = dz
        self.material = material
        self.outer_z = inner_z + dz

# scintilator material name
sci_MatName = "G4_PLASTIC_SC_VINYLTOLUENE"

# define the layers
layers = []
# eb layer
layers.append(Layer("eb",129.0,23.0,"lead-tungstate"))
# the gap
layers.append(Layer("gap",layers[-1].outer_z,177.7 - layers[-1].outer_z,"G4_AIR"))
# hb layer 0 
for layerIndex in range(0,17):

    # absorber material
    abs_MatName = "G4_BRASS"
    if layerIndex == 0 or layerIndex == 15:
        abs_MatName = "G4_Fe"

    # absorber thickness
    abs_dz = 5.05
    if layerIndex == 0:
        abs_dz = 4.0
    elif layerIndex == 15:
        abs_dz = 7.5
    elif layerIndex > 8:
        abs_dz = 5.65

    # scintilator thickness
    sci_dz = 0.37
    if layerIndex == 0 or layerIndex == 16:
        sci_dz = 0.9

    layers.append(Layer("hb_sci_{0}".format(layerIndex),layers[-1].outer_z,sci_dz,sci_MatName))
    if(layerIndex < 16):
        layers.append(Layer("hb_abs_{0}".format(layerIndex),layers[-1].outer_z,abs_dz,abs_MatName))

oFILE = open(filename,"w")

oFILE.write(extraMaterials)
oFILE.write(extraDefinitions)
for layer in layers:
    oFILE.write("""
:VOLU {name} BOX {halfdx}*cm {halfdy}*cm {halfdz}*cm {material} 
:PLACE {name} 1 world R0 0.*cm 0.*cm {z}*cm
""".format(name=layer.name,
           halfdx = worldR/2*0.9,
           halfdy = worldR/2*0.9,
           halfdz = layer.dz/2,
           material = layer.material,
           z = (layer.inner_z + layer.outer_z) / 2))

