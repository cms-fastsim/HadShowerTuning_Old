#!/usr/bin/env python

# Define a calorimeter geometry in asci format
# Calorimeter layers are square shaped layers
# The sides of the square are parallel to the x-axis and y-axis
# and have a lenght defined by the variable (layerDXY).
# All layers are perpendicular to the z-axis.
# The name, thickness and material of the layer are chosen by the user (see variable 'layers').
# Running this script results in 2 versions of the geometry
# In the first version all layers have nominal thickness (geometry_standard.txt).
# In the 2nd and 3rd version the thickness of the layers is multiplied with respectively 2 and 3.
# (geometry_strech2.txt, geometry_strecht3.txt)
# units are in cm

import copy
import ROOT as rt

# the world is a cube, sides have lenght worldDR
worlDR = 2000
# calorimeter layers are squared shaped, sides have length layerDXY
layerDXY = 400

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
// a zero rotation
:ROTM R0 0. 0. 0.

// world volume
:VOLU world BOX {halfR}*cm {halfR}.*cm {halfR}.*cm G4_AIR
""".format(halfR = worlDR/2)

# super simple container for layer information
class Layer:
    def __init__(self,name,dz,material):
        self.name = name          # layer name
        self.dz = dz              # layer thickness as observed by primary
        self.material = material  # material name

# scintilator material name
sci_MatName = "G4_PLASTIC_SC_VINYLTOLUENE"

################
# define the standard layers
################
layers = []
# in from of eb (dummy layer, to get eb at the right position)
layers.append(Layer("front",129.0,"G4_AIR"))
# eb layer
layers.append(Layer("eb",23.0,"lead-tungstate"))
# the gap
layers.append(Layer("gap",177.7 - 129 - 23,"G4_AIR"))
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

    layers.append(Layer("hb_sci_{0}".format(layerIndex),sci_dz,sci_MatName))
    if(layerIndex < 16):
        layers.append(Layer("hb_abs_{0}".format(layerIndex),abs_dz,abs_MatName))

################
# stretched layers
################
layers_stretch2 = copy.deepcopy(layers)
for l in range(len(layers_stretch2)):
    layers_stretch2[l].dz *= 2

layers_stretch3 = copy.deepcopy(layers)
for l in range(len(layers_stretch3)):
    layers_stretch3[l].dz *= 3

################
# write standard and stretched geometries
################
for key,value in {"standard":layers,"stretch2":layers_stretch2,"stretch3":layers_stretch3}.iteritems():
    
    filename = "geometry_" + key + ".txt"
    oFILE = open(filename,"w")
    oFILE.write(extraMaterials)
    oFILE.write(extraDefinitions)

    total_dz = 0
    for layer in value:
        total_dz += layer.dz
        oFILE.write("""
:VOLU {name} BOX {halfdx}*cm {halfdy}*cm {halfdz}*cm {material} 
:PLACE {name} 1 world R0 0.*cm 0.*cm {z}*cm
""".format(name=layer.name,
           halfdx = layerDXY/2,
           halfdy = layerDXY/2,
           halfdz = layer.dz/2,
           material = layer.material,
           z = total_dz - layer.dz/2))
    print total_dz
