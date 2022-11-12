#include <TMath.h>

std::vector<float> indexMeas(size_t nn, float* theta, float* delta, float* etheta, float* edelta, float depth) {
  const float side_size = 10; //in mm
  const float tolerance = 0.00001;
  const float degToRad = TMath::Pi()/180.;

  TGraphErrors* gr = new TGraphErrors(nn, theta, delta, etheta, edelta);
  gr->Draw("AP");
  TString histname = ";#theta [degrees]; #delta [mm]";
  gr->SetTitle(histname);

  TF1* f1;
  TFitResultPtr r;
  std::vector<float> output_vector;
  float a1  = 0;
  float a1e = 0;
  float a2  = 0;
  float a2e = 0;

  if (depth < 0.01) {
    f1 = new TF1(
      "delta",
      "[0]*sin(x*TMath::Pi()/180)*(1-cos(x*TMath::Pi()/180)/sqrt([1]**2-(sin(x*TMath::Pi()/180))**2))"
    );
    f1->SetParNames("d","n2");
    f1->SetParLimits(0,side_size-tolerance,side_size+tolerance);
    f1->SetParLimits(1,1,3);
    r = gr->Fit("delta","S","");
    a1  = 0;
    a1e = 0;
    a2  = r->Parameter(1);
    a2e = r->ParError(1);
  }else if (abs(side_size-2*depth) < 0.01) {
    f1 = new TF1(
      "delta",
      "[0]*sin(x*TMath::Pi()/180)*(1-cos(x*TMath::Pi()/180)/sqrt([1]**2-(sin(x*TMath::Pi()/180))**2))"
    );
    f1->SetParNames("d","n1");
    f1->SetParLimits(0,side_size-tolerance,side_size+tolerance);
    f1->SetParLimits(1,1,3);
    r = gr->Fit("delta","S","");
    a1  = r->Parameter(1);
    a1e = r->ParError(1);
    a2  = 0;
    a2e = 0;
  }else {
    f1 = new TF1(
      "delta",
      "[0]*sin(x*TMath::Pi()/180)*(1-cos(x*TMath::Pi()/180)/sqrt([3]**2-(sin(x*TMath::Pi()/180))**2))+"
      "2*[1]*sin(x*TMath::Pi()/180)*cos(x*TMath::Pi()/180)*(1/sqrt([3]**2-(sin(x*TMath::Pi()/180))**2)-"
      "1/sqrt([2]**2-(sin(x*TMath::Pi()/180))**2))"
    );
    f1->SetParNames("d","depth","n1","n2");
    f1->SetParLimits(0,side_size-tolerance,side_size+tolerance);
    f1->SetParLimits(1,depth-tolerance,depth+tolerance);
    f1->SetParLimits(2,1,3);
    f1->SetParLimits(3,1,3);
    r = gr->Fit("delta","S","");
    a1  = r->Parameter(2);
    a1e = r->ParError(2);
    a2  = r->Parameter(3);
    a2e = r->ParError(3);
  }

  output_vector.push_back(a1);
  output_vector.push_back(a1e);
  output_vector.push_back(a2);
  output_vector.push_back(a2e);

  return output_vector;
}
