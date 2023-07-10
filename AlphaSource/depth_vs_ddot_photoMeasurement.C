#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TString.h>
#include <TF1.h>
#include <iostream>
#include <vector>
#include <string>
#include "RootStyle.cc"

using namespace std;

struct measurement
{
  TString name;
  vector<float> ddot;
  vector<float> depth;
  vector<float> eddot_rel;
  vector<float> edepth;
  vector<TString> rodNumber;
  vector<float> dose;
  vector<TString> irrEndDate;
  vector<TString> irrFacility;
  short markerColor; // line color and marker fill color (if it applies) will use the same value
  short markerStyle;
  float markerSize;
  string legendLabel;
  short legendId;      // which legend does this go? 1 or 2 (there is also the PVT vs PS legend)
  TGraphErrors *graph; // this pointer gets filled in drawing section; it is initially null
};

void depth_vs_ddot_photoMeasurement(TString tagTime = "")
{
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
  incMeas.push_back("EJ200PS-1X1P photos");
  incMeas.push_back("EJ200PS-1X1P photos alt boundary");

  vector<measurement> measurements;

  measurements.push_back(
      {"EJ200PS-1X1P",
       {0.22, 8.53, 74.4, 390, 0.98, 0.31, 8.06, 364, 338, 46, 13, 13, 46, 46},
       {5.05, 3.0, 0.87, 0.4, 5.05, 5.05, 2.55, 0.18, 0.36, 0.88, 2.25, 2.15, 1.25, 1.25},
       {0.1, 0.012, 0.012, 0.012, 0.1, 0.1, 0.012, 0.012, 0.012, 0.05, 0.05, 0.05, 0.05, 0.05},
       {0.2, 0.1, 0.17, 0.1, 0.2, 0.2, 0.15, 0.07, 0.05, 0.17, 0.15, 0.15, 0.05, 0.05},
       {"4", "5", "8", "9", "13", "15", "T1_2", "T1_3", "T1_4", "L11R", "L7R", "L8R", "L9R", "L10R"},
       {1.32, 7, 7, 7, 4.2, 1.26, 7, 7, 7, 7, 6.9, 6.9, 6.9, 6.9},
       {"GIF++", "NIST", "NIST", "NIST", "GSFC REF", "GSFC REF", "NIST", "NIST", "NIST", "NIST", "Sandia", "NIST", "NIST", "NIST", "NIST"},
       {"20181026", "20161109", "20161202", "20161004", "20181206", "20181130", "20170412", "20170413", "20171107", "20210616", "20210924", "20210924", "20211014", "20211014"},
       1,
       34,
       1.4,
       "EJ200PS-1X1P old",
       1,
       NULL}); // EJ200PS-1X1P

  measurements.push_back(
      {"EJ200PS-1X1P photos",
       {0.22, 8.53, 74.4, 390, 0.98, 0.31, 8.06, 364, 338, 13, 13, 46, 46, 46},
       {5.05, 2.61, 0.82, 0.41, 5.05, 5.05, 2.67, 0.35, 0.32, 1.87, 1.76, 1.15, 1.06, 0.83},
       {0.1, 0.012, 0.012, 0.012, 0.1, 0.1, 0.012, 0.012, 0.012, 0.05, 0.05, 0.05, 0.05, 0.05},
       {0.2, 0.03, 0.04, 0.02, 0.2, 0.2, 0.04, 0.02, 0.01, 0.04, 0.09, 0.04, 0.04, 0.06},
       {"4", "5", "8", "9", "13", "15", "T1_2", "T1_3", "T1_4", "L7R", "L8R", "L9R", "L10R", "L11R"},
       {1.32, 7, 7, 7, 4.2, 1.26, 7, 7, 7, 6.9, 6.9, 6.9, 6.9, 7},
       {"GIF++", "NIST", "NIST", "NIST", "GSFC REF", "GSFC REF", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST"},
       {"20181026", "20161109", "20161202", "20161004", "20181206", "20181130", "20170412", "20170413", "20171107", "20210924", "20210924", "20211014", "20211014", "20210616"},
       2,
       34,
       1.4,
       "EJ200PS-1X1P photos",
       1,
       NULL}); // EJ200PS-1X1P

  measurements.push_back(
      {"EJ200PS-1X1P photos alt boundary",
       {8.53, 74.4, 390, 8.06, 364, 338, 13, 13, 46, 46, 46},
       {2.65, 0.79, 0.43, 1.75, 0.35, 0.33, 1.79, 1.70, 0.75, 0.77, 0.55},
       {0.012, 0.012, 0.012, 0.012, 0.012, 0.012, 0.05, 0.05, 0.05, 0.05, 0.05},
       {0.04, 0.05, 0.02, 0.07, 0.02, 0.02, 0.05, 0.06, 0.06, 0.04, 0.04},
       {"5", "8", "9", "T1_2", "T1_3", "T1_4", "L7R", "L8R", "L9R", "L10R", "L11R"},
       {7, 7, 7, 7, 7, 7, 6.9, 6.9, 6.9, 6.9, 7},
       {"NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST", "NIST"},
       {"20161109", "20161202", "20161004", "20170412", "20170413", "20171107", "20210924", "20210924", "20211014", "20211014", "20210616"},
       3,
       34,
       1.4,
       "EJ200PS-1X1P photos alt boundary",
       1,
       NULL}); // EJ200PS-1X1P

  measurements.push_back(
      {"EJ200PS-1X1P -30 C",
       {74.9},
       {3.94},
       {0.012},
       {0.03},
       {"3"},
       {7},
       {"NIST"},
       {"20161115"},
       4,
       34,
       1.4,
       "EJ200PS-1X1P -30 C",
       1,
       NULL}); // EJ200PS-1X1P

  measurements.push_back(
      {"EJ200PVT-1X1P old",
       {238, 74.4, 8.53, 0.22, 0.31, 0.98, 8.19, 370, 0.31, 0.31, 0.3, 293, 46, 46},
       {0.23, 0.46, 2.3, 5.05, 5.05, 5.05, 2.01, 0.27, 5.05, 5.05, 5.05, 0.3, 0.27, 0.97},
       {0.012, 0.012, 0.012, 0.1, 0.1, 0.1, 0.012, 0.1, 0.1, 0.1, 0.1, 0.012, 0.012, 0.05},
       {0.08, 0.15, 0.23, 0.2, 0.2, 0.2, 0.13, 0.13, 0.2, 0.2, 0.2, 0.1, 0.13, 0.19},
       {"N2", "N4", "N5", "N8", "N15", "N16", "T1_2", "T1_3", "T1_5", "T1_N1", "AO-1_1", "AO-1_2", "AO-1_3", "L4R"},
       {7, 7, 7, 1.32, 1.26, 4.2, 7, 7, 1.26, 1.71, 6, 7, 7, 7},
       {"NIST", "NIST", "NIST", "GIF++", "GSFC REF", "GSFC REF", "NIST", "NIST", "GSFC REF", "GSFC REF", "GSFC REF", "NIST", "NIST"},
       {"20200917", "20161202", "20161109", "20181026", "20181130", "20181206", "20170301", "20170302", "20181130", "20181130", "20191204", "20190222", "20201220"},
       1,
       33,
       1.4,
       "EJ200PVT-1X1P old",
       1,
       NULL}); // EJ200PVT-1X1P

  measurements.push_back(
      {"EJ200PVT-1X1P photos",
       {238, 74.4, 8.53, 0.22, 0.31, 0.98, 8.19, 370, 0.31, 0.31, 0.3, 293, 46},
       {0.31, 0.58, 2.35, 5.05, 5.05, 5.05, 2.16, 0.3, 5.05, 5.05, 5.05, 0.3, 0.34},
       {0.012, 0.012, 0.012, 0.1, 0.1, 0.1, 0.012, 0.1, 0.1, 0.1, 0.1, 0.012, 0.012},
       {0.02, 0.04, 0.1, 0.2, 0.2, 0.2, 0.08, 0.03, 0.2, 0.2, 0.2, 0.04, 0.02},
       {"N2", "N4", "N5", "N8", "N15", "N16", "T1_2", "T1_3", "T1_5", "T1_N1", "AO-1_1", "AO-1_2", "AO-1_3"},
       {7, 7, 7, 1.32, 1.26, 4.2, 7, 7, 1.26, 1.71, 6, 7, 7},
       {"NIST", "NIST", "NIST", "GIF++", "GSFC REF", "GSFC REF", "NIST", "NIST", "GSFC REF", "GSFC REF", "GSFC REF", "NIST"},
       {"20200917", "20161202", "20161109", "20181026", "20181130", "20181206", "20170301", "20170302", "20181130", "20181130", "20191204", "20190222"},
       2,
       33,
       1.4,
       "EJ200PVT-1X1P photos",
       1,
       NULL}); // EJ200PVT-1X1P

  measurements.push_back(
      {"EJ200PVT-1X1P -30 C",
       {74.9},
       {3.58},
       {0.012},
       {0.03},
       {"N3"},
       {7},
       {"NIST"},
       {"20161115"},
       4,
       34,
       1.4,
       "EJ200PVT-1X1P -30 C",
       1,
       NULL}); // EJ200PS-1X1P

  measurements.push_back(
      {"EJ200PS Dec. 2020",
       {46, 46, 46},
       {1.066, 1.103, 1.041},
       {0.05, 0.05, 0.05},
       {0.019, 0.050, 0.082},
       {"L2R", "L3R", "L4R"},
       {2.4, 4.53, 7},
       {"NIST", "NIST", "NIST"},
       {"20201216", "20201216", "20201216"},
       4,
       33,
       1.4,
       "EJ200PS-1X1P",
       1,
       NULL}); // EJ200PVT-1X1P

  // ======================================================================================================
  // End of results section
  // ======================================================================================================

  // ======================================================================================================
  // Begin of graph preparation section
  // ======================================================================================================

  TString filename = "";
  for (vector<measurement>::iterator i = measurements.begin(); i != measurements.end(); ++i)
  {
    // skip measurements not to be included
    bool includeMeasurement = 0;
    for (vector<TString>::iterator it = incMeas.begin(); it != incMeas.end(); ++it)
    {
      if ((*it).CompareTo(i->name) == 0)
      {
        includeMeasurement = 1;
      }
    }
    if (!includeMeasurement)
      continue;
    if (filename != "")
      filename += "_";
    filename += i->name;

    // check sanity of input
    unsigned int n = i->ddot.size();
    if (i->depth.size() != n ||
        i->eddot_rel.size() != n ||
        i->edepth.size() != n)
    {
      cout << "Check inputs in sample: " << i->legendLabel.c_str() << endl;
      continue;
    }

    vector<float> x_temp;
    vector<float> y_temp;
    vector<float> ex_temp;
    vector<float> ey_temp;
    // Remove entries not at 7 Mrad and > 40 krad/hr
    for (int j = 0; j < n; ++j)
    {
      // if ((i->ddot[j] > 40 && abs(i->dose[j]-7) < 0.1) || (i->ddot[j] < 40)) {
      x_temp.push_back(i->ddot[j]);
      y_temp.push_back(i->depth[j]);
      ex_temp.push_back(i->eddot_rel[j] * i->ddot[j]);
      ey_temp.push_back(i->edepth[j]);
      //}
    }

    if (useGray)
    {
      for (int j = 0; j < x_temp.size(); ++j)
      {
        x_temp[j] *= 0.01 * 1000;  // krad/hr to Gy/hr
        ex_temp[j] *= 0.01 * 1000; // krad/hr to Gy/hr
      }
    }

    TGraphErrors *graph = new TGraphErrors(x_temp.size(), x_temp.data(), y_temp.data(), ex_temp.data(), ey_temp.data());
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
  TCanvas *canv = new TCanvas("canv", "depth versus doserate", 200, 10, 800, 600);
  canv->SetFillColor(0);
  canv->SetLogx();
  // canv->SetLogx();

  // MultiGraph with all data points
  TMultiGraph *allgraphs = new TMultiGraph();
  for (vector<measurement>::iterator i = measurements.begin(); i != measurements.end(); ++i)
  {
    for (vector<TString>::iterator it = incMeas.begin(); it != incMeas.end(); ++it)
    {
      if ((*it).CompareTo(i->name) == 0)
      {
        allgraphs->Add(i->graph);
      }
    }
  }

  allgraphs->Draw("AP0");
  allgraphs->GetXaxis()->SetTitle("Dose rate (krad/h)");
  allgraphs->GetYaxis()->SetTitle("Depth (mm)");
  if (useGray)
  {
    allgraphs->GetXaxis()->SetTitle("Dose rate (Gy/h)");
    allgraphs->GetYaxis()->SetTitle("Depth (mm)");
  }
  allgraphs->GetYaxis()->SetTitleOffset(1.2);
  allgraphs->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  allgraphs->SetMinimum(0.0);
  allgraphs->SetMaximum(5.0);
  double xmin = 5.e-2;
  double xmax = 2.e3;
  if (useGray)
  {
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(5.0);
    xmin = 5.e-1;
    xmax = 2.e4;
  }
  allgraphs->GetXaxis()->SetLimits(xmin, xmax);

  // Text next to markers
  /*
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
  */

  // Prepare the legend
  TLegend *legendA = new TLegend(0.2, 0.25, 0.5, 0.35);
  legendA->SetBorderSize(0);
  legendA->SetFillColorAlpha(0, 0);
  legendA->SetTextSize(0.04);
  for (vector<measurement>::iterator i = measurements.begin(); i != measurements.end(); ++i)
  {
    for (vector<TString>::iterator it = incMeas.begin(); it != incMeas.end(); ++it)
    {
      if ((*it).CompareTo(i->name) == 0 && i->legendId == 1)
      {
        legendA->AddEntry(i->graph, i->legendLabel.c_str(), "p");
      }
    }
  }
  legendA->Draw();

  TLegend *legendB = new TLegend(0.32, 0.12, 0.62, 0.32);
  legendB->SetBorderSize(0);
  legendB->SetFillColor(0);
  for (vector<measurement>::iterator i = measurements.begin(); i != measurements.end(); ++i)
  {
    for (vector<TString>::iterator it = incMeas.begin(); it != incMeas.end(); ++it)
    {
      if ((*it).CompareTo(i->name) == 0 && i->legendId == 2)
      {
        legendB->AddEntry(i->graph, i->legendLabel.c_str(), "p");
      }
    }
  }
  // legendB->Draw();

  // use TPad to get border and fill color, TLatex does not have them
  // TLatex *material = new TLatex(0.8,0.94);
  // material->SetBorderSize(1);
  // material->SetFillColor(0);
  // material->SetTextAlign(11);
  // material->DrawLatex("#color[2]{#Box} PS based\\#color[4]{#Box} PVT based");

  TF1 *func[2];
  int j = 0;
  for (vector<measurement>::iterator i = measurements.begin(); i != measurements.end(); ++i)
  {
    for (vector<TString>::iterator it = incMeas.begin(); it != incMeas.end(); ++it)
    {
      if ((*it).CompareTo(i->name) == 0)
      {
        func[j] = new TF1("func" + (TString)to_string(j), "[0]/sqrt(x)");
        func[j]->SetParameters(1, 1);
        func[j]->SetLineColor(i->markerColor);
        i->graph->Fit("func" + (TString)to_string(j), "", "", 10, 10000);
        func[j]->Draw("same");
        cout << "chi2: " << func[j]->GetChisquare() / func[j]->GetNDF() << endl;
        float A = pow(func[j]->GetParameter(0), 2);
        float eA = 2 * func[j]->GetParameter(0) * func[j]->GetParError(0);
        cout << "Eq 9 parameter: " << A << " +/- " << eA << "mm^2Gy/h" << endl;
        /*
        float p0 = func[j]->GetParameter(0);
        float p1 = func[j]->GetParameter(1);
        float ep0 = func[j]->GetParError(0);
        float ep1 = func[j]->GetParError(1);
        float r5 = pow(p0 / (5 + p1), 2);
        float er5 = sqrt(pow(2 * p0 * ep0 / pow(5 + p1, 2), 2) + pow(2 * pow(p0, 2) * ep1 / pow(5 + p1, 3), 2));
        cout << "The dose rate for 5mm penetration is : " << r5 << " +/- " << er5 << " Gy/h" << endl;
        */
        float p0 = func[j]->GetParameter(0);
        float ep0 = func[j]->GetParError(0);
        float r5 = pow(p0 / 5, 2);
        float er5 = 2 * p0 * ep0 / 25;
        cout << "The dose rate for 5mm penetration is : " << r5 << " +/- " << er5 << " Gy/h" << endl;

        ++j;
      }
    }
  }

  canv->Update();

  filename += tagTime;

  // canv->SaveAs("paperPlots/ver15/"+filename+".pdf");
}
