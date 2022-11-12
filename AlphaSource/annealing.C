#include "RootStyle.cc"

/*
** Rods included in this number are all EJ200PS-T?_3
** which have been irradiated at 3640 Gy/hr to a total
** dose of 70 kGy.
*/
void annealing(){
  set_root_style();

  TCanvas* c1 = new TCanvas("c1","c1",200,10,800,600);
  gStyle->SetPalette(kViridis);

  //float t1[] = {0.2087233539,0.376208645,0.4547739771,0.4864266894};
  //float t2[] = {0.2232406769,0.4119699427,0.4981510846,0.5036381999};
  //float t3[] = {0.2552055238,0.4936085638,0.5124200947,0.5178644369};
  //float t4[] = {0.2988883821,0.5254185791,0.5335273098,0.5382369678};

  float t1[]  = {4.52866,7.16445,8.87806,9.69117};
  float et1[] = {0.10034,0.22718,0.34076,0.40258};
  float t2[]  = {4.71636,7.89244,10.02938,10.16321};
  float et2[] = {0.10777,0.27407,0.43299,0.44414};
  float t3[]  = {5.14493,9.90061,10.48171,10.62077};
  float et3[] = {0.12609,0.42441,0.47390,0.48609};
  float t4[]  = {5.80824,10.85800,11.12238,11.25958};
  float et4[] = {0.15680,0.50957,0.53411,0.54656};

  float days[] = {0,14,28,56};
  float edays[] = {0,0,0,0};

  if (1) {
    for (int i = 0; i<4; ++i){
      t1[i]*=10000;
      et1[i]*=10000;
      t2[i]*=10000;
      et2[i]*=10000;
      t3[i]*=10000;
      et3[i]*=10000;
      t4[i]*=10000;
      et4[i]*=10000;
    }
  }

  TGraphErrors* gr1 = new TGraphErrors(4,days,t1,edays,et1);
  TGraphErrors* gr2 = new TGraphErrors(4,days,t2,edays,et2);
  TGraphErrors* gr3 = new TGraphErrors(4,days,t3,edays,et3);
  TGraphErrors* gr4 = new TGraphErrors(4,days,t4,edays,et4);
  gr1->SetMarkerSize(1.4);
  gr2->SetMarkerSize(1.4);
  gr3->SetMarkerSize(1.4);
  gr4->SetMarkerSize(1.4);

  gr1->SetMarkerStyle(20);
  gr2->SetMarkerStyle(21);
  gr3->SetMarkerStyle(33);
  gr4->SetMarkerStyle(34);
  gr1->SetMarkerColor(1);
  gr2->SetMarkerColor(2);
  gr3->SetMarkerColor(3);
  gr4->SetMarkerColor(4);
  gr1->SetLineColor(1);
  gr2->SetLineColor(2);
  gr3->SetLineColor(3);
  gr4->SetLineColor(4);

  TMultiGraph* mg = new TMultiGraph();
  mg->Add(gr1);
  mg->Add(gr2);
  mg->Add(gr3);
  mg->Add(gr4);
  mg->SetTitle(";Days after irr. end date;Dose Constant (Gy)");
  mg->Draw("AP PLC PMC");

  TLegend* lg = new TLegend(0.45,0.2,0.7,0.4);
  lg->SetBorderSize(0);
  lg->SetFillColor(0);
  lg->SetTextSize(0.04);
  lg->AddEntry(gr1,"10 mm","p");
  lg->AddEntry(gr2, "8 mm","p");
  lg->AddEntry(gr3, "6 mm","p");
  lg->AddEntry(gr4, "4 mm","p");
  lg->Draw();

  c1->Print("paperPlots/ver17/fig10t_annealing_dcost.pdf");
}
