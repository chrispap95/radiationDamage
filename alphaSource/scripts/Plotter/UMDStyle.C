//
// UMD Style, based on a style file from BaBar
//

#include <iostream>

#include "UMDStyle.h"

#include "TROOT.h"

void SetUMDStyle()
{
  static TStyle* umdStyle = 0;
  std::cout << "\nApplying UMD style settings...\n" << std::endl;
  if ( umdStyle==0 ) umdStyle = UMDStyle();
  gROOT->SetStyle("UMD");
  gROOT->ForceStyle();
}

TStyle* UMDStyle() 
{
  TStyle *umdStyle = new TStyle("UMD","UMD style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  umdStyle->SetFrameBorderMode(icol);
  umdStyle->SetFrameFillColor(icol);
  umdStyle->SetCanvasBorderMode(icol);
  umdStyle->SetCanvasColor(icol);
  umdStyle->SetPadBorderMode(icol);
  umdStyle->SetPadColor(icol);
  umdStyle->SetStatColor(icol);
  //umdStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  umdStyle->SetPaperSize(20,26);

  // set margin sizes
  umdStyle->SetPadTopMargin(0.05);
  umdStyle->SetPadRightMargin(0.05);
  umdStyle->SetPadBottomMargin(0.16);
  umdStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  umdStyle->SetTitleXOffset(1.4);
  umdStyle->SetTitleYOffset(1.4);

  // set legend border size
  umdStyle->SetLegendBorderSize(0);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica:Arial
  //Int_t font=82; // Courier New
  Double_t tsize=0.05;
  umdStyle->SetTextFont(font);

  umdStyle->SetLegendFont(font);
  //umdStyle->SetLegendTextSize(tsize);

  umdStyle->SetTextSize(tsize);
  umdStyle->SetLabelFont(font,"x");
  umdStyle->SetTitleFont(font,"x");
  umdStyle->SetLabelFont(font,"y");
  umdStyle->SetTitleFont(font,"y");
  umdStyle->SetLabelFont(font,"z");
  umdStyle->SetTitleFont(font,"z");
  
  umdStyle->SetLabelSize(tsize,"x");
  umdStyle->SetTitleSize(tsize,"x");
  umdStyle->SetLabelSize(tsize,"y");
  umdStyle->SetTitleSize(tsize,"y");
  umdStyle->SetLabelSize(tsize,"z");
  umdStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  umdStyle->SetMarkerStyle(20);
  umdStyle->SetMarkerSize(1.2);
  umdStyle->SetHistLineWidth(2.);
  umdStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars 
  //umdStyle->SetErrorX(0.001);
  // get rid of error bar caps
  umdStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  umdStyle->SetOptTitle(0);
  //umdStyle->SetOptStat(1111);
  umdStyle->SetOptStat(0);
  //umdStyle->SetOptFit(1111);
  umdStyle->SetOptFit(0);

  //umdStyle->SetNdivisions(510, "XYZ");

  // put tick marks on top and RHS of plots
  umdStyle->SetPadTickX(1);
  umdStyle->SetPadTickY(1);

  return umdStyle;

}

