#!/usr/bin/python
import os, sys, math

sys.path.append(os.path.abspath(os.path.curdir))
from ROOT import gROOT, TH1, TH1F, TH1D, TCanvas, TTree, TLegend, TPad, TFile, TGraph, TMultiGraph, TLatex, TF1, TLine, gStyle, gPad, TGraphErrors
from ROOT import kRed, kBlue, kGreen, kWhite
from collections import OrderedDict
import numpy, array


##gROOT.Reset()
##gROOT.LoadMacro("Plotter/UMDStyle.C")
##from ROOT import SetUMDStyle
##SetUMDStyle()
##gROOT.SetBatch()

from Plotter import parseLYAnaInputArgs
options = parseLYAnaInputArgs()

gROOT.ProcessLine(".L Plotter/MyFunction.C+")
from ROOT import MyFunction, MyStruct, AddressOf
#from ROOT import Math
#Math.MinimizerOptions.SetDefaultMinimizer("Minuit2")

from math import pow,sqrt

####################################################################################################
####################################################################################################
def sortInputs(list_input,excl=[]):
    list_ = []
    tags = ['@', '#', '~', '=', '-', '+']
    comment = '//'

    current_tag = {}
    for t in tags:
        current_tag[t] = 'DEFAULT'
    for line in list_input:
        #print line
        line = line.strip()
        if line.startswith(comment):
            continue
        for t in tags:
            if line.startswith(t):
                current_tag[t] = line.strip(t+' ')
        if line.startswith('Emission'):
            if current_tag['~'] in excl:
                print 'Excluding [%s]: %s'%(current_tag['~'],line)
                continue
            line = line.strip('/')
            list_.append( (line, OrderedDict(sorted(current_tag.items(), key=lambda x: x[0]))) )
    print list_

    return list_

####################################################################################################
####################################################################################################
def getFiles(list_, tag_):
    files = {}
    navg = 0
    for line in list_:
        if line[1]["#"] != tag_: continue
        print 'Processing: %s'%line[0]
        ## filename: (order, legend, (color,style), avg)
        files[line[0]]=(navg,"%s %s %s%s"%(line[1]["@"],line[1]["#"],line[1]["~"],line[1]["+"]),(line[1]["="],line[1]["-"]),"1")
        navg += 1
    #print "[End getFiles]"
    return OrderedDict(sorted(files.items(), key=lambda x: x[1]))

####################################################################################################
####################################################################################################
def getNF(file_, tag_):
    f_ = open(file_,'r')
    for line in f_:
        list_ = (line.strip("\n")).split(":")
        #print list_
        if not isinstance(tag_,list):
            tmptag = tag_
            if list_[0].find(tag_) != -1:
                found = True
                print ">>>>>>>>> Normalization factor [%s] = %f"%(tmptag,float(list_[1]))
                return float(list_[1])
        else:
            tmptag = tag_[0]+"_"+tag_[1]
            if list_[0].find(tag_[0]) != -1 and list_[0].find(tag_[1]) != -1:
                found = True
                print ">>>>>>>>> Normalization factor [%s] = %f"%(tmptag,float(list_[1]))
                return float(list_[1])

    print ">>>>>>>>> NFTag[%s] not found. Using default normalization factor = 1.000"%(tmptag)
    return float(1)

####################################################################################################
####################################################################################################
def numi(s):
    try:
        return int(s)
    except ValueError:
        return 0

def numf(s):
    try:
        return float(s)
    except ValueError:
        return 0.0

####################################################################################################
####################################################################################################
#            
def PlotAlphaNoAvg(files, outtag, x_range, y_range, kw, plotOpts, mypedcut):
    ## plot options
    avghist = plotOpts['average']
    showratio = plotOpts['showratio']
    systplot = not plotOpts['nosystplot']
    myhists = {}

    hbins = 180
    ## kw format: sample name,excitation wavelength
    print "Making Alpha source measurement plot"
    for k in kw:
        print k

    ########################################
    ## Main result plots
    ########################################
    if showratio:
        canvas1 = TCanvas("c1","c1",800,800)
    else:
        canvas1 = TCanvas("c1","c1",800,600)

    canvas1.cd()
    tleg = TLegend(0.2,0.70,0.92,0.93)

    graph={}
    gfunc={}

    if showratio:
        pad1 = TPad("pad1", "", 0, 0.3, 1, 1)
        pad2 = TPad("pad2", "", 0, 0, 1, 0.3)
        pad1.SetBottomMargin(0.02)
        #pad2.SetTopMargin(0)
        pad2.SetBottomMargin(0.4)
        pad2.SetGridy(1)
        pad1.Draw()
        pad2.Draw()


    if showratio:
        pad1.cd()

    ## Loop over files and make graphs
    fidx = 0
    mfiles={}
    mtrees={}
    for fname,vals in files.items():
        print fname, fidx
        hname= "%i"%fidx
        varexp = "areaFromScope*1.e9"
        rootname = 'root/' + fname + ".root"

        mfiles["%i"%fidx]=TFile(rootname)
        mtrees["%i"%fidx]= mfiles["%i"%fidx].Get("tree")

        myhists["%i"%fidx] = TH1F("myhist_%s"%hname,"myhist_%s"%hname,hbins,float(x_range.split(',')[0]),float(x_range.split(',')[1]))
        mtrees["%i"%fidx].Draw("%s>>myhist_%s"%(varexp,hname),"amplitude>%f && %s>=%f && %s<=%f"%(mypedcut, varexp, float(x_range.split(',')[0]),varexp,float(x_range.split(',')[1])))

        if options.verbose:
            print '[Before normalization] histogram area of %40s = %15.2f'%(vals[1],myhists["%i"%fidx].Integral())
        myhists["%i"%fidx].SetLineColor(fidx+1)
        #myhists["%i"%fidx].SetLineColor(int(vals[2][0]))
        #myhists["%i"%fidx].SetLineStyle(int(vals[2][1]))
        myhists["%i"%fidx].SetLineStyle(1)
        myhists["%i"%fidx].SetLineWidth(3)

        if options.verbose:
            print '[After normalization ] Histogram area of %40s = %15.2f'%(vals[1],myhists["%i"%fidx].Integral())

        tleg.AddEntry(myhists["%i"%fidx],vals[1],"l")

        fidx+=1

    print myhists


    fidx = 0 
    for hn,hist in sorted(myhists.items()):
        hist.Scale(1./hist.Integral("width"))
        if fidx == 0:
            hist.GetYaxis().SetRangeUser(1.e-5,1000.)
            gPad.SetLogy()
            hist.Draw()
            hist.GetXaxis().SetTitle("Energy [V#timesns]")
            hist.GetYaxis().SetTitle("(A.U.)")
        else:
            hist.Draw("same")

        fidx+=1

    tleg.SetFillColor(0)
    tleg.SetBorderSize(0)
    tleg.SetTextFont(42)
    tleg.SetHeader(kw[0].replace('_',' '))
    tleg.Draw("same")


    ## save canvas
    canvas1.SaveAs("Results/%s.png"%outtag)
    canvas1.SaveAs("Results/%s.pdf"%outtag)
    canvas1.SaveAs("Results/%s.root"%outtag)
    canvas1.Close()

    ###########################
    ## End of main result plots
    ###########################


    #####################
    ## Systematics plots
    #####################
    if systplot:
        canvas2 = TCanvas("c2","c2",800,600)
        canvas2.cd()
        #hsyst = TH1F('hsyst','syst hist;;#left|#frac{A-#bar{A}}{#bar{A}}#right| [%]',len(hists.items()),0,len(hists.items()))
        hsyst = TH1F('hsyst','syst hist;;#frac{A-#bar{A}}{#bar{A}} [%]',len(hists.items())-1,0,len(hists.items())-1) ## -1 due to hists["havg"]
        gStyle.SetPaintTextFormat("4.2f")

        hidx = 0
        arr_rDeltaA = []
        for nh,hist in sorted(hists.items()):
            if nh == "havg":continue
            #rDeltaA = abs(float(areas[hist.GetTitle()]/areas['ref'])-1.)*100.
            rDeltaA = (float(areas[hist.GetTitle()]/areas['ref'])-1.)*100.
            arr_rDeltaA.append(rDeltaA)
            hsyst.SetBinContent(hidx+1,rDeltaA)
            hsyst.GetXaxis().SetBinLabel(hidx+1,hist.GetTitle())
            hsyst.GetXaxis().LabelsOption("<u")
            hidx += 1

        #hsyst.SetFillColor(3)
        hsyst.SetLineColor(2)
        hsyst.SetMarkerColor(4)
        hsyst.Draw("hist text0")

        ## reference line at delta=0
        reflzero = TLine()
        reflzero.SetLineStyle(3)
        reflzero.SetLineWidth(2)
        #reflzero.SetLineColor(3)
        reflzero.DrawLine(0.,0.,float(hidx),0.)

        ## Calculate std. deviation of rDeltaA as systematic uncertainty
        uncsyst = numpy.std(arr_rDeltaA)
        print 'Systematic uncertainty = %4.2f %%'%uncsyst

        systex = TLatex()
        systex.SetNDC(True)
        systex.SetTextAlign(12)
        systex.SetTextSize(0.03)
        systex.DrawLatex(0.56,0.25, '#splitline{A: integrated area of emission spectrum}{Syst. Unc. = %4.2f %%}'%uncsyst)

        systex1 = TLatex()
        systex1.SetNDC(True)
        systex1.SetTextAlign(12)
        systex1.SetTextSize(0.03)
        systex1.DrawLatex(0.56,0.35, '#splitline{#lambda_{exct} = %s nm}{%s}'%(kw[1],kw[0].replace('_',' ')))

        ## plot ref line of the calculated syst unc (if plotting abs(rDeltaA))
