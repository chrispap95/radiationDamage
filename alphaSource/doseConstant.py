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
from Plotter.CommonTools import DrawDvsTHist

gROOT.LoadMacro("Plotter/UMDStyle.C")
from ROOT import SetUMDStyle
SetUMDStyle()
gROOT.SetBatch()

####################################################################################################
####################################################################################################
if __name__ == '__main__':
    myfile = {}

    hxrng = {}
    hxrng["Ref"] = [32,-1,8]
    hxrng["EJ200"] = [32,-1,6]
    hxrng["EJ260"] = [32,-0.5,4]

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    ## Output directory
    outDir = "Results/20181119"
    try:
        os.makedirs(outDir)
    except:
        pass

    ## NIST set1 ## It's meaningless right now
    doselabel = "7 Mrad@ 390 krad/hr; 112 days aft. irr."

    ## 10/11/18
    myfile["EJ200PVT_1X1P_N8-20181011"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181011.root")]

    ## 10/12/18
    myfile["EJ200PVT_1X1P_N1-20181012"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181012.root")]

    plotSets = {}
    plotSets['0'] = ["EJ200_2X_03-20181023","EJ200_2X_03-20181024"]

    sampleSet = "EJ200PVT"
    doselabel = "GIF++"
    uncreftag = "EJ200PVT"
    dosescheme = "GIF++"
    D,sigmaD,R,sigmaR,NPE = DrawDvsTHist(myfile, plotSets, outDir, sampleSet, fTag, doselabel, hxrng, options, uncreftag, dosescheme)
