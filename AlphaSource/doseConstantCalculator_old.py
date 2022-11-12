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
from Plotter.CommonTools import AlphaSourceFitter, CalcD

gROOT.LoadMacro("Plotter/UMDStyle.C")
from ROOT import SetUMDStyle
SetUMDStyle()
gROOT.SetBatch()


if __name__ == '__main__':
    # Initializing the following lists:
    #     myfile: files
    #     myhist: histograms
    #     myfit: fit pointers
    #     fitopt: options
    #     hxrng: histogram ranges
    #     vEng: fit values
    #     sFit: fit uncertainties
    #     uncFit: relative fit uncertainties
    #     uncEng: relative uncertainties after quadrature
    myfile = {}
    myhist = {}
    myfit  = {}
    fitopt = {}
    hxrng = {}
    vEng   = {}
    sFit   = {}
    uncFit = {}
    uncEng = {}

    # binbase, xmin, xmax : nbins = binbase*(xmax-xmin)
    hxrng["Ref"]   = [256,0,8]
    hxrng["EJ200"] = [256,0,8]
    hxrng["EJ260"] = [256,0,8]
    hxrng["DarkCurrrent"] = [100,-1,0]

    # Define systematic uncertainties
    syst1 = 0.005
    syst2 = 0.03

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    f = open("%s"%options.input,"r")
    finput = f.readlines()

    myfile[finput[0][:-1]+"-UnIrr"] = ["T1",TFile(finput[1][:-1]),[.5,.5]]
    myfile[finput[0][:-1]+"-Irr"]  = ["T1",TFile(finput[2][:-1]),[.5,.5]]

    plotSet = [finput[0][:-1]+"-UnIrr", finput[0][:-1]+"-Irr"]

    ###############################
    ## Step 1: Find Dark Current and --> offset
    ###############################
    fDC50_i = TFile(finput[3][:-1])
    tDC50_i = fDC50_i.Get("tree")
    fDC50_f = TFile(finput[4][:-1])
    tDC50_f = fDC50_f.Get("tree")
    ###############################
    ## Use |amplitude|>mypedcut && time > 0 to skim the data.
    mypedcut = 0.0
    hDC50_i = TH1D("myhist_DC50_i","DC50",52,-1,0.)
    tDC50_i.Draw("area*1.e9>>myhist_DC50_i","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    hDC50_f = TH1D("myhist_DC50_f","DC50",52,-1,0.)
    tDC50_f.Draw("area*1.e9>>myhist_DC50_f","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    ## Find overall max (this is energy offset)
    DCName_i = "DC50_i"
    vEng[DCName_i],sFit[DCName_i],myfit[DCName_i]=AlphaSourceFitter().GausFitEngPeak(hDC50_i,DCName_i,[0.2,0.2],1.)
    DCName_f = "DC50_f"
    vEng[DCName_f],sFit[DCName_f],myfit[DCName_f]=AlphaSourceFitter().GausFitEngPeak(hDC50_f,DCName_f,[0.2,0.2],1.)

    vOffset_i = [vEng[DCName_i],sFit[DCName_i]]
    vOffset_f = [vEng[DCName_f],sFit[DCName_f]]

    ###############################
    ## Step 2: Find gain
    ###############################
    fSR_i = TFile(finput[5][:-1])
    tSR_i = fSR_i.Get("tree")
    fSR_f = TFile(finput[6][:-1])
    tSR_f = fSR_f.Get("tree")
    ###############################
    ## Use |amplitude|>mypedcut && time > 0 to skim the data.
    hSR_i = TH1D("myhist_SR_i","SR",250,0,8)
    tSR_i.Draw("area*1.e9>>myhist_SR_i","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    hSR_f = TH1D("myhist_SR_f","SR",250,0,8)
    tSR_f.Draw("area*1.e9>>myhist_SR_f","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    SRName_i = "SR_i"
    vEng[SRName_i],sFit[SRName_i],myfit[SRName_i]=AlphaSourceFitter().GausFitEngPeak(hSR_i,SRName_i,[0.5,0.5],1.)
    SRName_f = "SR_f"
    vEng[SRName_f],sFit[SRName_f],myfit[SRName_f]=AlphaSourceFitter().GausFitEngPeak(hSR_f,SRName_f,[0.5,0.5],1.)

    vRef_i = [vEng[SRName_i],sFit[SRName_i]]
    vRef_f = [vEng[SRName_f],sFit[SRName_f]]

    fNames = {}
    grTemp_ = {}
    trees_  = {}
    labels_ = {}
    refPlot_  = []
    irrPlots_ = []

    debug_ = False
    ### Loop over the plotSet[0] items for each plotSet[0] (Irr over each UnIrr)
    for ni in range(len(plotSet[0])):
        #### Give a temporary name of the form UnIrr_nameIrr_name)
        tmpName = "%s"%(plotSet[0])
        fNames[tmpName] = plotSet[0][ni]
        print(fNames[tmpName])
        trees_[tmpName]  = (myfile[fNames[tmpName]][1]).Get("tree")
        labels_[tmpName] = fNames[tmpName].split("-")[1]
        fitopt[tmpName] = myfile[fNames[tmpName]][2]

    if debug_:
        print(fNames)
        print(trees_)
        print(labels_)

    ###############################
    # Draw raw plots
    ###############################
    type = "EJ200"
    for ni in range(len(plotSet[0])):
        fntmp = "%s_%i"%(plotSet[0],ni)
        xmin, xmax = hxrng[type][1],hxrng[type][2]
        xbins = int(hxrng[type][0]*(xmax-xmin))
        if debug_: print(xbins, xmin, xmax)

        myhist[fNames[fntmp]] = TH1D("myhist_%s"%(fNames[fntmp]),"",xbins,xmin,xmax)
        trees_[fntmp].Draw("area*1.e9>>myhist_%s"%(fNames[fntmp]),"","")

    ###############################
    ## Step 3: Fit all data histograms
    ###############################
    for ni in range(len(plotSet[0])):
        tmpName = "%s_%i"%(plotSet[0],ni)
        sigName = fNames[tmpName]
        print(">>>>>>>>>>> Processing:", sigName)
        vEng[sigName],sFit[sigName],myfit[sigName]=AlphaSourceFitter().GausFitEngPeak(myhist[sigName],sigName,fitopt[tmpName],1.)
        uncFit[sigName] = sFit[sigName]/vEng[sigName]
        print("Uncertainty of %s Fit     = %-6.3f %%"%(sigName,uncFit[sigName]*100.))

    ###############################
    ## Step 4: D calculation
    ## CalcD(dose,v_o,v_i,offset,uncV)
    ###############################

    vDose = [float(finput[7][:-1]),float(finput[8][:-1])]
    print("\n")
    print("="*150)
    vDconst = {}
    vInput  = {}
    for ni in range(len(plotSet[1])):
        tmpName = "%s_%i"%(plotSet[0],ni)
        sigName = fNames[tmpName]
        uncEng[sigName]  = math.sqrt(math.pow(uncEng[type],2)+math.pow(uncFit[sigName],2))
        vInput[sigName]  = [vEng[sigName], uncEng[sigName]*vEng[sigName]]

    for ni in range(len(plotSet[1])):
        tmpName = "%s_%i"%(plotSet[0],ni)
        sigName = fNames[tmpName]
        if sigName.find("UnIrr") == -1:
            print("Calculating Dose Constant for :", sigName)
            vDconst[sigName] = CalcD(vDose,vInput[sigName],vInput[fNames["%s_0"%(nf)]],vOffset_i,vOffset_f,vRef_i,vRef_f)

    ## print uncertainties
    print("Offset = %-8.5f"%(vOffset_f[0]))
    print("Uncertainty of [%30s] = %6.3f %%"%("Offset",math.fabs(vOffset_f[1]/vOffset_f[0]*100.)))
    for n,v in sorted(uncEng.items()):
        print("Uncertainty of [%30s] = %6.3f %%"%(n,v*100.))
