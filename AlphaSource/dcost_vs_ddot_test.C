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

void dcost_vs_ddot_test(TString tagTime="") {
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
    //incMeas.push_back("EJ200AO-0");
    //incMeas.push_back("EJ200AO-1");
    //incMeas.push_back("EJ200AO-2");
    //incMeas.push_back("EJ200PS-1X1P");
    //incMeas.push_back("EJ200PS-1X2P");
    incMeas.push_back("EJ200PS-2X1P");
    //incMeas.push_back("EJ200PVT-1X1P");
    //incMeas.push_back("EJ200PVT-1X2P");
    //incMeas.push_back("EJ200PVT-2X1P");
    //incMeas.push_back("EJ260PS-1X1P");
    //incMeas.push_back("EJ260PS-1X2P");
    //incMeas.push_back("EJ260PS-2X1P");
    //incMeas.push_back("EJ260PVT-1X1P");
    //incMeas.push_back("EJ260PVT-1X2P");
    //incMeas.push_back("EJ260PVT-2X1P");

    vector<measurement> measurements;
    measurements.push_back(
        {
            "EJ200AO-0",
            {0.3,293},
            {3.99677,12.5335},
            {0.3*0.1,293*0.012},
            {0.49194,0.81812},
            {"Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20191204","20190222"},
            {"1","2"},
            41,43,1.4,"EJ200AO-0",1,NULL
        }
    ); // EJ200A0-0-1X1P

    measurements.push_back(
        {
            "EJ200AO-1",
            {0.3,238,293},
            {5.07549,13.20887,11.54897},
            {0.3*0.1,238*0.012,293*0.012},
            {0.68708,0.78181,0.69702},
            {"Gamma(GSFC REF)","Co-60 (NIST 2020)","Co-60 (NIST)"},
            {"20191204","20200917","20190222"},
            {"1","3","2"},
            30,45,1.4,"EJ200AO-1",1,NULL
        }
    ); // EJ200A0-1-1X1P

    measurements.push_back(
        {
            "EJ200AO-2",
            {0.3,293},
            {5.3729,12.52272},
            {0.3*0.1,293*0.012},
            {0.7479,0.81664},
            {"Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20191204","20190222"},
            {"1","2"},
            40,47,1.4,"EJ200AO-2",1,NULL
        }
    ); // EJ200A0-2-1X1P

    measurements.push_back(
        {
            "EJ200PS-1X1P",
            {0.22,8.53,74.4,390,0.98,0.31,8.06,364,338,0.31,0.31},
            {4.2147,12.24314,10.69303,9.88592,7.96139,5.69919,12.65279,9.08617,8.92548,4.28092,4.60692},
            {0.22*0.1,8.53*0.012,74.4*0.012,390*0.012,0.98*0.1,0.31*0.1,8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1},
            {0.58855,0.66937,0.51457,0.44249,0.91988,0.97144,0.71625,0.37693,0.36422,0.61784,0.69133},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20161109","20161202","20161004","20181206","20181130","20170412","20170413","20171107","20181130","20181130"},
            {"4","5","8","9","13","15","T1_2","T1_3","T1_4","T1_5","T1_N1"},
            1,34,1.4,"EJ200PS-1X1P",1,NULL
        }
    ); // EJ200PS-1X1P

    measurements.push_back(
        {
            "EJ200PS-1X2P",
            {0.22,8.53,74.4,390,0.98,0.31},
            {3.98239,12.3079,11.85038,11.74886,7.78896,5.49065},
            {0.22*0.1,8.53*0.012,74.4*0.012,390*0.012,0.98*0.1,0.31*0.1},
            {0.54185,0.67902,0.62942,0.61939,0.89641,0.91445},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20161109","20161202","20161004","20181206","20181130"},
            {"2","5","8","9","13","15"},
            2,33,1.4,"EJ200PS-1X2P",1,NULL
        }
    ); // EJ200PS-1X2P

    measurements.push_back(
        {
            "EJ200PS-2X1P",
            {0.22,8.34,47,47*1.115,257,390},
            {4.21795,11.75156,7.65359,8.53376,8.81149,8.08626},
            {0.22*0.1,8.34*0.012,47*0.012,47*1.115*0.012,257*0.012,390*0.012},
            {0.5901,0.61944,0.39272,0.43788,0.35523,0.30444},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST 2020)","Co-60 (NIST)","Co-60 (NIST)"},
            {"20181026","20170111","20200915","20200214","20161005"},
            {"2","5","4","8","9"},
            3,22,1.4,"EJ200PS-2X1P",1,NULL
        }
    ); // EJ200PS-2X1P

    measurements.push_back(
        {
            "EJ200PVT-1X1P",
            {238,74.4,8.53,0.22,390,0.31,0.98,8.19,370,0.31,0.31,0.3,293,238},
            {13.67347,11.95324,10.63638,2.97755,14.31727,4.51566,6.3462,11.25147,13.08882,4.09073,3.99646,5.07549,11.54897,13.20887},
            {238*0.012,74.4*0.012,8.53*0.012,0.22*0.1,390*0.012,0.31*0.1,0.98*0.1,8.19*0.012,370*0.1,0.31*0.1,0.31*0.1,0.3*0.1,293*0.012,238*0.012},
            {0.83788,0.64446,0.51327,0.3625,0.91934,0.67266,0.69982,0.57177,0.76828,0.5779,0.5586,0.68708,0.69702,0.78181},
            {"Co-60 (NIST 2020)","Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST 2020)"},
            {"20200917","20161202","20161109","20181026","20161004","20181130","20181206","20170301","20170302","20181130","20181130","20191204","20190222","20200917"},
            {"N2","N4","N5","N8","N9","N15","N16","T1_2","T1_3","T1_5","T1_N1","AO-1_1","AO-1_2","AO-1_3"},
            4,33,1.4,"EJ200PVT-1X1P",1,NULL
        }
    ); // EJ200PVT-1X1P

    measurements.push_back(
        {
            "EJ200PVT-1X2P",
            {74.4,8.53,0.22,390,0.98,0.31},
            {12.33893,12.05441,3.29719,15.0902,6.72615,4.50768},
            {74.4*0.012,8.53*0.012,0.22*0.1,390*0.012,0.98*0.1,0.31*0.1},
            {0.68563,0.65506,0.41599,1.01823,0.74986,0.67068},
            {"Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20161202","20161109","20181026","20181004","20181206","20181130"},
            {"N4","N5","N8","N9","N13","N15"},
            8,22,1.4,"EJ200PVT-1X2P",1,NULL
        }
    ); // EJ200PVT-1X2P

    measurements.push_back(
        {
            "EJ200PVT-2X1P",
            {47,8.34,0.22,390},
            {10.78274,12.44458,3.25535,15.35399},
            {47*0.012,8.34*0.056,0.22*0.1,390*0.056},
            {0.77229,0.69757,0.40868,1.05356},
            {"Co-60 (NIST 2020)","Co-60 (NIST)","GIF++","Co-60 (NIST)"},
            {"20200915","20181130","20170111","20161005"},
            {"N2","N5","N8","N9"},
            46,23,1.4,"EJ200PVT-2X1P",1,NULL
        }
    ); // EJ200PVT-2X1P

    measurements.push_back(
        {
            "EJ260PS-1X1P",
            {0.22,8.34,390,47,0.98,0.31},
            {4.39471,14.356,11.71238,10.00223,8.83387,6.37029},
            {0.22*0.1,8.34*0.012,390*0.012,47*0.012,0.98*0.1,0.31*0.1},
            {0.62184,0.89986,0.60383,0.6492,1.0444,1.15793},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST 2020)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20170111","20161005","20200915","20181206","20181130"},
            {"2","4","9","10","13","15"},
            28,20,1.4,"EJ260PS-1X1P",1,NULL
        }
    ); // EJ260PS-1X1P

    measurements.push_back(
        {
            "EJ260PS-1X2P",
            {0.22,238,47,0.98,0.31},
            {4.29314,11.17272,8.72834,8.17298,5.68346},
            {0.22*0.1,238*0.012,47*0.012,0.98*0.1,0.31*0.1},
            {0.59891,0.55027,0.49661,0.94612,0.95488},
            {"GIF++","Co-60 (NIST 2020)","Co-60 (NIST 2020)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20200917","20200915","20181130","20181206"},
            {"2","9","11","13","15"},
            2,34,1.4,"EJ260PS-1X2P",1,NULL
        }
    ); // EJ260PS-1X2P

    measurements.push_back(
        {
            "EJ260PS-2X1P",
            {0.22,238,47},
            {5.71858,14.32119,12.29738},
            {0.22*0.1,238*0.012,47*0.012},
            {0.93899,0.89592,0.98074},
            {"GIF++","Co-60 (NIST 2020)","Co-60 (NIST 2020)"},
            {"20181026","20200917","20200915"},
            {"2","4","9"},
            3,23,1.4,"EJ260PS-2X1P",1,NULL
        }
    ); // EJ260PS-2X1P

    measurements.push_back(
        {
            "EJ260PVT-1X1P",
            {8.34,0.22,390,47,0.98,0.31},
            {10.97039,3.37189,12.25216,10.39821,6.87897,5.13571},
            {8.34*0.012,0.22*0.1,390*0.012,47*0.012,0.98*0.1,0.31*0.1},
            {0.53598,0.42632,0.66438,0.70655,0.76745,0.81454},
            {"Co-60 (NIST)","GIF++","Co-60 (NIST)","Co-60 (NIST 2020)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170111","20181026","20161005","20200915","20181206","20181130"},
            {"N4","N8","N9","N11","N13","N15"},
            38,21,1.4,"EJ260PVT-1X1P",1,NULL
        }
    ); // EJ260PVT-1X1P

    measurements.push_back(
        {
            "EJ260PVT-1X2P",
            {0.98,0.31,0.22,238,47},
            {6.74271,4.43098,3.19901,12.4521,9.25801},
            {0.98*0.1,0.31*0.1,0.22*0.1,238*0.012,47*0.012},
            {0.74944,0.64698,0.39684,0.686,0.56225},
            {"Gamma(GSFC REF)","Gamma(GSFC REF)","GIF++","Co-60 (NIST 2020)","Co-60 (NIST 2020)"},
            {"20181206","20181130","20181026","20200917","20200915"},
            {"N4","N5","N8","N11","N16"},
            2,34,1.4,"EJ260PVT-1X2P",1,NULL
        }
    ); // EJ260PVT-1X2P

    measurements.push_back(
        {
            "EJ260PVT-2X1P",
            {47,0.22,238},
            {11.77551,3.97648,16.031},
            {47*0.012,0.22*0.1,238*0.012},
            {0.89917,0.53738,1.1219},
            {"Co-60 (NIST 2020)","GIF++","Co-60 (NIST 2020)"},
            {"20200915","20181026","20200917"},
            {"N5","N8","N10"},
            3,23,1.4,"EJ260PVT-2X1P",1,NULL
        }
    ); // EJ260PVT-2X1P

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
    for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
        for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
            if ((*it).CompareTo(i->include) == 0) {
                allgraphs->Add(i->graph);
            }
        }
    }

    allgraphs->Draw("AP");
    allgraphs->GetXaxis()->SetTitle("Dose rate (krad/h)");
    allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose rate (Gy/h)");
      allgraphs->GetYaxis()->SetTitle("Dose Constant (Gy)");
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

    canv->SaveAs("paperPlots/ver10/test_"+filename+".pdf");
}
