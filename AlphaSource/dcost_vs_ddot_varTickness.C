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
    TString name;
    vector<float> x;
    vector<float> y;
    vector<float> ex;
    vector<float> ey;
    vector<TString> irrEndDate;
    vector<TString> irrFacility;
    vector<TString> rodNumber;
    float thickness;
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void dcost_vs_ddot_varTickness(TString tagTime="") {
    set_root_style();
    Bool_t useGray = true;
    Bool_t multiplyOn = false;

    // ======================================================================================================
    // Begin of results section
    // ======================================================================================================

    // This is where people decides which measurements shall be included in the plot
    // and add new measurements to the list of plottable results.
    // Please make sure that there is a sensible comment identifying each new result.
    // Ideally, this is the ONLY section that needs to be updated whenever a new measurement is available.

    vector<TString> incMeas;
    incMeas.push_back("EJ200PS-T1");
    incMeas.push_back("EJ200PS-T2");
    incMeas.push_back("EJ200PS-T3");
    incMeas.push_back("EJ200PS-T4");

    vector<measurement> measurements;
    measurements.push_back(
        {
            "EJ200PVT-T1",
            {8.19,370,0.31,0.31},
            {11.25147,13.08882,4.09073,3.99646},
            {8.19*0.012,370*0.012,0.1*0.31,0.1*0.31},
            {0.57177,0.76828,0.5779,0.5586},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130","20181130"},
            {"2","3","5","N1"},
            1.0,1,20,1.4,"EJ200PVT 10 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-T2",
            {8.19,370,0.31,0.31},
            {11.9689,14.40618,4.497,4.05059},
            {8.19*0.012,370*0.012,0.1*0.31,0.1*0.31},
            {0.64748,0.93056,0.66906,0.57046},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130","20181130"},
            {"2","3","5","N1"},
            0.8,2,21,1.4,"EJ200PVT 8 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-T3",
            {8.19,370,0.31,0.31},
            {11.71462,13.92498,4.26997,4.57569},
            {8.19*0.012,370*0.012,0.1*0.31,0.1*0.31},
            {0.62215,0.87238,0.61838,0.68771},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130","20181130"},
            {"2","3","5","N1"},
            0.6,3,33,1.4,"EJ200PVT 6 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-T4",
            {8.19,370,0.31,0.31},
            {11.69649,13.79548,5.55214,6.62223},
            {8.19*0.012,370*0.012,0.1*0.31,0.1*0.31},
            {0.62153,0.85773,0.93927,1.26471},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130","20181130"},
            {"2","3","5","N1"},
            0.4,4,34,1.4,"EJ200PVT 4 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-T5",
            {0.31},
            {5.84174},
            {0.1*0.31},
            {1.02361},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N1"},
            0.2,28,22,1.4,"EJ200PVT 2 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS-T1",
            {8.06,364,338,0.31,0.31},
            {12.65279,9.08617,8.92548,4.28092,4.60692},
            {8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1},
            {0.71625,0.37693,0.36422,0.61784,0.69133},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            1.0,1,20,1.4,"EJ200PS 10 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS-T2",
            {8.06,364,338,0.31,0.31},
            {10.85626,9.73245,9.34377,4.77711,4.80643},
            {8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1},
            {0.53378,0.43188,0.39811,0.73249,0.73966},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.8,2,21,1.4,"EJ200PS 8 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS-T3",
            {8.06,364,338,0.31,0.31},
            {11.73816,10.25757,9.69536,4.94397,5.45539},
            {8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1},
            {0.6233,0.47768,0.4286,0.77433,0.90848},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.6,3,33,1.4,"EJ200PS 6 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS-T4",
            {8.06,364,338,0.31,0.31},
            {12.17034,10.48889,10.0132,4.9973,6.59146},
            {8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1},
            {0.6696,0.50005,0.45692,0.78879,1.25067},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.4,4,34,1.4,"EJ200PS 4 mm",1,NULL
        }
    );


    measurements.push_back(
        {
            "EJ200PS-T5",
            {0.31},
            {6.44575},
            {0.31*0.1},
            {1.21081},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N1"},
            0.2,28,22,1.4,"EJ200PS 2 mm",1,NULL
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
            if ((*it).CompareTo(i->name) == 0) {
                includeMeasurement = 1;
            }
        }
        if (!includeMeasurement) continue;
        if (filename != "") filename += "_";
        filename += i->name;

        // check sanity of input
        unsigned int n = i->x.size();
        if (i->y.size() !=n ||
        i->ex.size()!=n ||
        i->ey.size()!=n ) {
            cout << "Check inputs in sample: " << i->legendLabel.c_str() << endl;
            continue;
        }

        float thickness = i->thickness;
        vector<float> x_temp = i->x;
        vector<float> y_temp = i->y;
        vector<float> ex_temp = i->ex;
        vector<float> ey_temp = i->ey;
        if (useGray) {
          for(int i = 0; i<n; ++i) {
            x_temp[i]*=0.01*1000; // krad/hr to Gy/hr
            ex_temp[i]*=0.01*1000; // krad/hr to Gy/hr
            if (multiplyOn) {
              y_temp[i]*=0.01*1000*1000*thickness; // Mrad to Gy
              ey_temp[i]*=0.01*1000*1000*thickness; // Mrad to Gy
            } else {
              y_temp[i]*=0.01*1000*1000; // Mrad to Gy
              ey_temp[i]*=0.01*1000*1000; // Mrad to Gy
            }
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
    canv->SetLogx();
    gStyle->SetPalette(kViridis);
    //canv->SetGrayscale();
    //canv->SetLogx();

    // MultiGraph with all data points
    TMultiGraph *allgraphs = new TMultiGraph();
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->name) == 0) {
                allgraphs->Add(i->graph);
            }
        }
    }

    allgraphs->Draw("AP PMC PLC");
    allgraphs->GetXaxis()->SetTitle("Dose rate (krad/h)");
    if (multiplyOn){
      allgraphs->GetYaxis()->SetTitle("Dose Constant#times Thickness (Mrad#times cm)");
    }else {
      allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    }

    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose rate (Gy/h)");
      if (multiplyOn) {
        allgraphs->GetYaxis()->SetTitle("Dose Constant#times Thickness (Gy#times cm)");
      }else {
        allgraphs->GetYaxis()->SetTitle("Dose Constant (Gy)");
      }
    }
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(16.0);
    double xmin = 5.e-2;
    double xmax = 2.e3;
    if (useGray) {
      allgraphs->SetMinimum(0.0*0.01*1000*1000);
      allgraphs->SetMaximum(16.0*0.01*1000*1000);
      xmin = 5.e-1;
      xmax = 2.e4;
    }
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.4,0.2,0.75,0.4);
    legendA->SetBorderSize(0);
    legendA->SetFillColor(0);
    legendA->SetTextSize(0.04);
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->name) == 0 && i->legendId == 1) {
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
            if ((*it).CompareTo(i->name) == 0 && i->legendId == 2) {
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

    canv->SaveAs("paperPlots/ver17/"+filename+".pdf");
}
