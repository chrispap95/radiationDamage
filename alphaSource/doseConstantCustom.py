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
    ## Dark current common
    ###############################
    ## Load dark current file and the TTree
    fDC50 = TFile("root/AlphaSource/DarkCurrent_HV1700_trigger_101mV_amp50p5mVpDiv_20181121.root")
    tDC50 = fDC50.Get("tree")

    ###############################
    ## Dark current common
    ###############################
    #############
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

    for sigName in plotSets['1']
    vEng[sigName],sFit[sigName],myfit[sigName]=AlphaSourceFitter().GausFitEngPeak(myhist[sigName],sigName,fitopt[tmpName],1.)

    ###############################
    # The calculation
    # CalcD(dose,v_o,v_i,offset,uncV)
    ###############################
    for ni in range(len(fl)):
        tmpName = "%s_%i"%(nf,ni)
        sigName = fNames[tmpName]
        if sigName.find("UnIrr") == -1:
            print "Calculating Dose Constant for :", sigName
            vDconst[sigName] = CalcD(vDose[dosescheme],vInput_[sigName],vInput_[fNames_["%s_0"%(nf)]],vOffset)
            ### vDose["GIF++"] = [1.32,0.00022]
            ### vInput_[sigName, fNames_[...]] is vInput_[sigName]  = [vEng_[sigName], uncEng_[sigName]*vEng_[sigName]]

    DrawDvsTHist(myfile, plotSets, outDir, sampleSet, fTag, doselabel, hxrng, options, "EJ200PS", "GIF++")

    def DrawDvsTHist(myfiles_, plotSets_, outDir_, sampleSet_, fTag_, doselabel_, hxrng_, options_, uncreftag_, dosescheme_, debug_=False):
        if debug_: print "Style [DrawDvsTHist] = ",gStyle.GetName()
        ## Setting the value for the pedestal cut. You can define that by passing the option -myPedCut=someValue
        mypedcut = 0.0
        if options_.myPedCut:
            mypedcut = float(options_.myPedCut)

        ## Setting the time cut value
        timecut = float(options_.timeCut)

        ## Initializing the arrays
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
        ## Setting the dose schemes
        vDose_ = {}
        vDose_["GIF++"] = [1.32,0.00022]
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
        ## Uncertainties for each measurement type
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
        ## Load dark current file and the TTree
        fDC50 = TFile("root/AlphaSource/DarkCurrent_HV1700_trigger_101mV_amp50p5mVpDiv_20181121.root")
        tDC50 = fDC50.Get("tree")

        ###############################
        ## Dark current common
        ###############################

        #############
        ## Create 2 histograms and insert the area histogram from the tree.
        ## Use |amplitude|>mypedcut && time > 3600 to skim the data.
        ## The first hist has limited range and it is only for fitting.
        hDC50 = TH1D("myhist_DC50","DC50",52,-1,0.)
        tDC50.Draw("area*1.e9>>myhist_DC50","abs(amplitude)>%f && time>%f"%(mypedcut,3600.0))

        ## The second is for plotting (wide range).
        hDC50p = TH1D("myhist_DC50p","DC50p",288,-1.,8.)
        tDC50.Draw("area*1.e9>>myhist_DC50p","abs(amplitude)>%f && time>%f"%(mypedcut,3600.0))
        #############

        ###############################
        ## Normalized to same number of event
        ## Currently does nothing (???). One can just set the nscale to 1.
        ## Get number of events by integrating the 2nd hist
        nevt_num = hDC50p.Integral()
        print nevt_num

        nevt_den = {}
        nscale   = {}

        nevt_den["DC50p"] = hDC50p.Integral()
        print nevt_den["DC50p"]
        nscale["DC50p"] = float(nevt_num)/float(nevt_den["DC50p"])
        print nscale["DC50p"]

        ## Find overall max (this is energy offset)
        ## Fit 1st hist
        ## Range value seemed suspicious. Total range is [-0.2,0] but he fits in +-0.2 from the maxbin (???)
        DCName = "DC50"
        vEng_[DCName],sFit_[DCName],myfit_[DCName]=AlphaSourceFitter().GausFitEngPeak(hDC50,DCName,[0.2,0.2],nscale["DC50p"])

        ###############################
        ## offset
        ###############################
        ## Set fit output for peak as the offset value
        vOffset = [vEng_[DCName],sFit_[DCName]]

        ###############################
        ## Main files section        ##
        ###############################
        ## Loop over the plotSets items. nf is the UnIrr and fl is each one of the Irr corresponding to that.
        for nf, fl in sorted(plotSets_.items()):
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
                    tmpTxt_ = tmpTxt_[:len(tmpTxt_)]## don't need that for now ##-1] ## strip "d"
                    if debug_: print tmpTxt_
                    if len(tmpTxt_.lstrip("0")) == 0:
                        leg1.AddEntry(myhist_[fNames_[fntmp]],"0 day aft. irr.","l")
                    else:
                        if int(tmpTxt_.lstrip("0")) < 2:
                            leg1.AddEntry(myhist_[fNames_[fntmp]],"%s day aft. irr."%tmpTxt_.lstrip("0"),"l")
                        else:
                            #### Changed the code a bit to reflect the fact that we don't use measurements for days after irr.
                            leg1.AddEntry(myhist_[fNames_[fntmp]],"Measured on %s"%tmpTxt_.lstrip("0"),"l")

            leg.Draw()
            leg1.Draw()

            ## FIXME
            fnameTag = "%s_p%s"%(options_.outtag,fTag_)
            fheader_ = header_.replace(" ","_").replace("-","_")

            c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.png"%(outDir_,fheader_,sampleSet_,fnameTag))
            c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.pdf"%(outDir_,fheader_,sampleSet_,fnameTag))
            c1.SaveAs("%s/%s_FF_merged_%s_eng_%s.root"%(outDir_,fheader_,sampleSet_,fnameTag))

            del hDC50pp

            print "\n\n"
            print "="*150
            print "="*150
