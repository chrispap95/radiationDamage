#include "RootStyle.cc"

/*
** Rods included in this number are all EJ200PS-T?_3
** which have been irradiated at 3640 Gy/hr to a total
** dose of 70 kGy.
*/

void annealing_inverse(){
  set_root_style();

  float d0[]   = {4.52973,4.71407,5.15374,5.80160};
  float ed0[]  = {0.06518,0.06981,0.07881,0.09461};
  float d14[]  = {7.16921,7.88142,9.90304,10.87020};
  float ed14[] = {0.13017,0.15424,0.22885,0.27209};
  float d28[]  = {8.86680,10.04115,10.47552,11.09933};
  float ed28[] = {0.18725,0.23464,0.25369,0.28391};
  float d56[]  = {9.68870,10.16701,10.62995,11.23576};
  float ed56[] = {0.21817,0.23939,0.26030,0.28967};

  float thickness[]  = {1.0,0.8,0.6,0.4};
  float ethickness[] = {0.0,0.0,0.0,0.0};

  if (1) {
    for (int i = 0; i<4; ++i){
      d0[i]*=10000;
      ed0[i]*=10000;
      d14[i]*=10000;
      ed14[i]*=10000;
      d28[i]*=10000;
      ed28[i]*=10000;
      d56[i]*=10000;
      ed56[i]*=10000;

      d0[i]  = 1/d0[i];
      ed0[i] = 1/pow(ed0[i],2);
      d14[i]  = 1/d14[i];
      ed14[i] = 1/pow(ed14[i],2);
      d28[i]  = 1/d28[i];
      ed28[i] = 1/pow(ed28[i],2);
      d56[i]  = 1/d56[i];
      ed56[i] = 1/pow(ed56[i],2);
    }
  }

  TGraphErrors* gr1 = new TGraphErrors(4,thickness,d0,ethickness,ed0);
  TGraphErrors* gr2 = new TGraphErrors(4,thickness,d14,ethickness,ed14);
  TGraphErrors* gr3 = new TGraphErrors(4,thickness,d28,ethickness,ed28);
  TGraphErrors* gr4 = new TGraphErrors(4,thickness,d56,ethickness,ed56);

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
  mg->SetTitle(";Thickness (cm);D^{-1} (Gy^{-1})");
  mg->SetMaximum(3e-5);
  mg->SetMinimum(5e-6);
  mg->Draw("AP");

  TLegend* lg = new TLegend(0.45,0.2,0.7,0.4);
  lg->SetBorderSize(0);
  lg->AddEntry(gr1,"0 days","lp");
  lg->AddEntry(gr2,"14 days","lp");
  lg->AddEntry(gr3,"28 days","lp");
  lg->AddEntry(gr4,"56 days","lp");
  lg->Draw();

  TF1* f2 = new TF1("f2","pol1");
  TF1* f3 = new TF1("f3","pol1");
  TF1* f4 = new TF1("f4","pol1");
  f2->SetLineColor(2);
  f3->SetLineColor(3);
  f4->SetLineColor(4);
  cout << "\nFit for 14 days:";
  gr2->Fit("f2");
  cout << "\n" << f2->GetParameter(0);
  cout << "\nFit for 28 days:";
  gr3->Fit("f3");
  cout << "\nFit for 56 days:";
  gr4->Fit("f4");

  cout << "\n****************************";
  cout << "\nf(d) for 14 days is: ";
  cout << exp(-70000*f2->GetParameter(0));
  cout << " +/- ";
  cout << exp(-70000*f2->GetParameter(0))*sqrt(pow(70000*f2->GetParError(0),2)+pow(910*f2->GetParameter(0),2));
  cout << "\n****************************";
  cout << "\nf(d) for 28 days is: ";
  cout << exp(-70000*f3->GetParameter(0));
  cout << " +/- ";
  cout << exp(-70000*f3->GetParameter(0))*sqrt(pow(70000*f3->GetParError(0),2)+pow(910*f3->GetParameter(0),2));
  cout << "\n****************************";
  cout << "\nf(d) for 56 days is: ";
  cout << exp(-70000*f4->GetParameter(0));
  cout << " +/- ";
  cout << exp(-70000*f4->GetParameter(0))*sqrt(pow(70000*f4->GetParError(0),2)+pow(910*f4->GetParameter(0),2));
  cout << "\n****************************";

}
