/************************************************************************
**
** This script plots LY ratios for the NIST irradiated rods - Sept. 2021
**
************************************************************************/

// Improve styling
void set_root_style();

void nistSept2021()
{
  // Options
  bool opt3 = true;  // Omit titles at the top. For publication exports.
  bool opt5 = true;  // Export plots to the location defined below.
  bool opt6 = true;  // Include old data at

  // Location for PDF exports
  TString location = "paperPlots/ver14/";

  // Make plots look nice
  set_root_style();

  /*********************
  ** Input data section
  *********************/
  // Old data - Dec. 2020 irratiations
  double old1_irr[3] = {4.38135, 3.68790, 3.38832}; // EJ200PVT - L1R, L2R, L3R
  double old1_irr_REF = 5.61891;
  double old1_irr_DC = -0.12628;
  double old1_unirr = 6.00071; // EJ200PVT - L4R
  double LYratio_old1[3];

  double doses_old2[1] = {47};
  double old2_irr[1] = {3.66721}; // EJ200PVT-2X1P-N2
  double old2_irr_REF = 5.55500;
  double old2_irr_DC = -0.15801;
  double old2_unirr = 5.61944; // EJ200PVT-2X1P-N1
  double old2_unirr_REF = 5.42841;
  double old2_unirr_DC = -0.17897;
  double LYratio_old2[1];


  double days[] = {1, 2, 3, 4};
  const int nn = sizeof(days)/sizeof(days[0]);

  // Shift 1 applies to rods A[1-5] and L6R
  double pedestal[nn] = {-0.10497, -0.07930, -0.07806, -0.11055};
  double referen_rod[nn] = {5.66055, 5.64200, 5.65432, 5.63467};

  double L[2][nn] = {
    {1.67479, 1.72285, 1.80364, 1.80673}, // L7R
    {1.71003, 1.74674, 1.83542, 1.84513}, // L8R
  };

  double doses_L[2] = {69, 69};

  // Data and calibration measurements for the unirradiated
  double L_initial_REF = 5.63541;
  double L_initial_DC = -0.12056;
  double L_initial[2] = {4.97529,4.99194};

  double LYratio_L[2][nn];

  // Correct/Calibrate all measurements
  for (int i = 0; i < 2; i++) {
    L_initial[i] = (L_initial[i] - L_initial_DC)/(L_initial_REF - L_initial_DC);
  }
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < 2; j++) {
      L[j][i] = (L[j][i]-pedestal[i])/(referen_rod[i]-pedestal[i]);
      LYratio_L[j][i] = L[j][i]/L_initial[j];
    }
  }
  old1_unirr = (old1_unirr-old1_irr_DC)/(old1_irr_REF-old1_irr_DC);
  for (int i = 0; i < 3; i++) {
    old1_irr[i] = (old1_irr[i]-old1_irr_DC)/(old1_irr_REF-old1_irr_DC);
    LYratio_old1[i] = old1_irr[i]/old1_unirr;
  }
  old2_unirr = (old2_unirr-old2_unirr_DC)/(old2_unirr_REF-old2_unirr_DC);
  for (int i = 0; i < 1; i++) {
    old2_irr[i] = (old2_irr[i]-old2_irr_DC)/(old2_irr_REF-old2_irr_DC);
    LYratio_old2[i] = old2_irr[i]/old2_unirr;
  }

  // Define uncertainties - 3% for all LY ratios and 1% for the doses
  double eDays[nn];
  double eDoses_L[2], eDoses_old2[1];
  double eLYratio_L[2][nn], eLYratio_old1[3], eLYratio_old2[1];
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < 2; j++) {
      eLYratio_L[j][i] = 0.03*LYratio_L[j][i];
    }
    eDays[i] = 0;
  }
  for (int i = 0; i < 2; i++) {
    eDoses_L[i] = 0.01*doses_L[i];
    eLYratio_old1[i] = 0.03*LYratio_old1[i];
  }
  for (int i = 0; i < 1; i++) {
    eDoses_old2[i] = 0.01*doses_old2[i];
    eLYratio_old2[i] = 0.03*LYratio_old2[i];
  }

  /****************************************
  ** Plot 1: LY ratio vs time for all rods
  ****************************************/
  TCanvas *c1 = new TCanvas();
  TMultiGraph *gm = new TMultiGraph();
  TGraphErrors* gL[2];
  auto leg = new TLegend(0.3,0.2,0.8,0.4);
  leg->SetNColumns(2);

  for (int i = 0; i < 2; i++) {
    gL[i] = new TGraphErrors(nn, days, LYratio_L[i], eDays, eLYratio_L[i]);
    gL[i]->SetMarkerStyle(22);
    TString title;
    if (i == 0) title = "L"+to_string(7)+"R - "+to_string((int)doses_L[i])+" kGy";
    else if (i == 1) title = "L"+to_string(8)+"R - "+to_string((int)doses_L[i])+" kGy";
    gL[i]->SetTitle(title);
    gm->Add(gL[i]);
    leg->AddEntry(gL[i], title, "lp");
  }

  double maxY = LYratio_L[0][nn-1] > 1 ? LYratio_L[0][nn-1]*1.05 : 1;
  gm->GetXaxis()->SetLimits(0, days[nn-1]+2);
  gm->GetYaxis()->SetRangeUser(0, maxY);
  gm->GetXaxis()->SetTitle("days after irr.");
  gm->GetYaxis()->SetTitle("LY_{irr.}/^{}LY_{initial}");
  gm->Draw("APL PLC PMC");

  leg->SetLineWidth(0);
  leg->SetTextSize(0.03);
  leg->Draw();

  TPaveText *t = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
  t->AddText("rods from sandia");
  t->SetTextSize(0.05);
  t->SetBorderSize(0);
  t->SetFillColor(0);
  t->SetFillStyle(0);
  t->SetTextFont(42);
  if (!opt3) t->Draw();

  // Export plot
  if (opt5) c1->Print(location+"LYratio_vs_time.pdf");



  /**********************************************************
  ** Plot 2: LY ratio(latest) vs exponential fit & residuals
  **********************************************************/
  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  TPad *pad1  = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.018); // Upper and lower plot are joined
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad

  double LYratio_ps[2], eLYratio_ps[2];
  // Make it plot the latest measurements
  for (int i = 0; i < 2; i++) {
    LYratio_ps[i] = LYratio_L[i][nn-1];
    eLYratio_ps[i] = 0.03*LYratio_ps[i];
  }
  const int nnps = sizeof(LYratio_ps)/sizeof(LYratio_ps[0]);
  const int nnpvtold = sizeof(LYratio_old1)/sizeof(LYratio_old1[0]);
  TGraphErrors *gdose_rateps = new TGraphErrors(nnps, doses_L, LYratio_ps, eDoses_L, eLYratio_ps);
  TGraphErrors *gdose_ratepvtold = new TGraphErrors(nnpvtold, doses_L, LYratio_old1, eDoses_L, eLYratio_old1);
  TGraphErrors *gdose_ratepvtold2 = new TGraphErrors(1, doses_old2, LYratio_old2, eDoses_old2, eLYratio_old2);
  gdose_rateps->SetMarkerStyle(22);
  gdose_ratepvtold->SetMarkerStyle(22);
  gdose_ratepvtold2->SetMarkerStyle(22);
  gdose_rateps->SetMarkerColor(kBlue);
  gdose_ratepvtold->SetMarkerColor(8);
  gdose_ratepvtold2->SetMarkerColor(2);
  gdose_rateps->SetLineColor(kBlue);
  gdose_ratepvtold->SetLineColor(8);
  gdose_ratepvtold2->SetLineColor(2);
  gdose_rateps->SetTitle("PS");
  gdose_ratepvtold->SetTitle("PVT - old");
  gdose_ratepvtold2->SetTitle("PVT - old 2");

  TMultiGraph *gm2 = new TMultiGraph();
  gm2->Add(gdose_rateps);
  if (opt6) gm2->Add(gdose_ratepvtold);
  //if (opt6) gm2->Add(gdose_ratepvtold2);
  gm2->GetXaxis()->SetLimits(0, 80);
  gm2->GetYaxis()->SetRangeUser(0, 1.0);
  gm2->GetXaxis()->SetLabelSize(0.0);
  gm2->GetYaxis()->SetTitle("LY_{irr.}/^{}LY_{initial}");
  gm2->Draw("AP");

  auto leg2 = new TLegend(0.2,0.18,0.4,0.28);
  leg2->AddEntry(gdose_rateps, "PS", "lp");
  if (opt6) leg2->AddEntry(gdose_ratepvtold, "PVT - Dec. 2020 irr.", "lp");
  //if (opt6) leg2->AddEntry(gdose_ratepvtold2, "PVT-2X1P - Sept. 2020 irr.", "lp");
  leg2->SetFillColorAlpha(0,0);
  leg2->SetLineWidth(0);
  leg2->SetTextSize(0.04);
  if (opt6) leg2->Draw();

  TPaveText *t2 = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
  TString txt2 = "day " + to_string((int)days[nn-1]);
  t2->AddText(txt2);
  t2->SetTextSize(0.05);
  t2->SetBorderSize(0);
  t2->SetFillColor(0);
  t2->SetFillStyle(0);
  t2->SetTextFont(42);
  if (!opt3) t2->Draw();

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