##        if uncsyst > numpy.amax(arr_rDeltaA):
##            hsyst.GetYaxis().SetRangeUser(1.2*numpy.amin(arr_rDeltaA),1.2*uncsyst)
##            canvas2.Update()
##        reflStd = TLine()
##        reflStd.SetLineStyle(2)
##        reflStd.SetLineWidth(2)
##        reflStd.SetLineColor(3)
##        reflStd.DrawLine(0.,uncsyst,float(hidx),uncsyst)

        ## save canvas
        canvas2.SaveAs("Results/%s_syst.png"%outtag)
        canvas2.SaveAs("Results/%s_syst.root"%outtag)
        canvas2.Close()

    ###########################
    ## End of systematics plots
    ###########################

##    return hists
##    temp = raw_input("Click enter/return to quit the script")

    ###########################
    ## End of PlotAlpha function
    ###########################



####################################################################################################
# Find energy peak for dose constant calculation
####################################################################################################
class AlphaSourceFitter:
    def __init__(self):
        ## dummy
        self.allhist = {}
        self.iteration = 0
        self.vEng = 0.
        self.sigmaEng = 0.
        self.myfit = None
        self.syst_vEng = 0.
        self.syst_sigmaEng = 0.
        self.syst_myfit = None

    ####################################################################################################
    ####################################################################################################
    def GausFitEngPeakBase(self,mHist,sigName,fitRng,nScale=1.):
        binmax1 = mHist.GetMaximumBin();
        tempMax = mHist.GetXaxis().GetBinCenter(binmax1)

        myfit = TF1("myfit_%s"%sigName,"gaus",tempMax-fitRng[0],tempMax+fitRng[1])

        tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ0","")
        #tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ","same")
        print "[GausFit Base] chi2 = %f; ndf = %i; nChi2 = %8.5f"%(myfit.GetChisquare()/nScale,myfit.GetNDF()+1,
                                                                   myfit.GetChisquare()/nScale/(myfit.GetNDF()+1))
        #if tmpstatus.Status() != 0:
        if tmpstatus.Status() != 4000: # with "M" fit option
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit failed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"
            print "Fit Status = ",tmpstatus.Status()
            print "EDM = ",tmpstatus.Edm()
            #mHist.Fit("myfit_%s"%sigName,"REM","")
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Suggest to change fit range and retry <<<<<<<<<<<<<<<<<<<<<<<<<<<]"
    ##    else:
    ##        print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit converged <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"

        vEng     = myfit.GetParameter(1)
        sigmaEng = myfit.GetParError(1)*sqrt(nScale)

        #print "Delta Mean for  %-20s = %8.5f %%"%(sigName,(tempMax-vEng)/tempMax*100.)
        print "[GausFit Base] Energy peak for %-20s = %8.5f +/- %-8.5f"%(sigName,vEng,sigmaEng)
        return vEng,sigmaEng,myfit


    ####################################################################################################
    # Full Gaussian Fit including Systematics
    ####################################################################################################
    def GausFitEngPeak(self,mHist,sigName,fitRng,nScale=1.0):
        systEng = {}
        tmpFit = {}

        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
        ########################################################################
        ## Nominal
        ########################################################################
        vEng,sigmaEng,myfit = self.GausFitEngPeakBase(mHist,sigName,fitRng,nScale)
        print "="*100
        print "[GausFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
        print "="*100

        ########################################################################
        ## FitRange Systematics
        ########################################################################
        ## UP
        systName = "FitRngUp"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausFitEngPeakBase(mHist,sigName,systFitRng,nScale)
        print "[GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ## DOWN
        systName = "FitRngDN"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausFitEngPeakBase(mHist,sigName,systFitRng,nScale)
        print "[GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ########################################################################
        ## Rebin Systematics
        ########################################################################
        systName = "RebinHalf"
        systEng[systName] = [0.0,0.0]

        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
        systHist.Rebin(2)
        nScale_ = nScale*2.0

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausFitEngPeakBase(systHist,sigName,fitRng,nScale_)
        print "[GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

        ########################################################################
        ## Calculate Total Fit Systematics
        ########################################################################
        sigmaSyst = 0.0
        for n,v in sorted(systEng.items()):
            sigmaSyst += pow(v[0]-vEng,2)

        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

        print "="*100
        print "[GausFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
        print "="*100
        ########################################################################
        ## Return Nominal Values
        ########################################################################
        return vEng,sigmaEngTot,myfit


    ####################################################################################################
    ####################################################################################################
    def LandauFitEngPeakBase(self,mHist,sigName,fitRng,nScale=1.):
        binmax1 = mHist.GetMaximumBin();
        tempMax = mHist.GetXaxis().GetBinCenter(binmax1)

        myfit = TF1("myfit_%s"%sigName,"landau",tempMax-fitRng[0],tempMax+fitRng[1])

        tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ0","")
        #tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREM","same")
        print "[LandauFit Base] chi2 = %f; ndf = %i; nChi2 = %8.5f"%(myfit.GetChisquare()/nScale,myfit.GetNDF()+1,
                                                                     myfit.GetChisquare()/nScale/(myfit.GetNDF()+1))
        #if tmpstatus.Status() != 0:
        if tmpstatus.Status() != 4000: # with "M" fit option
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit failed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"
            print "Fit Status = ",tmpstatus.Status()
            print "EDM = ",tmpstatus.Edm()
            mHist.Fit("myfit_%s"%sigName,"R","")
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Suggest to change fit range and retry <<<<<<<<<<<<<<<<<<<<<<<<<<<]"
    ##    else:
    ##        print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit converged <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"

        vEng     = myfit.GetParameter(1)
        sigmaEng = myfit.GetParError(1)*sqrt(nScale)

        #print "Delta Mean for  %-20s = %8.5f %%"%(sigName,(tempMax-vEng)/tempMax*100.)
        print "[LandauFit Base] Energy peak for %-20s = %8.5f +/- %-8.5f"%(sigName,vEng,sigmaEng)
        return vEng,sigmaEng,myfit


    ####################################################################################################
    # Full Landau Fit including Systematics
    ####################################################################################################
    def LandauFitEngPeak(self,mHist,sigName,fitRng,nScale=1.0):
        systEng = {}
        tmpFit = {}

        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
        ########################################################################
        ## Nominal
        ########################################################################
        vEng,sigmaEng,myfit = self.LandauFitEngPeakBase(mHist,sigName,fitRng,nScale)
        print "="*100
        print "[LandauFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
        print "="*100

        ########################################################################
        ## FitRange Systematics
        ########################################################################
        ## UP
        systName = "FitRngUp"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.LandauFitEngPeakBase(mHist,sigName,systFitRng,nScale)
        print "[LandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ## DOWN
        systName = "FitRngDN"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.LandauFitEngPeakBase(mHist,sigName,systFitRng,nScale)
        print "[LandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ########################################################################
        ## Rebin Systematics
        ########################################################################
        systName = "RebinHalf"
        systEng[systName] = [0.0,0.0]

        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
        systHist.Rebin(2)
        nScale_ = nScale*2.0

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.LandauFitEngPeakBase(systHist,sigName,fitRng,nScale_)
        print "[LandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

        ########################################################################
        ## Calculate Total Fit Systematics
        ########################################################################
        sigmaSyst = 0.0
        for n,v in sorted(systEng.items()):
            sigmaSyst += pow(v[0]-vEng,2)

        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

        print "="*100
        print "[LandauFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
        print "="*100
        ########################################################################
        ## Return Nominal Values
        ########################################################################
        return vEng,sigmaEngTot,myfit


    ####################################################################################################
    ####################################################################################################
    def TwoGausFitEngPeakBase(self,mHist,sigName,fitRng,nScale=1.0,rType="default"):
        binmax1 = mHist.GetMaximumBin();
        tempMax = mHist.GetXaxis().GetBinCenter(binmax1)
        cScale  = 0.8

        if rType == "default":
            FitRng_ = [tempMax-fitRng[0],tempMax+fitRng[1]]
        elif rType == "EJ2604X4P":
            cScale = 0.6
            FitRng_ = fitRng
        elif rType == "EJ200HighDose":
            cScale = 0.5
            FitRng_ = fitRng
        else: # dummy
            print "[2GausFit Base] Type = %s not defined!"%rType
            exit()

        tempfit = TF1("tempfit_%s"%sigName,"gaus",FitRng_[0],FitRng_[1])
        tmpstatus0 = mHist.Fit("tempfit_%s"%sigName,"SRQ0","")
        #tmpstatus0 = mHist.Fit("tempfit_%s"%sigName,"SRQ","same")

        CosmicPeak = tempfit.GetParameter(1)*cScale
        print "[2GausFit Base] CosmicPeak = %8.5f (%8.5f)"%(CosmicPeak,tempfit.GetParameter(1))

        myfit = TF1("myfit_%s"%sigName,"gaus(0)+gaus(3)",FitRng_[0],FitRng_[1])

        if rType == "EJ2604X4P":
            myfit.SetParameter(1,CosmicPeak)
            myfit.SetParLimits(1,FitRng_[0],min(CosmicPeak*1.5,FitRng_[1],tempMax*0.95))
            myfit.SetParameter(2,0.4)
            myfit.SetParLimits(2,0.001,1.5)
            myfit.SetParameter(4,tempMax)
            myfit.SetParLimits(4,max(tempMax*0.5,FitRng_[0]),min(tempMax*1.2,FitRng_[1]))
            myfit.SetParameter(5,0.2)
            myfit.SetParLimits(5,0.001,1.0)

        else:
            myfit.SetParameter(1,CosmicPeak)
            myfit.SetParLimits(1,FitRng_[0],CosmicPeak*1.5)
            myfit.SetParameter(2,0.6)
            myfit.SetParLimits(2,0.001,1.5)
            myfit.SetParameter(4,tempMax)
            myfit.SetParLimits(4,tempMax*0.8,tempMax*1.2)
            myfit.SetParameter(5,0.4)
            myfit.SetParLimits(5,0.001,1.0)

        tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ0","")
        #tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREM","same")
        print "[2GausFit Base] chi2 = %f; ndf = %i; nChi2 = %8.5f"%(myfit.GetChisquare()/nScale,myfit.GetNDF()+1,
                                                                    myfit.GetChisquare()/nScale/(myfit.GetNDF()+1))
        #if tmpstatus.Status() != 0:
        if tmpstatus.Status() != 4000: # with "M" fit option
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit failed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"
            print "Fit Status = ",tmpstatus.Status()
            print "EDM = ",tmpstatus.Edm()
            #mHist.Fit("myfit_%s"%sigName,"REM","")
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Suggest to change fit range and retry <<<<<<<<<<<<<<<<<<<<<<<<<<<]"
    ##    else:
    ##        print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit converged <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"

        vEng     = myfit.GetParameter(4)
        sigmaEng = myfit.GetParError(4)*sqrt(nScale)

        fMaxX = myfit.GetMaximumX(FitRng_[0],FitRng_[1])
        print "[2GausFit Base] Function peak for %-18s = %8.5f"%(sigName,fMaxX)
        print "[2GausFit Base] Energy peak for %-20s = %8.5f +/- %-8.5f"%(sigName,vEng,sigmaEng)

        vDiff = abs(fMaxX-vEng)/fMaxX
        if  vDiff > 0.1:
            print "[WARNING] Exceedingly large difference btw Func max and energy peak [%5.2f%%]! Check fitting results!"%(vDiff*100.)

        sigmaEng = sqrt(pow(fMaxX-vEng,2)+sigmaEng*sigmaEng)
        return vEng,sigmaEng,myfit


    ####################################################################################################
    # Full TwoGaussian Fit including Systematics
    ####################################################################################################
    def TwoGausFitEngPeak(self,mHist,sigName,fitRng,nScale=1.0,rType="default"):
        systEng = {}
        tmpFit = {}

        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
        ########################################################################
        ## Nominal
        ########################################################################
        vEng,sigmaEng,myfit = self.TwoGausFitEngPeakBase(mHist,sigName,fitRng,nScale,rType)
        print "="*100
        print "[2GausFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
        print "="*100

        ########################################################################
        ## FitRange Systematics
        ########################################################################
        ## UP
        systName = "FitRngUp"
        systEng[systName] = [0.0,0.0]

        if rType == "EJ2604X4P":
            systFitRng = [fitRng[0]*0.95,fitRng[1]*1.05]
        else:
            systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ## DOWN
        systName = "FitRngDN"
        systEng[systName] = [0.0,0.0]

        if rType == "EJ2604X4P":
            systFitRng = [fitRng[0]*1.05,fitRng[1]*0.95]
        else:
            systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ########################################################################
        ## Rebin Systematics
        ########################################################################
        systName = "RebinHalf"
        systEng[systName] = [0.0,0.0]

        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
        systHist.Rebin(2)
        nScale_ = nScale*2.0

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(systHist,sigName,fitRng,nScale_,rType)
        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

        ########################################################################
        ## Calculate Total Fit Systematics
        ########################################################################
        sigmaSyst = 0.0
        for n,v in sorted(systEng.items()):
            sigmaSyst += pow(v[0]-vEng,2)

        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

        print "="*100
        print "[2GausFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
        print "="*100
        ########################################################################
        ## Return Nominal Values
        ########################################################################
        return vEng,sigmaEngTot,myfit


    ####################################################################################################
    ####################################################################################################
    def TriGausFitEngPeakBase(self,mHist,sigName,fitRng,nScale,rType="default"):
        binmax1 = mHist.GetMaximumBin();
        tempMax = mHist.GetXaxis().GetBinCenter(binmax1)
        cScale  = 0.6

        if rType == "EJ2604X4P":
            cScale = 0.4
            FitRng0_ = fitRng
            FitRng_  = fitRng
        else: #EJ260
            FitRng0_ = [tempMax-fitRng[0],tempMax+fitRng[1]]
            FitRng_  = [0.5+fitRng[0],tempMax+fitRng[1]]


        tempfit = TF1("tempfit_%s"%sigName,"gaus",FitRng0_[0],FitRng0_[1])
        tmpstatus0 = mHist.Fit("tempfit_%s"%sigName,"SRQ0","")
        #tmpstatus0 = mHist.Fit("tempfit_%s"%sigName,"SRQ","same")

        ThermalPeak = tempfit.GetParameter(1)*cScale
        print "[3GausFit Base] ThermalPeak = %8.5f (%8.5f)"%(ThermalPeak,tempfit.GetParameter(1))

        myfit = TF1("myfit_%s"%sigName,"gaus(0)+gaus(3)+gaus(6)",FitRng_[0],FitRng_[1])
        if rType == "EJ2604X4P":
            myfit.SetParameter(1,ThermalPeak)
            myfit.SetParLimits(1,min(ThermalPeak*0.8,FitRng_[0]),min(ThermalPeak*1.2,FitRng_[1]))
            myfit.SetParameter(2,0.4)
            myfit.SetParLimits(2,0.01,1.5)
            myfit.SetParameter(4,ThermalPeak*1.2)
            myfit.SetParLimits(4,ThermalPeak*1.1,tempMax*0.99)
            myfit.SetParameter(5,0.3)
            myfit.SetParLimits(5,0.01,0.8)
            myfit.SetParameter(7,tempMax)
            myfit.SetParLimits(7,max(tempMax*0.9,FitRng_[0]),min(tempMax*1.1,FitRng_[1]))
            myfit.SetParameter(8,0.3)
            myfit.SetParLimits(8,0.01,0.5)
        else: # EJ260
            myfit.SetParameter(1,ThermalPeak)
            myfit.SetParLimits(1,0.5,ThermalPeak*1.5)
            myfit.SetParameter(2,0.3)
            myfit.SetParLimits(2,0.01,2.0)
            myfit.SetParameter(4,ThermalPeak*1.2)
            myfit.SetParLimits(4,ThermalPeak*1.1,tempMax)
            myfit.SetParameter(5,0.3)
            myfit.SetParLimits(5,0.01,1.0)
            myfit.SetParameter(7,tempMax)
            myfit.SetParLimits(7,tempMax*0.8,tempMax*1.2)
            myfit.SetParameter(8,0.3)
            myfit.SetParLimits(8,0.01,1.0)

        tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ0","")
        #tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREM","same")
        print "[3GausFit Base] chi2 = %f; ndf = %i; nChi2 = %8.5f"%(myfit.GetChisquare()/nScale,myfit.GetNDF()+1,
                                                                    myfit.GetChisquare()/nScale/(myfit.GetNDF()+1))
        #if tmpstatus.Status() != 0:
        if tmpstatus.Status() != 4000: # with "M" fit option
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit failed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"
            print "Fit Status = ",tmpstatus.Status()
            print "EDM = ",tmpstatus.Edm()
            #mHist.Fit("myfit_%s"%sigName,"REM","")
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Suggest to change fit range and retry <<<<<<<<<<<<<<<<<<<<<<<<<<<]"
    ##    else:
    ##        print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit converged <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"

        vEng     = myfit.GetParameter(7)
        sigmaEng = myfit.GetParError(7)*sqrt(nScale)

        fMaxX = myfit.GetMaximumX(FitRng_[0],FitRng_[1])
        print "[3GausFit Base] Function peak for %-18s = %8.5f"%(sigName,fMaxX)
        print "[3GausFit Base] Energy peak for %-20s = %8.5f +/- %-8.5f"%(sigName,vEng,sigmaEng)

        vDiff = abs(fMaxX-vEng)/fMaxX
        if  vDiff > 0.1:
            print "[WARNING] Exceedingly large difference btw Func max and energy peak [%5.2f%%]! Check fitting results!"%(vDiff*100.)

        sigmaEng = sqrt(pow(fMaxX-vEng,2)+sigmaEng*sigmaEng)
        return vEng,sigmaEng,myfit


    ####################################################################################################
    # Full TriGaussian Fit including Systematics
    ####################################################################################################
    def TriGausFitEngPeak(self,mHist,sigName,fitRng,nScale=1.0,rType="default"):
        systEng = {}
        tmpFit = {}

        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
        ########################################################################
        ## Nominal
        ########################################################################
        vEng,sigmaEng,myfit = self.TriGausFitEngPeakBase(mHist,sigName,fitRng,nScale,rType)
        print "="*100
        print "[3GausFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
        print "="*100

        ########################################################################
        ## FitRange Systematics
        ########################################################################
        ## UP
        systName = "FitRngUp"
        systEng[systName] = [0.0,0.0]

        if rType == "EJ2604X4P":
            systFitRng = [fitRng[0]*0.95,fitRng[1]*1.05]
        else:
            systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TriGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
        print "[3GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ## DOWN
        systName = "FitRngDN"
        systEng[systName] = [0.0,0.0]

        if rType == "EJ2604X4P":
            systFitRng = [fitRng[0]*1.05,fitRng[1]*0.95]
        else:
            systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TriGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
        print "[3GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ########################################################################
        ## Rebin Systematics
        ########################################################################
        systName = "RebinHalf"
        systEng[systName] = [0.0,0.0]

        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
        systHist.Rebin(2)
        nScale_ = nScale*2.0

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TriGausFitEngPeakBase(systHist,sigName,fitRng,nScale_,rType)
        print "[3GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

        ########################################################################
        ## Calculate Total Fit Systematics
        ########################################################################
        sigmaSyst = 0.0
        for n,v in sorted(systEng.items()):
            sigmaSyst += pow(v[0]-vEng,2)

        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

        print "="*100
        print "[3GausFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
        print "="*100
        ########################################################################
        ## Return Nominal Values
        ########################################################################
        return vEng,sigmaEngTot,myfit


