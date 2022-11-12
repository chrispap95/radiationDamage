#!/usr/bin/python
import os, sys, math, datetime
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory

from collections import OrderedDict
import numpy

sys.path.append(os.path.abspath(os.path.curdir))

from Plotter import parseLYAnaInputArgs
from Plotter.CommonTools import DrawHistSimple
options = parseLYAnaInputArgs()

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
    print(options)
    myfile = {}

    hxrng = {}
    hxrng["Ref"] = [32,-1,8]
    hxrng["EJ200"] = [32,-1,6]
    hxrng["EJ260"] = [32,-0.5,4]

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    ## Output directory
    outDir = "Results/20181027"
    try:
        os.makedirs(outDir)
    except:
        pass

    ## NIST set1
    doselabel = "7 Mrad@ 390 krad/hr; 112 days aft. irr."

    ## 10/11/18
    myfile["EJ200_2X_03-20181011"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181011.root")]
    
    ## 10/12/18
    myfile["EJ200_2X_03-20181012"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181012.root")]

    ## 10/23/18
    myfile["EJ200_2X_03-20181023"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181023.root")]

    ## 10/24/18
    myfile["EJ200_2X_03-20181024"] = ["Ref",TFile("root/AlphaSource/Pu239new_EJ200-2X_3_Default_Nofoil_FaceA_FastFrame_20181024.root")]


    plotSets = {}
    plotSets['0'] = ["EJ200_2X_03-20181023","EJ200_2X_03-20181024","EJ200_2X_03-20181012","EJ200_2X_03-20181012"]
#   plotSets['1'] = ["EJ200PVT_1X1P_N9-Face_A","EJ200PVT_1X1P_N9-Face_B"]

    DrawHistSimple(myfile, plotSets, outDir, fTag, doselabel, hxrng, options)
