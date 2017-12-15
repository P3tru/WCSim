#include <stdio.h>     
#include <stdlib.h>    
#include "WCSimRootEvent.hh"
#include "WCSimRootGeom.hh"
#include "WCSimRootLinkDef.hh"

void myread_OD(char *filename=NULL) {
  /* A simple script to plot aspects of phototube hits 
   * 
   * I like to run this macro as 
   * $ root -l -x 'read_OD.C("../OD.root")'
   */

  gROOT->Reset();
  char* wcsimdirenv;
  wcsimdirenv = getenv ("WCSIMDIR");
  if(wcsimdirenv !=  NULL){
    //gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
    gSystem->Load("../libWCSimRoot.so");
    gSystem->Load("../libWCSimRootDict.rootmap");
    gSystem->Load("../libWCSimRootDict_rdict.pcm");
  }else{
    std::cout << "Can't load WCSim ROOT dictionaries" << std::endl;
  }
  gStyle->SetOptStat(1);

  TFile *f;
  /*char fTest[128];
  if (filename==NULL){
    std::cout << "Please provide filename in option" << std::endl;
    std::cout << "Will load auto wcsim.root in WCSIMDIR ..." << std::endl;
    //char *name = "../wcsim.root";
    char *name = "../wcsim_10MeV.root";
    strncpy(fTest, wcsimdirenv, sizeof(fTest));
    strncat(fTest, "/", (sizeof(fTest) - strlen(fTest)) );
    strncat(fTest, name, (sizeof(fTest) - strlen(fTest)) );
    f = new TFile(fTest);
  }else{*/
    f = new TFile(filename);
 // }
  if (!f->IsOpen()){
    cout << "Error, could not open input file: " << filename << endl;
    return; //-1;
  }else{
   // if (filename==NULL) cout << "File open bro: " << fTest << endl;
   /// else 
    cout << "File open bro: " << filename << endl;
  }

  //TTree  *wcsimT = f->Get("wcsimT");
  TTree *wcsimT = (TTree*)f->Get("wcsimT");

  WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
  wcsimT->SetBranchAddress("wcsimrootevent_OD",&wcsimrootsuperevent);

  // Force deletion to prevent memory leak when issuing multiple
  // calls to GetEvent()
  wcsimT->GetBranch("wcsimrootevent_OD")->SetAutoDelete(kTRUE);

   const long unsigned int nbEntries = wcsimT->GetEntries();
  //const long unsigned int nbEntries = 600;
  cout << "Nb of entries: " << wcsimT->GetEntries() << endl;

  //--- output file
  ofstream out;
  out.open("output.txt");
  //////////////////////////////////////////
  // HISTOGRAMS DEFINITION /////////////////
  //////////////////////////////////////////
  const int nbBins = 500;
  const int nbPEMax = nbBins;
  
  TH1D *hPEByEvtsByPMT = new TH1D("hPEByEvtsByPMT","RAW PE by Evts by PMT",nbBins,0,nbPEMax);
  hPEByEvtsByPMT->GetXaxis()->SetTitle("raw PE");
  hPEByEvtsByPMT->SetLineColor(kBlue-4);
  hPEByEvtsByPMT->SetMarkerColor(kBlue-4);  
  TH1D *hPECollectedByEvtsByPMT = new TH1D("hPECollectedByEvtsByPMT","collected PE by Evts by PMT",nbBins,0,nbPEMax);
  hPECollectedByEvtsByPMT->GetXaxis()->SetTitle("digi PE");
  hPECollectedByEvtsByPMT->SetLineColor(kRed-4);
  hPECollectedByEvtsByPMT->SetMarkerColor(kRed-4);
  
  TH1D *hPEByEvts = new TH1D("hPEByEvts","Total RAW PE by Evts",nbBins,0,nbPEMax);
  hPEByEvts->GetXaxis()->SetTitle("raw PE");
  hPEByEvts->SetLineColor(kBlue-4);
  hPEByEvts->SetMarkerColor(kBlue-4);
  hPEByEvts->SetFillColor(kBlue-4);  
  TH1D *hPECollectedByEvts = new TH1D("hPECollectedByEvts","Total collected PE (digitized) by Evts",nbBins,0,nbPEMax);
  hPECollectedByEvts->GetXaxis()->SetTitle("digi PE");
  hPECollectedByEvts->SetLineColor(kRed-4);
  //hPECollectedByEvts->SetMarkerColor(kRed-4);
  //hPECollectedByEvts->SetFillColor(kRed-4);

  TH1D *hNbTubesHit = new TH1D("hNbTubesHit","Nb of Cherenkov (Tirggered) Hits",1000,0,1000);
  TH1D *DigiMAX= new TH1D("DigiMAX", "Nb of Digitised Hits",1000,0,1000);
  TH1D *plot_PMTIDs = new TH1D( "plot_PMTIDs","PMT IDs with hits",5000,0,5000);
  TH1D *hNbTubesHit2 = new TH1D("hNbTubesHit2","Nb of Tubes with Digitized Hits",1000,0,1000);
  TH1D *HitTimesDiff = new TH1D("HitTimesDiff","Diffeernce between Hit Times",100,0,500);
  // END HISTOGRAMS DEFINITION //////////////
  //////////////////////////////////////////
  int totalrawpesPerRun=0; int totaldigitpesPerRun=0; int total_pmts_with_hits=0;
  for(long unsigned int iEntry = 0; iEntry < nbEntries; iEntry++){
   // Point to event iEntry inside WCSimTree
    wcsimT->GetEvent(iEntry); 
     
    // Nb of Trigger inside the event
    const unsigned int nbTriggers = wcsimrootsuperevent->GetNumberOfEvents();
    const unsigned int nbSubTriggers = wcsimrootsuperevent->GetNumberOfSubEvents();

     //cout << "---------- iEntry : " << iEntry << endl;
     //cout << "nbTrig : " << nbTriggers << endl;
     //cout << "nbSubTrig : " << nbSubTriggers << endl;
    
    for(long unsigned int iTrig = 0; iTrig < nbTriggers; iTrig++){
      WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(iTrig);
      
      // RAW HITS
      int rawMax = wcsimrootevent->GetNcherenkovhits();
      int totRawPE = 0;
      for (int i = 0; i < rawMax; i++){
	WCSimRootCherenkovHit *chit = (WCSimRootCherenkovHit*)wcsimrootevent->GetCherenkovHits()->At(i);
	
	hPEByEvtsByPMT->Fill(chit->GetTotalPe(1));
	totRawPE+=chit->GetTotalPe(1);
      } // END FOR RAW HITS
      
      hNbTubesHit->Fill(rawMax);
      hPEByEvts->Fill(totRawPE);
      totalrawpesPerRun+=totRawPE;
      // DIGI HITS
      int digiMax = wcsimrootevent->GetNcherenkovdigihits();
      int totDigiPE = 0; int pmts_with_hits=0; int start=0;
      int pmts_hit[1000] = {0}; double prevT=0.;
      for (int i = 0; i < digiMax; i++){
	WCSimRootCherenkovDigiHit *cDigiHit = (WCSimRootCherenkovDigiHit*)wcsimrootevent->GetCherenkovDigiHits()->At(i);
	//WCSimRootChernkovDigiHit has methods GetTubeId(), GetT(), GetQ()
	WCSimRootCherenkovHitTime *cHitTime = (WCSimRootCherenkovHitTime*)wcsimrootevent->GetCherenkovHitTimes()->At(i);
	//WCSimRootCherenkovHitTime has methods GetTubeId(), GetTruetime()
        
	hPECollectedByEvtsByPMT->Fill(cDigiHit->GetQ());
	totDigiPE+=cDigiHit->GetQ();
        bool skip=0;
        
        if(digiMax>1){
         if(start>0){
          HitTimesDiff->Fill( abs(cDigiHit->GetT()-prevT) );
          } prevT=cDigiHit->GetT(); start++;
        }
        for(int u=0; u<digiMax; u++){ //cout<<"pmts_hit[u]: "<<pmts_hit[u]<<endl; 
           if( (cDigiHit->GetTubeId()) == pmts_hit[u] ){ 
              //cout<<(cDigiHit->GetTubeId())<<", "<<pmts_hit[u]<<" this PMT ID already exists... Skip this ID in "<<iEntry<<endl;
              skip=1;
           }
        }  
        if(skip==0){
           pmts_hit[i]=(cDigiHit->GetTubeId());
           pmts_with_hits++;
           total_pmts_with_hits++;
           out<<cDigiHit->GetTubeId()<<","<<cDigiHit->GetQ()<<endl;
        }
 
        plot_PMTIDs->Fill(cDigiHit->GetTubeId());
      } // END FOR DIGI HITS
      hPECollectedByEvts->Fill(totDigiPE);
      hNbTubesHit2->Fill(pmts_with_hits);
      DigiMAX->Fill(digiMax);
      totaldigitpesPerRun+=totDigiPE;
    } // END FOR iTRIG
    
  } // END FOR iENTRY
  cout<<"total_pmts_with_hits: "<<total_pmts_with_hits<<endl;
  out.close();
  //////////////////////////////////////////
  // DRAWING ///////////////////////////////
  //////////////////////////////////////////
  cout<<"plots!!!"<<endl;
  TCanvas *c1;

  c1 = new TCanvas("cRaw","cRaw",600,400);
  //hPEByEvtsByPMT->Draw("HIST");
  hPEByEvts->Draw("");

  c1 = new TCanvas("cDigi","cDigi",600,400);
  //hPECollectedByEvtsByPMT->Draw("HIST");
  hPECollectedByEvts->Draw();
  c1 = new TCanvas("HitTimesDiff","HitTimesDiff",600,400);
   HitTimesDiff->Draw();

  c1 = new TCanvas("cNbTubesHit","cNbTubesHit/WCHCPMT->entries()",600,400);
  hNbTubesHit->Draw("HIST"); 
  c1 = new TCanvas("DigiMAX","Digis from WCHCPMT->entries()",600,400);
  DigiMAX->Draw("HIST");
  c1 = new TCanvas("cNbTubesHit2","cNbTubesHit2",600,400);
  hNbTubesHit2->Draw();
  //c1 = new TCanvas("plot_PMTIDs","plot_PMTIDs",600,400);
  //plot_PMTIDs->Draw();
  cout<<"totalrawpesPerRun: "<<totalrawpesPerRun<<" totaldigitpesPerRun: "<<totaldigitpesPerRun<<endl;
  cout << "DIGI: Mean nb of PMTs with hits by events : " << hNbTubesHit2->GetMean()
       << " +- " << hNbTubesHit2->GetRMS() << endl;
  cout<<"Mean nb of digis by events: "<<DigiMAX->GetMean()<<" +- " <<DigiMAX->GetRMS() << endl;
  cout << "Mean PE collected by events (digi) : " << hPECollectedByEvts->GetMean()
       << " +- " << hPECollectedByEvts->GetRMS() << endl;
  cout << "TRIG: Mean nb of Cherenkov (Tirggered) hits by events : " << hNbTubesHit->GetMean()
       << " +- " << hNbTubesHit->GetRMS() << endl;
  cout << "Mean raw PE by events : " << hPEByEvts->GetMean()
       << " +- " << hPEByEvts->GetRMS() << endl;

} // END MACRO