##    ####################################################################################################
##    # Full fits including all fit Systematics (range, binning, model...)
##    # Note : fit models are single Gaussian vs. Two Gaussian
##    ####################################################################################################
##    def FitEngPeak(self,mHist,sigName,fitRng,nScale=1.0,rType="default"):
##        systEng = {}
##        tmpFit = {}

##        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
##        ########################################################################
##        ## Nominal
##        ########################################################################
##        vEng,sigmaEng,myfit = self.TwoGausFitEngPeakBase(mHist,sigName,fitRng,nScale,rType)
##        print "="*100
##        print "[2GausFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
##        print "="*100

##        ########################################################################
##        ## FitRange Systematics
##        ########################################################################
##        ## UP
##        systName = "FitRngUp"
##        systEng[systName] = [0.0,0.0]

##        if rType == "EJ2604X4P":
##            systFitRng = [fitRng[0]*0.95,fitRng[1]*1.05]
##        else:
##            systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]

##        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
##        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
##        print "-"*100

##        ## DOWN
##        systName = "FitRngDN"
##        systEng[systName] = [0.0,0.0]

##        if rType == "EJ2604X4P":
##            systFitRng = [fitRng[0]*1.05,fitRng[1]*0.95]
##        else:
##            systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]

##        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(mHist,sigName,systFitRng,nScale,rType)
##        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
##        print "-"*100

