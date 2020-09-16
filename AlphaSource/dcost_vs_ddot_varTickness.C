#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <vector>
#include "RootStyle.cc"

using namespace std;

struct measurement {
    bool include;
    vector<float> x;
    vector<float> y;
    vector<float> ex;
    vector<float> ey;
    vector<TString> irrEndDate;
    vector<TString> irrFacility;
    vector<TString> ronNumber;
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

    vector<measurement> measurements;
    measurements.push_back(
        {
            false,
            {8.19,370,0.31},
            {11.58929,14.84599,4.57954},
            {8.19*0.008,370*0.008,0.1*0.31},
            {0.29148,0.46821,0.51974},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130"},
            {"2","3","5"},
            1.0,1,43,1.4,"EJ200PVT 10 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {8.19,370,0.31},
            {12.52827,15.48633,5.11092},
            {8.19*0.008,370*0.008,0.1*0.31},
            {0.33730,0.51168,0.59743},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130"},
            {"2","3","5"},
            0.8,2,43,1.4,"EJ200PVT 8 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {8.19,370,0.31},
            {12.76847,15.65691,5.08014},
            {8.19*0.008,370*0.008,0.1*0.31},
            {0.35405,0.52558,0.59322},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130"},
            {"2","3","5"},
            0.6,3,43,1.4,"EJ200PVT 6 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            false,
            {8.19,370,0.31},
            {13.22884,16.28974,7.43019},
            {8.19*0.008,370*0.008,0.1*0.31},
            {0.37940,0.57490,1.01341},
            {"Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
            {"20170301","20170302","20181130"},
            {"2","3","5"},
            0.4,4,43,1.4,"EJ200PVT 4 mm",1,NULL
        }
    );
/*
    measurements.push_back(
        {
            true,
            {8.06,364,338,0.31,0.31},
            {10.19846,9.68870,8.80205,4.58283,4.85625},
            {8.06*0.008,364*0.008,338*0.008,0.31*0.1,0.31*0.1},
            {0.22203,0.20022,0.16662,0.51391,0.55709},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            1.0,1,43,1.4,"EJ200PS 10 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {8.06,364,338,0.31,0.31},
            {10.78533,10.16701,9.53615,3.70255,4.85511},
            {8.06*0.008,364*0.008,338*0.008,0.31*0.1,0.31*0.1},
            {0.24765,0.22145,0.19591,0.40586,0.55728},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.8,2,43,1.4,"EJ200PS 8 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {8.06,364,338,0.31,0.31},
            {11.79627,10.62995,9.94405,3.98311,5.57522},
            {8.06*0.008,364*0.008,338*0.008,0.31*0.1,0.31*0.1},
            {0.29671,0.24231,0.21315,0.44072,0.66330},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.6,3,43,1.4,"EJ200PS 6 mm",1,NULL
        }
    );

    measurements.push_back(
        {
            true,
            {8.06,364,338,0.31,0.31},
            {11.60022,11.23576,10.61189,4.18924,6.13323},
            {8.06*0.008,364*0.008,338*0.008,0.31*0.1,0.31*0.1},
            {0.29005,0.27167,0.24270,0.46872,0.75419},
            {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170412","20170413","20171107","20181130","20181130"},
            {"2","3","4","5","N1"},
            0.4,4,43,1.4,"EJ200PS 4 mm",1,NULL
        }
    );
*/

measurements.push_back(
    {
        true,
        {8.06,364,338,0.31},
        {10.19846,9.68870,8.80205,4.85625},
        {8.06*0.008,364*0.008,338*0.008,0.31*0.1},
        {0.22203,0.20022,0.16662,0.55709},
        {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
        {"20170412","20170413","20171107","20181130"},
        {"2","3","4","N1"},
        1.0,1,43,1.4,"EJ200PS 10 mm",1,NULL
    }
);

measurements.push_back(
    {
        true,
        {8.06,364,338,0.31},
        {10.78533,10.16701,9.53615,4.85511},
        {8.06*0.008,364*0.008,338*0.008,0.31*0.1},
        {0.24765,0.22145,0.19591,0.55728},
        {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
        {"20170412","20170413","20171107","20181130"},
        {"2","3","4","N1"},
        0.8,2,43,1.4,"EJ200PS 8 mm",1,NULL
    }
);

measurements.push_back(
    {
        true,
        {8.06,364,338,0.31},
        {11.79627,10.62995,9.94405,5.57522},
        {8.06*0.008,364*0.008,338*0.008,0.31*0.1},
        {0.29671,0.24231,0.21315,0.66330},
        {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
        {"20170412","20170413","20171107","20181130"},
        {"2","3","4","N1"},
        0.6,3,43,1.4,"EJ200PS 6 mm",1,NULL
    }
);

measurements.push_back(
    {
        true,
        {8.06,364,338,0.31},
        {11.60022,11.23576,10.61189,6.13323},
        {8.06*0.008,364*0.008,338*0.008,0.31*0.1},
        {0.29005,0.27167,0.24270,0.75419},
        {"Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
        {"20170412","20170413","20171107","20181130"},
        {"2","3","4","N1"},
        0.4,4,43,1.4,"EJ200PS 4 mm",1,NULL
    }
);

   measurements.push_back(
        {
            true,
            {0.31},
            {4.66396},
            {0.31*0.1},
            {0.53442},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N1"},
            0.2,5,43,1.4,"EJ200PS 2 mm",1,NULL
        }
    );

    // ======================================================================================================
    // End of results section
    // ======================================================================================================

    // ======================================================================================================
    // Begin of graph preparation section
    // ======================================================================================================

    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i) {
        // skip measurements not to be included
        if (!i->include) continue;

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
    //canv->SetLogx();

    // MultiGraph with all data points
    TMultiGraph *allgraphs = new TMultiGraph();
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i)
    if (i->include)
    allgraphs->Add(i->graph);

    allgraphs->Draw("AP");
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
      allgraphs->SetMinimum(1.0*0.01*1000*1000);
      allgraphs->SetMaximum(18.0*0.01*1000*1000);
      xmin = 5.e-1;
      xmax = 2.e4;
    }
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.4,0.2,0.75,0.32);
    legendA->SetBorderSize(0);
    legendA->SetFillColor(0);
    legendA->SetTextSize(0.04);
    for (vector<measurement>::iterator i = measurements.begin();i!=measurements.end();++i)
    if (i->include && i->legendId==1)
    legendA->AddEntry(i->graph,i->legendLabel.c_str(),"p");
    legendA->Draw();

    TLegend *legendB = new TLegend(0.32,0.12,0.62,0.32);
    legendB->SetBorderSize(0);
    legendB->SetFillColor(0);
    for (vector<measurement>::iterator i = measurements.begin();i!=measurements.end();++i)
    if (i->include && i->legendId==2)
    legendB->AddEntry(i->graph,i->legendLabel.c_str(),"p");
    //legendB->Draw();

    // use TPad to get border and fill color, TLatex does not have them
    // TLatex *material = new TLatex(0.8,0.94);
    // material->SetBorderSize(1);
    // material->SetFillColor(0);
    // material->SetTextAlign(11);
    // material->DrawLatex("#color[2]{#Box} PS based\\#color[4]{#Box} PVT based");

    canv->Update();

    TString filename = TString("EJ200PS");
    filename+=tagTime;

    //canv->SaveAs("paperPlots/ver5/"+filename+".pdf");
}
