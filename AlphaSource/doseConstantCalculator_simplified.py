#!/usr/bin/python
import os, sys, math, datetime
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory

from collections import OrderedDict
import numpy

sys.path.append(os.path.abspath(os.path.curdir))

from Plotter import parseLYAnaInputArgs
from Plotter.CommonTools import DrawHistSimple, DrawDvsTHist
options = parseLYAnaInputArgs()

from Plotter.CommonTools import AlphaSourceFitter, CalcD

gROOT.LoadMacro("Plotter/UMDStyle.C")
from ROOT import SetUMDStyle
SetUMDStyle()
gROOT.SetBatch()

if __name__ == '__main__':
    # Initializing the following lists:
    #     myfit: fit pointers
    #     vEng: fit values
    #     sFit: fit uncertainties
    #     uncFit: relative fit uncertainties
    #     uncEng: relative uncertainties after quadrature
    myfit  = {}
    vEng   = {}
    sFit   = {}
    uncFit = {}
    uncEng = {}

    syst1 = 0.005
    syst2 = 0.03

    f = open("%s"%options.input,"r")
    finput = f.readlines()

    vDose = [float(finput[6][:-1]),float(finput[6][:-1])*float(finput[7][:-1])]

    today = datetime.date.today()
    fTag = today.strftime("%Y%m%d")

    # Step 1: Find Dark Current and --> offset
    #fDC_i = TFile(finput[1][:-1]+"DarkCurrent_AdTrig_PN6_TO86ms_file20200206_"+finput[2][:8]+"_FF1_"+finput[2][9:-1]+".root")
    fDC_i = TFile(finput[1][:-1]+"DarkCurrent_AdTrig_PN6_TO86ms_file20200206_"+finput[3][:8]+"_"+finput[3][9:-1]+".root")
    tDC_i = fDC_i.Get("tree")
    fDC_f = TFile(finput[1][:-1]+"DarkCurrent_AdTrig_PN6_TO86ms_file20200206_"+finput[3][:8]+"_"+finput[3][9:-1]+".root")
    tDC_f = fDC_f.Get("tree")
    # Use |amplitude|>mypedcut && time > 0 to skim the data.
    mypedcut = 0.0
    hDC_i = TH1D("myhist_DC_i","DC",100,-1,0.)
    tDC_i.Draw("area*1.e9>>myhist_DC_i","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    hDC_f = TH1D("myhist_DC_f","DC",100,-1,0.)
    tDC_f.Draw("area*1.e9>>myhist_DC_f","abs(amplitude)>%f && time>%f"%(mypedcut,0))
    DCName_i = "DC_i"
    vEng[DCName_i],sFit[DCName_i],myfit[DCName_i]=AlphaSourceFitter().GausFitEngPeak(hDC_i,DCName_i,[0.2,0.2],1.)
    DCName_f = "DC_f"
    vEng[DCName_f],sFit[DCName_f],myfit[DCName_f]=AlphaSourceFitter().GausFitEngPeak(hDC_f,DCName_f,[0.2,0.2],1.)
    vOffset_i = [vEng[DCName_i],sFit[DCName_i]]
    vOffset_f = [vEng[DCName_f],sFit[DCName_f]]

    # Step 2: Find gain
    fRef_i = TFile(finput[1][:-1]+"Pu239new_EJ200_2X_3_Default_Nofoil_FaceA_FastFrame_"+finput[2][:8]+"_"+finput[2][9:-1]+".root")
    tRef_i = fRef_i.Get("tree")
    fRef_f = TFile(finput[1][:-1]+"Pu239new_EJ200_2X_3_Default_Nofoil_FaceA_FastFrame_"+finput[3][:8]+"_"+finput[3][9:-1]+".root")
    tRef_f = fRef_f.Get("tree")
    hRef_i = TH1D("myhist_Ref_i","Ref",256,0,8)
    tRef_i.Draw("area*1.e9>>myhist_Ref_i")
    hRef_f = TH1D("myhist_Ref_f","Ref",256,0,8)
    tRef_f.Draw("area*1.e9>>myhist_Ref_f")
    RefName_i = "Ref_i"
    vEng[RefName_i],sFit[RefName_i],myfit[RefName_i] = AlphaSourceFitter().GausFitEngPeak(hRef_i,RefName_i,[0.5,0.5],1.)
    RefName_f = "Ref_f"
    vEng[RefName_f],sFit[RefName_f],myfit[RefName_f] = AlphaSourceFitter().GausFitEngPeak(hRef_f,RefName_f,[0.5,0.5],1.)
    vRef_i = [vEng[RefName_i],sFit[RefName_i]]
    vRef_f = [vEng[RefName_f],sFit[RefName_f]]
    uncFit[RefName_i] = sFit[RefName_i]/vEng[RefName_i]
    uncFit[RefName_f] = sFit[RefName_f]/vEng[RefName_f]
    uncEng[RefName_i] = math.sqrt(math.pow(syst1,2)+math.pow(uncFit[RefName_i],2))
    uncEng[RefName_f] = math.sqrt(math.pow(syst1,2)+math.pow(uncFit[RefName_f],2))
    vRef_i = [vEng[RefName_i],vEng[RefName_i]*uncEng[RefName_i]]
    vRef_f = [vEng[RefName_f],vEng[RefName_f]*uncEng[RefName_f]]

    # Step 3: Fit all data histograms
    fSample_unirr = TFile(finput[1][:-1]+"Pu239new_"+finput[0][:-1]+"_"+finput[4][:-1]+"_Default_Nofoil_FaceA_FastFrame_"+finput[2][:8]+"_"+finput[2][9:-1]+".root")
    tSample_unirr = fSample_unirr.Get("tree")
    fSample_irr = TFile(finput[1][:-1]+"Pu239new_"+finput[0][:-1]+"_"+finput[5][:-1]+"_Default_Nofoil_FaceA_FastFrame_"+finput[3][:8]+"_"+finput[3][9:-1]+".root")
    tSample_irr = fSample_irr.Get("tree")
    hSample_unirr = TH1D("myhist_Sample_unirr","Sample",256,0,8)
    tSample_unirr.Draw("area*1.e9>>myhist_Sample_unirr")
    hSample_irr = TH1D("myhist_Sample_irr","Sample",256,0,8)
    tSample_irr.Draw("area*1.e9>>myhist_Sample_irr")
    SampleName_unirr = "Sample_unirr"
    vEng[SampleName_unirr],sFit[SampleName_unirr],myfit[SampleName_unirr] = AlphaSourceFitter().GausFitEngPeak(hSample_unirr,SampleName_unirr,[0.5,0.5],1.)
    SampleName_irr = "Sample_irr"
    vEng[SampleName_irr],sFit[SampleName_irr],myfit[SampleName_irr] = AlphaSourceFitter().GausFitEngPeak(hSample_irr,SampleName_irr,[0.5,0.5],1.)
    vSample_unirr = [vEng[SampleName_unirr],sFit[SampleName_unirr]]
    vSample_irr = [vEng[SampleName_irr],sFit[SampleName_irr]]
    uncFit[SampleName_unirr] = sFit[SampleName_unirr]/vEng[SampleName_unirr]
    uncFit[SampleName_irr] = sFit[SampleName_irr]/vEng[SampleName_irr]
    uncEng[SampleName_unirr] = math.sqrt(math.pow(syst1,2)+math.pow(syst2,2)+math.pow(uncFit[SampleName_unirr],2))
    uncEng[SampleName_irr] = math.sqrt(math.pow(syst1,2)+math.pow(uncFit[SampleName_irr],2))
    vSample_unirr = [vEng[SampleName_unirr],vEng[SampleName_unirr]*uncEng[SampleName_unirr]]
    vSample_irr = [vEng[SampleName_irr],vEng[SampleName_irr]*uncEng[SampleName_irr]]

    # Step 4: D calculation
    # CalcD(dose,v_o,v_i,offset,uncV)
    vDconst = CalcD(vDose,vSample_irr,vSample_unirr,vOffset_i,vOffset_f,vRef_i,vRef_f)

    print "Uncertainty of %s's dose constant = %6.3f %%\n"%(finput[0][:-1],math.fabs(vDconst[1]/vDconst[0]*100.))
