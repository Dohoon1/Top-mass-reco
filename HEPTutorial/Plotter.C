/*
 * Plotter.C
 *
 *  Created on: 25.06.2012
 *      Author: csander
 */

#include "Plotter.h"

Plotter::Plotter() {
	// TODO Auto-generated constructor stub
   
}

Plotter::~Plotter() {
	// TODO Auto-generated destructor stub
}

void Plotter::Plot(std::string filename, bool DrawLog) {
   
	gROOT->Reset();
	gROOT->SetStyle("Plain");
   
	TStyle *MyStyle = new TStyle("MyStyle","My Root Styles");

        MyStyle->SetCanvasDefH(150);
        MyStyle->SetCanvasDefW(150);

        MyStyle->SetAxisColor(1, "XYZ");
        MyStyle->SetStripDecimals(1);
        MyStyle->SetTickLength(0.03, "XYZ");
        MyStyle->SetNdivisions(510, "XYZ");
        MyStyle->SetPadTickX(1);
        MyStyle->SetPadTickY(1);  

        MyStyle->SetTitleColor(1, "XYZ");
        MyStyle->SetTitleFont(42, "XYZ");

        MyStyle->SetLabelColor(1, "XYZ");
        MyStyle->SetLabelFont(42, "XYZ");

	MyStyle->SetStatColor(0);
	MyStyle->SetCanvasColor(0);
	MyStyle->SetPadColor(0);
	MyStyle->SetPadBorderMode(0);
	MyStyle->SetCanvasBorderMode(0);
	MyStyle->SetFrameBorderMode(0);
	MyStyle->SetOptStat(0);
	MyStyle->SetStatBorderSize(2);
	MyStyle->SetOptTitle(0);
	MyStyle->SetPadBorderSize(2);
	MyStyle->SetPalette(51, 0);
	MyStyle->SetPadBottomMargin(0.15);
	MyStyle->SetPadTopMargin(0.10);
	MyStyle->SetPadLeftMargin(0.15);
	MyStyle->SetPadRightMargin(0.10);
	MyStyle->SetTitleFillColor(0);
	MyStyle->SetTitleFontSize(0.05);
	MyStyle->SetTitleBorderSize(0);
	MyStyle->SetLineWidth(1);
	MyStyle->SetHistLineWidth(3);
	MyStyle->SetLegendBorderSize(0);
	MyStyle->SetMarkerSize(1);
	MyStyle->SetTitleOffset(1.5, "x");
	MyStyle->SetTitleOffset(1.5, "y");
	MyStyle->SetTitleOffset(1.0, "z");
	MyStyle->SetLabelSize(0.06, "x");
	MyStyle->SetLabelSize(0.06, "y");
	MyStyle->SetLabelSize(0.06, "z");
	MyStyle->SetLabelOffset(0.15, "x");
	MyStyle->SetLabelOffset(0.15, "y");
	MyStyle->SetLabelOffset(0.15, "z");
	MyStyle->SetTitleSize(0.05, "x");
	MyStyle->SetTitleSize(0.05, "y");
	MyStyle->SetTitleSize(0.05, "z");
	gROOT->SetStyle("MyStyle");
   
	gROOT->ForceStyle();
   
    //bool DrawLog = true;
    
	for (int i = 0; i < N_histos; ++i) {
		
		THStack *hs;
		TLegend *l;
		int Nset = data.size() + bg.size() + signal.size();
		if (Nset > 20)
			Nset = 20.;
		//l = new TLegend(0.76, 0.95 - 0.8 * Nset / 20., 1.0, 0.95);
		l = new TLegend(0.70, 0.87 - 0.8 * Nset / 20., 0.87, 0.87);
		l->SetFillStyle(1001);
		l->SetFillColor(kWhite);
		l->SetLineColor(kWhite);
		l->SetLineWidth(2);
		if (bg.size() > 0) {
			hs = new THStack("hs", bg.at(0).at(i)->GetName());
			int j = 0;
			for (std::vector<std::vector<TH1F*> >::const_iterator it = bg.begin(); it != bg.end(); ++it) {
				switch (j) {
				case 0:
					it->at(i)->SetFillColor(kRed);
					break;
				case 1:
					it->at(i)->SetFillColor(kOrange);
					break;
				case 2:
					it->at(i)->SetFillColor(kYellow);
					break;
				case 3:
					it->at(i)->SetFillColor(kGreen);
					break;
				case 4:
					it->at(i)->SetFillColor(kCyan);
					break;
				case 5:
					it->at(i)->SetFillColor(kBlue);
					break;
				case 6:
					it->at(i)->SetFillColor(kMagenta);
					break;
				case 7:
					it->at(i)->SetFillColor(kGray);
					break;
				case 8:
					it->at(i)->SetFillColor(kGray + 2);
					break;
				default:
					it->at(i)->SetFillColor(kBlack);
					break;
				}
                if(useLumi){
                  float scale = luminosity/(bg_N.at(j)/bg_X.at(j));
                  it->at(i)->Scale( scale );
                }
				hs->Add(it->at(i));
                
				l->AddEntry(it->at(i), bg_names.at(j).c_str(), "f");
				++j;
			}
		}
		//TCanvas *c = new TCanvas("c", "c", 800, 600);
        TCanvas *c = new TCanvas(Form("c_%i",i), "c", 1 );
		c->SetLogy(DrawLog);
        
		std::string plotname;
		if (data.size() > 0) {
			plotname = std::string(data.at(0).at(i)->GetName());
			//data.at(0).at(i)->SetMaximum(5 * data.at(0).at(i)->GetMaximum());
		    TH1 *histogram = data.at(0).at(i);
        
            data.at(0).at(i)->GetXaxis()->SetTitleOffset(1.5);
			data.at(0).at(i)->GetYaxis()->SetTitleOffset(1.5);
			data.at(0).at(i)->GetYaxis()->SetTitle("Events");
			data.at(0).at(i)->GetXaxis()->SetNdivisions(505);
			data.at(0).at(i)->Draw("");
		    l->AddEntry(data.at(0).at(i), data_names.at(0).c_str(), "p");
	        TF1 *gaussian = new TF1("gaussian", "gaus");
            histogram -> Fit(gaussian);
            
            int binmax = histogram->GetMaximumBin();
            double xpeak = histogram->GetXaxis()->GetBinCenter(binmax);
            double mean = gaussian->GetParameter(1);
            double peak = histogram->GetMaximum();  
            
            std::cout<< "mean : "<<mean<<std::endl; 
            std::cout<< "peak : "<<peak<<std::endl;
            std::cout<< "x_peak : "<<xpeak<<std::endl;
            
            TLine *line = new TLine(xpeak, 0, xpeak, peak);
            line->SetLineColor(kBlue);
            line->Draw("same");


            if (bg.size() > 0) hs->Draw("histsame");
			   data.at(0).at(i)->SetMarkerStyle(20);
			   data.at(0).at(i)->SetMarkerSize(0.3);
			   data.at(0).at(i)->Draw("psame");
			   l->Draw("same");
	       
           
            

        }
               



		if (data.size() == 0 && bg.size() > 0) {
			plotname = std::string(bg.at(0).at(i)->GetName());
            


            hs->Draw("hist");
            //hs->SetMaximum(5 * bg.at(0).at(i)->GetMaximum());
            hs->SetMaximum(1.2 * hs->GetMaximum());
            hs->GetXaxis()->SetTitleOffset(1.5);
            hs->GetYaxis()->SetTitleOffset(1.5);
            hs->GetYaxis()->SetTitle("Events");
            hs->GetXaxis()->SetNdivisions(505);
            hs->GetXaxis()->SetTitle(bg.at(0).at(i)->GetXaxis()->GetTitle()); 
            l->Draw("same");
		    
            
           
            
        }
              
        c->Print((std::string("plot_")+plotname+std::string(".pdf")).c_str());

		if (i == 0 && N_histos > 1){
			c->Print((filename+std::string("(")).c_str());
		}else if (i > 0 && i == N_histos - 1){
			c->Print((filename+std::string(")")).c_str());
		}else{
			c->Print(filename.c_str());
        }
	
    }	
}
