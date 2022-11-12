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
    print(options)

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
    doselabel = "1.32 Mrad@ 220 rad/hr"

    ## Un-irr samples
    ## myfile["EJ200PS_1X1P-20181115"] = ["T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root")]

    ## [fitrng0,fitrng1,fitterType,biasOffset]
    ## [1.3,1.1,"G2",0.3112]

    myfile["EJ200PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[.4,.4,"G1",0.]]
    myfile["EJ200PS_1X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_4_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.4,.4,"G1",0.3112]]
    #myfile["EJ200PS_1X2P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[.4,.4,"G1",0.]]
    #myfile["EJ200PS_1X2P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_2_Default_Nofoil_FaceA_FastFrame_20181116.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ200PS_2X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[.4,4.,"G1",0.]]
    myfile["EJ200PS_2X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ200PVT_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[.4,.4,"G1",0.]]
    myfile["EJ200PVT_1X1P-20181116"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ200PVT_1X2P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[.4,.4,"G1",0.]]
    myfile["EJ200PVT_1X2P-20181116"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[.4,.4,"G1",0.3112]]
    #myfile["EJ200PVT_2X1P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-2X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[.4,.4.,"G1",0.]]
    #myfile["EJ200PVT_2X1P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-2X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ260PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X1P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[.4,.4,"G1",0.]]
    myfile["EJ260PS_1X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ260PS_1X2P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X2P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[.4,.4,"G1",0.]]
    myfile["EJ260PS_1X2P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X2P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.4,.4,"G1",0.3112]]
    myfile["EJ260PS_2X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-2X1P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[.4,.4,"G1",0.]]
    myfile["EJ260PS_2X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-2X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.4,.4,"G1",0.3112]]
    #myfile["EJ260PVT_1X1P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-1X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    #myfile["EJ260PVT_1X1P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-1X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    #myfile["EJ260PVT_1X2P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    #myfile["EJ260PVT_1X2P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-1X2P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    #myfile["EJ260PVT_2X1P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-2X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    #myfile["EJ260PVT_2X1P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ260PVT-2X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]

    plotSets = {}

    #plotSets['1'] = ["EJ200PS_1X1P-UnIrr","EJ200PS_1X1P-20181115"]
    #plotSets['2'] = ["EJ200PS_1X2P-UnIrr","EJ200PS_1X2P-20181115"]
    #plotSets['3'] = ["EJ200PS_2X1P-UnIrr","EJ200PS_2X1P-20181115"]
    #plotSets['4'] = ["EJ200PVT_1X1P-UnIrr","EJ200PVT_1X1P-20181116"]
    #plotSets['5'] = ["EJ200PVT_1X2P-UnIrr","EJ200PVT_1X2P-20181116"]
    #plotSets['6'] = ["EJ200PVT_2X1P-UnIrr","EJ200PVT_2X1P-20181115"]
    plotSets['1'] = ["EJ260PS_1X1P-UnIrr","EJ260PS_1X1P-20181115"]
    plotSets['2'] = ["EJ260PS_1X2P-UnIrr","EJ260PS_1X2P-20181115"]
    plotSets['3'] = ["EJ260PS_2X1P-UnIrr","EJ260PS_2X1P-20181115"]
    #plotSets['10'] = ["EJ260PVT_1X1P-UnIrr","EJ260PVT_1X1P-20181115"]
    #plotSets['11'] = ["EJ260PVT_1X2P-UnIrr","EJ260PVT_1X2P-20181115"]
    #plotSets['12'] = ["EJ260PVT_2X1P-UnIrr","EJ260PVT_2X1P-20181115"]


    DrawDvsTHist(myfile, plotSets, outDir, sampleSet, fTag, doselabel, hxrng, options, "EJ260", "GIF++")
