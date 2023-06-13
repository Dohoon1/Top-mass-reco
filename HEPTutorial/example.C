#include "MyAnalysis.h"
#include "Plotter.h"
#include <iostream>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <string>

int main() {
   
   float lumi = 50.;
   
   MyAnalysis *A = new MyAnalysis();
   TChain* ch = new TChain("events");
   ch->Add("files/data.root");
   ch->Process(A);
   
   MyAnalysis *B = new MyAnalysis();
   TChain* ch2 = new TChain("events");
   ch2->Add("files/ttbar.root");
   ch2->Process(B);
   
   MyAnalysis *C = new MyAnalysis();
   TChain* ch3 = new TChain("events");
   ch3->Add("files/wjets.root");
   ch3->Process(C);
   
   MyAnalysis *D = new MyAnalysis();
   TChain* ch4 = new TChain("events");
   ch4->Add("files/dy.root");
   ch4->Process(D);
   
   MyAnalysis *E = new MyAnalysis();
   TChain* ch5 = new TChain("events");
   ch5->Add("files/ww.root");
   ch5->Process(E);

   MyAnalysis *F = new MyAnalysis();
   TChain* ch6 = new TChain("events");
   ch6->Add("files/wz.root");
   ch6->Process(F);

   MyAnalysis *G = new MyAnalysis();
   TChain* ch7 = new TChain("events");
   ch7->Add("files/zz.root");
   ch7->Process(G);

   MyAnalysis *H = new MyAnalysis();
   TChain* ch8 = new TChain("events");
   ch8->Add("files/qcd.root");
   ch8->Process(H);
   
   Plotter P;
   P.SetData(A->histograms, std::string("Data"));
   P.AddBg(B->histograms, std::string("TTbar"));
   P.AddBg(C->histograms, std::string("Wjets"));
   P.AddBg(D->histograms, std::string("DY"));
   P.AddBg(E->histograms, std::string("WW"));
   P.AddBg(F->histograms, std::string("WZ"));
   P.AddBg(G->histograms, std::string("ZZ"));
   P.AddBg(H->histograms, std::string("QCD"));
   P.Plot(string("results_log.pdf"), true);
   P.Plot(string("results.pdf"), false);
   
   Plotter P_MC;
   P_MC.AddBg(B->histograms_MC, std::string("TTbar"));
   P_MC.AddBg(C->histograms_MC, std::string("Wjets"));
   P_MC.AddBg(D->histograms_MC, std::string("DY"));
   P_MC.AddBg(E->histograms_MC, std::string("WW"));
   P_MC.AddBg(F->histograms_MC, std::string("WZ"));
   P_MC.AddBg(G->histograms_MC, std::string("ZZ"));
   P_MC.AddBg(H->histograms_MC, std::string("QCD"));
   P_MC.Plot(string("results_MC_log.pdf"), true);
   P_MC.Plot(string("results_MC.pdf"), false);
   

   //////////////////////////////
   TCanvas *c2 = new TCanvas("c2", "c2", 600, 600);
    
   TGraphAsymmErrors* h_trigg_eff = new TGraphAsymmErrors(B->h_selectedEvents_triggered_Muon1_Pt, B->h_selectedEvents_Muon1_Pt);
   h_trigg_eff->GetXaxis()->SetTitle("Muon p_{T} [GeV]");
   h_trigg_eff->GetYaxis()->SetTitle("Trigger efficiency");
   h_trigg_eff->UseCurrentStyle();
   h_trigg_eff->Draw("AP");
   
   c2->Print("Trigger_eff.pdf");
   //////////////////////////////

   //////////////////////////////
   ///Calculation of cross section

   // Acceptance from MC
   cout << "-----------------------------------------------" << endl;
   cout << "Monte Carlo:" << endl;
   cout << "Signal Number of generated events: " << B->GeneratedEvents << endl;
   cout << "Signal Number of selected events: " << B->SelectedEvents << endl;
   cout << "Signal Number of selected and triggered events: " << B->SelectedEvents_triggered << endl;

   double acc = B->SelectedEvents / B->GeneratedEvents;
   cout << "Signal Acceptance: " << acc << endl;
   double trigg_eff = B->SelectedEvents_triggered / B->SelectedEvents;
   cout << "Trigger efficiency:" << trigg_eff << endl;
   cout << "selected signal:" << B->SelectedEvents <<endl;
   double N_bg_wjet = C->SelectedEvents; 
   double N_bg_dy = D->SelectedEvents;  
   double N_bg_ww = E->SelectedEvents;
   double N_bg_wz = F->SelectedEvents;
   double N_bg_tot = N_bg_wjet + N_bg_dy + N_bg_ww + N_bg_wz;
  
   
   cout << "GEWjet:" << C->GeneratedEvents << "I GEdy:" << D->GeneratedEvents << "I GEww:" << E->GeneratedEvents << "I GEwz:" << F->GeneratedEvents << endl;
   
   
   
   cout << "bg of wjet:" << N_bg_wjet << endl;
   cout << "bg of dy:" << N_bg_dy << endl;   
   cout << "bg of ww:" << N_bg_ww << endl;
   cout << "bg of wz:" << N_bg_wz << endl;
   cout << "Background Number of selected and triggered events:" << N_bg_tot << endl;

   double purity = B->SelectedEvents_triggered / (B->SelectedEvents_triggered + N_bg_tot);
   cout << "Purity:" << purity << endl;
   cout << endl;
   cout << "Data:" << endl;
   cout << "Number of selected and triggered events: " << A->SelectedEvents_triggered << endl;
   double N_bgSub = A->SelectedEvents_triggered - A->SelectedEvents_triggered * (1. - purity);
   cout << "Number of selected and triggered events (bg subtracted): " << N_bgSub << endl;
   double N_corr = N_bgSub / acc / trigg_eff;
   cout << "Acceptance and trigg efficiency corrected yield: " << N_corr << endl;
   cout << "Cross section [pb]: " << N_corr / lumi << endl;
   cout << "-----------------------------------------------" << endl;






   //////////////////////////////


}
