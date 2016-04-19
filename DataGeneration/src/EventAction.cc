#include "EventAction.h"
#include "G4Event.hh"
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "TH1F.h"
#include "Data.h"
#include "iostream"

using namespace hadshowertuning;

EventAction::EventAction(TFile* file,Data * data,bool writeTree)
    : G4UserEventAction()
    , fFile(file)
    , fData(data)
    , fTree(0)
{
    // create TTree
    if(writeTree)
    {
	fFile->cd();
	fTree = new TTree("showers","showers");;
	
	fTree->Branch("hit_e",&data->hit_e);
	fTree->Branch("hit_x",&data->hit_x);
	fTree->Branch("hit_y",&data->hit_y);
	fTree->Branch("hit_z",&data->hit_z);
	fTree->Branch("hit_volumeId",&data->hit_volumeId);
	fTree->Branch("hit_particleIndex",&data->hit_particleIndex);
	
	fTree->Branch("particle_x",&data->particle_x);
	fTree->Branch("particle_y",&data->particle_y);
	fTree->Branch("particle_z",&data->particle_z);
	fTree->Branch("particle_px",&data->particle_px);
	fTree->Branch("particle_py",&data->particle_py);
	fTree->Branch("particle_pz",&data->particle_pz);
	fTree->Branch("particle_kinE",&data->particle_kinE);
	fTree->Branch("particle_pdgId",&data->particle_pdgId);
	fTree->Branch("particle_parentIndex",&data->particle_parentIndex);
	
	fTree->Branch("extraParticle_x",&data->extraParticle_x);
	fTree->Branch("extraParticle_y",&data->extraParticle_y);
	fTree->Branch("extraParticle_z",&data->extraParticle_z);
	fTree->Branch("extraParticle_px",&data->extraParticle_px);
	fTree->Branch("extraParticle_py",&data->extraParticle_py);
	fTree->Branch("extraParticle_pz",&data->extraParticle_pz);
	fTree->Branch("extraParticle_kinE",&data->extraParticle_kinE);
	fTree->Branch("extraParticle_pdgId",&data->extraParticle_pdgId);
	fTree->Branch("extraParticle_name",&data->extraParticle_name);
    }
}

EventAction::~EventAction()
{
    // write TTree
    if(fTree)
    {
	fFile->cd();
	fTree->Write();
    }
}

void EventAction::BeginOfEventAction(const G4Event *){
    // reset data at begin of event
    fData->clear();
    index_pi0_1 = -1;
    index_pi0_2 = -1;
}

void EventAction::EndOfEventAction(const G4Event * event){
    // write event to tree
    if(fTree)
    {
	fTree->Fill();
    }

    // create new directory for shower
    std::stringstream dirName;
    dirName << "shower_" << event->GetEventID();
    fFile->cd();
    fFile->mkdir(dirName.str().c_str())->cd();
    // find pi0s
    findPi0();
    // write extra particles
    writeExtraParticles();
    // create and write histograms with shower profiles
    writeHistograms();
}


void EventAction::findPi0()
{
    // find index of hardest and second hardest pi0
    for(unsigned particleIndex = 1;particleIndex < fData->particle_x.size();particleIndex++)
    {
	if(fData->particle_pdgId[particleIndex] == 111)
	{
	    if(index_pi0_1 < 0)
	    {
		index_pi0_1 = particleIndex;
	    }
	    else if(fData->particle_kinE[particleIndex] > fData->particle_kinE[index_pi0_1])
	    {
		index_pi0_2 = index_pi0_1;
		index_pi0_1 = particleIndex;
	    }
	    else if(index_pi0_2 < 0 || fData->particle_kinE[particleIndex] > fData->particle_kinE[index_pi0_2])
	    {
		index_pi0_2 = particleIndex;
	    }
	}
    }
    // add hardest pi0 to extra particle collection
    if(index_pi0_1 > 0)
    {
	fData->extraParticle_x.push_back(fData->particle_x[index_pi0_1]);
	fData->extraParticle_y.push_back(fData->particle_y[index_pi0_1]);
	fData->extraParticle_z.push_back(fData->particle_z[index_pi0_1]);
	fData->extraParticle_px.push_back(fData->particle_px[index_pi0_1]);
	fData->extraParticle_py.push_back(fData->particle_py[index_pi0_1]);
	fData->extraParticle_pz.push_back(fData->particle_pz[index_pi0_1]);
	fData->extraParticle_kinE.push_back(fData->particle_kinE[index_pi0_1]);
	fData->extraParticle_pdgId.push_back(fData->particle_pdgId[index_pi0_1]);
	fData->extraParticle_name.push_back("pi0_1");
    }
    // add 2nd hardest pi0 to extra particle collection
    if(index_pi0_2 > 0)
    {
	fData->extraParticle_x.push_back(fData->particle_x[index_pi0_2]);
	fData->extraParticle_y.push_back(fData->particle_y[index_pi0_2]);
	fData->extraParticle_z.push_back(fData->particle_z[index_pi0_2]);
	fData->extraParticle_px.push_back(fData->particle_px[index_pi0_2]);
	fData->extraParticle_py.push_back(fData->particle_py[index_pi0_2]);
	fData->extraParticle_pz.push_back(fData->particle_pz[index_pi0_2]);
	fData->extraParticle_kinE.push_back(fData->particle_kinE[index_pi0_2]);
	fData->extraParticle_pdgId.push_back(fData->particle_pdgId[index_pi0_2]);
	fData->extraParticle_name.push_back("pi0_2");
    }
}

