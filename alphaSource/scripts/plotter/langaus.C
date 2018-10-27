//-----------------------------------------------------------------------
//
// Convoluted Landau and Gaussian Fitting Function
//         (using ROOT's Landau and Gauss functions)
//
//  Based on a Fortran code by R.Fruehwirth (fruhwirth@hephy.oeaw.ac.at)
//  Adapted for C++/ROOT by H.Pernegger (Heinz.Pernegger@cern.ch) and
//   Markus Friedl (Markus.Friedl@cern.ch)
//
//  to execute this example, do:
//  root > .x langaus.C
// or
//  root > .x langaus.C++
//
// Modified by Geng-Yuan Jeng for UMD scintillator study using cosmic stand
//-----------------------------------------------------------------------

#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"

class PyLandGaus{
    double chisqr;   // chi square of the fit
    Int_t ndf;       // ndf of the fit

public:
    double fr[2];    // fitrange lo and hi boundaries of fit range
    double sv[4];    // startvalues  reasonable start values for the fit
    double pllo[4];  // lower parameter limits
    double plhi[4];  // upper parameter limits
    double fp[4];    // returns the final fit parameters
    double fpe[4];   // returns the final fit errors
    double SNRPeak, SNRFWHM;

    PyLandGaus();
    ~PyLandGaus();

    double langaufun(double *x, double *par);
    double pedgaus(double *x, double *par);
    double pedlandau(double *x, double *par);
    double gauslangaufun(double *x, double *par);
    double landlangaufun(double *x, double *par);

    TF1 *langaufit(TH1F *his, double *fitrange, double *startvalues, double *parlimitslo, double *parlimitshi, double *fitparams, double *fiterrors, double *ChiSqr, Int_t *NDF);
    Int_t langaupro(double *params, double &maxx, double &FWHM);
    void langaus(char* f=NULL, double* r=NULL, bool offset=false, double* r0=NULL);
    TF1 *langausf(TH1F* hSNR);

    int GetNDF() {return ndf;};
    double GetChisquare() {return chisqr;};

};

PyLandGaus::PyLandGaus()
{
    chisqr = 0.0;
    ndf=0;
    pllo[0]=0.1; pllo[1]=1.0; pllo[2]=1.0; pllo[3]=0.4;
    plhi[0]=10.0; plhi[1]=60.0; plhi[2]=100000.0; plhi[3]=10.0;
    sv[0]=1.0; sv[1]=3.0; sv[2]=1000.0; sv[3]=1.0;
}

PyLandGaus::~PyLandGaus()
{
}

double PyLandGaus::langaufun(double *x, double *par) {

    //Fit parameters:
    //par[0]=Width (scale) parameter of Landau density
    //par[1]=Most Probable (MP, location) parameter of Landau density
    //par[2]=Total area (integral -inf to inf, normalization constant)
    //par[3]=Width (sigma) of convoluted Gaussian function
    //
    //In the Landau distribution (represented by the CERNLIB approximation), 
    //the maximum is located at x=-0.22278298 with the location parameter=0.
    //This shift is corrected within this function, so that the actual
    //maximum is identical to the MP parameter.

    // Numeric constants
    double invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
    double mpshift  = -0.22278298;       // Landau maximum location

    // Control constants
    double np = 100.0;      // number of convolution steps
    double sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

    // Variables
    double xx;
    double mpc;
    double fland;
    double sum = 0.0;
    double xlow,xupp;
    double step;
    double i;


    // MP shift correction
    mpc = par[1] - mpshift * par[0]; 

    // Range of convolution integral
    xlow = x[0] - sc * par[3];
    xupp = x[0] + sc * par[3];

    step = (xupp-xlow) / np;

    // Convolution integral of Landau and Gaussian by sum
    for(i=1.0; i<=np/2; i++) {
        xx = xlow + (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);

        xx = xupp - (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);
    }

    return (par[2] * step * sum * invsq2pi / par[3]);
}

