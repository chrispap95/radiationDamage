#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <vector>
#include <string>
#include "RootStyle.cc"

using namespace std;

struct measurement {
    TString include;
    vector<float> x;
    vector<float> y;
    vector<float> ex;
    vector<float> ey;
    vector<TString> irrEndDate;
    vector<TString> irrFacility;
    vector<TString> ronNumber;
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void R_vs_dose(TString tagTime="") {
    set_root_style();
    Bool_t useGray = true;

    // ======================================================================================================
    // Begin of results section
    // ======================================================================================================

    // This is where people decides which measurements shall be included in the plot
    // and add new measurements to the list of plottable results.
    // Please make sure that there is a sensible comment identifying each new result.
    // Ideally, this is the ONLY section that needs to be updated whenever a new measurement is available.

    vector<TString> incMeas;
    incMeas.push_back("EJ200PS");
    incMeas.push_back("EJ200PVT");
    incMeas.push_back("EJ260PVT");
    incMeas.push_back("EJ260PS");

    vector<measurement> measurements;
    measurements.push_back(
        {
            "EJ200PS",
            {2.4,4.6,7.0},
            {2.75562/4.94908,2.36635/4.94908,3.36549/4.94908},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.05*2.75562/4.94908,0.05*2.36635/4.94908,0.05*3.36549/4.94908},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"","",""},
            {"L2R","L3R","L4R"},
            41,43,1.4,"EJ200PS",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT",
            {2.4,4.6,7.0},
            {4.38135/6.00071,3.6879/6.00071,3.38832/6.00071},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.05*4.38135/6.00071,0.05*3.6879/6.00071,0.05*3.38832/6.00071},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"","",""},
            {"L1R","L2R","L3R"},
            48,43,1.4,"EJ200PVT",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PVT",
            {2.4,4.6,7.0},
            {2.71055/3.75721,2.2885/3.75721,2.0491/3.75721},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.05*2.71055/3.75721,0.05*2.2885/3.75721,0.05*2.0491/3.75721},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"","",""},
            {"L1R","L2R","L3R"},
            58,43,1.4,"EJ260PVT",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PS",
            {2.4,4.6,7.0},
            {2.2391/3.27037,1.93542/3.27037,1.71389/3.27037},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.05*2.2391/3.27037,0.05*1.93542/3.27037,0.05*1.71389/3.27037},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"","",""},
            {"L1R","L2R","L3R"},
            68,43,1.4,"EJ260PS",1,NULL
        }
    );


    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Begin of graph preparation section
    // ======================================================================================================

    TString filename = "";
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i) {
        // skip measurements not to be included
        bool includeMeasurement = 0;
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->include) == 0) {
                includeMeasurement = 1;
            }
        }
        if (!includeMeasurement) continue;
        if (filename != "") filename += "_";
        filename += i->include;

        // check sanity of input
        unsigned int n = i->x.size();
        if (i->y.size() !=n ||
        i->ex.size()!=n ||
        i->ey.size()!=n ) {
            cout << "Check inputs in sample: " << i->legendLabel.c_str() << endl;
            continue;
        }

        vector<float> x_temp = i->x;
        vector<float> y_temp = i->y;
        vector<float> ex_temp = i->ex;
        vector<float> ey_temp = i->ey;
        if (useGray) {
          for(int i = 0; i<n; ++i) {
            x_temp[i]*=0.01*1000*1000; // Mrad to Gy
            y_temp[i]*=1; // Mrad to Gy
            ex_temp[i]*=0.01*1000*1000;
            ey_temp[i]*=1;
          }
        }

        TGraphErrors *graph = new TGraphErrors(n,x_temp.data(),y_temp.data(),ex_temp.data(),ey_temp.data());
        graph->SetTitle();
        graph->GetXaxis()->SetTitle();
        graph->SetMarkerColor(i->markerColor);
        graph->SetFillColor(i->markerColor);
        graph->SetLineColor(i->markerColor);
        graph->SetMarkerSize(i->markerSize);
        graph->SetMarkerStyle(i->markerStyle);

        // create working pointer only when measurement is to be plotted
        i->graph = graph;

    }

    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Drawing
    // ======================================================================================================

    // Canvas
    TCanvas *canv = new TCanvas("canv","D versus doserate",200,10,800,600);
    canv->SetFillColor(0);

    // MultiGraph with all data points
    TMultiGraph *allgraphs = new TMultiGraph();
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->include) == 0) {
                allgraphs->Add(i->graph);
            }
        }
    }

    allgraphs->SetTitle("December 2020 irr. - 49 krad/hr");
    allgraphs->Draw("AP");
    allgraphs->GetXaxis()->SetTitle("Dose (Mrad)");
    allgraphs->GetYaxis()->SetTitle("LY ratio R");
    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose (Gy)");
      allgraphs->GetYaxis()->SetTitle("LY ratio R");
    }
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(1.0);
    double xmin = 1.e1;
    double xmax = 1.e2;
    if (useGray) {
      allgraphs->SetMinimum(0.0);
      allgraphs->SetMaximum(1.0);
      xmin = 1.e4;
      xmax = 1.e5;
    }
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.4,0.2,0.75,0.32);
    legendA->SetBorderSize(0);
    legendA->SetFillColor(0);
    legendA->SetTextSize(0.04);
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->include) == 0 && i->legendId == 1) {
                legendA->AddEntry(i->graph,i->legendLabel.c_str(),"p");
            }
        }
    }
    legendA->Draw();

    TLegend *legendB = new TLegend(0.32,0.12,0.62,0.32);
    legendB->SetBorderSize(0);
    legendB->SetFillColor(0);
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->include) == 0 && i->legendId == 2) {
                legendB->AddEntry(i->graph,i->legendLabel.c_str(),"p");
            }
        }
    }
    //legendB->Draw();

    // use TPad to get border and fill color, TLatex does not have them
    // TLatex *material = new TLatex(0.8,0.94);
    // material->SetBorderSize(1);
    // material->SetFillColor(0);
    // material->SetTextAlign(11);
    // material->DrawLatex("#color[2]{#Box} PS based\\#color[4]{#Box} PVT based");

    canv->Update();

    filename+=tagTime;

    canv->SaveAs("paperPlots/ver11/RvsDose_"+filename+".pdf");
}
