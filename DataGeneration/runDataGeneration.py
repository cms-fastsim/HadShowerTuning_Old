#!/usr/bin/env python

import argparse,os
parser = argparse.ArgumentParser()
parser.add_argument("--nevents",type=int,help="number of events (default: %(default)i)",default=10)
parser.add_argument("--particle",help="particle name a la geant4 (default: %(default)s)",default="pi+")
parser.add_argument("--energy",type=float,help="kinetic energy of primary [GeV] (default: %(default)f)",default=10)
parser.add_argument("--direction",help="coordinates of direction of primary (default: %(default)s)",metavar="X Y Z",default="0 0 1")
parser.add_argument("--position",help="original position of the primary [cm] (default: %(default)s)",metavar="X Y Z",default="0 0 0")
parser.add_argument("--ofile",help="where to write output? (default: %(default)s)",default="test.root")
parser.add_argument("--geometry",help="path to geometry ASCI file (default: %(default)s)",default="geometry_standard.txt")
parser.add_argument("--seed",type=int,help="random seed for geant4 (default: %(default)i)",default=1)
parser.add_argument("--swallowlogs",type=bool,help="write logs to OUTPUTFILE.log (default: %(default)r)",default=False)
args = parser.parse_args()

macroTemplate = """
/gun/particle {particle}
/gun/energy {energy} GeV
/gun/direction {direction}
/gun/position {position} cm
/run/beamOn {nevents}
""".format(nevents = args.nevents,
           particle = args.particle,
           energy = args.energy,
           direction = args.direction.replace(","," "),
           position = args.position.replace(","," "))

macrofile = args.ofile + ".mac"
MACROFILE = open(macrofile,"w")
MACROFILE.write(macroTemplate)
MACROFILE.close()

print "=== BEGIN MACROFILE ==="
print macroTemplate
print "=== END   MACROFILE ==="

logfile = args.ofile + ".log"

command = "/nfs/dust/cms/user/lveldere/FastSimDev_Oct/g4fresh/HadShowerTuning/DataGeneration-build/DataGeneration oFile={oFile} g4Macro={g4Macro} seed={seed} geometry={geometry}".format(
    oFile=args.ofile,seed=args.seed,geometry=args.geometry,g4Macro=macrofile)
bigCommand = "(time " + command + ")"
if args.swallowlogs:
    bigCommand += "2>&1 >& " + logfile 
    print "writing logs and timing information to '" + logfile + "'"
import os
print "execute command '" + bigCommand + "'"
os.system(bigCommand)
