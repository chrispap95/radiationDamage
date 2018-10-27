#!/usr/bin/python
import os, sys, math
from ROOT import gROOT, gStyle, TFile, TTree, TH1, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory, TPaveStats
from ROOT import kRed, kBlue, kGreen, kWhite

from Plotter.CommonTools import CalcNPEMIP

from collections import OrderedDict
import numpy
from array import array

sys.path.append(os.path.abspath(os.path.curdir))

from Plotter import parseLYAnaInputArgs
options = parseLYAnaInputArgs()
import Plotter.utils as utils
#gROOT.LoadMacro("Plotter/langaus.C")
gROOT.ProcessLine(".L Plotter/langaus.C+")
from ROOT import PyLandGaus

####################################################################################################
####################################################################################################
class LYCosmicAna():

    def __init__(self, inputname, procname, myoptions, mypedcut, ftag, verbose = True):
        if verbose: print "My class for scintillator light yield analysis"
        self.verbose    = verbose
        self.myopts     = myoptions
        self.input_name = inputname
        self.isPlastic  = myoptions.isPlastic
        self.hist_title = ["Amplitude","Pedestal","Energy","Energy","Energy/Amp"]
        self.polarity   = -1 ## 1 for SiPM; -1 for PMT/HPK MPPC (SiPM)
        self.def_pol    = myoptions.polarity
        self.mypedcut   = mypedcut
        self.ftag       = ftag
        self.myfile     = {}
        self.mytree     = {}
        self.myhist     = {}
        self.myfit      = {}
        self.fobj       = PyLandGaus()
        self.get_init   = False
        self.get_amp    = False
        self.get_ped    = False
        self.get_sig    = False
        self.SPE        = 0.5 #FIXME
        self.nbin_amp   = 1000
        self.nbin_ped   = 100
        self.nbin_sig   = 10
        self.nbin_eng   = 10
        self.nbin_plot  = 10
        self.amp_rng    = [100,0.0,0.0]   ## x range for amplitude
        self.ped_rng    = [100,-0.05,1.0] ## x range for ped
        self.sig_rng    = [100,-1.0,20.0] ## x range for sig
        self.eng_rng    = [100,0.0,0.0]   ## x range for energy (in Npe)
        self.plot_rng   = [100,0.0,0.0]   ## x range for final plotting (in Npe)
        self.par_eng    = [0.,0.,0.,0.,0.,0.,0.] #fitting paramters for pedestal and signal of energy
        self.stats_pos  = [0.4,0.2,0.75,0.5]
        self.canvas     = TCanvas("c1","c1",800,600)
        self.outtag     = myoptions.outtag
        self.hardreset  = True
        self.procname   = procname

    def softreset(self):
        self.hardreset = False
        self.get_init  = False
        self.get_amp   = False
        self.get_ped   = False
        self.get_sig   = False

    ########################################
    ## Initial parameters
    ########################################
    def run_init(self,fitrange={}):
        print "="*150
        if self.mypedcut != None:
            self.mypedcut = float(self.mypedcut)
            print "[Run Init] Pedestal cut: %f"%self.mypedcut

        if self.hardreset:
            if (self.input_name==None):
                self.input_name=raw_input("Enter the input file name: ")
                print self.input_name
                print self.input_name[len(self.input_name)-4:]
                if(len(self.input_name)>=4 and (
                    self.input_name[len(self.input_name)-4:]==".out" or
                    self.input_name[len(self.input_name)-4:]==".txt")
                   ):
                    self.input_name=self.input_name[0:len(self.input_name)-4]

            print "Input file: %s"%(self.input_name)
            tmpTxt = (self.input_name.split("/")[1]).replace("-","_").replace(" ","_")
            #self.procname = "%s-%s-%s"%(tmpTxt.split("_")[0],tmpTxt.split("_")[1],tmpTxt.split("_")[2])
            #self.procname = "-".join(tmpTxt.split("_")[0:3])
            print "Sample Name: %20s"%(self.procname)
            self.myfile["%s_%s"%(self.procname,self.ftag)] = TFile("root/%s.root"%self.input_name)
            self.mytree["%s_%s"%(self.procname,self.ftag)] = self.myfile["%s_%s"%(self.procname,self.ftag)].Get("tree")

            ampdir = "Results/amp/%s"%("%s_%s"%(self.procname,self.ftag))

            try:
                os.makedirs(ampdir)
            except:
                pass

            engdir = "Results/energy/%s"%("%s_%s"%(self.procname,self.ftag))
            try:
                os.makedirs(engdir)
            except:
                pass

            histdir = "Results/hists/%s"%("%s_%s"%(self.procname,self.ftag))
            try:
                os.makedirs(histdir)
            except:
                pass

        if (not self.def_pol):
            self.polarity=float(raw_input("Enter the polarity (SiPM:1, PMT:-1) : "))
            self.def_pol = True

        if len(fitrange) > 2:
            print "Update fit range"
            self.amp_rng[0]  = fitrange["amp"][0]
            self.amp_rng[1]  = fitrange["amp"][1]
            self.amp_rng[2]  = fitrange["amp"][2]
            self.ped_rng[0]  = fitrange["ped"][0]
            self.ped_rng[1]  = fitrange["ped"][1]
            self.ped_rng[2]  = fitrange["ped"][2]
            self.sig_rng[0]  = fitrange["sig"][0]
            self.sig_rng[1]  = fitrange["sig"][1]
            self.sig_rng[2]  = fitrange["sig"][2]
            self.eng_rng[0]  = fitrange["eng"][0]
            self.eng_rng[1]  = fitrange["eng"][1]
            self.eng_rng[2]  = fitrange["eng"][2]
            self.plot_rng[0] = fitrange["plot"][0]
            self.plot_rng[1] = fitrange["plot"][1]
            self.plot_rng[2] = fitrange["plot"][2]
            self.stats_pos[0] = fitrange["stats"][0]
            self.stats_pos[1] = fitrange["stats"][1]
            self.stats_pos[2] = fitrange["stats"][2]
            self.stats_pos[3] = fitrange["stats"][3]
        else:
            ## 2016/7 EJ260 (10mV/div)
            self.amp_rng[0] = 100
            self.amp_rng[1] = -0.005
            self.amp_rng[2] = 0.04
            self.eng_rng[0] = 100
            self.eng_rng[1] = -0.1
            self.eng_rng[2] = 0.5
            self.ped_rng[0] = 100
            self.ped_rng[1] = -0.1
            self.ped_rng[2] = 0.5
            self.sig_rng[0] = 100
            self.sig_rng[1] = -0.1
            self.sig_rng[2] = 0.5
            self.plot_rng[0] = 100
            self.plot_rng[1] = -0.1
            self.plot_rng[2] = 0.8


        ## EJ200 and EJ260 30mV/div SiPM (10362-50C; Vop = 70.85V)
        self.nbin_amp  = int(self.amp_rng[0]*(self.amp_rng[2]-self.amp_rng[1]))
        self.nbin_ped  = int(self.ped_rng[0]*(self.ped_rng[2]-self.ped_rng[1]))
        self.nbin_sig  = int(self.sig_rng[0]*(self.sig_rng[2]-self.sig_rng[1]))
        self.nbin_eng  = int(self.eng_rng[0]*(self.eng_rng[2]-self.eng_rng[1]))
        self.nbin_plot = int(self.plot_rng[0]*(self.plot_rng[2]-self.plot_rng[1]))##dummy

        self.get_init = True
        if self.verbose:
            print self.amp_rng
            print self.eng_rng
            print self.ped_rng
            print self.sig_rng
            print self.plot_rng
            print self.nbin_amp,self.nbin_eng,self.nbin_ped,self.nbin_sig,self.nbin_plot
        print "="*150

    ###############################################
    ## amplitude plot to determine the pedestal cut
    ###############################################
    def run_amplitude(self,syst=None,fitrange={}): ## root file without surfix ".root"
        print "="*150
        #gROOT.SetStyle("Plain")
        gStyle.SetOptFit()
        #gStyle.SetOptStat(0)

        if (not self.get_init):
            self.run_init(self.ftag,fitrange)

        ampName = "amp_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "amp_%s_%s_Nominal"%(self.procname,self.ftag)
        sysName = "%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "%s_%s_Nominal"%(self.procname,self.ftag)
        if not self.myopts.batch:
            self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("amplitude>>hist_%s"%ampName)
            # determine a pedetstal cut from the plot
            self.canvas.Update()
            if (self.mypedcut == None):
                self.mypedcut = float(raw_input("Enter a desired pedestal cut: "))
        else:
            if (self.mypedcut == None): self.mypedcut = 0.005000

        #fit_ped = TF1("fit_ped","landau",self.amp_rng[1],self.mypedcut)
        fit_ped = TF1("fit_ped_%s"%sysName,"landau")
        fit_sig = TF1("fit_sig_%s"%sysName,"landau")
        par_amp = [0.,0.,0.,0.,0.,0.] #fitting paramters for pedestal and signal of amplitude
        par_amp_post = [0.,0.,0.,0.,0.,0.]

        hist_ped=TH1F("hist_ped_%s"%sysName,"hped",self.nbin_amp,self.amp_rng[1],self.mypedcut)
        hist_sig=TH1F("hist_sig_%s"%sysName,"hsig",self.nbin_amp,self.mypedcut,self.amp_rng[2])

        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("amplitude>>hist_ped_%s"%sysName,"amplitude<%s"%self.mypedcut)
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("amplitude>>hist_sig_%s"%sysName,"amplitude>%s"%self.mypedcut)
        hist_ped.Fit("fit_ped_%s"%sysName,"Q")
        par_amp[0],par_amp[1],par_amp[2] = fit_ped.GetParameter(0),fit_ped.GetParameter(1),fit_ped.GetParameter(2)
        hist_sig.Fit("fit_sig_%s"%sysName,"Q")
        par_amp[3],par_amp[4],par_amp[5] = fit_sig.GetParameter(0),fit_sig.GetParameter(1),fit_sig.GetParameter(2)

        print "[Run Amplitude] par_amp_%s: "%sysName
        print par_amp

        self.myhist[ampName] = TH1F("myhist_%s"%ampName,self.hist_title[0],self.nbin_amp,self.amp_rng[1]-par_amp[1],self.amp_rng[2]-par_amp[1])
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("amplitude-%f>>myhist_%s"%(par_amp[1],ampName))
        #self.myhist[ampName].Draw()
        #gDirectory.ls()
        self.myhist[ampName].GetXaxis().SetTitle("Pulse amplitude [V]")
        self.myhist[ampName].GetYaxis().SetTitle("Entries")

        self.myfit[ampName] = TF1("myfit_%s"%ampName,"landau(0)+landau(3)",self.amp_rng[1],self.amp_rng[2])
        self.myfit[ampName].SetLineColor(kRed)

        for i in xrange(len(par_amp)):
            self.myfit[ampName].SetParameter(i,par_amp[i])

        self.myfit[ampName].SetParLimits(4,self.mypedcut,self.amp_rng[2]-par_amp[1])
        self.myfit[ampName].SetParNames("Constant_P","MPV_P","Sigma_P","Constant_S","MPV_S","Sigma_S")

        self.myhist[ampName].Fit("myfit_%s"%ampName,"RQ","same")
        par_amp_post[0]=self.myfit[ampName].GetParameter(0)
        par_amp_post[1]=self.myfit[ampName].GetParameter(1)
        par_amp_post[2]=self.myfit[ampName].GetParameter(2)
        par_amp_post[3]=self.myfit[ampName].GetParameter(3)
        par_amp_post[4]=self.myfit[ampName].GetParameter(4)
        par_amp_post[5]=self.myfit[ampName].GetParameter(5)

        self.myfit["%s_2nd"%ampName] = TF1("myfit_%s_2nd"%ampName,"landau",self.amp_rng[1],self.amp_rng[2])
        self.myfit["%s_2nd"%ampName].SetLineColor(kBlue)
        self.myfit["%s_2nd"%ampName].SetParameters(par_amp_post[3],par_amp_post[4],par_amp_post[5])
        self.myfit["%s_2nd"%ampName].Draw("same")

        print "[Run Amplitude] par_amp_post: "
        print par_amp_post

        ##nped = all_par[0]*sqrt(2*TMath.Pi())*par_amp_pos[2]/myhist[0].GetXaxis().GetBinWidth(0)
        nped = par_amp_post[0]*par_amp_post[2]/self.myhist[ampName].GetXaxis().GetBinWidth(0)
        nsig = par_amp_post[3]*par_amp_post[5]/self.myhist[ampName].GetXaxis().GetBinWidth(0)
        print "[Run Amplitude] %f %f %f"%(nped,nsig,nsig/nped)

        self.get_amp = True

        self.canvas.Update()
        if (not self.myopts.batch):
            save = raw_input("Do you want to save this plot? (y/n): ")
        else:
            save = "y" if self.myopts.savefiles else "n"

        if (save=="y"):
            tempName = "amp_%s"%syst if syst != None else "amp_Nominal"
            self.canvas.SaveAs("Results/amp/%s_pedCut%s_%s_%s.png"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/amp/%s_pedCut%s_%s_%s.pdf"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/amp/%s_pedCut%s_%s_%s.root"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
        ## end of run_amplitude
        print "="*150

    ########################################
    ## energy plot for pedestal
    ########################################
    def run_energy_ped(self,syst=None,fitrange={}):
        print "="*150
        #gROOT.SetStyle("Plain")
        gStyle.SetOptFit()
        ##gStyle.SetOptStat(0)

        ## if the input is not specified, your can do it here
        if (not self.get_init): self.run_init(fitrange)
        ## if the pedestal cut is not determined
        #if (not self.get_amp): self.run_amplitude(syst,fitrange)

        pedName = "ped_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "ped_%s_%s_Nominal"%(self.procname,self.ftag)
        ## plot only the energy only for pedestal
        self.myhist[pedName] = TH1F("myhist_%s"%pedName,self.hist_title[1],self.nbin_ped,self.ped_rng[1],self.ped_rng[2])
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("areaFromScope*1.e9*%f>>myhist_%s"%(self.polarity,pedName),"amplitude<%f"%self.mypedcut)
        #self.myhist[pedName].Draw()
        self.myhist[pedName].GetXaxis().SetTitle("Energy [V#timesns]")
        #self.myhist[pedName].GetXaxis().SetTitle("N_{p.e.}")
        self.myhist[pedName].GetYaxis().SetTitle("Entries")

        ## fit the plot with a Gaussian and store the parameters in par_amp
        ## Note the fit parameter precision is fixed-20151111
        ped_fit_rng = [float("%.4f"%(self.myhist[pedName].GetMean()-2.0*self.myhist[pedName].GetRMS())),
                       float("%.4f"%(self.myhist[pedName].GetMean()+2.0*self.myhist[pedName].GetRMS()))]

        print "[Run Energy Ped] Fitting range: [%8.4f,%8.4f]"%(ped_fit_rng[0],ped_fit_rng[1])
        self.myhist[pedName].GetXaxis().SetRangeUser(ped_fit_rng[0],ped_fit_rng[1])
        self.canvas.Update()
        self.myfit[pedName] = TF1("myfit_%s"%pedName,"gaus",ped_fit_rng[0],ped_fit_rng[1])
        self.myfit[pedName].SetLineColor(kRed)

        self.myhist[pedName].Fit("myfit_%s"%pedName,"RL")
        nChi2_BL = self.myfit[pedName].GetChisquare()/self.myfit[pedName].GetNDF()
        self.myhist[pedName].Fit("myfit_%s"%pedName,"RWW")
        nChi2_CS = self.myfit[pedName].GetChisquare()/self.myfit[pedName].GetNDF()

        if nChi2_BL < nChi2_CS:
            print "[Run Energy Ped] Fit with Binned-likelihood method."
            self.myhist[pedName].Fit("myfit_%s"%pedName,"RQL")
        else: print "[Run Energy Ped] Fit with Chi2-square method."

        #print "[Run Energy Ped] %f %f %f"%(self.myhist[pedName].GetMean(),self.myhist[pedName].GetRMS(),self.myhist[pedName].GetMean()+self.myhist[pedName].GetRMS())
        ##if (self.isPlastic) self.myhist[pedName].Fit(myfit[pedName],"Q","same")
        self.par_eng[0]=float("%.6f"%self.myfit[pedName].GetParameter(0))
        self.par_eng[1]=float("%.6f"%self.myfit[pedName].GetParameter(1))
        self.par_eng[2]=float("%.6f"%self.myfit[pedName].GetParameter(2))

        print "[Run Energy Ped] Pedestal mean : %f"%self.par_eng[1]
        nped = self.par_eng[0]*TMath.Sqrt(2*TMath.Pi())*self.par_eng[2]/self.myhist[pedName].GetXaxis().GetBinWidth(0)
        ##cout<<"# of pedestal events: "<<nped<<endl

        self.get_ped = True
        self.canvas.Update()
        print "="*150

    ########################################
    ## area plot for signal
    ########################################
    def run_energy_sig(self,syst=None,fitrange={},type="EJ200"):
        print "="*150
        #gROOT.SetStyle("Plain")
        gStyle.SetOptFit()
        ##gStyle.SetOptStat(0)

        ## if the input is not specified, your can do it here
        if (not self.get_init): self.run_init(fitrange)
        ## if the pedestal is not fitted
        if (not self.get_ped): self.run_energy_ped(syst,fitrange)

        sigName = "sig_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "sig_%s_%s_Nominal"%(self.procname,self.ftag)
        ## plot only the energy only for signal
        self.myhist[sigName] = TH1F("myhist_%s"%sigName,self.hist_title[2],self.nbin_sig,self.sig_rng[1],self.sig_rng[2])
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("areaFromScope*1.e9*%f>>myhist_%s"%(self.polarity,sigName),"amplitude>%f"%self.mypedcut)
        #self.myhist[sigName].Draw()
        self.myhist[sigName].GetXaxis().SetTitle("Energy [V#timesns]")
        #self.myhist[sigName].GetXaxis().SetTitle("N_{p.e.}")
        self.myhist[sigName].GetYaxis().SetTitle("Entries")

        ## fit the plot with a convoluted Landau Gaussian and store the parameters in par_eng
        # Setting fit range
        if type == "EJ200":
            self.fobj.fr[0]=self.myhist[sigName].GetXaxis().GetXmin();
            self.fobj.fr[1]=self.myhist[sigName].GetXaxis().GetXmax();
        else:
            self.fobj.fr[0]=0.0
            self.fobj.fr[1]=3.0*self.myhist[sigName].GetMean()

        # Setting start values if desired
        #self.fobj.sv[0]=self.myhist[sigName].GetRMS()/4.25;
        #self.fobj.sv[1]=self.myhist[sigName].GetMean()*0.75;

        self.myfit[sigName] = self.fobj.langausf(self.myhist[sigName])
        self.myfit[sigName].SetLineColor(kBlue)

        self.par_eng[3]=float("%.6f"%self.myfit[sigName].GetParameter(0))
        self.par_eng[4]=float("%.6f"%self.myfit[sigName].GetParameter(1))
        self.par_eng[5]=float("%.6f"%self.myfit[sigName].GetParameter(2))
        self.par_eng[6]=float("%.6f"%self.myfit[sigName].GetParameter(3))

        print "[Run Energy Sig] Signal MP : %f"%self.par_eng[4] ## already corrected in langaus.C
        ##print "[Run Energy Sig] Signal MPV : %f"%self.par_eng[4]
        ## https://en.wikipedia.org/wiki/Talk%3ALandau_distribution
        ## https://root.cern.ch/root/html/tutorials/fit/langaus.C.html
        ##print "Most Probable Value (shifted) : %f"%(self.par_eng[4]+0.22278298*self.par_eng[5])

        self.get_sig = True
        self.canvas.Update()
        print "="*150

    ##/********************************************/
    ## Final fit for EJ260; scaled by self.SPE
    ##/********************************************/
    def run_energy_all(self,syst=None,fitrange={}):
        print "="*150
        #### energy plot for both pedetal and signal
        par_eng_post=[0.,0.,0.,0.,0.,0.,0.]
        err_eng_post=[0.,0.,0.,0.,0.,0.,0.]
        #### values of physical interest
        valPhys = {}
        histRes = {}

        #gROOT.SetStyle("Plain")
        gStyle.SetOptFit()
        ##gStyle.SetOptStat(0)
  
        ## if the input is not specified, your can do it here
        if (not self.get_init): self.run_init(fitrange)
        ## if the signal is fitted
        if (not self.get_sig): self.run_energy_sig(syst,fitrange)

        engName = "engall_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "engall_%s_%s_Nominal"%(self.procname,self.ftag)
        sigName = "sig_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "sig_%s_%s_Nominal"%(self.procname,self.ftag)

        ## plot the energy for both pedestal and signal
        self.myhist[engName] = TH1F("myhist_%s"%engName,self.hist_title[3],self.nbin_eng,self.plot_rng[1],self.plot_rng[2])
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("(areaFromScope*1.e9*%f-%f)/%f>>myhist_%s"%(self.polarity,self.par_eng[1],self.SPE,engName),"")
        #self.myhist[engName].Draw()
        #self.myhist[engName].GetXaxis().SetTitle("Energy [V#timesns]")
        self.myhist[engName].GetXaxis().SetTitle("N_{p.e.}")
        self.myhist[engName].GetYaxis().SetTitle("Entries")

        ## fit the plot with a Gaussian (ped) and a convoluted Landau and Gaussian distribution (sig)
        # Setting fit range and start values
        self.fobj.fr[0]= self.eng_rng[1]
        self.fobj.fr[1]= self.eng_rng[2]
        print "[Gaus+LanGaus] fit range: [%f to %f]"%(self.fobj.fr[0],self.fobj.fr[1])

        fobjtmp = PyLandGaus()
        #fgauslandgaus = TF1("fgauslandgaus",fobjtmp.gauslangaufun,self.eng_rng[1],self.eng_rng[2],7)
##        fobjtmp.fr[0]=self.myhist[engName].GetXaxis().GetXmin();
##        fobjtmp.fr[1]=self.myhist[engName].GetXaxis().GetXmax();
        fobjtmp.fr[0]=self.fobj.fr[0]
        fobjtmp.fr[1]=self.fobj.fr[1]

        self.myfit[engName] = TF1("myfit_%s"%engName,fobjtmp.gauslangaufun,self.eng_rng[1],self.eng_rng[2],7)
        self.myfit[engName].SetParNames("N_{Ped}","#mu_{Ped}","#sigma_{Ped}","Width","MP","Area","GSigma");
        self.myfit[engName].SetLineColor(kRed)
        self.myfit[engName].SetParameters(
            self.par_eng[0],
            0.,
            self.par_eng[2]/self.SPE,
            self.fobj.fp[0]/self.SPE,
            (self.par_eng[4]-self.par_eng[1])/self.SPE,
            self.fobj.fp[2],
            self.fobj.fp[3]/self.SPE
            )
        self.myfit[engName].SetNpx(1000)
        for i in xrange(self.myfit[engName].GetNumberFreeParameters()):
            print "[Gaus+LandGaus init pars] %-15s = %12.5f"%(self.myfit[engName].GetParName(i), self.myfit[engName].GetParameter(i))

        #self.myfit[engName].SetParNames("Width","MP","Area","GSigma")
        ## ped
        #self.myfit[engName].FixParameter(0,self.par_eng[0])
##        self.myfit[engName].SetParLimits(1,
##                                         self.par_eng[1]-0.5*self.par_eng[2],
##                                         self.par_eng[1]+self.par_eng[2])
        #self.myfit[engName].FixParameter(2,self.par_eng[2])
        ## sig
        self.myfit[engName].SetParLimits(3,
                                         self.fobj.pllo[0],
                                         self.fobj.plhi[0])
        self.myfit[engName].SetParLimits(4,
                                         self.fobj.pllo[1],
                                         self.fobj.plhi[1])
        self.myfit[engName].SetParLimits(5,
                                         self.fobj.pllo[2],
                                         self.fobj.plhi[2])
        self.myfit[engName].SetParLimits(6,
                                         self.fobj.pllo[3],
                                         self.fobj.plhi[3])
        self.myhist[engName].Fit("myfit_%s"%engName,"RL")
##        self.myhist[engName].Fit("myfit_%s"%engName,"RQWW")

        for i in xrange(self.myfit[engName].GetNumberFreeParameters()):
            par_eng_post[i] = self.myfit[engName].GetParameter(i)
            err_eng_post[i] = self.myfit[engName].GetParError(i)
            print "%-15s = %20.5f"%(self.myfit[engName].GetParName(i), self.myfit[engName].GetParameter(i))

        print "[Run Energy All] Signal MP : %f"%par_eng_post[4]  ## already corrected in langaus.C
        ##print "[Run Energy All] Signal MPV : %f"%par_eng_post[4]
        ## https://en.wikipedia.org/wiki/Talk%3ALandau_distribution
        ## https://root.cern.ch/root/html/tutorials/fit/langaus.C.html
        ##print "[Run Energy All] Most Probable Value (shifted) : %f"%(par_eng_post[4]+0.22278298*par_eng_post[6])

        ## Set plot range
        self.myhist[engName].Draw()
        self.myhist[engName].GetXaxis().SetRangeUser(self.plot_rng[1],self.plot_rng[2])
        gPad.Update()
        self.canvas.Update()

        ## draw extrapolation of fitted function to full plotting range:
        # lower end
##        self.myfit["%s_extL"%engName] = TF1("myfit_%s_extL"%engName,fobjtmp.gauslangaufun,self.plot_rng[1],self.eng_rng[1],7)
##        self.myfit["%s_extL"%engName].SetLineColor(kBlue)
##        self.myfit["%s_extL"%engName].SetParameters(par_eng_post[0],par_eng_post[1],par_eng_post[2],par_eng_post[3],par_eng_post[4],par_eng_post[5],par_eng_post[6])
##        self.myfit["%s_extL"%engName].Draw("same")
        # upper end
        self.myfit["%s_extR"%engName] = TF1("myfit_%s_extR"%engName,fobjtmp.gauslangaufun,self.eng_rng[2],self.plot_rng[2],7)
        self.myfit["%s_extR"%engName].SetLineColor(kBlue)
        self.myfit["%s_extR"%engName].SetParameters(par_eng_post[0],par_eng_post[1],par_eng_post[2],par_eng_post[3],par_eng_post[4],par_eng_post[5],par_eng_post[6])
        self.myfit["%s_extR"%engName].Draw("same")

        ####################################################
        # This estimation only for situation when nped is
        # not too small
        ####################################################
##        ##nped = all_par[0]*all_par[2]*sqrt(2*TMath.Pi())/(eng_rng[1]-eng_rng[0])*100.
##        nped = par_eng_post[0]*TMath.Sqrt(2*TMath.Pi())*par_eng_post[2]/self.myhist[engName].GetXaxis().GetBinWidth(0)
##        #nsig = par_eng_post[3]*par_eng_post[5]/self.myhist[engName].GetXaxis().GetBinWidth(0) # landau
##        nsig = par_eng_post[5]*par_eng_post[6]/self.myhist[engName].GetXaxis().GetBinWidth(0) # FIXME landgaus

##        print "[Run Energy All] (nped,nsig,nsig/nped) = (%f, %f, %f) "%(nped,nsig,nsig/nped)

        ## error calculation
##        print "[Run Energy All] err_eng_post[0],par_eng_post[0],err_eng_post[2],par_eng_post[2] = %f, %f, %f, %f"%(err_eng_post[0],par_eng_post[0],err_eng_post[2],par_eng_post[2])
##        nped_err = nped*TMath.Sqrt(TMath.Power(err_eng_post[0]/par_eng_post[0],2)+TMath.Power(err_eng_post[2]/par_eng_post[2],2))
##        #nsig_err = nsig*TMath.Sqrt(TMath.Power(err_eng_post[3]/par_eng_post[3],2)+TMath.Power(err_eng_post[5]/par_eng_post[5],2)) # landau
##        nsig_err = nsig*TMath.Sqrt(TMath.Power(err_eng_post[5]/par_eng_post[5],2)+TMath.Power(err_eng_post[6]/par_eng_post[6],2)) # FIXME landgaus
##        print "[Run Energy All] sig = %f +/- %f"%(nsig,nsig_err)
##        print "[Run Energy All] ped = %f +/- %f"%(nped,nped_err)

        ## efficiency and MPV
##        valPhys["eff"]     = (1.- nped/self.myhist[engName].GetEntries(),nped_err/self.myhist[engName].GetEntries())
##        valPhys["mpv"]     = (par_eng_post[4]-par_eng_post[1],err_eng_post[4])
##        valPhys["npe"]     = (-1*TMath.Log(nped/(nped+nsig)),-1*TMath.Log(nped/(nped+nsig))*nsig_err/nsig)

##        leg = TLegend(0.6,0.3,0.89,0.50)
##        leg.SetFillColor(kWhite)
##        leg.SetLineColor(kWhite)
##        leg.SetTextColor(kBlue)
##        ##leg.AddEntry(self.myhist[engName],Form("  Effi.  : %1.3f  #pm %1.3f",nsig/(nped+nsig),nsig/(nped+nsig)*nsig_err/nsig),"")
##        leg.AddEntry(self.myhist[engName],"Effi.  : %1.3f  #pm %1.3f"%valPhys["eff"],"")
##        leg.AddEntry(self.myhist[engName],"<N_{PE}> : %1.3f  #pm %1.3f"%valPhys["npe"],"")
##        leg.AddEntry(self.myhist[engName],"MPV : %1.3f  #pm %1.3f"%valPhys["mpv"],"")
##        leg.Draw()

##        ## determine the position of 1 PE peak
##        self.myhist[engName].Fit("myfit_%s"%sigName,"QN","R",0.015,0.050)
##        sig_1st = self.myfit[sigName].GetParameter(1)
##        print "[Run Energy All] Position of 1 PE: %f +/- %f "%(sig_1st,self.myfit[sigName].GetParameter(2))

        ####################################################
        # End of block
        ####################################################

        valPhys["npe"]     = (par_eng_post[4],err_eng_post[4])
        valPhys["ped"]     = (par_eng_post[1],err_eng_post[2])


        #gPad.Update()
        self.canvas.Update()

        if (not self.myopts.batch):
            save = raw_input("Do you want to save this plot? (y/n): ")
        else:
            save = "y" if self.myopts.savefiles else "n"

        if (save=="y"):
            tempName = "all_%s"%syst if syst != None else "all_Nominal"
            gPad.SetLogy()
            gPad.Update()

            ## Move stats box
            ps = self.myhist[engName].GetListOfFunctions().FindObject("stats").Clone("mystats_%s"%engName)
            ps.SetBorderSize(0)
            ps.SetX1NDC(self.stats_pos[0])
            ps.SetY1NDC(self.stats_pos[1])
            ps.SetX2NDC(self.stats_pos[2])
            ps.SetY2NDC(self.stats_pos[3])
            ps.Draw()
            self.myhist[engName].SetStats(0) ##This is needed; it removed "stats"
            self.canvas.Modified()

            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.png"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.pdf"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.root"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
        print "="*150
        return OrderedDict(sorted(valPhys.items(), key=lambda x: x[0])),OrderedDict(sorted(self.myhist.items(), key=lambda x: x[0]))




    ##/********************************************/
    ## Final fit for EJ200; scaled by self.SPE
    ##/********************************************/
    def run_energy_all2(self,syst=None,fitrange={}):
        print "="*150
        #### energy plot for both pedetal and signal
        par_eng_post=[0.,0.,0.,0.,0.,0.,0.]
        err_eng_post=[0.,0.,0.,0.,0.,0.,0.]
        #### values of physical interest
        valPhys = {}
        histRes = {}

        #gROOT.SetStyle("Plain")
        gStyle.SetOptFit()
        ##gStyle.SetOptStat(0)
  
        ## if the input is not specified, your can do it here
        if (not self.get_init): self.run_init(fitrange)
        ## if the signal is fitted
        if (not self.get_sig): self.run_energy_sig(syst,fitrange,"EJ260")

        engName = "engall_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "engall_%s_%s_Nominal"%(self.procname,self.ftag)
        sigName = "sig_%s_%s_%s"%(self.procname,self.ftag,syst) if syst != None else "sig_%s_%s_Nominal"%(self.procname,self.ftag)

        ## plot the energy for both pedestal and signal
        self.myhist[engName] = TH1F("myhist_%s"%engName,self.hist_title[3],self.nbin_eng,self.plot_rng[1],self.plot_rng[2])
        self.mytree["%s_%s"%(self.procname,self.ftag)].Draw("(areaFromScope*1.e9*%f-%f)/%f>>myhist_%s"%(self.polarity,self.par_eng[1],self.SPE,engName),"")
        #self.myhist[engName].Draw()
        #self.myhist[engName].GetXaxis().SetTitle("Energy [V#timesns]")
        self.myhist[engName].GetXaxis().SetTitle("N_{p.e.}")
        self.myhist[engName].GetYaxis().SetTitle("Entries")

        ## Re-fit ped peak
        self.myfit[engName+"_ped"] = TF1("myfit_%s_ped"%engName,"gaus",-self.par_eng[2]/self.SPE,self.par_eng[2]/self.SPE)

        fobjtmp = PyLandGaus()
        # Setting fit range
        fobjtmp.fr[0]=(self.par_eng[4]-2*self.par_eng[3])/self.SPE
        fobjtmp.fr[1]=(self.par_eng[4]+5*self.par_eng[3])/self.SPE

        # Setting start values if desired
        #fobjtmp.sv[0]=self.par_eng[3]/self.SPE
        #fobjtmp.sv[1]=self.par_eng[4]/self.SPE
        #fobjtmp.sv[2]=self.par_eng[5]
        #fobjtmp.sv[3]=self.par_eng[6]/self.SPE

        self.myfit[engName+"_sig"] = fobjtmp.langausf(self.myhist[engName])
        self.myfit[engName+"_sig"].SetLineColor(kBlue)

        self.myhist[engName].Fit(self.myfit[engName+"_ped"],"R+")
        for i in xrange(self.myfit[engName+"_ped"].GetNumberFreeParameters()):
            par_eng_post[i] = self.myfit[engName+"_ped"].GetParameter(i)
            err_eng_post[i] = self.myfit[engName+"_ped"].GetParError(i)
        for i in xrange(self.myfit[engName+"_sig"].GetNumberFreeParameters()):
            par_eng_post[3+i] = self.myfit[engName+"_sig"].GetParameter(i)
            err_eng_post[3+i] = self.myfit[engName+"_sig"].GetParError(i)

        ## fit the plot with a Gaussian (ped) and a convoluted Landau and Gaussian distribution (sig)
        # Setting fit range and start values
        fRngFit = [self.eng_rng[1] if self.eng_rng[1] < par_eng_post[4]-3*par_eng_post[3] else par_eng_post[4]-3*par_eng_post[3],
                   self.eng_rng[2] if self.eng_rng[2] < par_eng_post[4]+10*par_eng_post[3] else par_eng_post[4]+10*par_eng_post[3]]
        self.fobj.fr[0]= fRngFit[0]
        self.fobj.fr[1]= fRngFit[1]
        print "[Gaus+LanGaus] default fit range: [%f to %f]"%(self.eng_rng[1],self.eng_rng[2])
        print "[Gaus+LanGaus] updated fit range: [%f to %f]"%(self.fobj.fr[0],self.fobj.fr[1])

        self.myfit[engName] = TF1("myfit_%s"%engName,self.fobj.gauslangaufun,
                                  self.fobj.fr[0],#self.eng_rng[1],
                                  self.fobj.fr[1],#self.eng_rng[2],
                                  7)
        self.myfit[engName].SetLineColor(kRed)
        self.myfit[engName].SetParameters(
            par_eng_post[0],
            par_eng_post[1],
            par_eng_post[2],
            par_eng_post[3],
            par_eng_post[4],
            par_eng_post[5],
            par_eng_post[6]
            )

        self.myfit[engName].SetParNames("N_{Ped}","#mu_{Ped}","#sigma_{Ped}","Width","MP","Area","GSigma");
        self.myfit[engName].SetNpx(1000)
        for i in xrange(self.myfit[engName].GetNumberFreeParameters()):
            print "[Gaus+LandGaus init pars] %-15s = %12.5f"%(self.myfit[engName].GetParName(i), self.myfit[engName].GetParameter(i))

        ## ped
        self.myfit[engName].SetParLimits(1,
                                         par_eng_post[1]-0.1*par_eng_post[2],
                                         par_eng_post[1]+0.1*par_eng_post[2])
        ## sig
        self.myfit[engName].SetParLimits(3,
                                         self.fobj.pllo[0],
                                         self.fobj.plhi[0])
        self.myfit[engName].SetParLimits(4,
                                         self.fobj.pllo[1],
                                         self.fobj.plhi[1])
        self.myfit[engName].SetParLimits(5,
                                         self.fobj.pllo[2],
                                         self.fobj.plhi[2])
        self.myfit[engName].SetParLimits(6,
                                         self.fobj.pllo[3],
                                         self.fobj.plhi[3])

        self.myhist[engName].Fit("myfit_%s"%engName,"RL")
##        self.myhist[engName].Fit("myfit_%s"%engName,"RWW")

        for i in xrange(self.myfit[engName].GetNumberFreeParameters()):
            par_eng_post[i] = self.myfit[engName].GetParameter(i)
            err_eng_post[i] = self.myfit[engName].GetParError(i)
            print "%-15s = %20.5f"%(self.myfit[engName].GetParName(i), self.myfit[engName].GetParameter(i))

        print "[Run Energy All] Signal MP : %f"%par_eng_post[4]  ## already corrected in langaus.C

        ## Set plot range
        self.myhist[engName].Draw()
        print "[Gaus+LanGaus] default plot range: [%f to %f]"%(self.plot_rng[1],self.plot_rng[2])
        if self.plot_rng[2]>4.*par_eng_post[4]:
            self.plot_rng[2] = 4.*par_eng_post[4]
            print "[Gaus+LanGaus] updated plot range: [%f to %f]"%(self.plot_rng[1],self.plot_rng[2])

        self.myhist[engName].GetXaxis().SetRangeUser(self.plot_rng[1],self.plot_rng[2])
        gPad.Update()
        self.canvas.Update()

        self.myfit[engName].Draw("same")

        ## draw extrapolation of fitted function to full plotting range:
        # lower end
##        self.myfit["%s_extL"%engName] = TF1("myfit_%s_extL"%engName,fobjtmp.gauslangaufun,self.plot_rng[1],fRngFit[0],7)
##        self.myfit["%s_extL"%engName].SetLineColor(kBlue)
##        self.myfit["%s_extL"%engName].SetParameters(par_eng_post[0],par_eng_post[1],par_eng_post[2],par_eng_post[3],par_eng_post[4],par_eng_post[5],par_eng_post[6])
##        self.myfit["%s_extL"%engName].Draw("same")
        # upper end
        self.myfit["%s_extR"%engName] = TF1("myfit_%s_extR"%engName,fobjtmp.gauslangaufun,fRngFit[1],self.plot_rng[2],7)
        self.myfit["%s_extR"%engName].SetLineColor(kBlue)
        self.myfit["%s_extR"%engName].SetParameters(par_eng_post[0],par_eng_post[1],par_eng_post[2],par_eng_post[3],par_eng_post[4],par_eng_post[5],par_eng_post[6])
        self.myfit["%s_extR"%engName].SetNpx(1000)
        self.myfit["%s_extR"%engName].Draw("same")

        valPhys["npe"] = (par_eng_post[4],err_eng_post[4])
        valPhys["ped"] = (par_eng_post[1],err_eng_post[2])


        #gPad.Update()
        self.canvas.Update()

        if (not self.myopts.batch):
            save = raw_input("Do you want to save this plot? (y/n): ")
        else:
            save = "y" if self.myopts.savefiles else "n"

        if (save=="y"):
            tempName = "all_%s"%syst if syst != None else "all_Nominal"
            gPad.SetLogy()
            gPad.Update()

            ## Move stats box
            ps = self.myhist[engName].GetListOfFunctions().FindObject("stats").Clone("mystats_%s"%engName)
            ps.SetBorderSize(0)
            ps.SetX1NDC(self.stats_pos[0])
            ps.SetY1NDC(self.stats_pos[1])
            ps.SetX2NDC(self.stats_pos[2])
            ps.SetY2NDC(self.stats_pos[3])
            ps.SetFillStyle(0)
            #ps.SetFillColorAlpha(0,100)
            ps.Draw()
            self.myhist[engName].SetStats(0) ##This is needed; it removed "stats"
            self.canvas.Modified()

            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.png"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.pdf"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
            self.canvas.SaveAs("Results/energy/%s_pedCut%s_%s_%s.root"%(self.procname,str(self.mypedcut).replace(".","p"),tempName,self.ftag))
        print "="*150
        return OrderedDict(sorted(valPhys.items(), key=lambda x: x[0])),OrderedDict(sorted(self.myhist.items(), key=lambda x: x[0]))



###############################
## Estimate SPE
###############################
def EstimateSPE(filelist, mypedcut, ftag, fitrange, options={}):
    vSPE = []
    fidx = 0
    for line in filelist:
        tline=[]
        tline = line.split(" ")
        if tline[0].find("#") != -1: continue
        fileName = tline[0]
        print "+"*150
        print "+"*150
        tmpTxt = (line.split("/")[1]).replace("-","_").replace(" ","_")
        sampleName = tline[1]
        sample = tline[1].split("-")[0]
        print "[Est. SPE] Sample Name:", sampleName
        print "[Est. SPE] TagName:", ftag

        if fidx > len(mypedcut)-1: mypedcut.append(mypedcut[len(mypedcut)-1])
        anatmp = LYCosmicAna(fileName,sampleName,options,mypedcut[fidx],ftag,options.verbose)
        speName = "spe_%s_%s_Nominal"%(anatmp.procname,anatmp.ftag)
        anatmp.run_energy_ped(None,fitrange)
        anatmp.myhist[speName] = TH1F("myhist_%s"%speName,"%s_SPE"%anatmp.hist_title[3],anatmp.nbin_eng,anatmp.plot_rng[1],anatmp.plot_rng[2])
        anatmp.mytree["%s_%s"%(anatmp.procname,anatmp.ftag)].Draw("(areaFromScope*1.e9*%f-%f)>>myhist_%s"%(anatmp.polarity,anatmp.par_eng[1],speName),"")
        anatmp.myfit[speName] = TF1("myfit_%s"%speName,"gaus",0.15,0.35)
        anatmp.myhist[speName].Fit("myfit_%s"%speName,"RL")
        anatmp.SPE = anatmp.myfit[speName].GetParameter(1)
        print "[Est. SPE] SPE = %8.5f"%anatmp.SPE
        vSPE.append(anatmp.SPE)
        del anatmp
        fidx+=1

    return vSPE



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
        if len(tmpTxt.split("_"))>1: tmpTxt = tmpTxt.split("_")[0]
        tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
        fheader = header.replace(" ","-").replace("_","-")
        if len(tmpTxt.strip("0")) == 0:
            tblName[sigName] = ["%02i"%idx,"%s (%s; 0 day aft. irr.) "%(fheader, doselabel)]
        else:
            if int(tmpTxt.strip("0")) < 2:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(fheader, doselabel, tmpTxt.strip("0"))]
            else:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(fheader, doselabel, tmpTxt.strip("0"))]

        idx+=1

    ###############################
    ## Save to a text file 
    ###############################
    fnameTag = "%s_p%s"%(poptions.outtag,fTag)
    filetag = header.replace(" ","_").replace("-","_")
    outTxtFile = "DoseConstants/DoseConst_Cosmic_%s_%s_%s_%s.txt"%(filetag,sampleset,refPlot[0],fnameTag)
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

    print "    %-40s & %5.2f  \\\ "%(header,CalcNPEMIP(vInput[refPlot[0]][0],vOffset[0]))
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
        if len(tmpTxt.split("_"))>1: tmpTxt = tmpTxt.split("_")[0]
        tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
        fheader = header.replace(" ","-").replace("_","-")
        if len(tmpTxt.strip("0")) == 0:
            tblName[sigName] = ["%02i"%idx,"%s (%s; 0 day aft. irr.)"%(header, doselabel)]
        else:
            if int(tmpTxt.strip("0")) < 2:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(header, doselabel, tmpTxt.strip("0"))]
            else:
                tblName[sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(header, doselabel, tmpTxt.strip("0"))]
        idx+=1

    for k,n in sorted(tblName.items(), key=lambda x: x[1]):
        print "|  <verbatim>%s</verbatim>  |  <verbatim>%8.5f</verbatim>  |  <verbatim>%8.5f</verbatim>  |"%(n[1],vDconst[k][0],vDconst[k][1])






