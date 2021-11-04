#include <TCutG.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <TLegend.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TGraph.h>
#include <TMultiGraph.h>
#include "progressbar.h"
#include <vector>
#include <numeric>
#include <TTreeIndex.h>

double coincidenceWindow =1.5*1e3;

struct hit
{
	double E;
	double ch;
	double bd;
	double TS;
};

struct event
{
	std::vector<hit> Alist;
	std::vector<hit> Blist;
	std::vector<hit> Clist;
	std::vector<hit> Dlist;
};

class BuiltEvent {
public :
   hit Ahit;
   hit Bhit;
   hit Chit;
   hit Dhit;

   void Reset();
};


void BuiltEvent::Reset()
{
	Ahit.E=-1;
	Ahit.ch=-1;
	Ahit.TS=-1;
	Ahit.bd=-1;

	Bhit.E=-1;
	Bhit.ch=-1;
	Bhit.TS=-1;
	Bhit.bd=-1;

	Chit.E=-1;
	Chit.ch=-1;
	Chit.TS=-1;
	Chit.bd=-1;

	Dhit.E=-1;
	Dhit.ch=-1;
	Dhit.TS=-1;
	Dhit.bd=-1;
}

/*
Dictionary used:

Scint top  : ch0 
Scint bottom : ch2
Scint Long : ch3
SiPM : ch4
*/

void fillEvent(event* evt, hit h)
{
    if(h.ch==0 && h.bd ==0) evt->Alist.push_back(h);
    if(h.ch==2 && h.bd ==0) evt->Blist.push_back(h);
    if(h.ch==3 && h.bd ==0) evt->Clist.push_back(h);
    if(h.ch==4 && h.bd ==0) evt->Dlist.push_back(h);
}

/*Sort the Sabre Data in order of descending energy*/
bool DescSort(hit i, hit j) {
  return (i.E>j.E);
}


