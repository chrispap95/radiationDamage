/***************************************************************************
**
** This script plots LY ratios for the NIST irradiated rods - September 2021
**
****************************************************************************/

// Improve styling
void set_root_style();

void sept2021Rods()
{
  // Options
  bool opt1 = true;  // Omit titles at the top. For publication exports.
  bool opt2 = true;  // Export plots to the location defined below.

  // Location for PDF exports
  TString location = "paperPlots/ver15/";

  // Make plots look nice
  set_root_style();

  /*********************
  ** Input data section
  *********************/
  // _1 corresponds to L7R and L8R and _2 to L9R and L10R rods
  double days_1[] = {0, 1,  2,  3, 11, 20, 32, 39, 46};
  double days_2[] = {1, 5, 12, 19, 26, 33};
  const int nn_1 = sizeof(days_1)/sizeof(days_1[0]);
  const int nn_2 = sizeof(days_2)/sizeof(days_2[0]);

  double L_1[2][nn_1] = {
    {66.6593,67.7271,70.4843,71.5326,84.2487,98.1965,102.7989,103.7437,102.4142},
    {67.6634,68.2992,71.3863,72.6664,85.0978,98.6736,103.7045,104.7782,104.6711},
  };
  double eL_1[2][nn_1] = {
    {3.8719,3.9568,4.1648,4.2376,5.2621,6.5147,6.9724,7.0539,6.9334},
    {3.9475,3.9988,4.2335,4.3246,5.3307,6.5643,7.0590,7.1569,7.1579},
  };

  double L_2[2][nn_2] = {
    {53.1587,62.1670,77.4925,89.1425,100.0951,106.3680},
    {52.4901,60.9312,72.7012,90.6858,102.3213,108.1453},
  };
  double eL_2[2][nn_2] = {
    {2.9379,3.5523,4.7062,5.6825,6.7037,7.3185},
    {2.8941,3.4656,4.3299,5.8227,6.9200,7.4981},
  };

  double doses_L_1[2] = {70,70};
  double doses_L_2[2] = {70,70};

  // Define uncertainties
  double eDays_1[nn_1], eDays_2[nn_2];
  for (int i = 0; i < nn_1; ++i) eDays_1[i] = 0;
  for (int i = 0; i < nn_2; ++i) eDays_2[i] = 0;

  /****************************************
  ** Plot 1: LY ratio vs time for all rods
  ****************************************/
  TCanvas *c1 = new TCanvas();
  TMultiGraph *gm = new TMultiGraph();
  TGraphErrors* gL_1[2];
  TGraphErrors* gL_2[2];
  auto leg = new TLegend(0.3,0.2,0.8,0.4);

  for (int i = 0; i < 2; i++) {
    gL_1[i] = new TGraphErrors(nn_1, days_1, L_1[i], eDays_1, eL_1[i]);
    gL_1[i]->SetMarkerStyle(22);
    TString title = "L"+to_string(i+7)+" - "+to_string((int)doses_L_1[i])+" kGy @ 130 Gy/h";
    gL_1[i]->SetTitle(title);
    gm->Add(gL_1[i]);
    leg->AddEntry(gL_1[i], title, "lp");
  }
  for (int i = 0; i < 2; i++) {
    gL_2[i] = new TGraphErrors(nn_2, days_2, L_2[i], eDays_2, eL_2[i]);
    gL_2[i]->SetMarkerStyle(22);
    TString title = "L"+to_string(i+9)+" - "+to_string((int)doses_L_2[i])+" kGy @ 460 Gy/h";
    gL_2[i]->SetTitle(title);
    gm->Add(gL_2[i]);
    leg->AddEntry(gL_2[i], title, "lp");
  }

  gm->GetXaxis()->SetLimits(0, 50);
  gm->GetYaxis()->SetRangeUser(0, 150);
  gm->GetXaxis()->SetTitle("days after irr.");
  gm->GetYaxis()->SetTitle("D [kGy]");
  gm->Draw("APL PLC PMC");

  leg->SetLineWidth(0);
  leg->SetTextSize(0.03);
  leg->Draw();

  TPaveText *t = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
  t->AddText("rods from NIST");
  t->SetTextSize(0.05);
  t->SetBorderSize(0);
  t->SetFillColor(0);
  t->SetFillStyle(0);
  t->SetTextFont(42);
  if (!opt1) t->Draw();

  // Export plot
  if (opt2) c1->Print(location+"dcost_vs_time_NIST_Dec2021.pdf");

  return;
}

// Improve styling
void set_root_style()
{
  gStyle->SetTextFont        (42);

  // Canvas
  gStyle->SetCanvasColor     (0);
  gStyle->SetCanvasBorderSize(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasDefH      (800);
  gStyle->SetCanvasDefW      (1000);
  gStyle->SetCanvasDefX      (10);
  gStyle->SetCanvasDefY      (10);

  // Pads
  gStyle->SetPadColor       (0);
  gStyle->SetPadBorderSize  (10);
  gStyle->SetPadBorderMode  (0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadTopMargin   (0.07);
  gStyle->SetPadLeftMargin  (0.15);
  gStyle->SetPadRightMargin (0.07);
  gStyle->SetPadGridX       (0);
  gStyle->SetPadGridY       (0);
  gStyle->SetPadTickX       (1);
  gStyle->SetPadTickY       (1);


  // Frames
  gStyle->SetFrameFillStyle (0);
  gStyle->SetFrameFillColor (kWhite);
  gStyle->SetFrameLineColor (kBlack);
  gStyle->SetFrameLineStyle (0);
  gStyle->SetFrameLineWidth (1);
  gStyle->SetFrameBorderSize(10);
  gStyle->SetFrameBorderMode(0);

  // Markers
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(1.35);

  // Errors
  gStyle->SetEndErrorSize(4);
  gStyle->SetErrorX(0.);

  // Various
  gStyle->SetTickLength (0.03 ,"XYZ");
  gStyle->SetTitleSize  (0.055,"XYZ");
  gStyle->SetTitleFont  (42   ,"XYZ");
  gStyle->SetLabelFont  (42   ,"XYZ");
  gStyle->SetNdivisions (405  ,"XYZ");

  gStyle->SetTitleOffset(1.1,"X");
  gStyle->SetTitleOffset(1.3,"Y");
  gStyle->SetTitleOffset(1.15,"Z");

  gStyle->SetLabelOffset(0.004,"X");
  gStyle->SetLabelOffset(0.015,"Y");
  gStyle->SetLabelOffset(0.015,"Z");

  gStyle->SetLabelSize  (0.055,"X");
  gStyle->SetLabelSize  (0.055,"Y");
  gStyle->SetLabelSize  (0.055,"Z");

  gStyle->SetStatFont   (42);
  gStyle->SetTitleFont  (42);

  // Options
  gStyle->SetOptFit     (0);
  gStyle->SetOptStat    (0);
  gStyle->SetOptTitle   (0);
  gStyle->SetPalette    (kCMYK);

  gROOT->ForceStyle();

  return;
}
