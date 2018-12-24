#!/usr/bin/python
import os, sys, math, datetime
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory

from collections import OrderedDict
import numpy

sys.path.append(os.path.abspath(os.path.curdir))

from Plotter import parseLYAnaInputArgs
from Plotter.CommonTools import DrawHistSimple, DrawDvsTHist
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
    print options

    myfile = {}
    myhist = {}
    myfit  = {}
    fitopt = {}
    sFit   = {}
    uncFit = {}
    vEng   = {}
    uncEng = {}

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
    ## myfile["EJ200PS_1X1P-20181115"] = ["T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root")]

    ## [fitrng0,fitrng1,fitterType,biasOffset]
    ## [1.3,1.1,"G2",0.3112]

    myfile["EJ200PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[1.,1.,"G2",0.]]
    myfile["EJ200PS_1X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_4_Default_Nofoil_FaceA_FastFrame_20181115.root"),[1.,1.,"G2",0.3112]]
    #myfile["EJ200PS_1X2P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    #myfile["EJ200PS_1X2P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_2_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    myfile["EJ200PS_2X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[1.,1.,"G1",0.]]
    myfile["EJ200PS_2X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[1.,1.,"G1",0.3112]]
    myfile["EJ200PVT_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    myfile["EJ200PVT_1X1P-20181116"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    myfile["EJ200PVT_1X2P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    myfile["EJ200PVT_1X2P-20181116"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    #myfile["EJ200PVT_2X1P-UnIrr"] = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-2X1P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root"),[1.,1.,"G1",0.]]
    #myfile["EJ200PVT_2X1P-20181115"]  = [
    #    "T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-2X1P_N8_Default_Nofoil_FaceA_FastFrame_20181116.root"),[1.,1.,"G1",0.3112]]
    myfile["EJ260PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X1P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[.5,1.,"G1",0.]]
    myfile["EJ260PS_1X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.5,1.,"G1",0.3112]]
    myfile["EJ260PS_1X2P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X2P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[1.,1.,"G1",0.]]
    myfile["EJ260PS_1X2P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-1X2P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[1.,1.,"G1",0.3112]]
    myfile["EJ260PS_2X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-2X1P_1_Default_Nofoil_FaceA_FastFrame_20181126.root"),[.5,1.,"G1",0.]]
    myfile["EJ260PS_2X1P-20181115"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ260PS-2X1P_2_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.5,1.,"G1",0.3112]]
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

    plotSets['1'] = ["EJ200PS_1X1P-UnIrr","EJ200PS_1X1P-20181115"]
    #plotSets['2'] = ["EJ200PS_1X2P-UnIrr","EJ200PS_1X2P-20181115"]
    plotSets['3'] = ["EJ200PS_2X1P-UnIrr","EJ200PS_2X1P-20181115"]
    plotSets['4'] = ["EJ200PVT_1X1P-UnIrr","EJ200PVT_1X1P-20181116"]
    plotSets['5'] = ["EJ200PVT_1X2P-UnIrr","EJ200PVT_1X2P-20181116"]
    #plotSets['6'] = ["EJ200PVT_2X1P-UnIrr","EJ200PVT_2X1P-20181115"]
    plotSets['7'] = ["EJ260PS_1X1P-UnIrr","EJ260PS_1X1P-20181115"]
    plotSets['8'] = ["EJ260PS_1X2P-UnIrr","EJ260PS_1X2P-20181115"]
    plotSets['9'] = ["EJ260PS_2X1P-UnIrr","EJ260PS_2X1P-20181115"]
    #plotSets['10'] = ["EJ260PVT_1X1P-UnIrr","EJ260PVT_1X1P-20181115"]
    #plotSets['11'] = ["EJ260PVT_1X2P-UnIrr","EJ260PVT_1X2P-20181115"]
    #plotSets['12'] = ["EJ260PVT_2X1P-UnIrr","EJ260PVT_2X1P-20181115"]

    ###############################
    ## Step 1: Find Dark Current and --> offset
    ###############################
    ## Dark current common
    ###############################
    ## Load dark current file and the TTree
    fDC50 = TFile("root/AlphaSource/DarkCurrent_HV1700_trigger_101mV_amp50p5mVpDiv_20181121.root")
    tDC50 = fDC50.Get("tree")
    ###############################
    ## Create a histogram and insert the area histogram from the tree.
    ## Use |amplitude|>mypedcut && time > 500 to skim the data.
    ## The hist has limited range and it is only for fitting.
    mypedcut = 0.0
    hDC50 = TH1D("myhist_DC50","DC50",52,-1,0.)
    tDC50.Draw("area*1.e9>>myhist_DC50","abs(amplitude)>%f && time>%f"%(mypedcut,500.0))
    ## Find overall max (this is energy offset)
    ## Fit 1st hist
    ## Range value seemed suspicious. Total range is [-0.2,0] but he fits in +-0.2 from the maxbin (???)
    ## Need to find the right pedestal cut value
    DCName = "DC50"
    vEng[DCName],sFit[DCName],myfit[DCName]=AlphaSourceFitter().GausFitEngPeak(hDC50,DCName,[0.2,0.2],1.)

    vOffset = [vEng[DCName],sFit[DCName]]

    for nf, fl in sorted(plotSets.items()):
        fNames_ = {}
        grTemp_ = {}
        trees_  = {}
        labels_ = {}

        refPlot_  = []
        irrPlots_ = []

        ### Loop over the fl items for each nf (Irr over each UnIrr)
        for ni in range(len(fl)):
            #### Give a temporary name of the form UnIrr_nameIrr_name)
            tmpName = "%s_%i"%(nf,ni)
            if debug_: print tmpName
            fNames_[tmpName] = fl[ni]
            trees_[tmpName]  = (myfiles_[fNames_[tmpName]][1]).Get("tree")
            labels_[tmpName] = fNames_[tmpName].split("-")[1]
            fitopt_[tmpName] = myfiles_[fNames_[tmpName]][2]

        if debug_:
            print fNames_
            print trees_
            print labels_

        ###############################
        ## Step 2: Fit all data histograms
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            print ">>>>>>>>>>> Processing:", sigName
            vEng[sigName],sFit[sigName],myfit[sigName]=AlphaSourceFitter().GausFitEngPeak(myhist[sigName],sigName,fitopt[tmpName],1.)
            uncFit_[sigName] = sFit_[sigName]/vEng_[sigName]
            print "Uncertainty of %s Fit     = %-6.3f %%"%(sigName,uncFit_[sigName]*100.)

        ###############################
        ## Step 3: D calculation
        ## CalcD(dose,v_o,v_i,offset,uncV)
        ###############################
        vDose = [1.32,0.00022]
        print "\n"
        print "="*150
        vDconst_ = {}
        vInput  = {}
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            uncEng[sigName]  = math.sqrt(math.pow(uncEng[uncreftag_],2)+math.pow(uncFit[sigName],2))
            vInput[sigName]  = [vEng[sigName], uncEng[sigName]*vEng[sigName]]

        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            if sigName.find("UnIrr") == -1:
                print "Calculating Dose Constant for :", sigName
                vDconst[sigName] = CalcD(vDose,vInput[sigName],vInput[fNames["%s_0"%(nf)]],vOffset)
                ### vDose["GIF++"] = [1.32,0.00022]
                ### vInput_[sigName, fNames_[...]] is vInput_[sigName]  = [vEng_[sigName], uncEng_[sigName]*vEng_[sigName]]

    ## print uncertainties
    print "Offset = %-8.5f"%(vOffset[0])
    print "Uncertainty of [%30s] = %6.3f %%"%("Offset",math.fabs(vOffset[1]/vOffset[0]*100.))
    for n,v in sorted(uncEng_.items()):
        print "Uncertainty of [%30s] = %6.3f %%"%(n,v*100.)