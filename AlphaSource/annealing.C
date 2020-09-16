#include "RootStyle.cc"

/* 
** Rods included in this number are all EJ200PS-T?_3
** which have been irradiated at 3640 Gy/hr to a total
** dose of 70 kGy.
*/
void annealing(){
  set_root_style();

  //float t1[] = {0.2087233539,0.376208645,0.4547739771,0.4864266894};
  //float t2[] = {0.2232406769,0.4119699427,0.4981510846,0.5036381999};
  //float t3[] = {0.2552055238,0.4936085638,0.5124200947,0.5178644369};
  //float t4[] = {0.2988883821,0.5254185791,0.5335273098,0.5382369678};

  float t1[]  = {4.52973,7.16921,8.86680,9.68870};
  float et1[] = {0.06518,0.13017,0.18725,0.21817};
  float t2[]  = {4.71407,7.88142,10.04115,10.16701};
  float et2[] = {0.06981,0.15424,0.23464,0.23939};
  float t3[]  = {5.15374,9.90304,10.47552,10.62995};
  float et3[] = {0.07881,0.22885,0.25369,0.26030};
  float t4[]  = {5.80160,10.87020,11.09933,11.23576};
  float et4[] = {0.09461,0.27209,0.28391,0.28967};

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

  gr1->SetMarkerStyle(20);
  gr2->SetMarkerStyle(21);
  gr3->SetMarkerStyle(22);
  gr4->SetMarkerStyle(23);
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
  mg->Draw("AP");

  TLegend* lg = new TLegend(0.45,0.2,0.7,0.4);
  lg->SetBorderSize(0);
  lg->AddEntry(gr1,"1.0 cm","lp");
  lg->AddEntry(gr2,"0.8 cm","lp");
  lg->AddEntry(gr3,"0.6 cm","lp");
  lg->AddEntry(gr4,"0.4 cm","lp");
  lg->Draw();
}