double PyLandGaus::pedgaus(double *x, double *par) {
    return par[0]*TMath::Gaus(x[0], par[1], par[2]);
}

double PyLandGaus::pedlandau(double *x, double *par) {
    return par[0]*TMath::Landau(x[0], par[1], par[2]);
}

double PyLandGaus::gauslangaufun(double *x, double *par) {
    return pedgaus(x, par)+langaufun(x, &(par[3]));
}

double PyLandGaus::landlangaufun(double *x, double *par) {
    return pedlandau(x, par)+langaufun(x, &(par[3]));
}

TF1 *PyLandGaus::langaufit(TH1F *his, double *fitrange, double *startvalues, double *parlimitslo, double *parlimitshi, double *fitparams, double *fiterrors, double *ChiSqr, Int_t *NDF)
{
    Int_t i;
    Char_t FunName[100];

    sprintf(FunName,"Fitfcn_%s",his->GetName());

    TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
    if (ffitold) delete ffitold;

    PyLandGaus* fobj = new PyLandGaus();
    TF1 *ffit = new TF1(FunName,fobj, &PyLandGaus::langaufun,fitrange[0],fitrange[1],4,"PyLandGaus","langaufun");
    ffit->SetParameters(startvalues);
    ffit->SetParNames("Width","MP","Area","GSigma");
    ffit->SetLineColor(4);
    for (i=0; i<4; i++) {
        //std::cout << "Par[" << ffit->GetParName(i) <<"] = " << ffit->GetParameter(i) << std::endl;
        ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
    }

    his->Fit(FunName,"RB");   // fit within specified range, use ParLimits

    ffit->GetParameters(fitparams);    // obtain fit parameters
    for (i=0; i<4; i++) {
        fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
    }
    ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
    NDF[0] = ffit->GetNDF();           // obtain ndf

    return (ffit);              // return fit function

}


Int_t PyLandGaus::langaupro(double *params, double &maxx, double &FWHM) {

    // Seaches for the location (x value) at the maximum of the 
    // Landau-Gaussian convolute and its full width at half-maximum.
    //
    // The search is probably not very efficient, but it's a first try.

    double p,x,fy,fxr,fxl;
    double step;
    double l,lold;
    Int_t i = 0;
    Int_t MAXCALLS = 10000;

    PyLandGaus* fobj = new PyLandGaus();

    // Search for maximum

    p = params[1] - 0.1 * params[0];
    step = 0.05 * params[0];
    lold = -2.0;
    l    = -1.0;

    while ( (l != lold) && (i < MAXCALLS) ) {
        i++;

        lold = l;
        x = p + step;
        l = fobj->langaufun(&x,params);
 
        if (l < lold)
            step = -step/10;
 
        p += step;
    }

    if (i == MAXCALLS)
        return (-1);

    maxx = x;

    fy = l/2;


    // Search for right x location of fy

    p = maxx + params[0];
    step = params[0];
    lold = -2.0;
    l    = -1e300;
    i    = 0;


    while ( (l != lold) && (i < MAXCALLS) ) {
        i++;

        lold = l;
        x = p + step;
        l = TMath::Abs(fobj->langaufun(&x,params) - fy);
 
        if (l > lold)
            step = -step/10;
 
        p += step;
    }

    if (i == MAXCALLS)
        return (-2);

    fxr = x;


    // Search for left x location of fy

    p = maxx - 0.5 * params[0];
    step = -params[0];
    lold = -2.0;
    l    = -1e300;
    i    = 0;

    while ( (l != lold) && (i < MAXCALLS) ) {
        i++;

        lold = l;
        x = p + step;
        l = TMath::Abs(fobj->langaufun(&x,params) - fy);
 
        if (l > lold)
            step = -step/10;
 
        p += step;
    }

    if (i == MAXCALLS)
        return (-3);


    fxl = x;

    FWHM = fxr - fxl;
    return (0);
}

