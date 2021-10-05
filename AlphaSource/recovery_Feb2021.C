{
  Float_t ly_EJ200PS[] = {2.31407, 2.32676, 2.37285};
  Float_t ely_EJ200PS[] = {0.00386, 0.00342, 0.00355};
  Float_t ly_EJ200PVT[] = {3.32807, 3.36087, 3.38572};
  Float_t ely_EJ200PVT[] = {0.00325, 0.00288, 0.00576};
  Float_t ly_EJ260PS[] = {1.65751, 1.68114, 1.70246};
  Float_t ely_EJ260PS[] = {0.00257, 0.00190, 0.00348};
  Float_t ly_EJ260PVT[] = {2.02922, 2.11162, 2.07481};
  Float_t ely_EJ260PVT[] = {0.00305, 0.00411, 0.00346};
  Float_t days[] = {0, 10, 25};
  Float_t edays[] = {0, 0, 0};

  const size_t n = sizeof(ly_EJ200PS)/sizeof(ly_EJ200PS[0]);
  Float_t ly_EJ200PS_norm[n];
  Float_t ely_EJ200PS_norm[n];
  Float_t ly_EJ200PVT_norm[n];
  Float_t ely_EJ200PVT_norm[n];
  Float_t ly_EJ260PS_norm[n];
  Float_t ely_EJ260PS_norm[n];
  Float_t ly_EJ260PVT_norm[n];
  Float_t ely_EJ260PVT_norm[n];

  for(int i = 0; i < n; ++i) {
    ly_EJ200PS_norm[i] = ly_EJ200PS[i]/ly_EJ200PS[0];
    ely_EJ200PS_norm[i] = sqrt(pow(ely_EJ200PS[i]/ly_EJ200PS[0],2)+pow(ly_EJ200PS[i]*ely_EJ200PS[0]/pow(ly_EJ200PS[0],2),2));
    ly_EJ200PVT_norm[i] = ly_EJ200PVT[i]/ly_EJ200PVT[0];
    ely_EJ200PVT_norm[i] = sqrt(pow(ely_EJ200PVT[i]/ly_EJ200PVT[0],2)+pow(ly_EJ200PVT[i]*ely_EJ200PVT[0]/pow(ly_EJ200PVT[0],2),2));
    ly_EJ260PS_norm[i] = ly_EJ260PS[i]/ly_EJ260PS[0];
    ely_EJ260PS_norm[i] = sqrt(pow(ely_EJ260PS[i]/ly_EJ260PS[0],2)+pow(ly_EJ260PS[i]*ely_EJ260PS[0]/pow(ly_EJ260PS[0],2),2));
    ly_EJ260PVT_norm[i] = ly_EJ260PVT[i]/ly_EJ260PVT[0];
    ely_EJ260PVT_norm[i] = sqrt(pow(ely_EJ260PVT[i]/ly_EJ260PVT[0],2)+pow(ly_EJ260PVT[i]*ely_EJ260PVT[0]/pow(ly_EJ260PVT[0],2),2));
  }

  TGraphErrors* gr1 = new TGraphErrors(n,days,ly_EJ200PS_norm,edays,ely_EJ200PS_norm);
  TGraphErrors* gr2 = new TGraphErrors(n,days,ly_EJ200PVT_norm,edays,ely_EJ200PVT_norm);
  TGraphErrors* gr3 = new TGraphErrors(n,days,ly_EJ260PS_norm,edays,ely_EJ260PS_norm);
  TGraphErrors* gr4 = new TGraphErrors(n,days,ly_EJ260PVT_norm,edays,ely_EJ260PVT_norm);

  gr1->SetMarkerColor(kBlack);
  gr2->SetMarkerColor(kBlue);
  gr3->SetMarkerColor(kRed);
  gr4->SetMarkerColor(kGreen);
  gr1->SetLineColor(kBlack);
  gr2->SetLineColor(kBlue);
  gr3->SetLineColor(kRed);
  gr4->SetLineColor(kGreen);

  TMultiGraph* mgr = new TMultiGraph();
  mgr->Add(gr1);
  mgr->Add(gr2);
  mgr->Add(gr3);
  mgr->Add(gr4);
  mgr->Draw("AP*");

  mgr->GetXaxis()->SetTitle("days");
  mgr->GetYaxis()->SetTitle("LY/LY(day 0)");

  TLegend* lg = new TLegend(0.15,0.55,0.45,0.85);
  lg->SetBorderSize(0);
  lg->AddEntry(gr1,"EJ200PS_L3R","lp");
  lg->AddEntry(gr2,"EJ200PVT_L3R","lp");
  lg->AddEntry(gr3,"EJ260PS_L3R","lp");
  lg->AddEntry(gr4,"EJ260PVT_L3R","lp");
  lg->Draw();
}