##        ########################################################################
##        ## Rebin Systematics
##        ########################################################################
##        systName = "RebinHalf"
##        systEng[systName] = [0.0,0.0]

##        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
##        systHist.Rebin(2)
##        nScale_ = nScale*2.0

##        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.TwoGausFitEngPeakBase(systHist,sigName,fitRng,nScale_,rType)
##        print "[2GausFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

##        ########################################################################
##        ## Calculate Total Fit Systematics
##        ########################################################################
##        sigmaSyst = 0.0
##        for n,v in sorted(systEng.items()):
##            sigmaSyst += pow(v[0]-vEng,2)

##        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

##        print "="*100
##        print "[2GausFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
##        print "="*100
##        ########################################################################
##        ## Return Nominal Values
##        ########################################################################
##        return vEng,sigmaEngTot,myfit


    ####################################################################################################
    ####################################################################################################
    def GausLandFitEngPeakBase(self,mHist,sigName,fitRng,pars_=[],nScale=1.):
        binmax1 = mHist.GetMaximumBin();
        tempMax = mHist.GetXaxis().GetBinCenter(binmax1)

        #myfit = TF1("myfit_%s"%sigName,"gaus(0)+landau(3)",-2.2,-0.5)
        myfit = TF1("myfit_%s"%sigName,"gaus(0)+landau(3)",tempMax-fitRng[0],tempMax+fitRng[1])
        #myfit.SetParameters(3.62767e+03,-1.71454e+00,1.58723e-01,4.10776e+03,-1.34531e+00,9.12575e-02)

        if len(pars_)>0:
            print pars_
            myfit.SetParameters(pars_[0],pars_[1],pars_[2],pars_[3],pars_[4],pars_[5])

        tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ0","")
        #tmpstatus = mHist.Fit("myfit_%s"%sigName,"SREMQ","same")
        print "[GausLandauFit Base] chi2 = %f; ndf = %i; nChi2 = %8.5f"%(myfit.GetChisquare()/nScale,myfit.GetNDF()+1,
                                                                   myfit.GetChisquare()/nScale/(myfit.GetNDF()+1))
        #if tmpstatus.Status() != 0:
        if tmpstatus.Status() != 4000: # with "M" fit option
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit failed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"
            print "Fit Status = ",tmpstatus.Status()
            print "EDM = ",tmpstatus.Edm()
            #mHist.Fit("myfit_%s"%sigName,"REM","")
            print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Suggest to change fit range and retry <<<<<<<<<<<<<<<<<<<<<<<<<<<]"
    ##    else:
    ##        print "[>>>>>>>>>>>>>>>>>>>>>>>>>>> Fit converged <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]"

        vEng     = myfit.GetParameter(1)
        sigmaEng = myfit.GetParError(1)*sqrt(nScale)

        #print "Delta Mean for  %-20s = %8.5f %%"%(sigName,(tempMax-vEng)/tempMax*100.)
        print "[GausLandauFit Base] Energy peak for %-20s = %8.5f +/- %-8.5f"%(sigName,vEng,sigmaEng)
        return vEng,sigmaEng,myfit


    ####################################################################################################
    # Full Gaus+Landau Fit including Systematics
    ####################################################################################################
    def GausLandFitEngPeak(self,mHist,sigName,fitRng,pars_,nScale=1.0):
        systEng = {}
        tmpFit = {}
        print pars_
        print "{s:{c}>{n}}".format(s=" %s"%sigName,n=50,c='>'),"<"*49
        ########################################################################
        ## Nominal
        ########################################################################
        vEng,sigmaEng,myfit = self.GausLandFitEngPeakBase(mHist,sigName,fitRng,pars_,nScale)
        print "="*100
        print "[GausLandauFit Nominal] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEng)
        print "="*100

        ########################################################################
        ## FitRange Systematics
        ########################################################################
        ## UP
        systName = "FitRngUp"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*1.05,fitRng[1]*1.05]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausLandFitEngPeakBase(mHist,sigName,systFitRng,pars_,nScale)
        print "[GausLandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ## DOWN
        systName = "FitRngDN"
        systEng[systName] = [0.0,0.0]

        systFitRng = [fitRng[0]*0.95,fitRng[1]*0.95]
        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausLandFitEngPeakBase(mHist,sigName,systFitRng,pars_,nScale)
        print "[GausLandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])
        print "-"*100

        ########################################################################
        ## Rebin Systematics
        ########################################################################
        systName = "RebinHalf"
        systEng[systName] = [0.0,0.0]

        systHist = mHist.Clone("syst_RebinHalf_%s"%mHist.GetTitle())
        systHist.Rebin(2)
        nScale_ = nScale*2.0

        systEng[systName][0],systEng[systName][1],tmpFit[systName] = self.GausLandFitEngPeakBase(systHist,sigName,fitRng,pars_,nScale_)
        print "[GausLandauFit Systematic (%s)] vEng,sigmaEng = (%8.5f,%8.5f)"%(systName,systEng[systName][0],systEng[systName][1])

        ########################################################################
        ## Calculate Total Fit Systematics
        ########################################################################
        sigmaSyst = 0.0
        for n,v in sorted(systEng.items()):
            sigmaSyst += pow(v[0]-vEng,2)

        sigmaEngTot = sqrt(sigmaEng*sigmaEng+sigmaSyst)

        print "="*100
        print "[GausLandauFit Final (+syst.)] Energy peak for <<%s>> = %8.5f +/- %-8.5f"%("{:^20}".format(sigName),vEng,sigmaEngTot)
        print "="*100
        ########################################################################
        ## Return Nominal Values
        ########################################################################
        return vEng,sigmaEngTot,myfit


