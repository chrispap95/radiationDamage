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
    hxrng["Ref"]   = [256,0,6]
    hxrng["EJ200"] = [256,0,6]
    hxrng["EJ260"] = [256,0,6]

    hxrng["T1"] = [256,0,6]

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    ## Output directory
    outDir = "Results/DvsR"
    try:
        os.makedirs(outDir)
    except:
        pass

    f = open("%s"%options.input,"r")
    finput = f.readlines();

    myfile[finput[0][:-1]+"-UnIrr"] = [
        "T1",TFile(finput[1][:-1]),[.5,.5,"G1",0.]]
    myfile[finput[0][:-1]+"-Irr"]  = [
        "T1",TFile(finput[2][:-1]),[.5,.5,"G1",0.3112]]

    plotSets = {}
    plotSets['1'] = [finput[0][:-1]+"-UnIrr",finput[0][:-1]+"-Irr"] #ok

    print(float(finput[7][:-1])*float(finput[8][:-1]))

    ###############################
    ## Step 1: Find Dark Current and --> offset
    ###############################
    ## Dark current common
    ###############################
    ## Load dark current file and the TTree
    fDC50_i = TFile(finput[3][:-1])
    tDC50_i = fDC50_i.Get("tree")
    fDC50_f = TFile(finput[4][:-1])
    tDC50_f = fDC50_f.Get("tree")
    ###############################
    ## Create a histogram and insert the area histogram from the tree.
    ## Use |amplitude|>mypedcut && time > 500 to skim the data.
    ## The hist has limited range and it is only for fitting.
    mypedcut = 0.0
    hDC50_i = TH1D("myhist_DC50_i","DC50",52,-1,0.)
    tDC50_i.Draw("area*1.e9>>myhist_DC50_i","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    hDC50_f = TH1D("myhist_DC50_f","DC50",52,-1,0.)
    tDC50_f.Draw("area*1.e9>>myhist_DC50_f","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    ## Find overall max (this is energy offset)
    ## Fit 1st hist
    ## Range value seemed suspicious. Total range is [-0.2,0] but he fits in +-0.2 from the maxbin (???)
    ## Need to find the right pedestal cut value
    DCName_i = "DC50_i"
    vEng[DCName_i],sFit[DCName_i],myfit[DCName_i]=AlphaSourceFitter().GausFitEngPeak(hDC50_i,DCName_i,[0.2,0.2],1.)
    DCName_f = "DC50_f"
    vEng[DCName_f],sFit[DCName_f],myfit[DCName_f]=AlphaSourceFitter().GausFitEngPeak(hDC50_f,DCName_f,[0.2,0.2],1.)

    vOffset_i = [vEng[DCName_i],sFit[DCName_i]]
    vOffset_f = [vEng[DCName_f],sFit[DCName_f]]

    ###############################
    ## Step 2: Find gain
    ###############################
    ## Load  file and the TTree
    fSR_i = TFile(finput[5][:-1])
    tSR_i = fSR_i.Get("tree")
    fSR_f = TFile(finput[6][:-1])
    tSR_f = fSR_f.Get("tree")
    ###############################
    ## Create a histogram and insert the area histogram from the tree.
    ## Use |amplitude|>mypedcut && time > 500 to skim the data.
    ## The hist has limited range and it is only for fitting.
    hSR_i = TH1D("myhist_SR_i","SR",250,0,6)
    tSR_i.Draw("area*1.e9>>myhist_SR_i","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    hSR_f = TH1D("myhist_SR_f","SR",250,0,6)
    tSR_f.Draw("area*1.e9>>myhist_SR_f","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    ## Find overall max (this is energy offset)
    ## Fit 1st hist
    ## Range value seemed suspicious. Total range is [-0.2,0] but he fits in +-0.2 from the maxbin (???)
    ## Need to find the right pedestal cut value
    SRName_i = "SR_i"
    vEng[SRName_i],sFit[SRName_i],myfit[SRName_i]=AlphaSourceFitter().GausFitEngPeak(hSR_i,SRName_i,[0.5,0.5],1.)
    SRName_f = "SR_f"
    vEng[SRName_f],sFit[SRName_f],myfit[SRName_f]=AlphaSourceFitter().GausFitEngPeak(hSR_f,SRName_f,[0.5,0.5],1.)

    gain = [vEng[SRName_f]/vEng[SRName_i], math.sqrt((sFit[SRName_i]/vEng[SRName_i])**2+(sFit[SRName_f]/vEng[SRName_f])**2)*vEng[SRName_f]/vEng[SRName_i]]

    for nf, fl in sorted(plotSets.items()):
        fNames = {}
        grTemp_ = {}
        trees_  = {}
        labels_ = {}

        refPlot_  = []
        irrPlots_ = []

        debug_ = False
        ### Loop over the fl items for each nf (Irr over each UnIrr)
        for ni in range(len(fl)):
            #### Give a temporary name of the form UnIrr_nameIrr_name)
            tmpName = "%s_%i"%(nf,ni)
            if debug_: print tmpName
            fNames[tmpName] = fl[ni]
            print fNames[tmpName]
            trees_[tmpName]  = (myfile[fNames[tmpName]][1]).Get("tree")
            labels_[tmpName] = fNames[tmpName].split("-")[1]
            fitopt[tmpName] = myfile[fNames[tmpName]][2]

        if debug_:
            print fNames
            print trees_
            print labels_

        ###############################
        # Draw raw plots
        ###############################
        type = "EJ200"
        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            xmin, xmax = hxrng[type][1],hxrng[type][2]
            xbins = int(hxrng[type][0]*(xmax-xmin))
            if debug_: print xbins, xmin, xmax

            myhist[fNames[fntmp]] = TH1D("myhist_%s"%(fNames[fntmp]),"",xbins,xmin,xmax)
            trees_[fntmp].Draw("area*1.e9>>myhist_%s"%(fNames[fntmp]),"","")

        ###############################
        ## Step 3: Fit all data histograms
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            print ">>>>>>>>>>> Processing:", sigName
            vEng[sigName],sFit[sigName],myfit[sigName]=AlphaSourceFitter().GausFitEngPeak(myhist[sigName],sigName,fitopt[tmpName],1.)
            uncFit[sigName] = sFit[sigName]/vEng[sigName]
            print "Uncertainty of %s Fit     = %-6.3f %%"%(sigName,uncFit[sigName]*100.)

        ###############################
        ## Step 4: D calculation
        ## CalcD(dose,v_o,v_i,offset,uncV)
        ###############################
        uncEng["EJ200"] = 0.0110424
        uncEng["EJ260"] = 0.0329865
        vDose = [float(finput[7][:-1]),float(finput[8][:-1])]
        print "\n"
        print "="*150
        vDconst = {}
        vInput  = {}
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            uncEng[sigName]  = math.sqrt(math.pow(uncEng[type],2)+math.pow(uncFit[sigName],2))
            vInput[sigName]  = [vEng[sigName], uncEng[sigName]*vEng[sigName]]

        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            if sigName.find("UnIrr") == -1:
                print "Calculating Dose Constant for :", sigName
                #gain=[1,0]
                vDconst[sigName] = CalcD(vDose,vInput[sigName],vInput[fNames["%s_0"%(nf)]],vOffset_i,vOffset_f,gain)
                ### vDose["GIF++"] = [1.32,0.00022]
                ### vInput_[sigName, fNames_[...]] is vInput_[sigName]  = [vEng_[sigName], uncEng_[sigName]*vEng_[sigName]]

    ## print uncertainties
    print "Offset = %-8.5f"%(vOffset_f[0])
    print "Uncertainty of [%30s] = %6.3f %%"%("Offset",math.fabs(vOffset_f[1]/vOffset_f[0]*100.))
    for n,v in sorted(uncEng.items()):
        print "Uncertainty of [%30s] = %6.3f %%"%(n,v*100.)
