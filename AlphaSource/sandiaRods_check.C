
// Improve styling
void set_root_style();

void sandiaRods_check()
{
  set_root_style();
  double days[] = {1,10,25,32};
  const int nn = sizeof(days)/sizeof(days[0]);

  double pedestal[nn] = {
    -0.13022, -0.12958, -0.11066, -0.11698
  };
  double referen_rod[nn] = {
    5.62246,5.58650,5.60921,5.58462
  };

  double L[nn] = {
    3.32807,3.36087,3.38572,3.38832 //L3R
  };

  TCanvas *c1 = new TCanvas();
  TMultiGraph *gm = new TMultiGraph();
  TGraph* gL;
  auto leg = new TLegend(0.7,0.2,0.88,0.55);

  for (int i = 0; i < nn; i++) {
    L[i]= (L[i]-pedestal[i])/(referen_rod[i]-pedestal[i]);
  }

  gL = new TGraph(nn, days, L);
  gL->SetMarkerStyle(22);
  TString title;
  title = "L"+to_string(3)+"R - "+to_string(49)+" kGy";
  gL->SetTitle(title);
  gm->Add(gL);
  leg->AddEntry(gL, title, "lp");

  double maxY = L[nn-1] > 1 ? L[nn-1]*1.05 : 1;
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