void EventAction::writeExtraParticles()
{
    for(unsigned p = 0;p<fData->extraParticle_x.size();p++)
    {
	// create root objects
	TVector3 * position = new TVector3(fData->extraParticle_x[p],fData->extraParticle_y[p],fData->extraParticle_z[p]);
	TVector3 * momentum = new TVector3(fData->extraParticle_px[p],fData->extraParticle_py[p],fData->extraParticle_pz[p]);
	TVectorD * kinE = new TVectorD(1);
	TVectorD * pdgId = new TVectorD(1);
	kinE[0] = fData->extraParticle_kinE[p];
	pdgId[0] = fData->extraParticle_pdgId[p];
     
	// write objects
	position->Write((fData->extraParticle_name[p] + "_position").c_str());
	momentum->Write((fData->extraParticle_name[p] + "_momentum").c_str());
	kinE->Write((fData->extraParticle_name[p] + "_kinE").c_str());
	pdgId->Write((fData->extraParticle_name[p] + "_pdgId").c_str());
    }
}

void EventAction::writeHistograms()
{    
    double min = 0;
    double max = 1000;
    int nbins = 1000;
    
    // find binning: bin width = (max - min)/nbins
    // mip = primary before shower start
    TH1F * h_longEProf_fineBin_mip = new TH1F("longEProf_fineBin_mip","longEProf_fineBin_mip",nbins,min,max);
    // had = energy deposits in shower, not associated to hardest of second hardets pi0 in shower
    TH1F * h_longEProf_fineBin_had = new TH1F("longEProf_fineBin_had","longEProf_fineBin_had",nbins,min,max);
    // energy deposits in shower associated to hardest and second hardest pi0 in shower
    TH1F * h_longEProf_fineBin_pi0_1 = new TH1F("longEProf_fineBin_pi0_1","longEProf_fineBin_pi0_1",nbins,min,max);
    TH1F * h_longEProf_fineBin_pi0_2 = new TH1F("longEProf_fineBin_pi0_2","longEProf_fineBin_pi0_2",nbins,min,max);

    // one bin per detector layer
    TH1F * h_longEProf_perLayer_mip = new TH1F("longEProf_perLayer_mip","longEProf_perLayer_mip",100,0,100);
    TH1F * h_longEProf_perLayer_had = new TH1F("longEProf_perLayer_had","longEProf_perLayer_had",100,0,100);
    TH1F * h_longEProf_perLayer_pi0_1 = new TH1F("longEProf_perLayer_pi0_1","longEProf_perLayer_pi0_1",100,0,100);
    TH1F * h_longEProf_perlayer_pi0_2 = new TH1F("longEProf_perLayer_pi0_2","longEProf_perLayer_pi0_2",100,0,100);
    
    for(unsigned hitIndex = 0;hitIndex < fData->hit_x.size();hitIndex++)
    {
	// mip profiles
	if(fData->hit_particleIndex[hitIndex] == 0)
	{
	    h_longEProf_fineBin_mip->Fill(fData->hit_z[hitIndex],fData->hit_e[hitIndex]);
	    h_longEProf_perLayer_mip->Fill(fData->hit_volumeId[hitIndex],fData->hit_e[hitIndex]);
	}
	else {
	    // decide whether to associate hit to pi0s or not
	    int ancestorIndex = fData->hit_particleIndex[hitIndex];
	    while(ancestorIndex > 0)
	    {
		if(ancestorIndex == index_pi0_1
		   || ancestorIndex == index_pi0_2)
		{
		    break;
		}
		else 
		{
		    ancestorIndex = fData->particle_parentIndex[ancestorIndex];
		}
	    }
	    if(ancestorIndex == index_pi0_1)
	    {
		h_longEProf_fineBin_pi0_1->Fill(fData->hit_z[hitIndex],fData->hit_e[hitIndex]);
		h_longEProf_perLayer_pi0_1->Fill(fData->hit_volumeId[hitIndex],fData->hit_e[hitIndex]);
	    }
	    else if(ancestorIndex == index_pi0_2)
	    {
		h_longEProf_fineBin_pi0_2->Fill(fData->hit_z[hitIndex],fData->hit_e[hitIndex]);
		h_longEProf_perlayer_pi0_2->Fill(fData->hit_volumeId[hitIndex],fData->hit_e[hitIndex]);
	    }
	    else
	    {
		h_longEProf_fineBin_had->Fill(fData->hit_z[hitIndex],fData->hit_e[hitIndex]);
		h_longEProf_perLayer_had->Fill(fData->hit_volumeId[hitIndex],fData->hit_e[hitIndex]);
	    }
	}
	
    }
    
    // write histograms
    h_longEProf_fineBin_mip->Write();
    h_longEProf_fineBin_had->Write();
    h_longEProf_fineBin_pi0_1->Write();
    h_longEProf_fineBin_pi0_2->Write();
    h_longEProf_perLayer_mip->Write();
    h_longEProf_perLayer_had->Write();
    h_longEProf_perLayer_pi0_1->Write();
    h_longEProf_perlayer_pi0_2->Write();
}
