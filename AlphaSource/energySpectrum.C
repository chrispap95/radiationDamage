#include "RootStyle.cc"

void energySpectrum(bool logY = 0) {
  set_root_style();

  const Int_t Number = 3;
  Double_t Red[Number] = {1.00, 0.00, 0.00};
  Double_t Green[Number] = {0.00, 1.00, 0.00};
  Double_t Blue[Number] = {0.00, 0.00, 1.00};
  Double_t Length[Number] = {0.90, 1.00, 0.9};
  Int_t nb = 9;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

  /*
  TFile* fDark = TFile::Open("root/AlphaSource.nosync/DarkCurrent_HV1700_AutoTrigger_50.5mV_amp101mVpDiv_20161203.root");
  TFile* fPre = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N1_Default_Nofoil_FaceA_FastFrame_20161011.root");
  TFile* f0 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161109.root");
  TFile* f1 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161110.root");
  TFile* f8 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161117.root");
  TFile* f14 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161123.root");
  TFile* f21 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161130.root");
  TFile* f28 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20161207.root");
  TFile* f99 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PVT-1X1P_N5_Default_Nofoil_FaceA_FastFrame_20170216.root");
  */

  TFile* fDark = TFile::Open("root/AlphaSource.nosync/DarkCurrent_HV1700_trigger_101mV_amp50p5mVpDiv_20181121.root");
  TFile* fPre = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_20170109.root");
  TFile* f0 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_20170413.root");
  //TFile* f1 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_201.root");
  //TFile* f8 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_201.root");
  TFile* f14 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_20170427.root");
  //TFile* f21 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_201.root");
  TFile* f28 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_20170511.root");
  TFile* f99 = TFile::Open("root/AlphaSource.nosync/Pu239new_EJ200PS-T1_3_Default_Nofoil_FaceA_FastFrame_20170608.root");

  TTree* tDark = (TTree*)fDark->Get("tree");
  TTree* tPre = (TTree*)fPre->Get("tree");
  TTree* t0 = (TTree*)f0->Get("tree");
  //TTree* t1 = (TTree*)f1->Get("tree");
  //TTree* t8 = (TTree*)f8->Get("tree");
  TTree* t14 = (TTree*)f14->Get("tree");
  //TTree* t21 = (TTree*)f21->Get("tree");
  TTree* t28 = (TTree*)f28->Get("tree");
  TTree* t99 = (TTree*)f99->Get("tree");

  TH1F* hDark = new TH1F("hDark","hDark",256,-1,8);
  TH1F* hPre = new TH1F("hPre","hPre",256,-1,8);
  TH1F* h0 = new TH1F("h0","h0",256,-1,8);
  //TH1F* h1 = new TH1F("h1","h1",256,-1,8);
  //TH1F* h8 = new TH1F("h8","h8",256,-1,8);
  TH1F* h14 = new TH1F("h14","h14",256,-1,8);
  //TH1F* h21 = new TH1F("h21","h21",256,-1,8);
  TH1F* h28 = new TH1F("h28","h28",256,-1,8);
  TH1F* h99 = new TH1F("h99","h99",256,-1,8);

  tDark->Draw("area*1.e9>>hDark","","");
  tPre->Draw("area*1.e9>>hPre","","");
  t0->Draw("area*1.e9>>h0","","");
  //t1->Draw("area*1.e9>>h1","","");
  //t8->Draw("area*1.e9>>h8","","");
  t14->Draw("area*1.e9>>h14","","");
  //t21->Draw("area*1.e9>>h21","","");
  t28->Draw("area*1.e9>>h28","","");
  t99->Draw("area*1.e9>>h99","","");

  if (logY) hDark->Scale(hPre->Integral()/hDark->Integral());
  else {
    hDark->Scale(2.8/hDark->Integral());
    hPre->Scale(20./hPre->Integral());
    h0->Scale(8./h0->Integral());
    //h1->Scale(18./h1->Integral());
    //h8->Scale(18./h8->Integral());
    h14->Scale(12./h14->Integral());
    //h21->Scale(18./h21->Integral());
    h28->Scale(12./h28->Integral());
    h99->Scale(12./h99->Integral());
  }

  cout << "hDark area: " << hDark->Integral() << "\n";
  cout << "hPre area: " << hPre->Integral() << "\n";
  cout << "h0 area: " << h0->Integral() << "\n";
  //cout << "h1 area: " << h1->Integral() << "\n";
  //cout << "h8 area: " << h8->Integral() << "\n";
  cout << "h14 area: " << h14->Integral() << "\n";
  //cout << "h21 area: " << h21->Integral() << "\n";
  cout << "h28 area: " << h28->Integral() << "\n";
  cout << "h99 area: " << h99->Integral() << "\n";

  hPre->SetLineStyle(1);
  hDark->SetLineStyle(2);
  h0->SetLineStyle(3);
  //h1->SetLineStyle(4);
  //h8->SetLineStyle(5);
  h14->SetLineStyle(6);
  //h21->SetLineStyle(7);
  h28->SetLineStyle(8);
  h99->SetLineStyle(9);

  hDark->SetLineWidth(2);
  hPre->SetLineWidth(2);
  h0->SetLineWidth(2);
  //h1->SetLineWidth(2);
  //h8->SetLineWidth(2);
  h14->SetLineWidth(2);
  //h21->SetLineWidth(2);
  h28->SetLineWidth(2);
  h99->SetLineWidth(2);
  TF1* f1 = new TF1("f1","gaus");
  h99->Fit("f1");

  TCanvas* c = new TCanvas("c","c",1);
  if (logY) c->SetLogy();

  THStack* hs1 = new THStack();
  THStack* hs = new THStack();
  if (logY) hs->SetMaximum(1.e6);
  else {
    hs->SetMaximum(1.e0);
    TGaxis::SetMaxDigits(3);
  }
  hs->Add(hPre);
  hs->Add(hDark);
  hs1->Add(h0);
  //hs1->Add(h1);
  //hs1->Add(h8);
  hs1->Add(h14);
  // hs1->Add(h21);
  hs1->Add(h28);
  hs1->Add(h99);
  hs->Draw("nostack hist");
  hs1->Draw("plc nostack hist same");
  hs->SetTitle(";Energy (V#times ns);arbitrary units");
  hDark->SetLineColor(16);
  c->Update();

  TLegend* lg1;
  if (logY) lg1 = new TLegend(0.3,0.74,0.6,0.86);
  else lg1 = new TLegend(0.3,0.75,0.6,0.86);
  lg1->SetBorderSize(0);
  lg1->AddEntry(hDark,"Pedestal","l");
  lg1->AddEntry(hPre,"Non-irradiated","l");
  lg1->Draw();

  TLegend* lg2;
  if (logY) lg2 = new TLegend(0.6,0.67,0.89,0.92);
  else lg2 = new TLegend(0.6,0.62,0.89,0.92);
  lg2->SetBorderSize(0);
  lg2->AddEntry(h0,"0 days after irr.","l");
  //lg2->AddEntry(h1,"1 day after irr.","l");
  //lg2->AddEntry(h8,"8 days after irr.","l");
  lg2->AddEntry(h14,"14 days after irr.","l");
  //lg2->AddEntry(h21,"21 days after irr.","l");
  lg2->AddEntry(h28,"28 days after irr.","l");
  lg2->AddEntry(h99,"56 days after irr.","l");
  lg2->Draw();

  f1->Draw("same");

}