####################################################################################################
# Calculate dose constant and uncertainty
####################################################################################################
def CalcD(dose,v_f,v_i,voffset,type="alpha"):
    # v_i : initial light yield non-irr.
    # v_f : final light yield after irr.
    # all uncertaintiey treated as independent variables
    R = (v_f[0]-voffset[0])/(v_i[0]-voffset[0])
    sigmaR = R*sqrt(pow(v_f[1]/(v_f[0]-voffset[0]),2)+pow(v_i[1]/(v_i[0]-voffset[0]),2)+pow(voffset[1]*(v_f[0]-v_i[0])/((v_i[0]-voffset[0])*(v_f[0]-voffset[0])),2))
    D = -1.*dose[0]/math.log(R)
    sigmaD = D*sqrt(pow(dose[1]/dose[0],2)+pow(D*sigmaR/(dose[0]*R),2))
    ## sigma measurement
    sigma0 = pow(D,4)*pow(v_f[1]/dose[0],2)*pow(v_f[0]-voffset[0],-2)
    sigma1 = pow(D,4)*pow(v_i[1]/dose[0],2)*pow(v_i[0]-voffset[0],-2)
    ## sigma offset
    sigma2 = pow(D,4)*pow(voffset[1]/dose[0],2)*pow(v_i[0]-v_f[0],2)*pow((v_i[0]-voffset[0])*(v_f[0]-voffset[0]),-2)
    ## sigma dose
    sigma3 = pow(D*dose[1]/dose[0],2)

    print "%-30s = %8.5f"%("[CalcD] Light yield ratio",R)
    print "%-30s = %8.5f"%("[CalcD] Dose constant [Mrad]",D)
    print "%-30s = [%8.5f,%8.5f,%8.5f,%8.5f]"%("[CalcD] sigmas",sqrt(sigma0),sqrt(sigma1),sqrt(sigma2),sqrt(sigma3))
    print "%-30s = [%5.2f, %5.2f, %5.2f]"%("[CalcD] Uncertainties (%)",sqrt(sigma0+sigma1)/D*100.,sqrt(sigma2)/D*100.,sqrt(sigma3)/D*100.)
    #print "%-30s = %8.5f"%("sigmaD diff",sigmaD-sqrt(sigma0+sigma1+sigma2+sigma3))
    if type=="alpha":
        # SPE: 0.01497
        NPE = CalcNPE(v_f[0],voffset[0])
    elif type=="Na22":
        # SPE: 0.908949
        NPE = CalcNPENa22(v_f[0],voffset[0])
    else:
        NPE = CalcNPEMIP(v_f[0],voffset[0])

    return D,sigmaD,R,sigmaR,NPE

##def CalcD(dose,v_f,v_i,voffset,type="alpha"):
##    # v_i : initial light yield non-irr.
##    # v_f : final light yield after irr.
##    # all uncertaintiey treated as independent variables
##    R = (v_f[0]-voffset[0])/(v_i[0]-voffset[0])
##    sigmaR = R*sqrt(pow(v_f[1]/(v_f[0]-voffset[0]),2)+pow(v_i[1]/(v_i[0]-voffset[0]),2))
##    #D = dose[0]/(math.log(v_i[0]-voffset[0])-math.log(v_f[0]-voffset[0]))
##    D = -1.*dose[0]/math.log(R)
##    ## sigma measurement
##    sigma0 = pow(D,4)*pow(v_f[1]/dose[0],2)*pow(v_f[0]-voffset[0],-2)
##    sigma1 = pow(D,4)*pow(v_i[1]/dose[0],2)*pow(v_i[0]-voffset[0],-2)
##    ## sigma offset
##    sigma2 = pow(D,4)*pow(voffset[1]/dose[0],2)*pow(v_i[0]-v_f[0],2)*pow((v_i[0]-voffset[0])*(v_f[0]-voffset[0]),-2)
##    ## sigma dose
##    sigma3 = pow(D*dose[1],2)
##    print "%-30s = %8.5f"%("[CalcD] Light yield ratio",R)
##    print "%-30s = %8.5f"%("[CalcD] Dose constant [Mrad]",D)
##    print "%-30s = [%8.5f,%8.5f,%8.5f,%8.5f]"%("[CalcD] sigmas",sqrt(sigma0),sqrt(sigma1),sqrt(sigma2),sqrt(sigma3))
##    print "%-30s = [%5.2f, %5.2f, %5.2f]"%("[CalcD] Uncertainties (%)",sqrt(sigma0+sigma1)/D*100.,sqrt(sigma2)/D*100.,sqrt(sigma3)/D*100.)
##    if type=="alpha":
##        # SPE: 0.01497
##        NPE = CalcNPE(v_f[0],voffset[0])
##    elif type=="Na22":
##        # SPE: 0.908949
##        NPE = CalcNPENa22(v_f[0],voffset[0])
##    else:
##        NPE = CalcNPEMIP(v_f[0],voffset[0])

##    return D,sqrt(sigma0+sigma1+sigma2+sigma3),R,sigmaR,NPE


####################################################################################################
# Calculate Npe/MeV (Pu-239 source; HPK R6091 1700V)
####################################################################################################
def CalcNPE(E,voffset,vSPE=0.01497,ePar=5.2): #5.2MeV
    NPE = float((E-voffset)/vSPE/ePar)
    return NPE


####################################################################################################
# Calculate Npe/MeV (Na-22 source; CAEN-10C 71V)
####################################################################################################
def CalcNPENa22(E,voffset,vSPE=0.908949,ePar=1.27): # 1.27MeV
    NPE = float((E-voffset)/vSPE/ePar)
    return NPE


####################################################################################################
# Calculate Npe/MeV (Cosmic): FIXME
####################################################################################################
def CalcNPEMIP(E,voffset,vSPE=1.0,ePar=1.0): # 1 MeV
    NPE = float((E-voffset)/vSPE/ePar)
    return NPE

