#include <map>
#include "TObject.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

void DvsR() {
    // Preambles
    gROOT->SetStyle("UMD");
    // overwrite style para
    gStyle->SetEndErrorSize(2.5);
    gStyle->SetPadTickX(0);

    //gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.02);
    //gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.12);
    //gStyle->SetTitleXOffset(1.4);
    gStyle->SetTitleYOffset(1.2);

    TString tagTime("20190305"); // Date tag used in filenames. Change appropriately
    std::cout << "File date tag = " << tagTime << std::endl;

    // ======================================================================================================
    // Begin of results section
    // ======================================================================================================
    // The section containing all data
    map<string,int> colors_;
    colors_["insitu"] = 9;

    //Input file containing fit values
    ifstream fin;
    fin.open("EJ200PVT.txt");
    std::vector<string> id;
    std::vector<float> x, y, ex, ey;
    while(!fin.eof()) {
        string temp_id;
        float temp_x, temp_y, temp_ex, temp_ey;
        fin >> temp_id >> temp_x >> temp_y >> temp_ex >> temp_ey;
        id.push_back(temp_id);
        x.push_back(temp_x);
        y.push_back(temp_y);
        ex.push_back(temp_ex);
        ey.push_back(temp_ey);
    }

    // ********************************************************
    // All the graphs
    TGraphErrors *gr = new TGraphErrors(x.size(),&x[0],&y[0],&ex[0],&ey[0]);
    gr->SetTitle("D (Mrad) versus dose rate (krad/hr)");
    gr->SetMarkerColor(kBlue);
    gr->SetMarkerSize(1.5);
    gr->SetMarkerStyle(34);
    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Drawing
    // ======================================================================================================
    TCanvas *c1 = new TCanvas("c1","D versus doserate",200,10,800,600);
    c1->SetFillColor(0);
    c1->SetLogy();
    c1->SetLogx();

    gr->Draw();
    gr->GetXaxis()->SetTitle("Dose rate (krad/hr)");
    gr->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    // gr->SetMinimum(0.01);
    // gr->SetMaximum(300);
    gr->SetMinimum(2);
    gr->SetMaximum(50.0);
    double xmin = 1.e-2;//1.e-4;
    double xmax = 2.e3;//2.e3;
    gr->GetXaxis()->SetLimits(xmin,xmax);

    // ====================================
    // UMD PVT Fit
    // ====================================
    double fitxmin1 = TMath::MinElement(gr->GetN(),gr->GetX());
    double fitxmax1 = TMath::MaxElement(gr->GetN(),gr->GetX());

    TF1 *fpowUMDPVT = new TF1("fpowUMDPVT","sqrt(x)/([0]+[1]*sqrt(x))",fitxmin1,fitxmax1);
    fpowUMDPVT->SetParameters(0.08,0.05);

    TFitResultPtr resfitUMDPVT = gr->Fit("fpowUMDPVT","SREM0","");
    cout << "[FIT UMD PVT]: p0 = " << resfitUMDPVT->Parameter(0) << endl;
    cout << "[FIT UMD PVT]: p1 = " << resfitUMDPVT->Parameter(1) << endl;
    cout << "[FIT UMD PVT]: chi2/ndof = " << resfitUMDPVT->Chi2()/(resfitUMDPVT->Ndf()+1.) << endl;
    fpowUMDPVT->SetLineColor(2);
    fpowUMDPVT->SetLineStyle(4);
    fpowUMDPVT->SetLineWidth(2);
    fpowUMDPVT->SetRange(2.e-4,1.e3);
    fpowUMDPVT->Draw("same");

    // ====================================
    // Print fit paras
    // ====================================
    TLatex* fitlatexumd = new TLatex();
    fitlatexumd->SetNDC(true);
    fitlatexumd->SetTextAlign(12);
    fitlatexumd->SetTextSize(0.03);

    TString sfpowUMDPVT;
    sfpowUMDPVT.Form("D_{5 #times 1 #times 1 cm^{3}}^{EJ200PVT-1X2P} = #frac{#sqrt{R}}{%5.3f + %5.3f #sqrt{R}}",resfitUMDPVT->Parameter(0),resfitUMDPVT->Parameter(1));

    c1->Update();

    TString filename = TString("doserate_");
    filename+=tagTime;
    filename+="";

    if (mkdir("Results/"+tagTime, 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;
    else
        cout << "Directory created";

    c1->SaveAs("Results/"+tagTime+"/"+filename+".eps");
}
