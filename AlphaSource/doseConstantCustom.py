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

    ## Un-irr samples
    ## myfile["EJ200PS_1X1P-20181115"] = ["T1",TFile("root/AlphaSource/Pu239new_EJ200PVT-1X2P_N1_Default_Nofoil_FaceA_FastFrame_20181120.root")]

    ## [fitrng0,fitrng1,fitterType,biasOffset]
    ## [1.3,1.1,"G2",0.3112]

    myfile["EJ200PS_1X1P-UnIrr"] = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[.5,.5,"G1",0.]]
#    myfile["EJ200PS_1X2P-UnIrr"] = [
#        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_1_Default_Nofoil_FaceA_FastFrame_20181206.root"),[.5,.5,"G1",0.]]
#    myfile["EJ200PS_2X1P-UnIrr"] = [
#        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_1_Default_Nofoil_FaceA_FastFrame_20181129.root"),[.5,.5,"G1",0.]]

    myfile["EJ200PS_1X1P-Irr"]  = [
        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X1P_4_Default_Nofoil_FaceA_FastFrame_20181115.root"),[.5,.5,"G1",0.3112]]
#    myfile["EJ200PS_1X2P-20190117"]  = [
#        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-1X2P_15_Default_Nofoil_FaceA_FastFrame_20190117.root"),[.5,.5,"G1",0.3112]]
#    myfile["EJ200PS_2X1P-20170125"]  = [
#        "T1",TFile("root/AlphaSource/Pu239new_EJ200PS-2X1P_11_Default_Nofoil_FaceA_FastFrame_20170125.root"),[.5,.5,"G1",0.3112]]

    plotSets = {}

    plotSets['1'] = ["EJ200PS_1X1P-UnIrr","EJ200PS_1X1P-Irr"] #ok
#    plotSets['2'] = ["EJ200PS_1X2P-UnIrr","EJ200PS_1X2P-20190117"] #ok
    #plotSets['3'] = ["EJ200PS_2X1P-UnIrr","EJ200PS_2X1P-20190207"] #ok

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
    tDC50.Draw("area*1.e9>>myhist_DC50","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    ## Find overall max (this is energy offset)
    ## Fit 1st hist
    ## Range value seemed suspicious. Total range is [-0.2,0] but he fits in +-0.2 from the maxbin (???)
    ## Need to find the right pedestal cut value
    DCName = "DC50"
    vEng[DCName],sFit[DCName],myfit[DCName]=AlphaSourceFitter().GausFitEngPeak(hDC50,DCName,[0.2,0.2],1.)

    vOffset = [vEng[DCName],sFit[DCName]]

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
        ## Step 2: Fit all data histograms
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames[tmpName]
            print ">>>>>>>>>>> Processing:", sigName
            vEng[sigName],sFit[sigName],myfit[sigName]=AlphaSourceFitter().GausFitEngPeak(myhist[sigName],sigName,fitopt[tmpName],1.)
            uncFit[sigName] = sFit[sigName]/vEng[sigName]
            print "Uncertainty of %s Fit     = %-6.3f %%"%(sigName,uncFit[sigName]*100.)

        ###############################
        ## Step 3: D calculation
        ## CalcD(dose,v_o,v_i,offset,uncV)
        ###############################
        uncEng["EJ200"] = 0.0110424
        uncEng["EJ260"] = 0.0329865
        vDose = [1.32,0.00022]
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
                vDconst[sigName] = CalcD(vDose,vInput[sigName],vInput[fNames["%s_0"%(nf)]],vOffset)
                ### vDose["GIF++"] = [1.32,0.00022]
                ### vInput_[sigName, fNames_[...]] is vInput_[sigName]  = [vEng_[sigName], uncEng_[sigName]*vEng_[sigName]]

    ## print uncertainties
    print "Offset = %-8.5f"%(vOffset[0])
    print "Uncertainty of [%30s] = %6.3f %%"%("Offset",math.fabs(vOffset[1]/vOffset[0]*100.))
    for n,v in sorted(uncEng.items()):
        print "Uncertainty of [%30s] = %6.3f %%"%(n,v*100.)
