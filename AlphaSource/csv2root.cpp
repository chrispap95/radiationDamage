#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "RootStyle.cc"
using namespace std;

void csv2root() {
  set_root_style();

  vector<float> x1,y1,x2,y2;
  float x_temp1, y_temp1, x_temp2, y_temp2;

  ifstream myfile1("EJ-200_emspec.csv");
  ifstream myfile2("EJ-260_emspec.csv");
  while (myfile1 >> x_temp1 >> y_temp1) {
    x1.push_back(x_temp1);
    y1.push_back(y_temp1);
  }
  myfile1.close();
  while (myfile2 >> x_temp2 >> y_temp2) {
    x2.push_back(x_temp2);
    y2.push_back(y_temp2);
  }
  myfile2.close();

  TCanvas* c = new TCanvas("c","c",1);
  TGraph* gr1 = new TGraph(x1.size(),x1.data(),y1.data());
  TGraph* gr2 = new TGraph(x2.size(),x2.data(),y2.data());
  TMultiGraph* gr = new TMultiGraph();
  gr->Add(gr1);
  gr->Add(gr2);
  gr1->SetLineColor(kBlue);
  gr1->SetLineStyle(2);
  gr1->SetLineWidth(5);
  gr2->SetLineColor(kGreen);
  gr2->SetLineStyle(4);
  gr2->SetLineWidth(5);
  gr->SetTitle(";Wavelength (nm);Amplitude");
  gr->SetMaximum(1.1);
  gr->SetMinimum(0.0);
  gr->Draw("AC");

  TLegend* lg = new TLegend(0.65,0.65,0.89,0.85);
  lg->AddEntry(gr1,"EJ-200","l");
  lg->AddEntry(gr2,"EJ-260","l");
  lg->SetBorderSize(0);
  lg->Draw();

  return;
}
