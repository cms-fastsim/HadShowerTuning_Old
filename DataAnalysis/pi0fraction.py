#!/usr/bin/env python

import ROOT as rt
import sys

MAXSHOWERS = 100

ifile = rt.TFile.Open(sys.argv[1])
showers = ifile.Get("showers")

ofile = rt.TFile.Open(sys.argv[2],"RECREATE")
ofile.cd()
h_log_earlyPi0Fraction = rt.TH1D("earlyPi0Fraction","earlyPi0Fraction",100,-5,5)
h_log_latePi0Fraction = rt.TH1D("latePi0Fraction","latePi0Fraction",100,-5,5)
h_log_latePi0Fraction_vs_log_earlyPi0Fraction = rt.TH2D("log_latePi0Fraction_vs_log_earlyPi0Fraction","log_latePi0Fraction_vs_log_earlyPi0Fraction",100,-5,5,100,-5,5)
h_nEarlyPi0 = rt.TH1D("nEarlyPi0","nEarlyPi0",10,0,10)
h_nLatePi0 = rt.TH1D("nLatePi0","nLatePi0",10,0,10)
h_nLatePi0_vs_nEarlyPi0 = rt.TH2D("nLatePi0_vs_nEarlyPi0","nLatePi0_vs_nEarlyPi0",10,0,10,10,0,10)

for e in range(0,min(showers.GetEntries(),MAXSHOWERS)):

    print "shower",e
    showers.GetEntry(e)

    nEarlyPi0 = 0
    nLatePi0 = 0
    earlyPi0E = 0.
    latePi0E = 0.
    primaryE = showers.particle_kinE[0]
    for particleIndex in range(0,len(showers.particle_px)):
        if showers.particle_pdgId[particleIndex] == 111:
            parentIndex = showers.particle_parentIndex[particleIndex]
            # early pion
            isEarly = False
            if parentIndex == 0:
                isEarly = True
            elif showers.particle_pdgId[parentIndex] == 221 and showers.particle_parentIndex[parentIndex] == 0:
                isEarly = True

            if isEarly:
                earlyPi0E += showers.particle_kinE[particleIndex]
                nEarlyPi0 += 1
            else:
                latePi0E += showers.particle_kinE[particleIndex]
                nLatePi0 += 1

    if nEarlyPi0 == 0 and nLatePi0 != 0:
        for particleIndex in range(0,len(showers.particle_px)):
            if showers.particle_pdgId[particleIndex] == 111:
                print "  ",showers.particle_z[1],showers.particle_z[particleIndex],showers.particle_kinE[particleIndex]/primaryE

    h_nEarlyPi0.Fill(nEarlyPi0)
    h_nLatePi0.Fill(nLatePi0)
    h_nLatePi0_vs_nEarlyPi0.Fill(nEarlyPi0,nLatePi0)
    if earlyPi0E > 0.:
        h_log_earlyPi0Fraction.Fill(rt.TMath.Log10(earlyPi0E/primaryE))
    if latePi0E > 0.:
        h_log_latePi0Fraction.Fill(rt.TMath.Log10(latePi0E/primaryE))
    h_log_latePi0Fraction_vs_log_earlyPi0Fraction.Fill(rt.TMath.Log10(earlyPi0E/primaryE),rt.TMath.Log10(latePi0E/primaryE))

ofile.Write()
ofile.Close()
ifile.Close()
