
// Improve styling
void set_root_style();

void sandiaRods()
{
  set_root_style();
  double days[] = {1,4,9,16,23};
  const int nn = sizeof(days)/sizeof(days[0]);

  // Shift 1 applies to rods A[1-5] and L6R
  double pedestal[2][nn] = {
    {-0.11678, -0.1196, -0.12718, -0.1222,(-0.1077-0.1161)/2}, // shift 1
    {-0.129, -0.10972, -0.1001, -0.09508}    // shift 2
  };
  double referen_rod[2][nn] = {
    {5.56248, 5.622990, 5.60593, 5.626096, (5.607507+5.625880)/2},          // shift 1
    {(5.622+5.61116)/2, 5.57485, 5.604418, 5.57525} // shift 2
  };

  double A[10][nn] = {
    {4.87787, 4.967149, 5.00011, 5.007314, 5.005909}, // A1
    {4.70806, 4.744589, 4.81308, 4.799527, 4.798452},
    {4.58580, 4.593320, 4.64826, 4.649427, 4.624193},
    {4.05188, 4.144884, 4.25546, 4.196091, 4.157400},
    {4.17720, 4.203350, 4.29741, 4.265709, 4.213584},
    {3.186, 3.48283,3.683851, 3.71112},     // A6
    {3.187, 3.24250, 3.287365, 3.29590},
    {3.47710, 3.50197, 3.517087, 3.55820},
    {2.963, 3.00163, 3.005944, 3.05093},
    {2.87, 2.92600, 2.931136, 2.97247}      // A10
  };

  double L[3][nn] = {
    {3.14161,3.19091, 3.203634, 3.25110}, // L5R
    {2.32,2.410936, 2.46946, 2.439731, 2.428170},    // L6R
    {1.922,2.02097, 2.038967, 2.030258, 2.022143},    // L11R
  };

  int doses_A[10] = {8, 12, 16, 20, 24, 33, 42, 46, 58, 70};
  int doses_L[3] = {24, 46, 70};

  TCanvas *c1 = new TCanvas();
  TMultiGraph *gm = new TMultiGraph();
  TGraph* gA[10];
  TGraph* gL[3];
  auto leg = new TLegend(0.7,0.2,0.88,0.55);

  for (int i = 0; i < nn; i++) {
    for (int j = 0; j < 10; j++) {
      if (j < 5) A[j][i]= (A[j][i]-pedestal[0][i])/(referen_rod[0][i]-pedestal[0][i]);
      else A[j][i]= (A[j][i]-pedestal[1][i])/(referen_rod[1][i]-pedestal[1][i]);
    }
    for (int j = 0; j < 3; j++) {
      if (j == 1) L[j][i]= (L[j][i]-pedestal[0][i])/(referen_rod[0][i]-pedestal[0][i]);
      else L[j][i]= (L[j][i]-pedestal[1][i])/(referen_rod[1][i]-pedestal[1][i]);
    }
  }

  for (int i = 0; i < 10; i++) {
    gA[i] = new TGraph(nn, days, A[i]);
    gA[i]->SetMarkerStyle(22);
    TString title = "A"+to_string(i+1)+" - "+to_string(doses_A[i])+" kGy";
    gA[i]->SetTitle(title);
    gm->Add(gA[i]);
    leg->AddEntry(gA[i], title, "lp");
  }
  for (int i = 0; i < 3; i++) {
    gL[i] = new TGraph(nn, days, L[i]);
    gL[i]->SetMarkerStyle(22);
    TString title;
    if (i == 0) title = "L"+to_string(5)+"R - "+to_string(doses_L[i])+" kGy";
    else if (i == 1) title = "L"+to_string(6)+"R - "+to_string(doses_L[i])+" kGy";
    else title = "L"+to_string(11)+"R - "+to_string(doses_L[i])+" kGy";
    gL[i]->SetTitle(title);
    gm->Add(gL[i]);
    leg->AddEntry(gL[i], title, "lp");
  }

  double maxY = A[0][nn-1] > 1 ? A[0][nn-1]*1.05 : 1;
  gm->GetXaxis()->SetLimits(0, days[nn-1]+4);
  gm->GetYaxis()->SetRangeUser(0, maxY);
  gm->GetXaxis()->SetTitle("days after irr.");
  gm->GetYaxis()->SetTitle("LY/LY_{reference}");
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
  t->Draw();




    TCanvas *c2 = new TCanvas();
    double doses_pvtx[10],doses_pvty[10];
    double doses_psx[3],doses_psy[3];
    // Make it plot the latest measurements
    for (int i = 0; i < 10; i++) {
        doses_pvtx[i]=doses_A[i];
        doses_pvty[i]=A[i][nn-1];
    }
    for (int i = 0; i < 3; i++) {
        doses_psx[i]=doses_L[i];
        doses_psy[i]=L[i][nn-1];
    }
    const int nnpvt = sizeof(doses_pvtx)/sizeof(doses_pvtx[0]);
    const int nnps = sizeof(doses_psx)/sizeof(doses_psx[0]);
    TGraph *gdose_ratepvt = new TGraph(nnpvt, doses_pvtx, doses_pvty);
    TGraph *gdose_rateps = new TGraph(nnps, doses_psx, doses_psy);
    gdose_rateps->SetMarkerStyle(22);
    gdose_ratepvt->SetMarkerStyle(22);
    gdose_rateps->SetTitle("PS");
    gdose_ratepvt->SetTitle("PVT");

    auto leg2 = new TLegend(0.2,0.2,0.4,0.3);
    leg2->AddEntry(gdose_rateps, "PS", "lp");
    leg2->AddEntry(gdose_ratepvt, "PVT", "lp");

    TMultiGraph *gm2 = new TMultiGraph();
    gm2->Add(gdose_rateps);
    gm2->Add(gdose_ratepvt);
    gm2->GetXaxis()->SetLimits(0, 80);
    gm2->GetYaxis()->SetRangeUser(0, 1.0);
    gm2->GetXaxis()->SetTitle("dose [kGy]");
    gm2->GetYaxis()->SetTitle("LY/LY_{reference}");
    gm2->Draw("AP PLC PMC");

    leg2->SetLineWidth(0);
    leg2->SetTextSize(0.03);
    leg2->Draw();


    TPaveText *t2 = new TPaveText(0.35, 0.92, 0.6, 1.0, "brNDC"); // left-up
    TString txt2 = "day " + to_string((int)days[nn-1]);
    t2->AddText(txt2);
    t2->SetTextSize(0.05);
    t2->SetBorderSize(0);
    t2->SetFillColor(0);
    t2->SetFillStyle(0);
    t2->SetTextFont(42);
    t2->Draw();

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