####################################################################################################
####################################################################################################
## Modified from example from here:
## http://nullege.com/codes/show/src%40f%40i%40FinalStateAnalysis-HEAD%40PlotTools%40python%40HistToTGRaphErrors.py/13/ROOT.TGraphErrors/python
## Note: this is different from PSAnalysis, what concerns here is energy along x-axis
####################################################################################################
def HistToTGRaphErrorsUniform(h,nf=1,type=0,unc_=0.02):
    'Transforms a TH1/ rootpy.THist into a TGRaphErrors with same values and errors'
    x, y, ex, ey = (array.array('d',[]), array.array('d',[]), array.array('d',[]), array.array('d',[]) )

    print "HERE",unc_
    for ibin in xrange(h.GetNbinsX()):
        if ibin%nf == 0 and h.GetBinContent(ibin+1)>0.:
            x.append(h.GetBinCenter(ibin+1))
            y.append(h.GetBinContent(ibin+1))
            ey.append(sqrt(h.GetBinContent(ibin+1)))
            if type==0:
                ex.append(abs(h.GetBinCenter(ibin+1)*unc_))
                if options.verbose:
                    print "%i = %5.3f"%(ibin,abs(h.GetBinCenter(ibin+1)*unc_))
            else:
                print "WARNING: no other type."
                exit()

    ## Debug
    #print x[0],":",ey[0]
    gr = TGraphErrors(len(x),x,y,ex,ey)
    gr.SetTitle("gr_"+h.GetTitle())
    gr.SetLineColor(h.GetLineColor())
    gr.SetLineStyle(h.GetLineStyle())
    gr.SetLineWidth(h.GetLineWidth())
    gr.SetMarkerColor(h.GetMarkerColor())
    gr.SetMarkerStyle(1)
    gr.SetMarkerSize(0)
    gr.SetFillColor(h.GetLineColor())
    gr.SetFillStyle(3002)

    return gr
  
def HistStackToTGRaphErrorsUniform(stack):
    'Transforms a rootpy.HistStack into a TGRaphErrors with same values and errors'
    return HistToTGRaphErrorsUniform(sum(stack.GetHists()))

####################################################################################################
# Common drawing routine for SimpleAlpha comparirons
####################################################################################################
def DrawHistSimple(myfiles_, plotSets_, outDir_, fTag_, doselabel_, hxrng_, options_, debug_=False):

    ### Looping and making histos
    myhist_ = {}
    fNames_ = {}
    trees_  = {}
    labels_ = {}
    colors_ = [2,4,6,9,8,3,7]

    for nf, fl in sorted(plotSets_.items()):
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            if debug_: print tmpName
            fNames_[tmpName] = fl[ni]
            trees_[tmpName] = (myfiles_[fNames_[tmpName]][1]).Get("tree")
            labels_[tmpName] = fNames_[tmpName].split("-")[1]

        if debug_:
            print fNames_
            print trees_
            print labels_

        cvsName = "c%s"%nf
        c1 = TCanvas(cvsName,cvsName,800,600)


        ## Hist label
        ## e.g.: EJ200PVT_1X2P_N14-20170224 => label_ = 20170224 ; header_ = EJ200PVT-1X2P-N14
        header_ = fNames_["%s_0"%(nf)].split("-")[0]
        type_   = myfiles_[fNames_["%s_0"%(nf)]][0]

        leg = TLegend(0.45,0.7,0.89,0.92)
        leg.SetFillColor(kWhite)
        leg.SetLineColor(kWhite)

        ###############################
        # Draw raw plots
        ###############################
        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            xmin, xmax = hxrng_[type_][1],hxrng_[type_][2]
            xbins = int(hxrng_[type_][0]*(xmax-xmin))
            if debug_: print xbins, xmin, xmax

            myhist_[fNames_[fntmp]] = TH1D("myhist_%s"%(fNames_[fntmp]),"",xbins,xmin,xmax)
            trees_[fntmp].Draw("area*1.e9>>myhist_%s"%(fNames_[fntmp]),"","")

        ###############################
        # Draw plots with style
        ###############################
        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            if ni == 0:
                myhist_[fNames_[fntmp]].SetLineColor(colors_[ni])
                myhist_[fNames_[fntmp]].Draw()
                myhist_[fNames_[fntmp]].GetXaxis().SetTitle("Energy [V#timesns]")
                myhist_[fNames_[fntmp]].GetYaxis().SetTitle("A.U.")
                myhist_[fNames_[fntmp]].GetYaxis().SetRangeUser(5.e-2,1.e6)

                gPad.SetLogy()
                gPad.Update()
                c1.Update()
            else:
                myhist_[fNames_[fntmp]].Scale(myhist_[fNames_["%s_0"%nf]].Integral()/myhist_[fNames_[fntmp]].Integral())
                myhist_[fNames_[fntmp]].SetLineColor(colors_[ni])
                myhist_[fNames_[fntmp]].SetLineStyle(1)
                myhist_[fNames_[fntmp]].Draw("same")

            if type_.find("Ref")!=-1:
                leg.SetHeader("%s Face A"%(header_.replace("_","-")))
                leg.AddEntry(myhist_[fNames_[fntmp]],"%s"%(labels_[fntmp].replace("_"," ")),"l")
            elif type_.find("Merged")!=-1:
                tmpHeaders = header_.split("_")
                leg.SetHeader("%s-%s"%(tmpHeaders[0],tmpHeaders[1]))
                leg.AddEntry(myhist_[fNames_[fntmp]],"%s"%(labels_[fntmp].replace("_"," ").lstrip("0")),"l")
            else:
                leg.SetHeader("#splitline{%s}{%s}"%(header_.replace("_","-"),doselabel_))
                leg.AddEntry(myhist_[fNames_[fntmp]],"%s"%(labels_[fntmp].replace("_"," ")),"l")

            leg.Draw()

        fnameTag = "%s_%s_p%s"%(header_,options_.outtag,fTag_)

        gPad.RedrawAxis()
        c1.SaveAs("%s/Alpha_eng_%s.png"%(outDir_,fnameTag))
        c1.SaveAs("%s/Alpha_eng_%s.pdf"%(outDir_,fnameTag))
        c1.SaveAs("%s/Alpha_eng_%s.root"%(outDir_,fnameTag))


        ###############################
        # Some simple comparison
        ###############################
        print "\n\n"
        print "="*150
        print "="*150
        means_={}
        RMSs_ ={}
        fnref = "%s_0"%nf
        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            means_[fntmp] = myhist_[fNames_[fntmp]].GetMean()
            RMSs_[fntmp] = myhist_[fNames_[fntmp]].GetRMS()

            print "(Mean, RMS) %s %s = (%6.3f,%6.3f)"%(header_,labels_[fntmp],means_[fntmp],RMSs_[fntmp])

            if ni>0:
                print "(#Delta mean,#Delta RMS) %s vs. %s [%%] = (%5.2f,%5.2f)\n\n"%(
                    labels_[fntmp],labels_[fnref],abs(means_[fntmp]-means_[fnref])/(means_[fntmp]+means_[fnref])*2*100.,
                    abs(RMSs_[fntmp]-RMSs_[fnref])/(RMSs_[fntmp]+RMSs_[fnref])*2*100.)

        del c1




