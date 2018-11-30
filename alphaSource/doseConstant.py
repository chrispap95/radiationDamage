#!/usr/bin/python
import os, sys, math, datetime
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory

from collections import OrderedDict
import numpy

sys.path.append(os.path.abspath(os.path.curdir))

from Plotter import parseLYAnaInputArgs
from Plotter.CommonTools import DrawHistSimple, DrawDvsTHist
options = parseLYAnaInputArgs()

gROOT.LoadMacro("Plotter/UMDStyle.C")
from ROOT import SetUMDStyle
SetUMDStyle()
gROOT.SetBatch()

####################################################################################################
####################################################################################################
if __name__ == '__main__':
    print options

    myfile = {}

    ## binbase, xmin, xmax : nbins = binbase*(xmax-xmin)
    hxrng = {}
    hxrng["Ref"]   = [32,-1,8]
    hxrng["EJ200"] = [32,-1,6]
    hxrng["EJ260"] = [32,-0.5,4]

    hxrng["T1"] = [32,-1,8.0]

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    ## Output directory
    outDir = "Results/GIF++"
    try:
        os.makedirs(outDir)
    except:
        pass

    ## GIF++ set
    sampleSet = "GIF++"
    doselabel = "300 krad@ 50 rad/hr"

    ## Un-irr samples
    myfile["EJ200PS_1X1P-20181115"] = ["T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root")]

    ## [fitrng0,fitrng1,fitterType,biasOffset]
    ## [1.3,1.1,"G2",0.3112]

    myfile["EJ200PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G2",0.]]
    myfile["EJ200PS_1X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G2",0.3112]]

    plotSets = {}

    plotSets['1'] = ["EJ200PS_1X1P-UnIrr","EJ200PS_1X1P-20181115"]


    DrawDvsTHist(myfile, plotSets, outDir, sampleSet, fTag, doselabel, hxrng, options, "EJ200PS", "GIF++")