void PyLandGaus::langaus(char* fname, double* range, bool offset, double* range0) {

    TFile* fin;

    int xbins_ = 210;
    double xmin_=-1.0,xmax_=20.0;

    int xbins0_ = 10;
    double xmin0_=-1.5,xmax0_=-0.15;

    if (range != NULL) {
        xmin_=range[1];
        xmax_=range[2];
        xbins_=int(range[0]*(xmax_-xmin_));
    }
    if (range0 != NULL) {
        xmin0_=range0[1];
        xmax0_=range0[2];
        xbins0_=int(range0[0]);
    }
    if (fname == NULL) {
        fin = new TFile("root/cosmic/EJ200_1X_8_CosmicStand20170307_CH2_NonMirr_Tcomp24p4C_20170327.root");
    }
    else {
        std::cout << "[LANGAUS] Open file: " << fname << std::endl;
        fin = new TFile(fname);
    }
    TTree* mytree = (TTree*) fin->Get("tree");

    double voffset = 0.0;
    TH1F* hSNR = new TH1F("hSNR","Signal-to-noise",xbins_,xmin_,xmax_);

    if (offset) {
        TH1F* hPed = new TH1F("hPed","Pedestal",xbins0_,xmin0_,xmax0_);
        mytree->Draw("areaFromScope*-1.e9>>hPed","","");
        TF1 myfit = TF1("myfit","gaus",xmin0_,xmax0_);
        hPed->Fit("myfit","RL");
        voffset = myfit.GetParameter(1);
        std::cout << "vOffset = " << voffset << std::endl;
        mytree->Draw(Form("areaFromScope*-1.e9-%f>>hSNR",voffset),"","");
    }
    else {
        mytree->Draw("areaFromScope*-1.e9>>hSNR","","");
        //mytree->Draw("area*1.e9>>hSNR","","");//Na22
        //mytree->Draw("(areaFromScope*-1.e9+1.7)/2.>>hSNR","","");
        //hSNR->Draw();
    }
    // Fitting SNR histo
    printf("[LANGAUS] Fitting...\n");

    // Setting fit range and start values
    fr[0]=0.3*hSNR->GetMean();
    fr[1]=3.0*hSNR->GetMean();
    std::cout<<"[LANGAUS] fit range:"<<fr[0]<<" to "<<fr[1]<<std::endl;

    TF1 *fitsnr = langaufit(hSNR,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);

    langaupro(fp,SNRPeak,SNRFWHM);

    printf("[LANGAUS] Fitting done\nPlotting results...\n");

    // Global style settings
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(111);
    gStyle->SetLabelSize(0.03,"x");
    gStyle->SetLabelSize(0.03,"y");

    hSNR->GetXaxis()->SetRangeUser(xmin_,xmax_);
    hSNR->Draw();
    fitsnr->Draw("lsame");
}

TF1 *PyLandGaus::langausf(TH1F* hSNR) {

    // Fitting SNR histo
    std::cout << "[LANGAUS] Fitting..." << hSNR->GetName() << std::endl;

    // Setting fit range and start values if needed
    //fr[0]=hSNR->GetXaxis()->GetXmin();
    //fr[1]=hSNR->GetXaxis()->GetXmax();
    //sv[0] = hSNR->GetRMS()/4.25;
    //sv[1] = hSNR->GetMean()*0.75;
    std::cout<<"[LANGAUS] fit range:"<<fr[0]<<" to "<<fr[1]<<std::endl;

    TF1 *fitsnr = langaufit(hSNR,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
    //fitsnr->SetNpx(1000);

    langaupro(fp,SNRPeak,SNRFWHM);
    std::cout << "[LANGAUS] SNRPeak, SNRFWHM = " << SNRPeak << ", " << SNRFWHM << std::endl;

    // Global style settings
//     gStyle->SetOptStat(1111);
//     gStyle->SetOptFit(111);
//     gStyle->SetLabelSize(0.03,"x");
//     gStyle->SetLabelSize(0.03,"y");

    fitsnr->Draw("lsame");

    return fitsnr;
}
