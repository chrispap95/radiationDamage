#include "indexMeas.C"
#include "RootStyle.cc"

struct measurement {
  TString rodNumber;
  vector<float> theta; // Angle of rotation in degrees
  vector<float> delta; // Delta - deviation of optical path in mm
  vector<float> etheta; // Uncertainty of angle in degrees
  vector<float> edelta; // Uncertainty of delta in mm
  vector<float> dose; // Total dose in Mrad and uncertainty in %
  vector<float>  doseRate; // Dose rate in krad/hr and uncertainty in %
  vector<float>  depth; // Depth of index change in mm and uncertainty in mm
};

void indexLoop() {
  set_root_style();

  vector<measurement> measurements;

  vector<float> stdErrTheta = {1, 1, 1, 1, 1, 1};
  vector<float> stdErrDelta = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25};

  measurements.push_back(
    {
      "EJ200PS_1X1P_L10R",
      {  0,  15,  30,  45,  60,  75},
      {0.0, 1.0, 2.0, 3.5, 5.5, 8.0},
      stdErrTheta,
      stdErrDelta,
      {6.90, 0.05},
      {46.0, 0.05},
      {1.25, 0.05}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_1",
      {  0,  15,  30,  45,  60,  75},
      {0.5, 1.5, 2.5, 4.0, 6.0, 8.5},
      stdErrTheta,
      stdErrDelta,
      {0.0,  0.0},
      {0.0,  0.0},
      {0.0, 0.05}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_4",
      {  0,  15,  30,  45,  60,  75},
      {0.5, 1.5, 2.5, 3.5, 6.0, 8.0},
      stdErrTheta,
      stdErrDelta,
      {1.32, 0.1},
      {0.22, 0.1},
      {5.00, 0.2}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_5",
      {  0,  15,  30,  45,  60,  75},
      {0.0, 1.0, 2.0, 3.5, 5.0, 7.5},
      stdErrTheta,
      stdErrDelta,
      {7.00, 0.012},
      {8.53, 0.012},
      {3.00, 0.1}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_9",
      {  0,  15,  30,  45,  60,  75},
      {0.0, 1.0, 2.0, 3.5, 5.5, 8.0},
      stdErrTheta,
      stdErrDelta,
      {7.0, 0.012},
      {390, 0.012},
      {0.4, 0.1}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_13",
      {  0,  15,  30,  45,  60,  75},
      {0.0, 1.0, 2.0, 3.5, 5.0, 8.0},
      stdErrTheta,
      stdErrDelta,
      {4.20, 0.1},
      {0.98, 0.1},
      {5.00, 0.2}
    }
  );

  measurements.push_back(
    {
      "EJ200PS_1X1P_15",
      {  0,  15,  30,  45,  60,  75},
      {0.0, 1.0, 2.0, 3.5, 5.0, 7.5},
      stdErrTheta,
      stdErrDelta,
      {1.26, 0.1},
      {0.31, 0.1},
      {5.00, 0.2}
    }
  );

  size_t nn = measurements.size();
  float depth[nn];
  float n1[nn];
  float n2[nn];
  float edepth[nn];
  float en1[nn];
  float en2[nn];

  gStyle->SetOptStat(0);
  gStyle->SetOptFit();

  // Find how many canvases are needed and create them
  int nCanv = measurements.size()/4;
  if (nCanv%4 != 0) nCanv++;
  TCanvas* c[nCanv];
  for (int i = 0; i < nCanv; ++i) {
    char canvName[] = {(char)i};
    c[i] = new TCanvas(canvName,canvName,1000,800);
    c[i]->Divide(2,2);
  }

  int j = 0;
  for (vector<measurement>::iterator i = measurements.begin(); i!=measurements.end();++i){
    if(j%4 == 0) c[j/4]->cd(j%4+1);
    else if(j%4 == 1) c[j/4]->cd(j%4+1);
    else if(j%4 == 2) c[j/4]->cd(j%4+1);
    else if(j%4 == 3) c[j/4]->cd(j%4+1);

    // check sanity of input
    unsigned int n = i->theta.size();
    if (i->delta.size() !=n ||
    i->etheta.size()!=n ||
    i->edelta.size()!=n ) {
      cout << "Check inputs in sample: " << i->rodNumber << endl;
      continue;
    }
    vector<float> theta_temp;
    vector<float> delta_temp;
    vector<float> etheta_temp;
    vector<float> edelta_temp;
    for (int k = 0; k < n; ++k) {
      theta_temp.push_back(i->theta[k]);
      delta_temp.push_back(i->delta[k]);
      etheta_temp.push_back(i->etheta[k]);
      edelta_temp.push_back(i->edelta[k]);
    }
    std::vector<float> temp = indexMeas(
      theta_temp.size(),
      theta_temp.data(),
      delta_temp.data(),
      etheta_temp.data(),
      edelta_temp.data(),
      i->depth[0]
    );
    n1[j] = temp[0];
    en1[j] = temp[1];
    n2[j] = temp[2];
    en2[j] = temp[3];
    depth[j] = i->depth[0];
    edepth[j] = i->depth[1];
    j++;
  }

  // Plot and fit the resolutioin vs energy
  TCanvas* c_res = new TCanvas("c_res","c_res",1);
  TGraphErrors *gr1 = new TGraphErrors(nn, depth, n1, edepth, en1);
  TGraphErrors *gr2 = new TGraphErrors(nn, depth, n2, edepth, en2);
  gr1->SetMarkerColor(4);
  gr1->SetMarkerStyle(21);
  gr2->SetMarkerColor(2);
  gr2->SetMarkerStyle(22);

  TMultiGraph* allgraphs = new TMultiGraph();
  allgraphs->Add(gr1);
  allgraphs->Add(gr2);

  TString histname = ";depth [mm];n";
  allgraphs->SetTitle(histname);
  allgraphs->Draw("AP");

  allgraphs->SetMinimum(1.0);
  allgraphs->SetMaximum(3.0);
  double xmin = -0.5;
  double xmax = 5.5;
  allgraphs->GetXaxis()->SetLimits(xmin,xmax);

  TLegend *legend = new TLegend(0.2,0.2);
  legend->SetBorderSize(0);
  legend->SetFillColorAlpha(0,0);
  legend->SetTextSize(0.04);
  legend->AddEntry(gr1,"n1","p");
  legend->AddEntry(gr2,"n2","p");
  legend->Draw();
}
