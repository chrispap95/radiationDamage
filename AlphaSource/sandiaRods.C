/************************************************************************
**
** This script plots LY ratios for the Sandia irradiated rods - May 2021
**
************************************************************************/

/*
** Options
** opt1 - Include PS_L rods
** opt2 - Plot LY ratio vs time
** opt3 - Omit titles at the top. For publication exports.
** opt4 - Fit with range (0,40) kGy
** opt5 - Export plots to the location defined below.
** opt6 - Include old data from Dec 2020
** opt7 - Fit with full range
** opt8 - Include old data from Sep 2020 (default = false)
** opt9 - Fit PS_L (if opt1 is false then it has no effect)
*/

/*
** Options for specific plots:
**  - Figure 4 [left]:
**      bool opt1 = false, bool opt2 = false, bool opt3 = true, bool opt4 = true,
**      bool opt5 = true, bool opt6 = true, bool opt7 = true, bool opt8 = false, bool opt9 = false
**
**  - Figure 4 [right]:
**      bool opt1 = true, bool opt2 = false, bool opt3 = true, bool opt4 = true,
**      bool opt5 = true, bool opt6 = false, bool opt7 = false, bool opt8 = false, bool opt9 = true
*/

// Improve styling
void set_root_style();

void sandiaRods(
  bool opt1 = true, bool opt2 = false, bool opt3 = true, bool opt4 = true,
  bool opt5 = true, bool opt6 = false, bool opt7 = false, bool opt8 = false, bool opt9 = true
){

  // Location for PDF exports
  TString location = "paperPlots/ver17/";

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


  double days[] = {1, 4, 9, 16, 23};
  const int nn = sizeof(days)/sizeof(days[0]);

  // Shift 1 applies to rods A[1-5] and L6R
  double pedestal[2][nn] = {
    {-0.11678, -0.1196, -0.12718, -0.1222,(-0.1077-0.1161)/2}, // shift 1
    {-0.129, -0.10972, -0.1001, -0.09508, -0.12011}            // shift 2
  };
  double referen_rod[2][nn] = {
    {5.56248, 5.622990, 5.60593, 5.626096, (5.607507+5.625880)/2}, // shift 1
    {(5.622+5.61116)/2, 5.57485, 5.604418, 5.57525, 5.61751}       // shift 2
  };

  double A[10][nn] = {
    {4.87787, 4.967149, 5.00011,  5.007314, 5.005909}, // A1
    {4.70806, 4.744589, 4.81308,  4.799527, 4.798452},
    {4.58580, 4.593320, 4.64826,  4.649427, 4.624193},
    {4.05188, 4.144884, 4.25546,  4.196091, 4.157400},
    {4.17720, 4.203350, 4.29741,  4.265709, 4.213584},
    {3.186,   3.48283,  3.683851, 3.71112,  3.75495},  // A6
    {3.187,   3.24250,  3.287365, 3.29590,  3.33927},
    {3.47710, 3.50197,  3.517087, 3.55820,  3.59398},
    {2.963,   3.00163,  3.005944, 3.05093,  3.04564},
    {2.87,    2.92600,  2.931136, 2.97247,  2.96783}  // A10
  };

  double L[3][nn] = {
    {3.14161, 3.19091,  3.203634, 3.25110,  3.20793},  // L5R
    {2.32,    2.410936, 2.46946,  2.439731, 2.428170}, // L6R
    {1.922,   2.02097,  2.038967, 2.030258, 2.022143}, // L11R
  };

  double doses_A[10] = {8, 12, 16, 20, 24, 33, 42, 46, 58, 70};
  double doses_L[3] = {24, 46, 70};

  // Data and calibration measurements for the unirradiated
  double A_initial_REF = 5.61281;
  double L_initial_REF = 5.61281;
  double A_initial_DC = -0.12056;
  double L_initial_DC = -0.12056;
  double A_initial[10] = {5.89289,5.88181,5.88727,5.87026,5.85736,
                          5.90795,5.86200,5.89146,5.84563,5.88397};
  double L_initial[3] = {4.93974,4.95453,4.92470};

  double LYratio_A[10][nn], LYratio_L[3][nn];

  // Correct/Calibrate all measurements
  for (int i = 0; i < 10; i++) {
    A_initial[i] = (A_initial[i] - A_initial_DC)/(A_initial_REF - A_initial_DC);
  }
  for (int i = 0; i < 3; i++) {
    L_initial[i] = (L_initial[i] - L_initial_DC)/(L_initial_REF - L_initial_DC);
  }
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < 10; j++) {
      if (j < 5) A[j][i] = (A[j][i]-pedestal[0][i])/(referen_rod[0][i]-pedestal[0][i]);
      else A[j][i] = (A[j][i]-pedestal[1][i])/(referen_rod[1][i]-pedestal[1][i]);
      LYratio_A[j][i] = A[j][i]/A_initial[j];
    }
    for (int j = 0; j < 3; j++) {
      if (j == 1) L[j][i] = (L[j][i]-pedestal[0][i])/(referen_rod[0][i]-pedestal[0][i]);
      else L[j][i] = (L[j][i]-pedestal[1][i])/(referen_rod[1][i]-pedestal[1][i]);
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

  // Define uncertainties - 4.36% for all LY ratios and 1.2% for the doses
  // Adding all 4 measurements in quadrature yields these numbers
  double eDays[nn];
  double eDoses_A[10], eDoses_L[3], eDoses_old2[1];
  double eLYratio_A[10][nn], eLYratio_L[3][nn], eLYratio_old1[3], eLYratio_old2[1];
  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < 10; j++) {
      eLYratio_A[j][i] = 0.0436*LYratio_A[j][i];
    }
    for (int j = 0; j < 3; j++) {
      eLYratio_L[j][i] = 0.0436*LYratio_L[j][i];
    }
    eDays[i] = 0;
  }
  for (int i = 0; i < 10; i++) {
    eDoses_A[i] = 0.012*doses_A[i];
  }
  for (int i = 0; i < 3; i++) {
    eDoses_L[i] = 0.012*doses_L[i];
    eLYratio_old1[i] = 0.0436*LYratio_old1[i];
  }
  for (int i = 0; i < 1; i++) {
    eDoses_old2[i] = 0.012*doses_old2[i];
    eLYratio_old2[i] = 0.0436*LYratio_old2[i];
  }

  /****************************************
  ** Plot 1: LY ratio vs time for all rods
  ****************************************/
  TCanvas *c1;
  if (opt2) c1 = new TCanvas();
  TMultiGraph *gm = new TMultiGraph();
  TGraphErrors* gA[10];
  TGraphErrors* gL[3];
  auto leg = new TLegend(0.3,0.2,0.8,0.4);
  leg->SetNColumns(2);

  for (int i = 0; i < 10; i++) {
    gA[i] = new TGraphErrors(nn, days, LYratio_A[i], eDays, eLYratio_A[i]);
    gA[i]->SetMarkerStyle(22);
    TString title = "A"+to_string(i+1)+" - "+to_string((int)doses_A[i])+" kGy";
    gA[i]->SetTitle(title);
    gm->Add(gA[i]);
    leg->AddEntry(gA[i], title, "p");
  }
  for (int i = 0; i < 3 && opt1; i++) {
    gL[i] = new TGraphErrors(nn, days, LYratio_L[i], eDays, eLYratio_L[i]);
    gL[i]->SetMarkerStyle(22);
    TString title;
    if (i == 0) title = "L"+to_string(5)+"R - "+to_string((int)doses_L[i])+" kGy";
    else if (i == 1) title = "L"+to_string(6)+"R - "+to_string((int)doses_L[i])+" kGy";
    else title = "L"+to_string(11)+"R - "+to_string((int)doses_L[i])+" kGy";
    gL[i]->SetTitle(title);
    gm->Add(gL[i]);
    leg->AddEntry(gL[i], title, "p");
  }

  double maxY = LYratio_A[0][nn-1] > 1 ? LYratio_A[0][nn-1]*1.05 : 1;
  gm->GetXaxis()->SetLimits(0, days[nn-1]+4);
  gm->GetYaxis()->SetRangeUser(0, maxY);
  gm->GetXaxis()->SetTitle("days after irr.");
  gm->GetYaxis()->SetTitle("LY_{irr.}/^{}LY_{initial}");
  if (opt2) gm->Draw("APL PLC PMC");

  leg->SetLineWidth(0);
  leg->SetTextSize(0.03);
  if (opt2) leg->Draw();

  TPaveText *t = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
  t->AddText("rods from sandia");
  t->SetTextSize(0.05);
  t->SetBorderSize(0);
  t->SetFillColor(0);
  t->SetFillStyle(0);
  t->SetTextFont(42);
  if (!opt3 && opt2) t->Draw();

  // Export plot
  if (opt5 && opt2) c1->Print(location+"LYratio_vs_time.pdf");


  /**********************************************************
  ** Plot 2: LY ratio(latest) vs exponential fit & residuals
  **********************************************************/
  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  TPad *pad1  = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.023); // Upper and lower plot are joined
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad

  // Define colors
  Color_t clr_PS_markers = 46;
  Color_t clr_PS_fit = 46;
  Color_t clr_PVT_markers = kBlue;
  Color_t clr_PVT_fit = opt1 ? kBlue : kBlack;
  Color_t clr_PVTold_markers = 8;  // Deep green
  Color_t clr_PVTold2_markers = 2; // Bright red

  double LYratio_pvt[10], eLYratio_pvt[10];
  double LYratio_ps[3], eLYratio_ps[3];
  // Make it plot the latest measurements
  for (int i = 0; i < 10; i++) {
    LYratio_pvt[i] = LYratio_A[i][nn-1];
    eLYratio_pvt[i] = 0.0436*LYratio_pvt[i];
  }
  for (int i = 0; i < 3; i++) {
    LYratio_ps[i] = LYratio_L[i][nn-1];
    eLYratio_ps[i] = 0.0436*LYratio_ps[i];
  }
  const int nnpvt = sizeof(LYratio_pvt)/sizeof(LYratio_pvt[0]);
  const int nnps = sizeof(LYratio_ps)/sizeof(LYratio_ps[0]);
  const int nnpvtold = sizeof(LYratio_old1)/sizeof(LYratio_old1[0]);
  TGraphErrors *gdose_ratepvt = new TGraphErrors(nnpvt, doses_A, LYratio_pvt, eDoses_A, eLYratio_pvt);
  TGraphErrors *gdose_rateps = new TGraphErrors(nnps, doses_L, LYratio_ps, eDoses_L, eLYratio_ps);
  TGraphErrors *gdose_ratepvtold = new TGraphErrors(nnpvtold, doses_L, LYratio_old1, eDoses_L, eLYratio_old1);
  TGraphErrors *gdose_ratepvtold2 = new TGraphErrors(1, doses_old2, LYratio_old2, eDoses_old2, eLYratio_old2);
  gdose_rateps->SetMarkerStyle(20);
  gdose_ratepvt->SetMarkerStyle(21);
  gdose_ratepvtold->SetMarkerStyle(22);
  gdose_ratepvtold2->SetMarkerStyle(23);
  gdose_rateps->SetMarkerColor(clr_PS_markers);
  gdose_ratepvt->SetMarkerColor(clr_PVT_markers);
  gdose_ratepvtold->SetMarkerColor(clr_PVTold_markers);
  gdose_ratepvtold2->SetMarkerColor(clr_PVTold2_markers);
  gdose_rateps->SetLineColor(clr_PS_markers);
  gdose_ratepvt->SetLineColor(clr_PVT_markers);
  gdose_ratepvtold->SetLineColor(clr_PVTold_markers);
  gdose_ratepvtold2->SetLineColor(clr_PVTold2_markers);
  gdose_rateps->SetTitle("PS");
  gdose_ratepvt->SetTitle("PVT");
  gdose_ratepvtold->SetTitle("PVT - old");
  gdose_ratepvtold2->SetTitle("PVT - old 2");

  TMultiGraph *gm2 = new TMultiGraph();
  gm2->Add(gdose_ratepvt);
  if (opt1 && !opt6) gm2->Add(gdose_rateps);
  if (opt6 && !opt1) gm2->Add(gdose_ratepvtold);
  if (opt8) gm2->Add(gdose_ratepvtold2);
  gm2->GetXaxis()->SetLimits(0, 80);
  gm2->GetYaxis()->SetRangeUser(0, 1.0);
  gm2->GetXaxis()->SetLabelSize(0.0);
  gm2->GetYaxis()->SetTitle("LY_{irr.}/^{}LY_{initial}");
  gm2->Draw("AP PLC PMC");

  auto leg2 = new TLegend(0.2,0.18,0.4,0.28);
  leg2->AddEntry(gdose_ratepvt, "PVT - Sandia", "p");
  if (opt1 && !opt6) leg2->AddEntry(gdose_rateps, "PS - Sandia", "p");
  if (opt6 && !opt1) leg2->AddEntry(gdose_ratepvtold, "PVT - NIST (Dec. 2020)", "p");
  if (opt8) leg2->AddEntry(gdose_ratepvtold2, "PVT-2X1P - Sept. 2020 irr.", "p");
  leg2->SetFillColorAlpha(0,0);
  leg2->SetLineWidth(0);
  leg2->SetTextSize(0.05);
  if (opt1 | opt6) leg2->Draw();

  TPaveText *t2 = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
  TString txt2 = "day " + to_string((int)days[nn-1]);
  t2->AddText(txt2);
  t2->SetTextSize(0.05);
  t2->SetBorderSize(0);
  t2->SetFillColor(0);
  t2->SetFillStyle(0);
  t2->SetTextFont(42);
  if (!opt3) t2->Draw();

  c2->Update();

  // Fit the PVT points
  TF1 *f1 = new TF1("f1","expo",0,80);
  TF1 *f2 = new TF1("f2","expo",0,80);
  gdose_ratepvt->Fit(f1,"0","",0,40);
  gdose_ratepvt->Fit(f2,"0","",0,80);
  f1->SetLineColor(gdose_ratepvt->GetMarkerColor());
  f2->SetLineColor(gdose_ratepvt->GetMarkerColor());
  f2->SetLineStyle(kDashed);
  if (opt4) f1->Draw("SAME");
  if (opt7) f2->Draw("SAME");

  cout << gdose_ratepvt->GetMarkerColor() << "\n";
  cout << gdose_rateps->GetMarkerColor() << "\n";

  // Fit the PS points
  TF1 *f3 = new TF1("f3","expo",0,80);
  gdose_rateps->Fit(f3,"0","",0,80);
  f3->SetLineColor(gdose_rateps->GetMarkerColor());
  f3->SetLineStyle(kDashed);
  if (opt1 && opt9) f3->Draw("SAME");

  // Legend for fit lines
  TLegend* leg3 = new TLegend(0.67,0.15,0.85,0.30);
  leg3->SetHeader("Fit range","C");
  if (opt4) leg3->AddEntry(f1, "(0,40) kGy", "l");
  if (opt7) leg3->AddEntry(f2, "(0,80) kGy", "l");
  if (opt9) leg3->AddEntry(f3, "(0,80) kGy", "l");
  leg3->SetFillColorAlpha(0,0);
  leg3->SetLineWidth(0);
  leg3->SetTextSize(0.05);
  leg3->Draw();

  // Make plot of residuals
  c2->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0.07);
  pad2->SetBottomMargin(0.31);
  pad2->SetGridx(); // vertical grid
  pad2->SetGridy(); // horizontal grid
  pad2->Draw();
  pad2->cd();       // pad2 becomes the current pad
  c2->Update();

  double res_ps[3], eRes_ps[3];
  double res_pvt[10], eRes_pvt[10];
  double res_pvtold[3], eRes_pvtold[3];
  double res_pvtold2[1], eRes_pvtold2[1];
  double res_pvt_full[10], eRes_pvt_full[10];
  double res_pvtold_full[3], eRes_pvtold_full[3];
  double res_pvtold2_full[1], eRes_pvtold2_full[1];

  // The uncertainty of the normalized residuals is calculated by the normalized residual that used the range of the uncertainty
  // This yields +/-1 uncertainty for the residual plot
  for (int i = 0; i < 10; i++) {
    res_pvt[i] = (LYratio_A[i][nn-1]-f1->Eval(doses_A[i]))/eLYratio_pvt[i];
    eRes_pvt[i] = 1;
    res_pvt_full[i] = (LYratio_A[i][nn-1]-f2->Eval(doses_A[i]))/eLYratio_pvt[i];
    eRes_pvt_full[i] = 1;
  }
  for (int i = 0; i < 3; i++) {
    res_ps[i] = (LYratio_ps[i]-f3->Eval(doses_L[i]))/eLYratio_ps[i];
    eRes_ps[i] = 1;
    res_pvtold[i] = (LYratio_old1[i]-f1->Eval(doses_L[i]))/eLYratio_old1[i];
    eRes_pvtold[i] = 1;
    res_pvtold_full[i] = (LYratio_old1[i]-f2->Eval(doses_L[i]))/eLYratio_old1[i];
    eRes_pvtold_full[i] = 1;
  }
  for (int i = 0; i < 1; i++) {
    res_pvtold2[i] = (LYratio_old2[i]-f1->Eval(doses_old2[i]))/eLYratio_old2[i];
    eRes_pvtold2[i] = 1;
    res_pvtold2_full[i] = (LYratio_old2[i]-f2->Eval(doses_old2[i]))/eLYratio_old2[i];
    eRes_pvtold2_full[i] = 1;
  }

  TGraphErrors *resplot_ps = new TGraphErrors(nnps, doses_L, res_ps, eDoses_L, eRes_ps);
  TGraphErrors *resplot_pvt = new TGraphErrors(nnpvt, doses_A, res_pvt, eDoses_A, eRes_pvt);
  TGraphErrors *resplot_pvtold = new TGraphErrors(nnpvtold, doses_L, res_pvtold, eDoses_L, eRes_pvtold);
  TGraphErrors *resplot_pvtold2 = new TGraphErrors(1, doses_old2, res_pvtold2, eDoses_old2, eRes_pvtold2);
  TGraphErrors *resplot_pvt_full = new TGraphErrors(nnpvt, doses_A, res_pvt_full, eDoses_A, eRes_pvt_full);
  TGraphErrors *resplot_pvtold_full = new TGraphErrors(nnpvtold, doses_L, res_pvtold_full, eDoses_L, eRes_pvtold_full);
  TGraphErrors *resplot_pvtold2_full = new TGraphErrors(1, doses_old2, res_pvtold2_full, eDoses_old2, eRes_pvtold2_full);
  resplot_ps->SetMarkerColor(gdose_rateps->GetMarkerColor());
  resplot_pvt->SetMarkerColor(gdose_ratepvt->GetMarkerColor());
  resplot_pvtold->SetMarkerColor(gdose_ratepvtold->GetMarkerColor());
  resplot_pvtold2->SetMarkerColor(gdose_ratepvtold2->GetMarkerColor());
  resplot_ps->SetLineColor(gdose_rateps->GetMarkerColor());
  resplot_pvt->SetLineColor(gdose_ratepvt->GetMarkerColor());
  resplot_pvtold->SetLineColor(gdose_ratepvtold->GetMarkerColor());
  resplot_pvtold2->SetLineColor(gdose_ratepvtold2->GetMarkerColor());
  resplot_ps->SetMarkerStyle(23);
  resplot_pvt->SetMarkerStyle(22);
  resplot_pvtold->SetMarkerStyle(22);
  resplot_pvtold2->SetMarkerStyle(22);
  resplot_pvt_full->SetMarkerColor(gdose_ratepvt->GetMarkerColor());
  resplot_pvtold_full->SetMarkerColor(gdose_ratepvtold->GetMarkerColor());
  resplot_pvtold2_full->SetMarkerColor(gdose_ratepvtold2->GetMarkerColor());
  resplot_pvt_full->SetLineColor(gdose_ratepvt->GetMarkerColor());
  resplot_pvtold_full->SetLineColor(gdose_ratepvtold->GetMarkerColor());
  resplot_pvtold2_full->SetLineColor(gdose_ratepvtold2->GetMarkerColor());

  cout << gdose_ratepvt->GetMarkerColor() << "\n";
  cout << gdose_rateps->GetMarkerColor() << "\n";

  resplot_pvt_full->SetMarkerStyle(23);
  resplot_pvtold_full->SetMarkerStyle(23);
  resplot_pvtold2_full->SetMarkerStyle(23);
  TMultiGraph *resplot = new TMultiGraph();
  if (opt4) {
    resplot->Add(resplot_pvt);
    if (opt6) resplot->Add(resplot_pvtold);
    if (opt8) resplot->Add(resplot_pvtold2);
  }
  if (opt7) {
    resplot->Add(resplot_pvt_full);
    if (opt6) resplot->Add(resplot_pvtold_full);
    if (opt8) resplot->Add(resplot_pvtold2_full);
  }
  if (opt1 && opt9) {
    resplot->Add(resplot_ps);
  }
  resplot->GetXaxis()->SetLimits(0, 80);
  resplot->GetYaxis()->SetRangeUser(-10, 10);
  resplot->GetXaxis()->SetTitle("dose [kGy]");
  resplot->GetYaxis()->SetTitle("#frac{data #minus fit}{#sigma_{data}}");
  resplot->GetYaxis()->SetNdivisions(505);
  resplot->GetYaxis()->SetTitleSize(0.15);
  resplot->GetYaxis()->SetTitleFont(42);
  resplot->GetYaxis()->SetTitleOffset(0.45);
  resplot->GetYaxis()->SetLabelFont(42); // Absolute font size in pixel (precision 3)
  resplot->GetYaxis()->SetLabelSize(0.14);
  resplot->GetXaxis()->SetTitleSize(0.15);
  resplot->GetXaxis()->SetTitleFont(42);
  resplot->GetXaxis()->SetTitleOffset(1.0);
  resplot->GetXaxis()->SetLabelFont(42); // Absolute font size in pixel (precision 3)
  resplot->GetXaxis()->SetLabelSize(0.15);
  resplot->GetXaxis()->SetLabelOffset(0.02);
  resplot->Draw("AP");

  // Legend for residuals
  TLegend* leg4 = new TLegend(0.28,0.28,0.82,0.52);
  leg4->SetNColumns(2);
  TGraphErrors *bufferGraph1 = new TGraphErrors();
  TGraphErrors *bufferGraph2 = new TGraphErrors();
  bufferGraph1->SetMarkerStyle(22);
  bufferGraph2->SetMarkerStyle(23);
  if (opt4) leg4->AddEntry(bufferGraph1, "fit in (0,40) kGy", "p");
  if (opt7 || opt9) leg4->AddEntry(bufferGraph2, "fit in (0,80) kGy", "p");
  leg4->SetFillColorAlpha(0,0);
  leg4->SetLineWidth(0);
  leg4->SetTextSize(0.12);
  leg4->Draw();

  // Export plot
  TString title2 = "LYratio";
  if (opt1) title2 += "_PS";
  if (opt6 || opt8) title2 += "_oldPVT";
  if (opt4 || opt7) title2 += "_fit";
  if (opt5) c2->Print(location+title2+".pdf");

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
  gStyle->SetPalette    (kViridis);

  gROOT->ForceStyle();

  return;
}
