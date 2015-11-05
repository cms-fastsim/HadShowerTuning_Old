#!/usr/bin/env python

import ROOT as rt
import sys

MAXSHOWERS = 100000000000000

ifile = rt.TFile.Open(sys.argv[1])
showers = ifile.Get("showers")

ofile = rt.TFile.Open(sys.argv[2],"RECREATE")
ofile.cd()
h_log_earlyPi0Fraction = rt.TH1D("earlyPi0Fraction","earlyPi0Fraction",100,-5,5)
h_log_latePi0Fraction = rt.TH1D("latePi0Fraction","latePi0Fraction",100,-5,5)

for e in range(0,min(showers.GetEntries(),MAXSHOWERS)):

    print "shower",e
    showers.GetEntry(e)

    earlyPi0E = 0.
    latePi0E = 0.
    primaryE = showers.particle_kinE[0]
    for particleIndex in range(0,len(showers.particle_px)):
        if showers.particle_pdgId[particleIndex] == 111:
            # early pion
            if showers.particle_parentIndex[particleIndex] == 0:
                earlyPi0E += showers.particle_kinE[particleIndex]
            else:
                latePi0E += showers.particle_kinE[particleIndex]

    if earlyPi0E > 0.:
        h_log_earlyPi0Fraction.Fill(rt.TMath.Log10(earlyPi0E/primaryE))
    if latePi0E > 0.:
        h_log_latePi0Fraction.Fill(rt.TMath.Log10(latePi0E/primaryE))

ofile.Write()
ofile.Close()
ifile.Close()
