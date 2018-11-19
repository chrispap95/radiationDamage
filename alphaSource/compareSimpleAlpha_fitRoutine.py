#!/usr/bin/python

###################################################################################################
##                                                                                               ##
##   This routine is going to fit one measurement and give the peak and the stat+syst error      ##
##                                                                                               ##
###################################################################################################

# Import some python libraries
import os, sys, math, datetime

# Import ROOT
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory
from ROOT import kRed, kBlue, kGreen, kWhite

# Some more python libraries
from collections import OrderedDict
import numpy

# Pass the current working directory to the compiler
sys.path.append(os.path.abspath(os.path.curdir))

# Import some custom styling methods
from Plotter import parseLYAnaInputArgs
options = parseLYAnaInputArgs()

# Import fitter
from Plotter.CommonTools import AlphaSourceFitter

gROOT.LoadMacro("Plotter/UMDStyle.C")
from ROOT import SetUMDStyle
SetUMDStyle()
gROOT.SetBatch()

####################################################################################################
####################################################################################################
if __name__ == '__main__':
    #gROOT.SetStyle("Plain")
    #gStyle.SetOptFit()
    #gStyle.SetOptStat(0)
    print options
    myfile = {}
    mytree = {}
    myhist = {}
    myfit = {}
    valphys = {}
    valsyst = {}
    runSyst = False

    ## Import all measurements
    ## Meas 1
    ### Import the file
    myfile["meas1"] = TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P-2_Default_Nofoil_FaceD_FastFrame_20181108_shiyuan.root")
    ## and then get the TTree
    mytree["meas1"] = myfile["meas1"].Get("tree")

    ## Then define a TCanvas to contain all graphics
    c1 = TCanvas("c1","c1",800,600)
    c1.Divide(2,2)
    c1.cd(1)

    ## Get all measurement histograms
    ## FastFrame
    ### First create an empty TH1D histogram
    myhist["meas1"] = TH1D("myhist_meas1","meas1",256,-0.5,7.5)
    ### Then get the saved histogram into the new one and normalize to ns from s by *1.e9
    mytree["meas1"].Draw("area*1.e9>>myhist_meas1","","")

    ## Draw the first measurement
    myhist["meas1"].SetLineColor(2)
    ## Fit meas1
    fitopt = [0.4,1.4]
    vEng_1, sFit_1, myfit_1 = AlphaSourceFitter().GausFitEngPeak(myhist["meas1"],"meas1",fitopt,1)
    ## Some drawing options for meas1
    myhist["meas1"].GetXaxis().SetTitle("Energy [V#timesns]")
    myhist["meas1"].GetYaxis().SetTitle("A.U.")
    #myhist["meas1"].GetYaxis().SetRangeUser(5.e-2,2.e6)     ##Used for LogY
    myhist["meas1"].GetYaxis().SetRangeUser(0,3.e3)
    ## Update the TCanvas to actually draw meas1
    gPad.Update()
    c1.Update()
