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
  short markerColor; // line color and marker fill color (if it applies) will use the same value
  short markerStyle;
  float markerSize;
  string legendLabel;
  short legendId; // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
  TGraphErrors* graph; // this pointer gets filled in drawing section; it is initially null
};

void dcost_vs_ddot_merged(TString tagTime="") {
  set_root_style();
  Bool_t useGray = true;

  // ======================================================================================================
  // Begin of results section
  // ======================================================================================================

  // This is where people decide which measurements shall be included in the plot
  // and add new measurements to the list of plottable results.
  // Please make sure that there is a sensible comment identifying each new result.
  // Ideally, this is the ONLY section that needs to be updated whenever a new measurement is available.

  vector<TString> incMeas;
  incMeas.push_back("EJ200PS-1X1P");
  //incMeas.push_back("EJ200PVT-1X2P");
  //incMeas.push_back("EJ200PVT-2X1P");

  vector<measurement> measurements;

  measurements.push_back(
    {
      "EJ200PS-1X1P",
      {0.22,8.53,74.4,390,0.98,0.31,8.06,364,338,0.31,0.31,46,46,13,13},
      {4.2147,12.24314,10.69303,9.88592,7.96139,5.69919,12.65279,9.08617,8.92548,4.28092,4.60692,18.52005,8.16197,7.05107,7.16283},
      {0.22*0.1,8.53*0.012,74.4*0.012,390*0.012,0.98*0.1,0.31*0.1,8.06*0.012,364*0.012,338*0.012,0.31*0.1,0.31*0.1,46*0.012,46*0.05,13*0.05,13*0.05},
      {0.58855,0.66937,0.51457,0.44249,0.91988,0.97144,0.71625,0.37693,0.36422,0.61784,0.69133,1.53559,0.50368,0.41771,0.42628},
      {"GIF++","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","NIST","Sandia","NIST","NIST"},
      {"20181026","20161109","20161202","20161004","20181206","20181130","20170412","20170413","20171107","20181130","20181130","20201220","20210616","20210924","20210924"},
      {"4","5","8","9","13","15","T1_2","T1_3","T1_4","T1_5","T1_N1","L4R","L11R","L7R","L8R"},
      {1.32,7,7,7,4.2,1.26,7,7,7,1.26,1.26,7,7,6.9,6.9},
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
      {1.32,7,7,7,4.2,1.26},
      2,33,1.4,"EJ200PS-1X2P",1,NULL
    }
  ); // EJ200PS-1X2P

  measurements.push_back(
    {
      "EJ200PS-2X1P",
      {0.22,47,8.34,257,390},
      {4.21795,7.65359,11.75156,8.81149,8.08626},
      {0.22*0.1,47*0.012,8.34*0.012,257*0.012,390*0.012},
      {0.5901,0.39272,0.61944,0.35523,0.30444},
      {"GIF++","NIST","Co-60 (NIST)","Co-60 (NIST)","Co-60 (NIST)"},
      {"20181026","20200915","20170111","20200214","20161005"},
      {"2","4","5","8","9"},
      {1.32,4.7,7,7,7},
      3,22,1.4,"EJ200PS-2X1P",1,NULL
    }
  ); // EJ200PS-2X1P

  measurements.push_back(
    {
      "EJ200PVT-1X1P",
      {238,74.4,8.53,0.22,390,0.31,0.98,8.19,370,0.31,0.31,0.3,293,46},
      {13.67347,11.95324,10.63638,2.97755,14.31727,4.51566,6.3462,11.25147,13.08882,4.09073,3.99646,5.07549,11.54897,12.53412},
      {238*0.012,74.4*0.012,8.53*0.012,0.22*0.1,390*0.012,0.31*0.1,0.98*0.1,8.19*0.012,370*0.1,0.31*0.1,0.31*0.1,0.3*0.1,293*0.012,46*0.012},
      {0.83788,0.64446,0.51327,0.3625,0.91934,0.67266,0.69982,0.57177,0.76828,0.5779,0.5586,0.68708,0.69702,0.71385},
      {"Co-60 (NIST 2020)","Co-60 (NIST)","Co-60 (NIST)","GIF++","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","Co-60 (NIST)","Gamma(GSFC REF)","Gamma(GSFC REF)","Gamma(GSFC REF)","Co-60 (NIST)","NIST"},
      {"20200917","20161202","20161109","20181026","20161004","20181130","20181206","20170301","20170302","20181130","20181130","20191204","20190222","20201220"},
      {"N2","N4","N5","N8","N9","N15","N16","T1_2","T1_3","T1_5","T1_N1","AO-1_1","AO-1_2","AO-1_3","L4R"},
      {7,7,7,1.32,7,1.26,4.2,7,7,1.26,1.71,6,7,7},
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
      {7,7,1.32,7,4.2,1.26},
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
      {"NIST","Co-60 (NIST)","GIF++","Co-60 (NIST)"},
      {"20200915","20181130","20170111","20161005"},
      {"N2","N5","N8","N9"},
      {4.7,7,1.32,7},
      46,23,1.4,"EJ200PVT-2X1P",1,NULL
    }
  ); // EJ200PVT-2X1P

  measurements.push_back(
    {
      "EJ260PS-1X1P",
      {0.22,8.34,390,47,0.98,0.31,46,46,46},
      {4.39471,14.356,11.71238,10.00223,8.83387,6.37029,6.64756,9.24508,11.50421},
      {0.22*0.1,8.34*0.012,390*0.012,47*0.012,0.98*0.1,0.31*0.1,46*0.012,46*0.012,46*0.012},
      {0.62184,0.89986,0.60383,0.6492,1.0444,1.15793,0.56713,0.57709,0.593},
      {"GIF++","Co-60 (NIST)","Co-60 (NIST)","NIST","Gamma(GSFC REF)","Gamma(GSFC REF)","NIST","NIST","NIST"},
      {"20181026","20170111","20161005","20200915","20181206","20181130","20201220","20201220","20201220"},
      {"2","4","9","10","13","15","L2R","L3R","L4R"},
      {1.32,7,7,4.7,4.2,1.26,2.4,4.6,7},
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
      {"GIF++","NIST","NIST","Gamma(GSFC REF)","Gamma(GSFC REF)"},
      {"20181026","20200917","20200915","20181130","20181206"},
      {"2","9","11","13","15"},
      {1.32,7,4.7,4.2,1.26},
      1,21,1.4,"EJ260PS-1X2P",1,NULL
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
      {1.32,7,4.7},
      1,22,1.4,"EJ260PS-2X1P",1,NULL
    }
  ); // EJ260PS-2X1P

  measurements.push_back(
    {
      "EJ260PVT-1X1P",
      {8.34,0.22,390,47,0.98,0.31,46,46,46},
      {10.97039,3.37189,12.25216,10.39821,6.87897,5.13571,7.55433,9.63463,11.96351},
      {8.34*0.012,0.22*0.1,390*0.012,47*0.012,0.98*0.1,0.31*0.1,46*0.012,46*0.012,46*0.012},
      {0.53598,0.42632,0.66438,0.70655,0.76745,0.81454,0.74142,0.63449,0.64772},
      {"Co-60 (NIST)","GIF++","Co-60 (NIST)","Co-60 (NIST 2020)","Gamma(GSFC REF)","Gamma(GSFC REF)","NIST","NIST","NIST"},
      {"20170111","20181026","20161005","20200915","20181206","20181130","20201220","20201220","20201220"},
      {"N4","N8","N9","N11","N13","N15","L2R","L3R","L4R"},
      {7,1.32,7,4.7,4.2,1.26,2.4,4.6,7},
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
      {"Gamma(GSFC REF)","Gamma(GSFC REF)","GIF++","NIST","NIST"},
      {"20181206","20181130","20181026","20200917","20200915"},
      {"N4","N5","N8","N11","N16"},
      {4.2,1.26,1.32,7,4.7},
      1,21,1.4,"EJ260PVT-1X2P",1,NULL
    }
  ); // EJ260PVT-1X2P

  measurements.push_back(
    {
      "EJ260PVT-2X1P",
      {47,0.22,238},
      {11.77551,3.97648,16.031},
      {47*0.012,0.22*0.1,238*0.012},
      {0.89917,0.53738,1.1219},
      {"Co-60 (NIST 2020)","GIF++","NIST"},
      {"20200915","20181026","20200917"},
      {"N5","N8","N10"},
      {4.7,1.32,7},
      1,22,1.4,"EJ260PVT-2X1P",1,NULL
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

    vector<float> x_temp;
    vector<float> y_temp;
    vector<float> ex_temp;
    vector<float> ey_temp;
    // Remove entries not at 7 Mrad and > 40 krad/hr
    for (int j = 0; j < n; ++j) {
      if ((i->x[j] > 40 && abs(i->dose[j]-7) < 0.1) || (i->x[j] < 40)) {
        x_temp.push_back(i->x[j]);
        y_temp.push_back(i->y[j]);
        ex_temp.push_back(i->ex[j]);
        ey_temp.push_back(i->ey[j]);
      }
    }

    if (useGray) {
      for(int j = 0; j < x_temp.size(); ++j) {
        x_temp[j]*=0.01*1000; // krad/hr to Gy/hr
        y_temp[j]*=0.01*1000; // Mrad to kGy
        ex_temp[j]*=0.01*1000; // krad/hr to Gy/hr
        ey_temp[j]*=0.01*1000; // Mrad to kGy
      }
    }


    TGraphErrors *graph = new TGraphErrors(x_temp.size(),x_temp.data(),y_temp.data(),ex_temp.data(),ey_temp.data());
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
      if ((*it).CompareTo(i->name) == 0) {
        allgraphs->Add(i->graph);
      }
    }
  }

  allgraphs->Draw("AP");
  allgraphs->GetXaxis()->SetTitle("Dose rate (krad/h)");
  allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
  if (useGray) {
    allgraphs->GetXaxis()->SetTitle("Dose rate (Gy/h)");
    allgraphs->GetYaxis()->SetTitle("Dose Constant (kGy)");
  }
  allgraphs->GetYaxis()->SetTitleOffset(1.2);
  allgraphs->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  allgraphs->SetMinimum(0.0);
  allgraphs->SetMaximum(16.0);
  double xmin = 5.e-2;
  double xmax = 2.e3;
  if (useGray) {
    allgraphs->SetMinimum(0.0*0.01*1000);
    allgraphs->SetMaximum(20.0*0.01*1000);
    xmin = 5.e-1;
    xmax = 2.e4;
  }
  allgraphs->GetXaxis()->SetLimits(xmin,xmax);

  // Text next to markers
  TLatex txt;
  txt.SetTextSize(0.033);
  float offset = 5;
  for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
    for (vector<TString>::iterator it = incMeas.begin(); it!=incMeas.end();++it){
      if ((*it).CompareTo(i->name) == 0) {
        unsigned int n = i->x.size();
        for (unsigned int j = 0; j < n; j++) {
          if (abs(i->dose[j]-7) > 0.1 && i->x[j] < 40) {
            float xtxt = useGray ? i->x[j]*=0.01*1000 : i->x[j];
            float ytxt = useGray ? i->y[j]*=0.01*1000 : i->y[j];
            char desc[20];
            sprintf(desc, "%.1f kGy", i->dose[j]*0.01*1000);
            txt.DrawLatex(xtxt*1.2, ytxt+offset, desc);
            //offset*=-1;
          }
        }
      }
    }
  }

  // Prepare the legend
  TLegend *legendA = new TLegend(0.45,0.2,0.75,0.3);
  legendA->SetBorderSize(0);
  legendA->SetFillColorAlpha(0,0);
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

  canv->SaveAs("paperPlots/ver15/"+filename+".pdf");
}