####################################################################################################
# Common drawing routine for Alpha source drawing and D calculation
####################################################################################################
def DrawDvsTHist(myfiles_, plotSets_, outDir_, sampleSet_, fTag_, doselabel_, hxrng_, options_, uncreftag_, dosescheme_, debug_=False):
    if debug_: print "Style [DrawDvsTHist] = ",gStyle.GetName()
    mypedcut = 0.0
    if options_.myPedCut:
        mypedcut = float(options_.myPedCut)

    timecut = float(options_.timeCut)

    myhist_ = {}
    myfit_  = {}
    fitopt_ = {}
    sFit_   = {}
    uncFit_ = {}
    vEng_   = {}
    uncEng_ = {}

    colors_ = [1,2,4,6,9,8,7,12,28,30]

    ###############################
    # dose
    # Note: sigma_d = 1.6% (NIST);
    # 20% for Castor table; 5% for UMD high dose;
    # Goddard (GSFC) 10% temporary
    ###############################
    vDose_ = {}
    vDose_["NIST"]  = [2.95,0.016]
    vDose_["NIST1"] = [4.00,0.017]
    vDose_["NIST2"] = [5.82,0.016]
    vDose_["NIST3"] = [6.95,0.017]
    vDose_["NIST4"] = [7.00,0.017]
    vDose_["CT"]    = [0.24,0.2]   # 2015
    #vDose_["CT16"]  = [0.xx,0.yy] # 2016 unknown
    vDose_["UMD30"] = [30.0,0.05]
    vDose_["UMD50"] = [50.0,0.05]
    vDose_["GSFC"]  = [0.38,0.038]

    ###############################
    ## calculate measurement+fit uncertainties
    uncEng_["EJ200"]               = 0.0110424
    uncEng_["EJ200PVT"]            = 0.0110424
    uncEng_["EJ200PS"]             = 0.0110424
    uncEng_["EJ200OLD1X"]          = 0.0410579
    uncEng_["EJ200OLD2X"]          = 0.0182349
    uncEng_["EJ200SP2P"]           = 0.0110424
    uncEng_["EJ200SP1P"]           = 0.0077065
    uncEng_["EJ200COLD"]           = 0.0097712*3.0
    uncEng_["EJ200PS1X2PRefCold"]  = 0.0242427
    uncEng_["EJ200PVT1X2PRefCold"] = 0.0249234
    uncEng_["EJ260PS1X2PRefCold"]  = 0.0251583
    uncEng_["EJ260PVT1X2PRefCold"] = 0.0195749 #0.0219391 (16a only)
    uncEng_["EJ260"]               = 0.0329865 ## estimated by EJ-260_4ab
    uncEng_["EJ260N"]              = 0.0140443 ## estimated by EJ-260_N1ab and EJ-260_N6ab

    ###############################
    ## Dark current common
    ###############################
    fDC50 = TFile("root/AlphaSource/DarkCurrent_HV1700_201608_merged_FastFrame.root")
    tDC50 = fDC50.Get("tree")

    ###############################
    ## Dark current common
    ###############################

    #############
    ## fit only
    hDC50 = TH1D("myhist_DC50","DC50",52,-0.2,0.0)
    tDC50.Draw("area*1.e9>>myhist_DC50","abs(amplitude)>%f && time>%f"%(mypedcut,3600.0))

    ## plot only
    hDC50p = TH1D("myhist_DC50p","DC50p",288,-1.,8.)
    tDC50.Draw("area*1.e9>>myhist_DC50p","abs(amplitude)>%f && time>%f"%(mypedcut,3600.0))
    #############

    ###############################
    ## Normalized to same number of events
    nevt_num = hDC50p.Integral()
    print nevt_num

    nevt_den = {}
    nscale   = {}

    nevt_den["DC50p"] = hDC50p.Integral()
    print nevt_den["DC50p"]
    nscale["DC50p"] = float(nevt_num)/float(nevt_den["DC50p"])
    print nscale["DC50p"]

    ## Find overall max (this is energy offset)
    DCName = "DC50"
    vEng_[DCName],sFit_[DCName],myfit_[DCName]=AlphaSourceFitter().GausFitEngPeak(hDC50,DCName,[0.2,0.2],nscale["DC50p"])

    ###############################
    ## offset
    ###############################
    vOffset = [vEng_[DCName],sFit_[DCName]]

    ###############################
    ## Main files section        ##
    ###############################
    for nf, fl in sorted(plotSets_.items()):
        fNames_ = {}
        grTemp_ = {}
        trees_  = {}
        labels_ = {}

        refPlot_  = []
        irrPlots_ = []

        for ni in range(len(fl)):
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

        cvsName = "c%s"%nf
        c1 = TCanvas(cvsName,cvsName,800,600)

        ###############################
        ## Hist header sample label
        ## e.g.: EJ200PVT_1X2P_N14-20170224 => label_ = 20170224 ; sample_ = EJ200PVT-1X2P-N14; header_ = EJ200PVT 1X2P
        sample_ = fNames_["%s_0"%(nf)].split("-")[0]
        header_ = "%s-%s"%(sample_.split("_")[0],sample_.split("_")[1])
        type_   = myfiles_[fNames_["%s_0"%(nf)]][0]

        ###############################
        # Plot range
        ###############################
        xmin, xmax = hxrng_[type_][1],hxrng_[type_][2]
        xbins = int(hxrng_[type_][0]*(xmax-xmin))

        ###############################
        ## Dark current
        ###############################
        ## plot only
        hDC50pp = TH1D("myhist_DC50pp","DC50pp",xbins,xmin,xmax)
        tDC50.Draw("area*1.e9>>myhist_DC50pp","abs(amplitude)>%f && time>%f"%(mypedcut,3600.0))
        #############


        nevt_den["DC50pp"] = hDC50pp.Integral()
        print nevt_den["DC50pp"]
        nscale["DC50pp"] = float(nevt_num)/float(nevt_den["DC50pp"])
        print nscale["DC50pp"]

        ## for plot only
        hDC50pp.Scale(nscale["DC50pp"])
        hDC50pp.SetLineColor(1)
        hDC50pp.SetLineStyle(3)

        ###############################
        # Draw raw plots
        ###############################
        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            if debug_: print xbins, xmin, xmax

            if ni == 0:
                refPlot_.append(fNames_[fntmp])
            else:
                irrPlots_.append(fNames_[fntmp])

            biasOffset_ = 0.0
            if len(fitopt_[fntmp])>3 : biasOffset_ = fitopt_[fntmp][3]
            print "biasOffset_[%30s] = %f"%(fNames_[fntmp],biasOffset_)

            myhist_[fNames_[fntmp]] = TH1D("myhist_%s"%(fNames_[fntmp]),"%s"%(fNames_[fntmp]),xbins,xmin,xmax)
            trees_[fntmp].Draw("area*1.e9+%6.4f>>myhist_%s"%(biasOffset_,fNames_[fntmp]),"abs(amplitude)>%f && time>%f"%(mypedcut,timecut))


        ###############################
        ## Needed for the first plot ##
        ## Redraw DarkCurrent
        hDC50pp.Draw()
        hDC50pp.GetXaxis().SetTitle("Energy [V#timesns]")
        hDC50pp.GetYaxis().SetTitle("A.U.")
        hDC50pp.GetYaxis().SetRangeUser(5.e-1,5.e9)

        gPad.SetLogy()
        gPad.Update()
        c1.Update()
        ## Needed for the first plot ##
        ###############################

        ###############################
        # Draw plots with color/style
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames_[tmpName]
            print ">>>>>>>>>>> Processing:", sigName
            nevt_den[sigName] = myhist_[sigName].Integral()
            print nevt_den[sigName]
            nscale[sigName] = float(nevt_num)/float(nevt_den[sigName])
            print nscale[sigName]

            myhist_[sigName].Scale(nscale[sigName])
            myhist_[sigName].SetLineColor(colors_[ni])
            myhist_[sigName].SetLineStyle(int(ni/len(colors_))+1)
            myhist_[sigName].Draw("same")

            ## Find overall max
            if fitopt_[tmpName][2] == "G1":
                tmpFitter = AlphaSourceFitter().GausFitEngPeak
            elif fitopt_[tmpName][2] == "G2":
                tmpFitter = AlphaSourceFitter().TwoGausFitEngPeak
            elif fitopt_[tmpName][2] == "L1":
                tmpFitter = AlphaSourceFitter().LandauFitEngPeak
            elif fitopt_[tmpName][2] == "GL":
                tmpFitter = AlphaSourceFitter().GausLandFitEngPeak
            else:
                print "Fitter identifier [%s] not defined. Aborted."%(fitopt_[tmpName][2])
                exit()

            vEng_[sigName],sFit_[sigName],myfit_[sigName]=tmpFitter(myhist_[sigName],sigName,fitopt_[tmpName],nscale[sigName])
            uncFit_[sigName] = sFit_[sigName]/vEng_[sigName]
            print "Uncertainty of %s Fit     = %-6.3f %%"%(sigName,uncFit_[sigName]*100.)


        #############################################################################################
        ## Calculate dose constant: light loss = exp (-dose/D); D = dose constant
        #############################################################################################
        print "\n"
        print "="*150
        vDconst_ = {}
        vInput_  = {}

        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames_[tmpName]
            uncEng_[sigName]  = math.sqrt(math.pow(uncEng_[uncreftag_],2)+math.pow(uncFit_[sigName],2))
            vInput_[sigName]  = [vEng_[sigName], uncEng_[sigName]*vEng_[sigName]]

        ## print uncertainties
        print "Offset = %-8.5f"%(vOffset[0])
        print "Uncertainty of [%30s] = %6.3f %%"%("Offset",math.fabs(vOffset[1]/vOffset[0]*100.))
        for n,v in sorted(uncEng_.items()):
            print "Uncertainty of [%30s] = %6.3f %%"%(n,v*100.)

        ###############################
        # The calculation
        # CalcD(dose,v_o,v_i,offset,uncV)
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames_[tmpName]
            if sigName.find("UnIrr") == -1:
                print "Calculating Dose Constant for :", sigName
                vDconst_[sigName] = CalcD(vDose_[dosescheme_],vInput_[sigName],vInput_[fNames_["%s_0"%(nf)]],vOffset)

        ###############################
        ## Draw uncertainty band
        ###############################
        for ni in range(len(fl)):
            tmpName = "%s_%i"%(nf,ni)
            sigName = fNames_[tmpName]
            if sigName.find("DC50") != -1: continue
            ## Plot uncertainty band
            try:
                print "Hist: ",sigName
                grTemp_[sigName] = HistToTGRaphErrorsUniform(myhist_[sigName],1,0,uncEng_[sigName])
                grTemp_[sigName].Draw("same e2")
            except:
                continue


        gPad.RedrawAxis()
        ###################################
        ## Legend
        ###################################
        leg = TLegend(0.2,0.73,0.6,0.93)
        leg.SetFillColor(kWhite)
        leg.SetLineColor(kWhite)

        leg.SetHeader("#splitline{%s}{%s}"%(header_,doselabel_))
        leg.AddEntry(hDC50pp,"Dark Current","l")

        leg1 = TLegend(0.62,0.6,0.92,0.93)
        leg1.SetFillColor(kWhite)
        leg1.SetLineColor(kWhite)

        for ni in range(len(fl)):
            fntmp = "%s_%i"%(nf,ni)
            if ni==0:
                leg.AddEntry(myhist_[fNames_[fntmp]],"Non-irradiated","l")
            else:
                tmpTxt_ = fNames_[fntmp].split("-")[1]
                tmpTxt_ = tmpTxt_[:len(tmpTxt_)-1] ## strip "d"
                if debug_: print tmpTxt_
                if len(tmpTxt_.lstrip("0")) == 0:
                    leg1.AddEntry(myhist_[fNames_[fntmp]],"0 day aft. irr.","l")
                else:
                    if int(tmpTxt_.lstrip("0")) < 2:
                        leg1.AddEntry(myhist_[fNames_[fntmp]],"%s day aft. irr."%tmpTxt_.lstrip("0"),"l")
                    else:
                        leg1.AddEntry(myhist_[fNames_[fntmp]],"%s days aft. irr."%tmpTxt_.lstrip("0"),"l")

        leg.Draw()
        leg1.Draw()

        ## FIXME
        fnameTag = "%s_p%s"%(options_.outtag,fTag_)
        fheader_ = header_.replace(" ","_").replace("-","_")

        c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.png"%(outDir_,fheader_,sampleSet_,fnameTag))
        c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.pdf"%(outDir_,fheader_,sampleSet_,fnameTag))
        c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.root"%(outDir_,fheader_,sampleSet_,fnameTag))

        del hDC50pp

        PrintResults(vDconst_, vInput_, vOffset, sampleSet_, header_, doselabel_, refPlot_, irrPlots_, fTag_, options_)
        print "\n\n"
        print "="*150
        print "="*150



