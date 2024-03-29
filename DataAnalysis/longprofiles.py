#!/usr/bin/env python

import ROOT as rt
import sys

MAXSHOWERS = 1

ifile = rt.TFile.Open(sys.argv[1])
showers = ifile.Get("showers")

ofile = rt.TFile.Open(sys.argv[2],"RECREATE")
ofile.cd()
h_showerStart_z = rt.TH1D("showerStart_z","showerStart_z",100,100,300)

for e in range(0,min(showers.GetEntries(),MAXSHOWERS)):
    print "shower",e
    showers.GetEntry(e)
    # draw shower start
    h_showerStart_z.Fill(showers.particle_z[1])

    # draw shower profile
    ofile.cd()
    ofile.mkdir("shower_{0}".format(e)).cd()
    h_longProfile = rt.TH1D("longProfile","lonfProfile",100,100,300)
    h_longProfile_outsideShower = rt.TH1D("longProfile_outsideShower","lonfProfile_outsideShower",100,100,300)
    h_longProfile_inShower = rt.TH1D("longProfile_inShower","lonfProfile_inShower",100,100,300)
    h_longProfile_earlyPi0 = rt.TH1D("longProfile_earlyPi0","lonfProfile_earlyPi0",100,100,300)
    h_longProfile_latePi0 = rt.TH1D("longProfile_latePi0","lonfProfile_latePi0",100,100,300)
    h_longProfile_hadronic = rt.TH1D("longProfile_hadronic","lonfProfile_hadronic",100,100,300)
    
    for hitIndex in range(0,showers.hit_x.size()):
        h_longProfile.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
        if showers.hit_particleIndex[hitIndex] == 0:    # hit is related to the primary
            h_longProfile_outsideShower.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
        else:                                            # hit is part of shower
            # the actual shower profile
            h_longProfile_inShower.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
            
            # early / late pi0?
            # note: having several early pions is not unusual
            fromPi0 = False
            fromEarlyPi0 = False
            particleIndex = showers.hit_particleIndex[hitIndex]
            while particleIndex > 0:
                if showers.particle_pdgId[particleIndex] == 111:
                    fromPi0 = True
                    if showers.particle_parentIndex[particleIndex] == 0:
                        fromEarlyPi0 = True
                    break
                else:
                    particleIndex = showers.particle_parentIndex[particleIndex]
            # early pi0 profile
            if fromEarlyPi0:
                h_longProfile_earlyPi0.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
            # late pi0
            elif fromPi0:
                h_longProfile_latePi0.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
            # hadronic
            else:
                h_longProfile_hadronic.Fill(showers.hit_z[hitIndex],showers.hit_e[hitIndex])
                
    rt.gDirectory.Write()

ofile.Write()
ofile.Close()