void simpleEventBuilder(int runID)
{



std::cout << "\nAnalysing file run_" << runID << " ...\n";
//gStyle->SetOptTitle(0);

TFile *_file1 = TFile::Open(Form("./DAQ/run_%d/UNFILTERED/compass_run_%d.root",runID,runID));
TTree *Tdata = (TTree*)_file1->Get("Data");    // compassx.root has only one thing in it, a TTree called 'Data' 

//Container variables for spanning the tree nodes - refer readBin.C
UShort_t Energy;
ULong64_t Timestamp;
UShort_t Channel, Board; 
UInt_t Flags;

Tdata->SetName("Data_Tree"); //Set name for the tree
Tdata->SetBranchAddress("Energy",&Energy);
Tdata->SetBranchAddress("Timestamp",&Timestamp);
Tdata->SetBranchAddress("Channel",&Channel);
Tdata->SetBranchAddress("Board",&Board);
Tdata->SetBranchAddress("Flags",&Flags);

double startTime=0, previousEventTime = 0;;

BuiltEvent BE;

TFile *builtFile = new TFile(Form("./run_%d_builtFile.root",runID),"RECREATE");
TTree *builtTree = new TTree("builtTree","Offline Built Tree From DPP data");

builtTree->Branch("A_TS",&(BE.Ahit.TS),"A_TS/D");
builtTree->Branch("A_E",&(BE.Ahit.E),"A_E/D");
builtTree->Branch("A_ch",&(BE.Ahit.ch),"A_ch/D");

builtTree->Branch("B_TS",&(BE.Bhit.TS),"B_TS/D");
builtTree->Branch("B_E",&(BE.Bhit.E),"B_E/D");
builtTree->Branch("B_ch",&(BE.Bhit.ch),"B_ch/D");

builtTree->Branch("C_TS",&(BE.Chit.TS),"C_TS/D");
builtTree->Branch("C_E",&(BE.Chit.E),"C_E/D");
builtTree->Branch("C_ch",&(BE.Chit.ch),"C_ch/D");

builtTree->Branch("D_TS",&(BE.Dhit.TS),"D_TS/D");
builtTree->Branch("D_E",&(BE.Dhit.E),"D_E/D");
builtTree->Branch("D_ch",&(BE.Dhit.ch),"D_ch/D");


int nentries = Tdata->GetEntries();
Tdata->BuildIndex("Timestamp");
TTreeIndex* att_index = (TTreeIndex*)Tdata->GetTreeIndex();

double TSref, dTS=0.0;

std::cout << setprecision(40);
ULong64_t outOfOrderCounter = 0;
std::vector <event*> eventList; 
std::vector <event*>::iterator eventList_it;

std::vector <double> timestamps;
std::vector <double> x;

for (int k=0;k<nentries;k++)
{
	//Tdata->GetEntry(k);
	Tdata->GetEntry(att_index->GetIndex()[k]);

	hit compasshit;
	double flags;
	compasshit.E = static_cast<double>(Energy);
	compasshit.TS = static_cast<double>(Timestamp)/1.0e3;
	compasshit.bd = static_cast<double>(Board);
	compasshit.ch = static_cast<double>(Channel);
	flags = static_cast<double>(Flags);

	timestamps.push_back(compasshit.TS);
	x.push_back(k);
	
//	std::cout << "\n" << compasshit.TS << " " << Timestamp;

	if(eventList.empty()&&flags != 0x80)
	{
		startTime = compasshit.TS;
		event* evptr = new event;
		fillEvent(evptr, compasshit);
		eventList.push_back(evptr);
	}
	else
	{
	// Diagnostic to make sure the tree is sorted by Timestamp
		if(compasshit.TS < previousEventTime) 
		{
			//cerr << evt.TS << " " << previousEventTime << " " << std::endl;
			//cerr << eventList.size() << " !!!Out of order!!!" << std::endl;
			//cerr << "\n" << std::fixed << previousEventTime << " " << compasshit.TS << " " << (compasshit.TS - previousEventTime) ;
			outOfOrderCounter++;
		}
		// Ignore board offsets for now. Small compared to coincidence window
		if( (compasshit.TS - startTime) < coincidenceWindow && flags != 0x80) 
		{
			event* evptr = new event;
			fillEvent(evptr,compasshit);
			eventList.push_back(evptr);
		}
		else
		{
			BE.Reset();

			//process the event
			for(eventList_it = eventList.begin();
			eventList_it < eventList.end(); eventList_it++)
			{
				sort((*eventList_it)->Alist.begin(),(*eventList_it)->Alist.end(),DescSort);
				sort((*eventList_it)->Blist.begin(),(*eventList_it)->Blist.end(),DescSort);
				sort((*eventList_it)->Clist.begin(),(*eventList_it)->Clist.end(),DescSort);
				sort((*eventList_it)->Dlist.begin(),(*eventList_it)->Dlist.end(),DescSort);


				//Warning printout for multiple events in a single channel within eventbuilding window
				if((*eventList_it)->Alist.size()>1 || (*eventList_it)->Blist.size() >1 || 
								(*eventList_it)->Clist.size() > 1 || (*eventList_it)->Dlist.size() > 1)

				std::cout << "\n"	<< (*eventList_it)->Alist.size() << " " <<  (*eventList_it)->Blist.size() << " " 
							<< (*eventList_it)->Clist.size() << "   " << (*eventList_it)->Dlist.size();;

				if(!(*eventList_it)->Alist.empty())		
					BE.Ahit = (*eventList_it)->Alist[0]; //Store the largest energy hits
				if(!(*eventList_it)->Blist.empty())		
					BE.Bhit = (*eventList_it)->Blist[0]; //Store the largest energy hits

				if(!(*eventList_it)->Clist.empty())		
					BE.Chit = (*eventList_it)->Clist[0];
				if(!(*eventList_it)->Dlist.empty())		
					BE.Dhit = (*eventList_it)->Dlist[0];
				
				builtTree->Fill();

			}
			//end of process event
			//Reset Event List
			for (eventList_it = eventList.begin(); 
					   eventList_it < eventList.end(); eventList_it++) 
			{
				delete *eventList_it;
			}
			k--;
			eventList.clear();
		}//end else part of if(event not in coincidence window)
	}//if(eventList.empty())

previousEventTime = compasshit.TS;
if(k%100==0) progressbar((double)k,(double)nentries);

}

//End of for (reading through the file entries with GetEntry(i))
builtTree->Write(builtTree->GetName(), TObject::kOverwrite);
builtFile->Close();

std::cout <<"\nNentries:" << nentries <<" Out of order:" << outOfOrderCounter << std::endl << std::flush;
}
