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
    vector<float> dose;
    vector<float> LYratio;
    short markerColor; // line color and marker fill color (if it applies) will use the same value
    short markerStyle;
    float markerSize;
    string legendLabel;
    short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
    TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void LYratio_vs_dose(TString tagTime="") {
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
    incMeas.push_back("EJ200PVT-1X1P");
    incMeas.push_back("EJ200PVT-1X1P Sept. 2020");
    incMeas.push_back("EJ200PVT-1X1P Dec. 2020");

    vector<measurement> measurements;

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
            {1.32,7,7,7,4.2,1.26,7,7,7,1.26,1.26},
            {0.7213,0.5491,0.5020,0.4732,0.5745,0.7984,0.5646,0.4430,0.6717,0.7381,0.7582},
            1,20,1.4,"EJ200PS-1X1P",1,NULL
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
            {1.32,7,7,7,4.2,1.26},
            {0.7161,0.5542,0.5396,0.5373,0.5662,0.7857},
            1,21,1.4,"EJ200PS-1X2P",1,NULL
        }
    ); // EJ200PS-1X2P

    measurements.push_back(
        {
            "EJ200PS-2X1P",
            {0.22,8.34,257,390},
            {4.21795,11.75156,8.81149,8.08626},
            {0.22*0.1,8.34*0.012,257*0.012,390*0.012},
            {0.5901,0.61944,0.35523,0.30444},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
            {"20181026","20170111","20200214","20161005"},
            {"2","5","8","9"},
            {1.32,7,7,7},
            {0.7252,0.5401,0.6827,0.4073},
            1,22,1.4,"EJ200PS-2X1P",1,NULL
        }
    ); // EJ200PS-2X1P

    measurements.push_back(
        {
            "EJ200PVT-1X1P",
            {74.4,8.53,0.22,390,0.31,0.98,8.19,370,0.31,0.31,0.3,293},
            {11.95324,10.63638,2.97755,14.31727,4.51566,6.3462,11.25147,13.08882,4.09073,3.99646,5.07549,11.54897},
            {74.4*0.012,8.53*0.012,0.22*0.1,390*0.012,0.31*0.1,0.98*0.1,8.19*0.012,370*0.1,0.31*0.1,0.31*0.1,0.3*0.1,293*0.012},
            {0.64446,0.51327,0.3625,0.91934,0.67266,0.69982,0.57177,0.76828,0.5779,0.5586,0.68708,0.69702},
            {"Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)"},
            {"20161202","20161109","20181026","20161004","20181130","20181206","20170301","20170302","20181130","20181130","20191204","20190222"},
            {"N4","N5","N8","N9","N15","N16","T1_2","T1_3","T1_5","T1_N1","AO-1_1","AO-1_2"},
            {7,7,1.32,7,1.26,4.2,7,7,1.26,1.26,1.71,6,7},
            {0.5436,0.5057,0.6313,0.6015,0.7494,0.5037,0.5255,0.5756,0.5608,0.7227,0.7038,0.5649},
            1,20,1.4,"EJ200PVT-1X1P",1,NULL
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
            {7,7,1.32,7,4.2,1.26},
            {0.5561,0.5469,0.6634,0.6206,0.5250,0.7515},
            1,21,1.4,"EJ200PVT-1X2P",1,NULL
        }
    ); // EJ200PVT-1X2P

    measurements.push_back(
        {
            "EJ200PVT-2X1P",
            {8.34,0.22,390},
            {12.44458,3.25535,15.35399},
            {8.34*0.056,0.22*0.1,390*0.056},
            {0.69757,0.40868,1.05356},
            {"Co-60 (NIST)","GIF++","Co-60 (NIST)"},
            {"20181130","20170111","20161005"},
            {"N5","N8","N9"},
            {7,1.32,7},
            {0.5588,0.6624,0.6270},
            1,22,1.4,"EJ200PVT-2X1P",1,NULL
        }
    ); // EJ200PVT-2X1P

    measurements.push_back(
        {
            "EJ260PS-1X1P",
            {0.22,8.34,390,0.98,0.31},
            {4.39471,14.356,11.71238,8.83387,6.37029},
            {0.22*0.1,8.34*0.012,390*0.012,0.98*0.1,0.31*0.1},
            {0.62184,0.89986,0.60383,1.0444,1.15793},
            {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20170111","20161005","20181206","20181130"},
            {"2","4","9","13","15"},
            {1.32,7,7,4.2,1.26},
            {0.7280,0.5926,0.5240,0.5999,0.8096},
            1,20,1.4,"EJ260PS-1X1P",1,NULL
        }
    ); // EJ260PS-1X1P

    measurements.push_back(
        {
            "EJ260PS-1X2P",
            {0.22,0.98,0.31},
            {4.29314,8.17298,5.68346},
            {0.22*0.1,0.98*0.1,0.31*0.1},
            {0.59891,0.94612,0.95488},
            {"GIF++","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20181026","20181130","20181206"},
            {"2","13","15"},
            {1.32,4.2,1.26},
            {0.7281,0.5731,0.7913},
            1,21,1.4,"EJ260PS-1X2P",1,NULL
        }
    ); // EJ260PS-1X2P

    measurements.push_back(
        {
            "EJ260PS-2X1P",
            {0.22},
            {5.71858},
            {0.22*0.1},
            {0.93899},
            {"GIF++"},
            {"20181026"},
            {"2"},
            {1.32},
            {0.7795},
            1,22,1.4,"EJ260PS-2X1P",1,NULL
        }
    ); // EJ260PS-2X1P

    measurements.push_back(
        {
            "EJ260PVT-1X1P",
            {8.34,0.22,390,0.98,0.31},
            {10.97039,3.37189,12.25216,6.87897,5.13571},
            {8.34*0.012,0.22*0.1,390*0.012,0.98*0.1,0.31*0.1},
            {0.53598,0.42632,0.66438,0.76745,0.81454},
            {"Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)"},
            {"20170111","20181026","20161005","20181206","20181130"},
            {"N4","N8","N9","N13","N15"},
            {7,1.32,7,4.2,1.26},
            {0.5091,0.6618,0.5469,0.5205,0.8922},
            1,20,1.4,"EJ260PVT-1X1P",1,NULL
        }
    ); // EJ260PVT-1X1P

    measurements.push_back(
        {
            "EJ260PVT-1X2P",
            {0.98,0.31,0.22},
            {6.74271,4.43098,3.19901},
            {0.98*0.1,0.31*0.1,0.22*0.1},
            {0.74944,0.64698,0.39684},
            {"Gamma(GSFC REF)","Gamma(GSFC REF)","GIF++"},
            {"20181206","20181130","20181026"},
            {"N4","N5","N8"},
            {4.2,1.26,1.32},
            {0.5163,0.7460,0.6517},
            1,21,1.4,"EJ260PVT-1X2P",1,NULL
        }
    ); // EJ260PVT-1X2P

    measurements.push_back(
        {
            "EJ260PVT-2X1P",
            {0.22},
            {3.97648},
            {0.22*0.1},
            {0.53738},
            {"GIF++"},
            {"20181026"},
            {"N8"},
            {1.32},
            {0.7092},
            1,22,1.4,"EJ260PVT-2X1P",1,NULL
        }
    ); // EJ260PVT-2X1P

////////////////////////////////////////////////////////////////////////////////

    measurements.push_back(
        {
            "EJ200PS-1X1P Dec. 2020",
            {46,46,46},
            {4.2035,6.42007,18.52005},
            {46*0.012,46*0.012,46*0.012},
            {0.23358,0.28804,1.53559},
            {"","",""},
            {"","",""},
            {"L2R","L3R","L4R"},
            {2.4,4.6,7.0},
            {0.5568,0.4781,0.6800},
            3,20,1.4,"EJ200PS-1X1P Dec. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PS-2X1P Sept. 2020",
            {47},
            {7.65359},
            {47*0.012},
            {0.39272},
            {""},
            {""},
            {"4"},
            {4.7},
            {0.5403},
            2,22,1.4,"EJ200PS-2X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-1X1P Sept. 2020",
            {238},
            {13.67347},
            {238*0.012},
            {0.83788},
            {""},
            {""},
            {"N2"},
            {7.0},
            {0.6017},
            2,20,1.4,"EJ200PVT-1X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-1X1P Dec. 2020",
            {46,46,46},
            {7.70338,9.59388,12.53412},
            {46*0.012,46*0.012,46*0.012},
            {0.77443,0.63328,0.71385},
            {"","",""},
            {"","",""},
            {"L2R","L3R","L4R"},
            {2.4,4.6,7.0},
            {0.7301,0.6146,0.5647},
            3,20,1.4,"EJ200PVT-1X1P Dec. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ200PVT-2X1P Sept. 2020",
            {47},
            {10.78274},
            {47*0.012},
            {0.77229},
            {""},
            {""},
            {"N2"},
            {4.7},
            {0.6525},
            2,22,1.4,"EJ200PVT-2X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PS-1X1P Dec. 2020",
            {46,46,46},
            {6.64756,9.24508,11.50421},
            {46*0.012,46*0.012,46*0.012},
            {0.56713,0.57709,0.593},
            {"","",""},
            {"","",""},
            {"L2R","L3R","L4R"},
            {2.4,4.6,7.0},
            {0.6847,0.5918,0.5241},
            3,20,1.4,"EJ260PS-1X1P Dec. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PS-1X1P Sept. 2020",
            {47},
            {10.00223},
            {47*0.012},
            {0.6492},
            {""},
            {""},
            {"10"},
            {4.7},
            {0.6243},
            2,20,1.4,"EJ260PS-2X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PS-1X2P Sept. 2020",
            {47,238},
            {8.72834,11.17272},
            {47*0.012,238*0.012},
            {0.49661,0.55027},
            {"",""},
            {"",""},
            {"11","9"},
            {4.7,7.0},
            {0.5253,0.5780},
            2,21,1.4,"EJ260PS-2X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PS-2X1P Sept. 2020",
            {47,238},
            {12.29738,14.32119},
            {47*0.012,238*0.012},
            {0.98074,0.89592},
            {"",""},
            {"",""},
            {"9","4"},
            {4.7,7.0},
            {0.6078,0.6809},
            2,22,1.4,"EJ260PS-2X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PVT-1X1P Dec. 2020",
            {46,46,46},
            {7.55433,9.63463,11.96351},
            {46*0.012,46*0.012,46*0.012},
            {0.74142,0.63449,0.64772},
            {"","",""},
            {"","",""},
            {"L2R","L3R","L4R"},
            {2.4,4.6,7.0},
            {0.7214,0.6091,0.5454},
            3,20,1.4,"EJ260PVT-1X1P Dec. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PVT-1X1P Sept. 2020",
            {47},
            {10.39821},
            {47*0.012},
            {0.70655},
            {""},
            {""},
            {"N11"},
            {4.7},
            {0.6387},
            2,20,1.4,"EJ260PVT-1X1P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PVT-1X2P Sept. 2020",
            {47,238},
            {9.25801,12.4521},
            {47*0.012,238*0.012},
            {0.56225,0.686},
            {"",""},
            {"",""},
            {"N16","N11"},
            {4.7,7.0},
            {0.5644,0.6013},
            2,21,1.4,"EJ260PVT-1X2P Sept. 2020",1,NULL
        }
    );

    measurements.push_back(
        {
            "EJ260PVT-2X1P Sept. 2020",
            {47,238},
            {11.77551,16.031},
            {47*0.012,238*0.012},
            {0.89917,1.1219},
            {"",""},
            {"",""},
            {"N5","N10"},
            {4.7,7.0},
            {0.6743,0.6444},
            2,22,1.4,"EJ260PVT-2X1P Sept. 2020",1,NULL
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

        vector<float> x_temp = i->x;
        vector<float> y_temp = i->y;
        vector<float> ex_temp = i->ex;
        vector<float> ey_temp = i->ey;
        vector<float> dose_temp = i->dose;
        vector<float> LYratio_temp = i->LYratio;
        if (useGray) {
          for(int i = 0; i<n; ++i) {
            x_temp[i]*=0.01*1000; // krad/hr to Gy/hr
            y_temp[i]*=0.01*1000*1000; // Mrad to Gy
            ex_temp[i]*=0.01*1000; // krad/hr to Gy/hr
            ey_temp[i]*=0.01*1000*1000; // Mrad to Gy
            dose_temp[i]*=0.01*1000*1000; // Mrad to Gy
          }
        }

        vector<float> edose_temp;
        vector<float> logLYratio_temp;
        vector<float> elogLYratio_temp;
        for(int i = 0; i < n; ++i) {
          edose_temp.push_back(dose_temp[i]*0.1);
          logLYratio_temp.push_back(-log(LYratio_temp[i]));
          elogLYratio_temp.push_back(0.1*sqrt(2));
        }

        TGraphErrors *graph = new TGraphErrors(n,dose_temp.data(),logLYratio_temp.data(),edose_temp.data(),elogLYratio_temp.data());
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
    TCanvas *canv = new TCanvas("canv","LY ratio versus dose",200,10,800,600);
    canv->SetFillColor(0);
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

    allgraphs->Draw("AP");
    allgraphs->GetXaxis()->SetTitle("Dose (Mrad)");
    allgraphs->GetYaxis()->SetTitle("-log(LY ratio)");
    if (useGray) {
      allgraphs->GetXaxis()->SetTitle("Dose (Gy)");
    }
    allgraphs->GetYaxis()->SetTitleOffset(1.2);
    allgraphs->GetXaxis()->SetTitleOffset(1.3);

    // NEED TO TUNE THESE:
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(1.0);
    double xmin = 0.0;
    double xmax = 10.0;
    if (useGray) {
      xmin *= 0.01*1000*1000;
      xmax *= 0.01*1000*1000;
    }
    allgraphs->GetXaxis()->SetLimits(xmin,xmax);

    // Prepare the legend
    TLegend *legendA = new TLegend(0.4,0.2,0.75,0.32);
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

    canv->SaveAs("paperPlots/ver12/LYratio"+filename+".pdf");
}
