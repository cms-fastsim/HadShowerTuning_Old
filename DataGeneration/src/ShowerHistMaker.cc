#include "ShowerHistMaker.h"

#include "Data.h"

#include "iostream"

#include "TDirectory.h"
#include "TH1F.h"
#include "TVector3.h"

using namespace hadshowertuning;

void ShowerHistMaker::makeHist(TDirectory * dir,Data * data)
{
    dir->cd();

    int nbins = 320;
    
    TH1F * h_longEProf_fineBin_mip = new TH1F("longEProf_fineBin_mip","longEProf_fineBin_mip",nbins,120,280);
    TH1F * h_longEProf_fineBin_had = new TH1F("longEProf_fineBin_had","longEProf_fineBin_had",nbins,120,280);
    TH1F * h_longEProf_fineBin_pi0_1 = new TH1F("longEProf_fineBin_pi0_1","longEProf_fineBin_pi0_1",nbins,120,280);
    TH1F * h_longEProf_fineBin_pi0_2 = new TH1F("longEProf_fineBin_pi0_2","longEProf_fineBin_pi0_2",nbins,120,280);

    TH1F * h_longEProf_perLayer_mip = new TH1F("longEProf_perLayer_mip","longEProf_perLayer_mip",100,0,100);
    TH1F * h_longEProf_perLayer_had = new TH1F("longEProf_perLayer_had","longEProf_perLayer_had",100,0,100);
    TH1F * h_longEProf_perLayer_pi0_1 = new TH1F("longEProf_perLayer_pi0_1","longEProf_perLayer_pi0_1",100,0,100);
    TH1F * h_longEProf_perlayer_pi0_2 = new TH1F("longEProf_perLayer_pi0_2","longEProf_perLayer_pi0_2",100,0,100);
    
    TVector3 * showerStart = new TVector3();
    TVector3 * vertex_pi0_1 = new TVector3();
    TVector3 * vertex_pi0_2 = new TVector3();


    if(data->particle_x.size() > 1)
    {
	// store shower start
	showerStart->SetXYZ(data->particle_x[1],data->particle_y[1],data->particle_z[1]);
	
	// find hardest and second hardest pi0
	int index_pi0_1 = -1;
	int index_pi0_2 = -1;
	for(unsigned particleIndex = 1;particleIndex < data->particle_x.size();particleIndex++)
	{
	    if(data->particle_pdgId[particleIndex] == 111)
	    {
		if(index_pi0_1 < 0)
		{
		    index_pi0_1 = particleIndex;
		}
		else if(data->particle_kinE[particleIndex] > data->particle_kinE[index_pi0_1])
		{
		    index_pi0_2 = index_pi0_1;
		    index_pi0_1 = particleIndex;
		}
		else if(index_pi0_2 < 0 || data->particle_kinE[particleIndex] > data->particle_kinE[index_pi0_2])
		{
		    index_pi0_2 = particleIndex;
		}
	    }
	}
	if(index_pi0_1 > 0)
	{
	    vertex_pi0_1->SetXYZ(data->particle_x[index_pi0_1],data->particle_y[index_pi0_1],data->particle_z[index_pi0_1]);
	}
	if(index_pi0_2 > 0)
	{
	    vertex_pi0_2->SetXYZ(data->particle_x[index_pi0_2],data->particle_y[index_pi0_2],data->particle_z[index_pi0_2]);
	}

	// shower profiles
	for(unsigned hitIndex = 0;hitIndex < data->hit_x.size();hitIndex++)
	{
	    if(data->hit_particleIndex[hitIndex] == 0)
	    {
		h_longEProf_fineBin_mip->Fill(data->hit_z[hitIndex],data->hit_e[hitIndex]);
		h_longEProf_perLayer_mip->Fill(data->hit_volumeId[hitIndex],data->hit_e[hitIndex]);
	    }
	    else {
	        int ancestorIndex = data->hit_particleIndex[hitIndex];
		while(ancestorIndex > 0)
		{
		    if(ancestorIndex == index_pi0_1
		       || ancestorIndex == index_pi0_2)
		    {
			break;
		    }
		    else 
		    {
			ancestorIndex = data->particle_parentIndex[ancestorIndex];
		    }
		}
		if(ancestorIndex == index_pi0_1)
		{
		    h_longEProf_fineBin_pi0_1->Fill(data->hit_z[hitIndex],data->hit_e[hitIndex]);
		    h_longEProf_perLayer_pi0_1->Fill(data->hit_volumeId[hitIndex],data->hit_e[hitIndex]);
		}
		else if(ancestorIndex == index_pi0_2)
		{
		    h_longEProf_fineBin_pi0_2->Fill(data->hit_z[hitIndex],data->hit_e[hitIndex]);
		    h_longEProf_perlayer_pi0_2->Fill(data->hit_volumeId[hitIndex],data->hit_e[hitIndex]);
		}
		else
		{
		    h_longEProf_fineBin_had->Fill(data->hit_z[hitIndex],data->hit_e[hitIndex]);
		    h_longEProf_perLayer_had->Fill(data->hit_volumeId[hitIndex],data->hit_e[hitIndex]);
		}
	    }
	    
	}

    }

    h_longEProf_fineBin_mip->Write();
    h_longEProf_fineBin_had->Write();
    h_longEProf_fineBin_pi0_1->Write();
    h_longEProf_fineBin_pi0_2->Write();
    h_longEProf_perLayer_mip->Write();
    h_longEProf_perLayer_had->Write();
    h_longEProf_perLayer_pi0_1->Write();
    h_longEProf_perlayer_pi0_2->Write();
    showerStart->Write("showerStart");
    vertex_pi0_1->Write("vertex_pi0_1");
    vertex_pi0_2->Write("vertex_pi0_2");
    
}


