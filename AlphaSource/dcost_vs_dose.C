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
    vector<TString> rodNumber;
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void dcost_vs_dose(TString tagTime="") {
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
    incMeas.push_back("EJ200PVT-1X1P May 2021");
    incMeas.push_back("EJ200PS May 2021");
    //incMeas.push_back("EJ200PVT");
    //incMeas.push_back("EJ260PVT");
    //incMeas.push_back("EJ260PS");

    vector<measurement> measurements;
    measurements.push_back(
        {
            "EJ200PS",
            {2.4,4.6,7.0},
            {4.2035,6.42007,18.52005},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.23358,0.28804,1.53559},
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
            {7.70338,9.59388,12.53412},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.77443,0.63328,0.71385},
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
            {7.55433,9.63463,11.96351},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.74142,0.63449,0.64772},
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
            {6.64756,9.24508,11.50421},
            {2.4*0.012,4.6*0.012,7.0*0.012},
            {0.56713,0.57709,0.593},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"","",""},
            {"L1R","L2R","L3R"},
            68,43,1.4,"EJ260PS",1,NULL
        }
    );

    ////////////////////////////////////////////////////////////////////////////

    measurements.push_back(
        {
            "EJ200PVT-1X1P May 2021",
            {0.8,1.2,1.6,2.0,2.4,3.3,4.2,4.6,5.8,7.0}, // Mrad
            {4.92632,5.94148,6.65586,5.89128,7.37121,7.46682,7.65451,9.53392,9.17561,10.51769}, // Mrad
            {0.8*0.05,1.2*0.05,1.6*0.05,2.0*0.05,2.4*0.05,3.3*0.05,4.2*0.05,4.6*0.05,5.8*0.05,7.0*0.05},
            {0.97482,0.96189,0.92334,0.61474,0.79452,0.64433,0.57823,0.77748,0.64337,0.71973},
            {"Sandia","Sandia","Sandia","Sandia","Sandia","Sandia","Sandia","Sandia","Sandia","Sandia"},
            {"20210616","20210616","20210616","20210616","20210616","20210616","20210616","20210616","20210616","20210616"},
            {"A1","A2","A3","A4","A5","A6","A7","A8","A9","A10"},
            4,22,1.4,"EJ200PVT-1X1P May 2021",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS May 2021",
            {2.4,4.6,7.0}, // Mrad
            {5.70698,6.60486,8.16197}, // Mrad
            {2.4*0.05,4.6*0.05,7.0*0.05},
            {0.50795,0.44261,0.50368},
            {"Sandia","Sandia","Sandia"},
            {"20210616","20210616","20210616"},
            {"L5R","L6R","L11R"},
            2,21,1.4,"EJ200PS May 2021",1,NULL
        }
    );//***** Need to correct data for L11R

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
            y_temp[i]*=0.01*1000*1000; // Mrad to Gy
            ex_temp[i]*=0.01*1000*1000; // Mrad to Gy
            ey_temp[i]*=0.01*1000*1000; // Mrad to Gy
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
    allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose (Gy)");
      allgraphs->GetYaxis()->SetTitle("Dose Constant (Gy)");
    }
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // Force exponential notation
    allgraphs->GetXaxis()->SetMaxDigits(3);
    gPad->SetRightMargin(0.1);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(15.0);
    double xmin = 0.e1;
    double xmax = 8.e1;
    if (useGray) {
      allgraphs->SetMinimum(0.0*0.01*1000*1000);
      allgraphs->SetMaximum(15.0*0.01*1000*1000);
      xmin = 0.e4;
      xmax = 8.e4;
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

    canv->SaveAs("paperPlots/ver14/dCostvsDose_"+filename+".pdf");
}