###############################
## print results on screen method2
###############################
def PrintResults2(vDconst, vInput, vOffset, sampleset, header, doselabel, refPlots, IrrPlots, fTag, poptions):
    print "\n\n"
    print "*"*150
    for n0,v0 in sorted(vDconst.items()):
        print "[PrintResults2] >>>>>> D w.r.t. un-irr. sample [%-30s]"%n0
        for n,v in sorted(v0.items()):
            print "[PrintResults2] Dose constant %-30s = %8.5f +/- %-8.5f [%5.2f %%]; R = %8.5f+/- %-8.5f"%(n,v[0],v[1],v[1]/v[0]*100.,v[2],v[3])

    tblName={}

    for refName in sorted(refPlots):
        tblName[refName]={}
        idx=2
        for sigName in sorted(IrrPlots):
            #print sigName
            tmpTxt = sigName.split("-")[1]
            if len(tmpTxt.split("_"))>1: tmpTxt = tmpTxt.split("_")[0]
            tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
            fheader = header.replace(" ","-").replace("_","-")
            if len(tmpTxt.strip("0")) == 0:
                tblName[refName][sigName] = ["%02i"%idx,"%s (%s; 0 day aft. irr.) "%(fheader, doselabel)]
            else:
                if int(tmpTxt.strip("0")) < 2:
                    tblName[refName][sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(fheader, doselabel, tmpTxt.strip("0"))]
                else:
                    tblName[refName][sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(fheader, doselabel, tmpTxt.strip("0"))]

            idx+=1
    ###############################
    ## Print values only
    ###############################
    vDCtemp = {}
    for n0,v0 in sorted(vDconst.items()):
        for n,v in sorted(v0.items()):
            vDCtemp["%s:%s"%(n0,n)] = v

    print "\n"
    print "*"*150
    for k,v in sorted(vDCtemp.items(), key=lambda x: x[1]):
        print "%8.5f\t%-8.5f\t%8.5f\t%-8.5f"%(v[0],v[1],v[2],v[3])

    ###############################
    ## Save to a text file 
    ###############################
    fnameTag = "%s_p%s"%(poptions.outtag,fTag)
    filetag = header.replace(" ","_").replace("-","_")
    outTxtFile = "DoseConstants/DoseConst_Cosmic_%s_%s_%s_all.txt"%(filetag,sampleset,fnameTag)
    outf=open(outTxtFile,'w')
    print "\n"
    print "*"*150
    ## sorted by dose const
    outf.write("%-80s\t%8s\t%8s\t%8s\t%8s\n"%("Sample label","D","sigD ","R","sigR "))
    outf.write("Sorted by value:\n")
    for n0,v0 in sorted(vDconst.items(), key=lambda x: x[1]):
        for n,v in sorted(v0.items(), key=lambda x: x[1]):
            #print "%-80s: %8.5f +/- %-8.5f; R = %8.5f+/- %-8.5f"%(tblName[n0][n][1],v[0],v[1],v[2],v[3])
            outf.write("%-80s\t%8.5f\t%-8.5f\t%8.5f\t%-8.5f\n"%(tblName[n0][n][1],v[0],v[1],v[2],v[3]))

    ## sorted by naming order
    outf.write("\nSorted by name:\n")
    for k0,n0 in sorted(tblName.items(), key=lambda x: x[1]):
        kidx = 0
        for k,n in sorted(n0.items(), key=lambda x: x[1]):
            #print k
            print "[PrintResults2] %-80s: D = %8.5f +/- %-8.5f; R = %8.5f+/- %-8.5f"%(
                n[1],vDconst[k0][k][0],vDconst[k0][k][1],vDconst[k0][k][2],vDconst[k0][k][3])
            outf.write("%-80s\t%8.5f\t%-8.5f\t%8.5f\t%-8.5f\n"%(
                n[1],vDconst[k0][k][0],vDconst[k0][k][1],vDconst[k0][k][2],vDconst[k0][k][3]))
            if kidx == 0: tmparray = numpy.array(vDconst[k0][k])
            else: tmparray += numpy.array(vDconst[k0][k])
            kidx+=1

        tmparray/=kidx
        outf.write("%-80s\t%8.5f\t%-8.5f\t%8.5f\t%-8.5f\n"%(
            "Average",tmparray[0],tmparray[1],tmparray[2],tmparray[3]))

    outf.close()
    print "\n"
    print "*"*150
    print "[PrintResults2] Save results to %s"%(outTxtFile)
    print "*"*150

    ###############################
    ## print results for latex
    ###############################
    print "\n\n[PrintResults2] Result table in latex format\n"
    print "\n"
    print "\\begin{table}[!htbp]"
    print "  \\begin{center}"
    print "    \\begin{tabular}{l c c}"
    print "    \hline\hline"
    print "    Sample name & Dose constant (D) & Uncertainty ($\sigma_\mathrm{D}$)  \\\ "

    for k0,n0 in sorted(tblName.items(), key=lambda x: x[1]):
        print "    \hline"
        print "    \multicolumn{3}{c}{$\\rm Un-irr~sample: %s$}  \\\ \hline"%(k0.replace("_","~"))
        for k,n in sorted(n0.items()):
            print "    %-40s & %8.5f & %-8.5f  \\\ "%(n[1],vDconst[k0][k][0],vDconst[k0][k][1])

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
    print "\n\n[PrintResults2] Result npe/MIP table in latex format\n"
    print "\n"
    print "\\begin{table}[!htbp]"
    print "  \\begin{center}"
    print "    \\begin{tabular}{l c}"
    print "    \hline\hline"
    print "    Sample name & $n_{P.E.}/MeV$ \\\ "

    for k0,n0 in sorted(tblName.items(), key=lambda x: x[1]):
        print "    \hline"
        print "    \multicolumn{2}{c}{$ \\rm Un-irr.~sample: %s$}  \\\ \hline"%(k0.replace("_","~"))
        print "    %-40s & %5.2f  \\\ "%("%s (Un-irr.)"%header,CalcNPEMIP(vInput[k0][0],vOffset[0]))
        for k,n in sorted(n0.items()):
            print "    %-40s & %5.2f  \\\ "%(n[1],vDconst[k0][k][4])

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
    print "\n\n[PrintResults2] Result table for twiki\n"
    for refName in sorted(refPlots):
        idx=2
        for sigName in sorted(IrrPlots):
            tmpTxt = sigName.split("-")[1]
            if len(tmpTxt.split("_"))>1: tmpTxt = tmpTxt.split("_")[0]
            tmpTxt = tmpTxt[:len(tmpTxt)-1] ## strip "d"
            fheader = header.replace(" ","-").replace("_","-")
            if len(tmpTxt.strip("0")) == 0:
                tblName[refName][sigName] = ["%02i"%idx,"%s (%s; 0 day aft. irr.)"%(header, doselabel)]
            else:
                if int(tmpTxt.strip("0")) < 2:
                    tblName[refName][sigName] = ["%02i"%idx,"%s (%s; %3s day aft. irr.) "%(header, doselabel, tmpTxt.strip("0"))]
                else:
                    tblName[refName][sigName] = ["%02i"%idx,"%s (%s; %3s days aft. irr.)"%(header, doselabel, tmpTxt.strip("0"))]
            idx+=1

    for k0,n0 in sorted(tblName.items(), key=lambda x: x[1]):
        print "%s %s %s"%("*"*64,k0,"*"*64)
        for k,n in sorted(n0.items()):
            print "|  <verbatim>%s</verbatim>  |  <verbatim>%8.5f</verbatim>  |  <verbatim>%8.5f</verbatim>  |"%(n[1],vDconst[k0][k][0],vDconst[k0][k][1])
