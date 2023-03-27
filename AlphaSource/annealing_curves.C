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

// Define the data structure for this program
struct rod
{
  TString rodName;                 // full rod name
  vector<float> doseConstant;      // in Mrad
  vector<float> edoseConstant;     // in Mrad
  vector<float> days;              // number of days after the end of the irradiation
  vector<float> irrDetails;        // array with (dose (Mrad), dose rate (krad/hr), rel. unc.)
  vector<float> finalDoseConstant; // latest dose constant calculation and unc. (in Mrad)
  TString legendLabel;             // what goes inside the legend entry
  TGraphErrors *graph;             // initially NULL pointer. To be filled by the code.
  TGraphErrors *graph2;            // initially NULL pointer. To be filled by the code.
  TGraphErrors *graph3;            // initially NULL pointer. To be filled by the code.
  vector<float> fitPar;            // fit results [parA, eparA, parB, eparB]
};

void annealing_curves()
{
  set_root_style();
  Bool_t useGray = true;
  Bool_t useLogx = false;

  // ======================================================================================================
  // Begin of data section
  // ======================================================================================================

  vector<TString> incRod;
  incRod.push_back("EJ200PS_1X1P_5");
  incRod.push_back("EJ200PS_1X1P_8");
  incRod.push_back("EJ200PS_1X1P_9");
  incRod.push_back("EJ200PS_L7R");
  incRod.push_back("EJ200PS_L9R");

  vector<rod> rods;

  rods.push_back(
      {"EJ200PS_T1_2",
       {7.44176, 8.97231, 10.60580, 10.62790},
       {0.25693, 0.36852, 0.50690, 0.51020},
       {1, 14, 28, 56},
       {7, 8.06, 0.012},
       {12.65279, 0.71625},
       "T1_2 - 8.06 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_1X1P_5",
       {9.47933, 9.77075, 11.24456, 10.92533, 11.55031, 13.03913, 13.20756},
       {0.40798, 0.43317, 0.56841, 0.53707, 0.59833, 0.75450, 0.77417},
       {0.25, 1, 8, 14, 21, 28, 99},
       {7, 8.53, 0.012},
       {12.24314, 0.66937},
       "1X1P_5 - 8.53 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_L7R",
       {6.56921, 6.67027, 6.94141, 7.05005, 8.30342, 9.67744, 10.12352, 10.22159, 10.08588, 10.09521},
       {0.20918, 0.21539, 0.23222, 0.23916, 0.32609, 0.43713, 0.47685, 0.48537, 0.47333, 0.47347},
       {0.25, 1, 2, 3, 11, 20, 32, 39, 46, 73},
       {6.9, 13, 0.012},
       {10.09521, 0.47347},
       "L7R - 13 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_L8R",
       {6.66820, 6.72682, 7.03043, 7.16182, 8.38714, 9.72453, 10.21303, 10.32367, 10.30835, 10.10144},
       {0.21543, 0.21873, 0.23783, 0.24634, 0.33181, 0.44185, 0.48460, 0.49480, 0.49379, 0.47453},
       {0.25, 1, 2, 3, 11, 20, 32, 39, 46, 73},
       {6.9, 13, 0.012},
       {10.10144, 0.47453},
       "L8R - 13 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_L9R",
       {5.31529, 6.21347, 7.74378, 8.91083, 10.00060, 10.63115, 10.50403},
       {0.14033, 0.18652, 0.28116, 0.36726, 0.45892, 0.51677, 0.50551},
       {0.25, 4, 11, 18, 25, 32, 53},
       {7, 46, 0.012},
       {10.50403, 0.50551},
       "L9R - 46 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_L10R",
       {5.24844, 6.09003, 7.26533, 9.06506, 10.22286, 10.80872, 10.59452},
       {0.13713, 0.17976, 0.24945, 0.38001, 0.47874, 0.53342, 0.51261},
       {0.25, 4, 11, 18, 25, 32, 53},
       {7, 46, 0.012},
       {10.59452, 0.51261},
       "L10R - 46 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_1X1P_8",
       {5.17459, 7.97002, 11.54411, 11.31787},
       {0.13136, 0.29214, 0.59523, 0.57438},
       {0.25, 5, 54, 103},
       {7, 74.4, 0.012},
       {10.69303, 0.51457},
       "1X1P_8 - 74.4 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_T1_3",
       {4.57588, 7.29604, 9.08555, 9.94050},
       {0.10661, 0.24753, 0.37594, 0.44665},
       {1, 15, 29, 57},
       {7, 364, 0.012},
       {9.08617, 0.37693},
       "T1_3 - 364 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});
  rods.push_back(
      {"EJ200PS_1X1P_9",
       {4.88652, 6.37980, 7.59853, 8.87695, 9.93691, 9.43912, 10.79556},
       {0.11959, 0.19304, 0.26787, 0.35996, 0.44656, 0.40482, 0.52207},
       {0.25, 6, 13, 20, 26, 46, 111},
       {7, 390, 0.012},
       {9.88592, 0.44249},
       "1X1P_9 - 390 krad/hr",
       NULL,
       NULL,
       NULL,
       {}});

  // ======================================================================================================
  // End of data section
  // ======================================================================================================

  // ======================================================================================================
  // Begin of graph preparation section
  // ======================================================================================================

  // Define lists of colors and styles
  unsigned st_i = 0;
  Color_t colors[] = {1, 4, 8, 46, 34};
  Style_t styles[] = {34, 33, 23, 22, 20, 21};

  TString filename = "annealing_PS";
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    // skip rods not to be included
    bool includeRod = 0;
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        includeRod = 1;
      }
    }
    if (!includeRod)
      continue;
    // if (filename != "") filename += "_";
    // filename += i->name;

    // check sanity of input
    unsigned int n = i->days.size();
    if (i->doseConstant.size() != n ||
        i->edoseConstant.size() != n)
    {
      cout << "Check inputs in sample: " << i->rodName << endl;
      continue;
    }

    vector<float> x_temp;
    vector<float> y_temp;
    vector<float> ex_temp;
    vector<float> ey_temp;
    for (int j = 0; j < n; ++j)
    {
      x_temp.push_back(i->days[j]);
      y_temp.push_back(i->doseConstant[j]);
      ex_temp.push_back(0.0);
      ey_temp.push_back(i->edoseConstant[j]);
    }

    if (useGray)
    {
      for (int j = 0; j < x_temp.size(); ++j)
      {
        y_temp[j] *= 10;  // Mrad to kGy
        ey_temp[j] *= 10; // Mrad to kGy
      }
    }

    size_t nn = x_temp.size();
    float *days = &x_temp[0];
    float *edays = &ex_temp[0];
    float *dcost = &y_temp[0];
    float *edcost = &ey_temp[0];

    // Find positions of max and min elements
    size_t max_i = distance(dcost, max_element(dcost, dcost + nn));
    size_t min_i = distance(dcost, min_element(dcost, dcost + nn));
    float diff = dcost[max_i] - dcost[min_i];

    float days_n[nn];
    float edays_n[nn];
    float dcost_n1[nn];
    float edcost_n1[nn];
    float dcost_n2[nn];
    float edcost_n2[nn];

    for (int j = 0; j < nn; ++j)
    {
      days_n[j] = days[j] / days[max_i];
      edays_n[j] = 0.0;
      dcost_n1[j] = dcost[j] / dcost[max_i];
      edcost_n1[j] = sqrt(pow(edcost[j] / dcost[max_i], 2) + pow(dcost[j] * edcost[max_i] / pow(dcost[max_i], 2), 2));
      dcost_n2[j] = (dcost[j] - dcost[min_i]) / diff;
      edcost_n2[j] = sqrt(pow(edcost[j] / diff, 2) + pow((dcost[j] - dcost[min_i]) * edcost[max_i] / pow(diff, 2), 2) + pow((dcost[j] - dcost[max_i]) * edcost[min_i] / pow(diff, 2), 2));
    }

    TGraphErrors *graph = new TGraphErrors(x_temp.size(), x_temp.data(), y_temp.data(), ex_temp.data(), ey_temp.data());
    graph->SetTitle();
    graph->GetXaxis()->SetTitle("days after irr.");

    // create working pointer only when rod is to be plotted
    i->graph = graph;

    TGraphErrors *graph2 = new TGraphErrors(nn, days_n, dcost_n1, edays_n, edcost_n1);
    graph2->SetTitle();
    graph2->GetXaxis()->SetTitle("days after irr.");

    // create working pointer only when rod is to be plotted
    i->graph2 = graph2;

    TGraphErrors *graph3 = new TGraphErrors(nn, days_n, dcost_n2, edays_n, edcost_n2);
    graph3->SetTitle();
    graph3->GetXaxis()->SetTitle("days after irr.");

    // create working pointer only when rod is to be plotted
    i->graph3 = graph3;

    TF1 *f1 = new TF1("f1", "[0]*tanh([1]*x)", 0, 1);
    f1->SetParameters(1, 1);
    graph3->Fit("f1", "Q0");
    vector<float> fitPar{(float)f1->GetParameter(0), (float)f1->GetParError(0),
                         (float)f1->GetParameter(1), (float)f1->GetParError(1)};
    i->fitPar = fitPar;
  }

  // ======================================================================================================
  // Drawing
  // ======================================================================================================

  // Canvas
  TCanvas *canv1 = new TCanvas("canv1", "annealing curves", 200, 10, 800, 600);
  canv1->SetFillColor(0);
  if (useLogx)
    canv1->SetLogx();

  // MultiGraph with all data points
  TMultiGraph *allgraphs = new TMultiGraph();
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        allgraphs->Add(i->graph);
      }
    }
  }

  allgraphs->Draw("AP PMC");
  allgraphs->GetXaxis()->SetTitle("Days after irr.");
  allgraphs->GetYaxis()->SetTitle("Dose Constant (Mrad)");
  if (useGray)
  {
    allgraphs->GetYaxis()->SetTitle("Dose Constant (kGy)");
  }
  allgraphs->GetYaxis()->SetTitleOffset(1.2);
  allgraphs->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  allgraphs->SetMinimum(0.0);
  allgraphs->SetMaximum(16.0);
  double xmin = (useLogx) ? 0.1 : 0.0;
  double xmax = (useLogx) ? 200 : 120;
  if (useGray)
  {
    allgraphs->SetMinimum(0.0);
    allgraphs->SetMaximum(150.0);
  }
  allgraphs->GetXaxis()->SetLimits(xmin, xmax);

  // Prepare the legend
  double w = 0.63, h = 0.21, w_d = 0, h_d = 0.16; // h_d = 0.17 and 0.68 for bottom and top placement, respectively
  double x1, x2, y1, y2;
  x1 = (1 - w) / 2 + w_d;
  x2 = x1 + w;
  y1 = h_d;
  y2 = y1 + h;
  TLegend *legendA = new TLegend(x1, y1, x2, y2);
  legendA->SetNColumns(2);
  legendA->SetBorderSize(0);
  legendA->SetFillColorAlpha(0, 0);
  legendA->SetTextSize(0.04);
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        legendA->AddEntry(i->graph, i->legendLabel, "p");
      }
    }
  }
  legendA->Draw();

  canv1->Update();
  // if (useLogx) filename+="_logx";
  // canv1->SaveAs("paperPlots/ver17/"+filename+".pdf");

  // Canvas
  TCanvas *canv2 = new TCanvas("canv2", "annealing curves", 200, 10, 800, 600);
  canv2->SetFillColor(0);
  if (useLogx)
    canv2->SetLogx();

  // MultiGraph with all data points
  TMultiGraph *allgraphs2 = new TMultiGraph();
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        allgraphs2->Add(i->graph2);
      }
    }
  }

  allgraphs2->Draw("AP PMC");
  allgraphs2->GetXaxis()->SetTitle("normalized days");
  allgraphs2->GetYaxis()->SetTitle("normalized dose constant");
  if (useGray)
  {
    allgraphs2->GetYaxis()->SetTitle("normalized dose constant");
  }
  allgraphs2->GetYaxis()->SetTitleOffset(1.2);
  allgraphs2->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  allgraphs2->SetMinimum(0.0);
  allgraphs2->SetMaximum(1.0);
  double xmin2 = 0;
  double xmax2 = 1;
  if (useGray)
  {
    allgraphs2->SetMinimum(0.0);
    allgraphs2->SetMaximum(1.0);
  }
  allgraphs2->GetXaxis()->SetLimits(xmin2, xmax2);

  canv2->Update();

  // Canvas
  TCanvas *canv3 = new TCanvas("canv3", "annealing curves", 200, 10, 800, 600);
  canv3->SetFillColor(0);
  if (useLogx)
    canv3->SetLogx();

  // MultiGraph with all data points
  TMultiGraph *allgraphs3 = new TMultiGraph();
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        allgraphs3->Add(i->graph3);
      }
    }
  }

  allgraphs3->Draw("AP PMC PFC");
  allgraphs3->GetXaxis()->SetTitle("normalized days");
  allgraphs3->GetYaxis()->SetTitle("normalized dose constant");
  if (useGray)
  {
    allgraphs3->GetYaxis()->SetTitle("normalized dose constant");
  }
  allgraphs3->GetYaxis()->SetTitleOffset(1.2);
  allgraphs3->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  allgraphs3->SetMinimum(0.0);
  allgraphs3->SetMaximum(1.0);
  double xmin3 = 0;
  double xmax3 = 1;
  if (useGray)
  {
    allgraphs3->SetMinimum(0.0);
    allgraphs3->SetMaximum(1.0);
  }
  allgraphs3->GetXaxis()->SetLimits(xmin3, xmax3);

  canv3->Update();

  /*
  ** Make parameters vs dose rate plots
  */
  size_t nm = incRod.size();
  float doseRates[nm];
  float edoseRates[nm];
  float par0[nm];
  float epar0[nm];
  float par1[nm];
  float epar1[nm];
  // MultiGraph with all data points
  size_t j = 0;
  for (vector<rod>::iterator i = rods.begin(); i != rods.end(); ++i)
  {
    for (vector<TString>::iterator it = incRod.begin(); it != incRod.end(); ++it)
    {
      if ((*it).CompareTo(i->rodName) == 0)
      {
        doseRates[j] = i->irrDetails[1];
        edoseRates[j] = (i->irrDetails[2]) * (i->irrDetails[1]);
        par0[j] = i->fitPar[0];
        epar0[j] = i->fitPar[1];
        par1[j] = i->fitPar[2];
        epar1[j] = i->fitPar[3];
        cout << doseRates[j] << ", " << par0[j] << ", " << par1[j] << "\n";
        ++j;
      }
    }
  }

  // Parameter 0
  TCanvas *canv4 = new TCanvas("canv4", "parameter 0", 200, 10, 800, 600);
  canv4->SetFillColor(0);
  TGraphErrors *gPar0 = new TGraphErrors(nm, doseRates, par0, edoseRates, epar0);
  gPar0->Draw("AP PMC PFC");
  gPar0->GetXaxis()->SetTitle("dose rate (Gy/hr)");
  gPar0->GetYaxis()->SetTitle("Parameter 0");
  gPar0->GetYaxis()->SetTitleOffset(1.2);
  gPar0->GetXaxis()->SetTitleOffset(1.3);
  gPar0->SetMinimum(0.0);
  gPar0->SetMaximum(2.0);
  double xmin4 = 1;
  double xmax4 = 500;
  gPar0->GetXaxis()->SetLimits(xmin4, xmax4);
  canv4->Update();

  // Parameter 1
  TCanvas *canv5 = new TCanvas("canv5", "parameter 1", 200, 10, 800, 600);
  canv5->SetFillColor(0);
  TGraphErrors *gPar1 = new TGraphErrors(nm, doseRates, par1, edoseRates, epar1);
  gPar1->Draw("AP PMC PFC");
  gPar1->GetXaxis()->SetTitle("dose rate (Gy/hr)");
  gPar1->GetYaxis()->SetTitle("Parameter 1");
  gPar1->GetYaxis()->SetTitleOffset(1.2);
  gPar1->GetXaxis()->SetTitleOffset(1.3);
  gPar1->SetMinimum(0.0);
  gPar1->SetMaximum(10.0);
  double xmin5 = 1;
  double xmax5 = 500;
  gPar1->GetXaxis()->SetLimits(xmin5, xmax5);
  canv5->Update();
}
