#include "RootStyle.cc"

using namespace std;

void annealing_curves_normalization() {
  set_root_style();
  TString filename = "annealing_PS_single";
  Bool_t useGray = true;
  Bool_t useLogx = false;
  Bool_t printOn = false;

  float dcost[] = {6.66820, 6.72682, 7.03043, 7.16182, 8.38714, 9.72453, 10.21303, 10.32367, 10.30835, 10.10144};
  float edcost[] = {0.21543, 0.21873, 0.23783, 0.24634, 0.33181, 0.44185,  0.48460,  0.49480,  0.49379,  0.47453};
  float days[] = {0.25, 1, 2, 3, 11, 20, 32, 39, 46, 73};
  float edays[] = {0,0,0,0,0,0,0,0,0};
  float irrDet[] = {6.9, 13, 0.012};
  float lastDcost[] = {10.10144, 0.47453};


  size_t nn = sizeof(days)/sizeof(days[0]);
  if (useGray) {
    for(int j = 0; j < nn; ++j) {
      dcost[j]*=10; // Mrad to kGy
      edcost[j]*=10; // Mrad to kGy
    }
  }

  TCanvas *canv1 = new TCanvas("canv1","annealing curves",200,10,800,600);
  canv1->SetFillColor(0);
  TGraphErrors *graph1 = new TGraphErrors(nn, days, dcost, edays, edcost);
  graph1->SetTitle();
  graph1->Draw("AP");
  graph1->GetXaxis()->SetTitle("Days after irr.");
  graph1->GetYaxis()->SetTitle("Dose Constant (Mrad)");
  if (useGray) {
    graph1->GetYaxis()->SetTitle("Dose Constant (kGy)");
  }
  graph1->GetYaxis()->SetTitleOffset(1.2);
  graph1->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  graph1->SetMinimum(0.0);
  graph1->SetMaximum(16.0);
  double xmin1 = (useLogx) ? 0.1 : 0.0;
  double xmax1 = (useLogx) ? 200 : 120;
  if (useGray) {
    graph1->SetMinimum(0.0);
    graph1->SetMaximum(150.0);
  }
  graph1->GetXaxis()->SetLimits(xmin1,xmax1);

  canv1->Update();
  if (useLogx) filename+="_logx";
  if (printOn) canv1->SaveAs("paperPlots/ver17/"+filename+".pdf");

  // Find positions of max and min elements
  size_t max_i = distance(dcost, max_element(dcost, dcost + nn));
  size_t min_i = distance(dcost, min_element(dcost, dcost + nn));
  float diff = dcost[max_i] - dcost[min_i];

  float days_n[nn];
  float edays_n[nn];
  float dcost_n1[nn];
  float edcost_n1[nn];
  float dcost_n2[nn];
  float edcost_n2[nn];

  cout << dcost[max_i] << ", " << max_i << ", " << dcost[min_i] << ", " << min_i << endl;

  for(int j = 0; j < nn; ++j) {
    days_n[j] = days[j]/days[max_i];
    edays_n[j] = 0.0;
    dcost_n1[j] = dcost[j]/dcost[max_i];
    edcost_n1[j] = sqrt(pow(edcost[j]/dcost[max_i],2)+pow(dcost[j]*edcost[max_i]/pow(dcost[max_i],2),2));
    dcost_n2[j] = (dcost[j]-dcost[min_i])/diff;
    edcost_n2[j] = sqrt(pow(edcost[j]/diff,2)+pow((dcost[j]-dcost[min_i])*edcost[max_i]/pow(diff,2),2)+pow((dcost[j]-dcost[max_i])*edcost[min_i]/pow(diff,2),2));

    cout << days[j] << ", " << dcost_n2[j] << endl;
  }

  // Now plot thenm
  TCanvas *canv2 = new TCanvas("canv2","annealing curves",200,10,800,600);
  canv2->SetFillColor(0);
  TGraphErrors *graph2 = new TGraphErrors(nn, days_n, dcost_n1, edays_n, edcost_n1);
  graph2->SetTitle();

  graph2->Draw("AP");
  graph2->GetXaxis()->SetTitle("normalized time");
  graph2->GetYaxis()->SetTitle("normalized D");
  graph2->GetYaxis()->SetTitleOffset(1.2);
  graph2->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  graph2->SetMinimum(0.0);
  graph2->SetMaximum(1.0);
  double xmin2 = 0;
  double xmax2 = 1;
  graph2->GetXaxis()->SetLimits(xmin2,xmax2);

  canv2->Update();

  // Now plot thenm
  TCanvas *canv3 = new TCanvas("canv3","annealing curves",200,10,800,600);
  canv3->SetFillColor(0);
  TGraphErrors *graph3 = new TGraphErrors(nn, days_n, dcost_n2, edays_n, edcost_n2);
  graph3->SetTitle();

  graph3->Draw("AP");
  graph3->GetXaxis()->SetTitle("normalized time");
  graph3->GetYaxis()->SetTitle("normalized D");
  graph3->GetYaxis()->SetTitleOffset(1.2);
  graph3->GetXaxis()->SetTitleOffset(1.3);

  // NEED TO TUNE THESE:
  graph3->SetMinimum(0.0);
  graph3->SetMaximum(1.0);
  double xmin3 = 0;
  double xmax3 = 1;
  graph3->GetXaxis()->SetLimits(xmin3,xmax3);

  TF1 *f1 = new TF1("f1","[0]*tanh([1]*x)",0,1);
  f1->SetParameters(1,1);
  graph3->Fit("f1");
  canv3->Update();
}