###############################
## print results on screen
###############################
def PrintResults(vDconst, vInput, vOffset, sampleset, header, doselabel, refPlot, IrrPlots, fTag, poptions):
    print "\n\n"
    print "*"*150
    for n,v in sorted(vDconst.items()):
        print "[PrintResults] Dose constant %-30s = %8.5f +/- %-8.5f [%5.2f %%]; R = %8.5f+/- %-8.5f"%(n,v[0],v[1],v[1]/v[0]*100.,v[2],v[3])

    tblName={}

    idx=2

    for sigName in sorted(IrrPlots):
        #print sigName
        tmpTxt = sigName.split("-")[1]
        tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
        fheader = header.replace(" ","-").replace("_","-")
        if len(tmpTxt.lstrip("0")) == 0:
            tblName[sigName] = ["%02i"%idx,"%s (%s;   0 day aft. irr.) "%(fheader, doselabel)]
        else:
            if int(tmpTxt.lstrip("0")) < 2:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(fheader, doselabel, tmpTxt.lstrip("0"))]
            else:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(fheader, doselabel, tmpTxt.lstrip("0"))]

        idx+=1

    ###############################
    ## Save to a text file 
    ###############################
    fnameTag = "%s_p%s"%(poptions.outtag,fTag)
    filetag = header.replace(" ","_").replace("-","_")
    outTxtFile = "DoseConstants/DoseConst_NS_%s_%s_%s.txt"%(filetag,sampleset,fnameTag)
    outf=open(outTxtFile,'w')
    print "\n"
    print "*"*150
    ## sorted by dose const
    outf.write("%-80s\t%8s\t%8s\t%8s\t%8s\n"%("Sample label","D","sigD ","R","sigR "))
    outf.write("Sorted by value:\n")
    for n,v in sorted(vDconst.items(), key=lambda x: x[1]):
        #print "%-80s: %8.5f +/- %-8.5f; R = %8.5f+/- %-8.5f"%(tblName[n][1],v[0],v[1],v[2],v[3])
        outf.write("%-80s\t%8.5f\t%-8.5f\t%8.5f\t%-8.5f\n"%(tblName[n][1],v[0],v[1],v[2],v[3]))

    ## sorted by naming order
    outf.write("\nSorted by name:\n")
    for k,n in sorted(tblName.items(), key=lambda x: x[1]):
        #print k
        print "[PrintResults] %-80s: D = %8.5f +/- %-8.5f; R = %8.5f+/- %-8.5f"%(n[1],vDconst[k][0],vDconst[k][1],vDconst[k][2],vDconst[k][3])
        outf.write("%-80s\t%8.5f\t%-8.5f\t%8.5f\t%-8.5f\n"%(n[1],vDconst[k][0],vDconst[k][1],vDconst[k][2],vDconst[k][3]))

    outf.close()
    print "\n"
    print "*"*150
    print "[PrintResults] Save results to %s"%(outTxtFile)
    print "*"*150

    ###############################
    ## print results for latex
    ###############################
    print "\n\n[PrintResults] Result table in latex format\n"
    print "\n"
    print "\\begin{table}[!htbp]"
    print "  \\begin{center}"
    print "    \\begin{tabular}{l c c}"
    print "    \hline\hline"
    print "    Sample name & Dose constant (D) & Uncertainty ($\sigma_\mathrm{D}$)  \\\ "
    print "    \hline"

    for k,n in sorted(tblName.items(), key=lambda x: x[1]):
        print "    %-40s & %8.5f & %-8.5f  \\\ "%(n[1],vDconst[k][0],vDconst[k][1])

    print "    \hline"
    print "    \end{tabular}"
    print "  \end{center}"
    print "\caption{Dose constant}"
    print "\label{table:doseConst}"
    print "\end{table}"
    print "\n"


    ###############################
    ## print npe/MeV results for latex
    ###############################
    print "\n\n[PrintResults] Result npe/MIP table in latex format\n"
    print "\n"
    print "\\begin{table}[!htbp]"
    print "  \\begin{center}"
    print "    \\begin{tabular}{l c}"
    print "    \hline\hline"
    print "    Sample name & $n_{P.E.}/MeV$ \\\ "
    print "    \hline"

    print "    %-40s & %5.2f  \\\ "%(header,CalcNPE(vInput[refPlot[0]][0],vOffset[0]))
    for k,n in sorted(tblName.items(), key=lambda x: x[1]):
        print "    %-40s & %5.2f  \\\ "%(n[1],vDconst[k][4])

    print "    \hline"
    print "    \end{tabular}"
    print "  \end{center}"
    print "\caption{Number of photoelectrons per MeV.}"
    print "\label{table:nPEpMeV}"
    print "\end{table}"
    print "\n"


    ###############################
    ## print results for twiki
    ###############################
    print "\n\n[PrintResults] Result table for twiki\n"
    idx=2
    for sigName in sorted(IrrPlots):
        tmpTxt = sigName.split("-")[1]
        tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
        fheader = header.replace(" ","-").replace("_","-")
        if len(tmpTxt.lstrip("0")) == 0:
            tblName[sigName] = ["%02i"%idx,"%s (%s;   0 day aft. irr.)"%(header, doselabel)]
        else:
            if int(tmpTxt.lstrip("0")) < 2:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(header, doselabel, tmpTxt.lstrip("0"))]
            else:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(header, doselabel, tmpTxt.lstrip("0"))]
        idx+=1

    for k,n in sorted(tblName.items(), key=lambda x: x[1]):
        print "|  <verbatim>%s</verbatim>  |  <verbatim>%8.5f</verbatim>  |  <verbatim>%8.5f</verbatim>  |"%(n[1],vDconst[k][0],vDconst[k][1])



####################################################################################################
####################################################################################################
if __name__ == '__main__':
    print "[Common tools for plotting]"
