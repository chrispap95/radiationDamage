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
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void dcost_vs_ddot(TString tagTime="") {
    set_root_style();
    Bool_t useGray = true;

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
            true,
            {0.3,293},
            {3.56098,12.19651},
            {0.3*0.1,293*0.008},
            {0.37297,0.40345},
            {"Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20191204","20190222"},
            {"1","2"},
            41,43,1.4,"EJ200A0-0-1X1P",1,NULL
        }
    ); // EJ200A0-0-1X1P

    measurements.push_back(
        {
            true,
            {0.3,293},
            {4.88767,12.88419},
            {0.3*0.1,293*0.008},
            {0.53116,0.44629},
            {"Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20191204","20190222"},
            {"1","2"},
            30,45,1.4,"EJ200A0-1-1X1P",1,NULL
        }
    ); // EJ200A0-1-1X1P

    measurements.push_back(
        {
            true,
            {0.3,293},
            {4.65356,12.7266},
            {0.3*0.1,293*0.008},
            {0.50192,0.43516},
            {"Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20191204","20190222"},
            {"1","2"},
            40,47,1.4,"EJ200A0-2-1X1P",1,NULL
        }
    ); // EJ200A0-2-1X1P

    measurements.push_back(
        {
            false,
            {0.22,8.53,74.4,390,0.98,0.31,364,338,0.31},
            {4.40333,12.88039,11.11065,9.24823,7.88074,5.45017,8.35449,8.97797,4.53911},
            {0.22*0.1,8.53*0.008,74.4*0.008,390*0.008,0.98*0.1,0.31*0.1,364*0.008,338*0.008,0.31*0.1},
            {0.48779,0.34875,0.25850,0.19064,0.81452,0.64176,0.15783,0.17647,0.50814},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)"},
            {"20181026","20161109","20161202","20161004","20181206","20181130","20170413","20171107","20181130"},
            {"4","5","8","9","13","15","T1_3","T1_4","T1_5"},
            1,34,1.3,"EJ200PS-1X1P",1,NULL
        }
    ); // EJ200PS-1X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {7.3069},
            {0.31*0.1},
            {0.86380},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"11"},
            2,33,1.2,"EJ200PS-1X1P double shot",2,NULL
        }
    ); // EJ200PS-1X1P

    measurements.push_back(
        {
            false,
            {0.22,8.53,74.4,390,0.98,0.31},
            {4.3199,12.91542,12.71296,12.58299,7.59314,5.60722},
            {0.22*0.1,8.53*0.008,74.4*0.008,390*0.008,0.98*0.1,0.31*0.1},
            {0.47595,0.39174,0.33572,0.33018,0.78269,0.66323},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20161109","20161202","20161004","20181206","20181130"},
            {"2","5","8","9","13","15"},
            1,33,1.2,"EJ200PS-1X2P single shot",2,NULL
        }
    ); // EJ200PS-1X2P
    measurements.push_back(
        {
            false,
            {0.31},
            {7.28576},
            {0.31*0.1},
            {0.85811},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"11"},
            2,33,1.2,"EJ200PS-1X2P double shot",2,NULL
        }
    ); // EJ200PS-1X2P

    measurements.push_back(
        {
            false,
            {0.22,8.34,257,390},
            {4.40622,12.07512,9.1705,8.40505},
            {0.22*0.1,8.34*0.008,257*0.008,390*0.008},
            {0.48829,0.30896,0.17603,0.15587},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"20181026","20170111","20200214","20161005"},
            {"2","5","8","9","11"},
            1,22,1.2,"EJ200PS-2X1P single shot",2,NULL
        }
    ); // EJ200PS-2X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {6.39462},
            {0.31*0.1},
            {0.73044},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"11"},
            2,33,1.2,"EJ200PS-2X1P double shot",2,NULL
        }
    ); // EJ200PS-2X1P

    measurements.push_back(
        {
            false,
            {74.4,8.53,0.22,390,0.31,0.98,0.31,0.3,293},
            {13.46707,12.31375,3.31789,15.89024,4.87321,6.78443,4.64466,4.88767,12.88419},
            {74.4*0.008,8.53*0.008,0.22*0.1,390*0.008,0.31*0.1,0.98*0.1,0.31*0.1,0.3*0.1,293*0.008},
            {0.3866,0.3264,0.3532,0.5365,0.5594,0.6964,0.5249,0.53116,0.44629},
            {"Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20161202","20161109","20181026","20161004","20181130","20181206","20181130","20191204","20190222"},
            {"N4","N5","N8","N9","N15","N16","T1_5","A0-1","A0-2"},
            4,33,1.3,"EJ200PVT-1X1P",1,NULL
        }
    ); // EJ200PVT-1X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {5.74305},
            {0.31*0.1},
            {0.6426},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N11"},
            2,33,1.2,"EJ200PVT-1X1P double shot",1,NULL
        }
    ); // EJ200PVT-1X1P

    measurements.push_back(
        {
            false,
            {74.4,8.53,0.22,390,0.31,0.98},
            {14.02444,13.11734,3.72787,17.20746,5.16963,6.99842},
            {74.4*0.008,8.53*0.008,0.22*0.1,390*0.008,0.31*0.1,0.98*0.1},
            {0.4589,0.3678,0.4025,0.6251,0.6023,0.7190},
            {"Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20161202","20161109","20181026","20181004","20181130","20181206"},
            {"N4","N5","N8","N9","N15","N13"},
            8,22,1.2,"EJ200PVT-1X2P",1,NULL
        }
    ); // EJ200PVT-1X2P
    measurements.push_back(
        {
            false,
            {0.31},
            {6.50593},
            {0.31*0.1},
            {0.7477},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N11"},
            2,33,1.2,"EJ200PVT-1X2P double shot",1,NULL
        }
    ); // EJ200PVT-1X2P

    measurements.push_back(
        {
            false,
            {0.22,8.34,390},
            {3.53379,13.18657,17.29626},
            {0.22*0.1,8.34*0.056,390*0.056},
            {0.3797,0.3692,0.6313},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)"},
            {"20170111","20181130","20161005"},
            {"N8","N5","N9"},
            46,23,1.2,"EJ200PVT-2X1P",1,NULL
        }
    ); // EJ200PVT-2X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {6.42334},
            {0.31*0.1},
            {0.7368},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N11"},
            2,33,1.2,"EJ200PVT-2X1P double shot",1,NULL
        }
    ); // EJ200PVT-2X1P

    measurements.push_back(
        {
            false,
            {0.22,8.34,390,0.98,0.31},
            {4.4927,15.12669,13.10698,9.09121,6.43075},
            {0.22*0.1,8.34*0.008,390*0.008,0.98*0.1,0.31*0.1},
            {0.49493,0.44840,0.33495,0.94679,0.78206},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20170111","20161005","20181206","20181130"},
            {"2","4","9","13","15"},
            28,20,1.2,"EJ260PS-1X1P",1,NULL
        }
    ); // EJ260PS-1X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {4.0709},
            {0.31*0.1},
            {0.43864},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"8"},
            2,33,1.2,"EJ260PS-1X1P double shot",2,NULL
        }
    ); // EJ260PS-1X1P

    measurements.push_back(
        {
            false,
            {0.22,0.31,0.98},
            {4.49457,8.49707,5.90285},
            {0.22*0.1,0.31*0.1,0.98*0.1},
            {0.49303,0.87823,0.69744},
            {"GIF++","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026""20181206","20181130"},
            {"2","13","15"},
            1,34,1.2,"EJ260PS-1X2P single shot",2,NULL
        }
    ); // EJ260PS-1X2P
    measurements.push_back(
        {
            false,
            {0.31},
            {7.74553},
            {0.31*0.1},
            {0.95676},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"8"},
            2,33,1.2,"EJ260PS-1X2P double shot",2,NULL
        }
    ); // EJ260PS-1X2P

    measurements.push_back(
        {
            false,
            {0.22},
            {6.30911,},
            {0.22*0.1},
            {0.75009,},
            {"GIF++"},
            {"20181026"},
            {"2"},
            1,23,1.2,"EJ260PS-2X1P single shot",2,NULL
        }
    ); // EJ260PS-2X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {11.73975},
            {0.31*0.1},
            {1.62915},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"8"},
            2,33,1.2,"EJ260PS-2X1P double shot",2,NULL
        }
    ); // EJ260PS-2X1P

    measurements.push_back(
        {
            false,
            {8.34,0.22,390,0.98,0.31},
            {12.77335,3.67532,15.34901,7.36742,5.71187},
            {8.34*0.008,0.22*0.1,390*0.008,0.98*0.1,0.31*0.1},
            {0.32657,0.39381,0.47046,0.75655,0.67517},
            {"Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170111","20181026","20161005","20181206","20181130"},
            {"N4","N8","N9","N13","N15"},
            38,21,1.2,"EJ260PVT-1X1P",1,NULL
        }
    ); // EJ260PVT-1X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {6.20954},
            {0.31*0.1},
            {0.70026},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N3"},
            2,33,1.2,"EJ260PVT-1X1P double shot",1,NULL
        }
    ); // EJ260PVT-1X1P

    measurements.push_back(
        {
            false,
            {0.22,0.98,0.31},
            {3.40791,7.07664,4.73209},
            {0.22*0.1,0.98*0.1,0.31*0.1},
            {0.36172,0.72510,0.53236},
            {"GIF++","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20181206","20181130"},
            {"N8","N4","N5","N3"},
            1,34,1.2,"EJ260PVT-1X2P single shot",1,NULL
        }
    ); // EJ260PVT-1X2P
    measurements.push_back(
        {
            false,
            {0.31},
            {6.20954},
            {0.31*0.1},
            {0.70026},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N3"},
            2,33,1.2,"EJ260PVT-1X2P double shot",1,NULL
        }
    ); // EJ260PVT-1X2P

    measurements.push_back(
        {
            false,
            {0.22},
            {4.3876},
            {0.22*0.1},
            {0.48500},
            {"GIF++"},
            {"20181026"},
            {"N8"},
            1,23,1.2,"EJ260PVT-2X1P single shot",1,NULL
        }
    ); // EJ260PVT-2X1P
    measurements.push_back(
        {
            false,
            {0.31},
            {8.18087},
            {0.31*0.1},
            {0.98633},
            {"Gamma(GSFC REF)"},
            {"20181130"},
            {"N3"},
            2,33,1.2,"EJ260PVT-2X1P double shot",1,NULL
        }
    ); // EJ260PVT-2X1P

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

        vector<float> x_temp = i->x;
        vector<float> y_temp = i->y;
        vector<float> ex_temp = i->ex;
        vector<float> ey_temp = i->ey;
        if (useGray) {
          for(int i = 0; i<n; ++i) {
            x_temp[i]*=0.01*1000; // krad/hr to Gy/hr
            y_temp[i]*=0.01*1000*1000; // Mrad to Gy
            ex_temp[i]*=0.01*1000; // krad/hr to Gy/hr
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
    canv->SetLogx();
    //canv->SetLogx();

    // MultiGraph with all data points
    TMultiGraph *allgraphs = new TMultiGraph();
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i)
    if (i->include)
    allgraphs->Add(i->graph);

    allgraphs->Draw("AP");
    allgraphs->GetXaxis()->SetTitle("Dose rate (krad/hr)");
    allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose rate (Gy/hr)");
      allgraphs->GetYaxis()->SetTitle("Dose Constant (Gy)");
    }
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(2.0);
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

    TString filename = TString("EJ200A0");
    filename+=tagTime;

    canv->SaveAs("paperPlots/ver5/"+filename+".pdf");
}
