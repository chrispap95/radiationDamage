#!/usr/bin/python

###################################################################################################
##                                                                                               ##
##   This routine is going to fit 4 different measurements and plot a 2x2 canvas with the        ##
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
    myfile["meas1"] = TFile("../data/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181011.root")
    ## and then get the TTree
    mytree["meas1"] = myfile["meas1"].Get("tree")

    ## Meas 2
    myfile["meas2"] = TFile("../data/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181012.root")

    mytree["meas2"] = myfile["meas2"].Get("tree")

    ## Meas 3
    myfile["meas3"] = TFile("../data/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181023.root")

    mytree["meas3"] = myfile["meas3"].Get("tree")

    ## Meas 4
    myfile["meas4"] = TFile("../data/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181024.root")

    mytree["meas4"] = myfile["meas4"].Get("tree")
    
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

    myhist["meas2"] = TH1D("myhist_meas2","meas2",256,-0.5,7.5)
    mytree["meas2"].Draw("area*1.e9>>myhist_meas2","","")

    myhist["meas3"] = TH1D("myhist_meas3","meas3",256,-0.5,7.5)
    mytree["meas3"].Draw("area*1.e9>>myhist_meas3","","")

    myhist["meas4"] = TH1D("myhist_meas4","meas4",256,-0.5,7.5)
    mytree["meas4"].Draw("area*1.e9>>myhist_meas4","","")

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

    ## Draw meas2 using another TPad
    c1.cd(2)
    myhist["meas2"].Scale(myhist["meas1"].Integral()/myhist["meas2"].Integral())
    myhist["meas2"].SetLineColor(4)
    myhist["meas2"].SetLineStyle(1)
    ## Some drawing options for meas2
    myhist["meas2"].GetXaxis().SetTitle("Energy [V#timesns]")
    #myhist["meas2"].GetYaxis().SetTitle("Events")
    myhist["meas2"].GetYaxis().SetTitle("A.U.")
    #myhist["meas2"].GetYaxis().SetRangeUser(5.e-2,2.e6)
    myhist["meas2"].GetYaxis().SetRangeUser(0,3.e3)
    gPad.Update()
    vEng_2, sFit_2, myfit_2 = AlphaSourceFitter().GausFitEngPeak(myhist["meas2"],"meas2",fitopt,1)
    c1.Update()

    ## The same...
    c1.cd(3)
    myhist["meas3"].Scale(myhist["meas1"].Integral()/myhist["meas3"].Integral())
    myhist["meas3"].SetLineColor(3)
    myhist["meas3"].SetLineStyle(1)
    ## Some drawing options for mas
    myhist["meas3"].GetXaxis().SetTitle("Energy [V#timesns]")
    myhist["meas3"].GetYaxis().SetTitle("A.U.")
    myhist["meas3"].GetYaxis().SetRangeUser(0,3.e3)
    gPad.Update()
    c1.Update()
    vEng_3, sFit_3, myfit_3 = AlphaSourceFitter().GausFitEngPeak(myhist["meas3"],"meas3",fitopt,1)
    c1.Update()

    c1.cd(4)
    myhist["meas4"].Scale(myhist["meas1"].Integral()/myhist["meas4"].Integral())
    myhist["meas4"].SetLineColor(5)
    myhist["meas4"].SetLineStyle(1)
    ## Some drawing options for meas4
    myhist["meas4"].GetXaxis().SetTitle("Energy [V#timesns]")
    myhist["meas4"].GetYaxis().SetTitle("A.U.")
    myhist["meas4"].GetYaxis().SetRangeUser(0,3.e3)
    gPad.Update()
    c1.Update()
    vEng_4, sFit_4, myfit_4 = AlphaSourceFitter().GausFitEngPeak(myhist["meas4"],"meas4",fitopt,1)
    c1.Update()

    ## Create a legend and customize it
    #leg = TLegend(0.58,0.7,0.89,0.92)
    #leg.SetFillColor(kWhite)
    #leg.SetLineColor(kWhite)
    #leg.SetHeader("EJ200PS-2X_3")
    #leg.AddEntry(myhist["meas1"],"10/11/2018","l")
    #leg.AddEntry(myhist["meas2"],"10/12/2018","l")
    #leg.AddEntry(myhist["meas3"],"10/23/2018","l")
    #leg.AddEntry(myhist["meas4"],"10/24/2018","l")
    #leg.Draw()

    ## Get today's date to use in filenames
    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")
    #fTag="20181027"
    fnameTag = "%s_p%s"%(options.outtag,fTag)

    ## Save results
    c1.SaveAs("../results/20181027/Alpha_eng_%s_fit.png"%fnameTag)
    c1.SaveAs("../results/20181027/Alpha_eng_%s_fit.pdf"%fnameTag)
    c1.SaveAs("../results/20181027/Alpha_eng_%s_fit.root"%fnameTag)
