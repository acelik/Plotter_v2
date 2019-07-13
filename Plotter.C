#include <iostream>
#include <vector>
#include <cstdlib>
#include <TH1F.h>
#include <string>
#include <utility>
#include "TEfficiency.h"


void Plotter(std::string root_1, std::string root_2)
{
    gROOT->ForceStyle();
    gROOT->SetStyle("Plain");
    gStyle->SetPalette(1);
    gStyle->SetOptTitle(kTRUE);
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(111);
    gStyle->SetStatY(1);
    gStyle->SetStatX(0.9);
    gStyle->SetStatW(0.1);
    gStyle->SetStatH(0.11);
    TCanvas* c1 = new TCanvas("c1","c1", 2600,1400);
    c1->Divide(2,1);

    std::vector<string> root_files = {root_1, root_2};
    std::vector<string> directories = {"GEMRecHits", "GEMHits", "GEMDigis"};//GemHits and GemDigis directory does not have similar eff plots. Thus, No need to loop over these two directories. 

    std::vector<string> numerator = {"rh_eta_st2_l1and2", "rh_eta_st2_l1","rh_eta_st2_l2","rh_eta_st2_l1or2","rh_phi_st2_l1and2_all", "rh_phi_st2_l1_all","rh_phi_st2_l2_all","rh_phi_st2_l1or2_all"};
        std::vector<string> denominator = {"track_eta_st2", "track_phi_st2_all"};

    int k_max;
    int k;
    double fit_range_min;
    double fit_range_max;

    std::string x_axis_name_ ;
    std::string y_axis_name_ = "Eff.";
    std::string Name_in_title;

    for (int z = 0; z < denominator.size(); ++z){


        if (z==0){
            k_max = 4;
            k=0;
            x_axis_name_ = "track |#eta|";
            fit_range_min = 1.65;
            fit_range_max = 2.45;

        }
        
        else {
        k_max = numerator.size();
        k=4;
        x_axis_name_ = "track #phi";
        fit_range_min = -3.14159;
        fit_range_max = 3.14159;

        }

        for (k; k < k_max; ++k){

            

            for (int i = 0; i < root_files.size(); ++i){
                Name_in_title = numerator[k];
                c1->cd(i+1);
                TFile *_file0=TFile::Open(root_files[i].c_str());
                std::string numerator_ = "DQMData/Run 1/Muon"+directories[0]+"V/Run summary/"+ directories[0]+"Task/" +numerator[k];
                TH1F *num = (TH1F*)_file0->Get(numerator_.c_str());
                std::string denominator_ = "DQMData/Run 1/Muon"+directories[0]+"V/Run summary/"+ directories[0]+"Task/" + denominator[z];
                TH1F *den = (TH1F*)_file0->Get(denominator_.c_str());
                TEfficiency* effpassed = new TEfficiency(*num, *den);
                effpassed->SetLineColor(kBlue);
                effpassed->SetLineWidth(2);
                effpassed->SetMarkerStyle(1);
                effpassed->SetMarkerSize(1);
                TF1 *f1 = new TF1("f1","pol0", fit_range_min, fit_range_max);
                f1->SetLineColor(kRed);
                effpassed-> Fit(f1, "r");
                Name_in_title.erase(0,7);
                std::string title_ = "Eff. for a simTrack to have an associated GEMRecHits in "+ Name_in_title+";"+x_axis_name_+";"+ y_axis_name_;
                
                effpassed->SetTitle(title_.c_str());

                effpassed->Draw();
                gPad-> Update();
                effpassed->GetPaintedGraph()->GetHistogram()->SetBins(1000, fit_range_min, fit_range_max);

   
                std::string watermark;
                if (i==0){
                    watermark = "Old";
                }

                else{
                    watermark = "New";

                }
                   TLatex latex;
                   latex.SetTextSize(.1);
                   latex.SetTextColor(17);
                   latex.DrawLatex((fit_range_min+fit_range_max)/2,.4, watermark.c_str());
             
                delete num;
                delete den;
                std::string name_of_the_plot = numerator[k]+directories[0]+".png"; 
                c1->SaveAs(name_of_the_plot.c_str());
            }

        }

    }
}
