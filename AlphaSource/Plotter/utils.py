# encoding: utf-8
'''
utils.py

description:

'''
__author__    = "Will Davey"
__email__     = "will.davey@cern.ch"
__created__   = "2013-02-28"
__copyright__ = "Copyright 2013 Will Davey"
__license__   = "GPL http://www.gnu.org/licenses/gpl.html"



## modules
import os
import ROOT
from array import array 

## globals
open_files = []
# - - - - - - - - - - - class defs  - - - - - - - - - - - - #

# - - - - - - - - - - function defs - - - - - - - - - - - - #
#____________________________________________________________
def open_file( file_name ):
    '''
    open ROOT file, or get handle if already open
    '''
    ## check if file open 
    if ROOT.gROOT.GetListOfFiles():
        f = ROOT.gROOT.GetListOfFiles().FindObject(file_name)
        if f:
            return f
    ## open new file
    f = ROOT.TFile(file_name)
    if f:
        global open_files
        open_files.append(f)
        return f
    else:
        print 'ERROR file: %s doesn\'t exist' % file_name 
        return None

#____________________________________________________________
def new_file( file_name ):
    '''
    create new ROOT file, or get handle if already open
    '''
    ## check if already open 
    if ROOT.gROOT.GetListOfFiles():
        f = ROOT.gROOT.GetListOfFiles().FindObject(file_name)
        if f:
            return f
    f = ROOT.TFile(file_name,'RECREATE')
    if f:
        global open_files
        open_files.append(f)
        print 'created new file: %s' % file_name 
        return f
    else:
        print 'ERROR file: %s doesn\'t exist' % file_name 
        return None


#____________________________________________________________
def mkdir( f, dir_name ):
    directory = f.GetDirectory('')
    if f.GetDirectory(dir_name): 
        directory = f.GetDirectory(dir_name)
        return directory
    else: 
        dirs = []
        temp = dir_name
        while temp != '' and temp != '/' :
            dirs.insert(0,os.path.basename(temp))
            temp = os.path.dirname( temp )
        for dir in dirs:
            if directory.GetDirectory(dir):
                directory = directory.GetDirectory(dir)
            else:
                directory = directory.mkdir(dir)

    return directory


#____________________________________________________________
def get_object(obj_name,filename):
    if not os.path.isfile(filename):
        ##print 'ERROR file: %s doesn\'t exist' % filename
        return False
    f = open_file( filename )
    assert f, 'ERROR failure opening file: %s' % (filename)
    obj = f.Get(obj_name)
    assert obj, 'ERROR cant retrieve %s from %s' % (obj_name,filename)
    return obj


#____________________________________________________________
def save_object(obj, filename, dirname = None ):
    f = new_file( filename )
    if dirname: d    = mkdir( f, dirname )
    else            : d    = f
    d.WriteTObject( obj )


#____________________________________________________________
def set_palette():
    NRGBs = 5
    NCont = 255
    stops = array( 'd', [0.00, 0.34, 0.61, 0.84, 1.00 ] )
    red   = array( 'd', [ 0.00, 0.00, 0.87, 1.00, 0.51 ] )
    green = array( 'd', [ 0.00, 0.81, 1.00, 0.20, 0.00 ] )
    blue  = array( 'd', [ 0.51, 1.00, 0.12, 0.00, 0.00 ] )
    ROOT.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
    ROOT.gStyle.SetNumberContours(NCont)


#____________________________________________________________
def integral_and_error(h,xmin=None,xmax=None):
    '''
    get integral and error in specified range
    '''
    min_bin = 1
    max_bin = h.GetNbinsX()
    if xmin: min_bin = h.GetXaxis().FindBin(xmin)
    if xmax: max_bin = h.GetXaxis().FindBin(xmax)
    error = ROOT.Double()
    total = h.IntegralAndError(min_bin,max_bin,error)
    return total,error

#____________________________________________________________
def integral(h,xmin=None,xmax=None):
    return integral_and_error(h,xmin,xmax)[0]

#____________________________________________________________
def integral_error(h,xmin=None,xmax=None):
    return integral_and_error(h,xmin,xmax)[1]



#____________________________________________________________
def generate_random_mc_hist(hin,hout):
    global grandom
    for i in xrange(hin.GetNbinsX()+1):
        n = hin.GetBinContent(i)
        en = hin.GetBinError(i)

        n = max(0.,grandom.Gaus(n,en))
        hout.SetBinContent(i,n)
        hout.SetBinError(i,en)

#____________________________________________________________
def generate_random_data_hist(mc_hists,hout):
    global grandom
    # method 1 
    hout.SetEntries(0)
    for i in xrange(hout.GetNbinsX()+1):
        mctot = 0.
        for h in mc_hists: mctot+=h.GetBinContent(i)
        mctot = max(0.,mctot)
        n = float(grandom.Poisson(mctot))
        en = sqrt(n)
        hout.SetBinContent(i,n)
        hout.SetBinError(i,en)

    # method 2
    '''
    clear_hist(hout)
    for h in mc_hists: 
        for i in xrange(grandom.Poisson(h.Integral())):
            hout.Fill(h.GetRandom())
    '''

#____________________________________________________________
def reset_hist(hin,hout):
    for i in xrange(hin.GetNbinsX()+1):
        hout.SetBinContent(i,hin.GetBinContent(i))
        hout.SetBinError(i,hin.GetBinError(i))


#____________________________________________________________
def clear_hist(h):
    for i in xrange(h.GetNbinsX()+1):
        h.SetBinContent(i,0)
        h.SetBinError(i,0)

    h.SetEntries(0)

#____________________________________________________________
def zero_error_hist(h):
    for i in xrange(h.GetNbinsX()+1):
        h.SetBinError(i,0)





#____________________________________________________________
def eff_err(Np,Nf,eNp,eNf):

        # e = Np / (Np + Nf) = Np * (Np+Nf)^-1
        # 
        # de^2 = (de/dNp)^2(eNp)^2 + (de/dNf)^2(eNf)
        #
        # de/dNp = 1.(Np+Nf)^-1 + Np * -1(Np+Nf)^-2*1
        #   = (Np+Nf)^-1 - Np (Np+Nf)^-2
        #
        # de/dNf = -1 * Np * (Np+Nf)^-2
        #   = -Np(Np+Nf)^-2
        #
        #
        dedNp = pow(Np+Nf,-1) - Np*pow(Np+Nf,-2)
        dedNf = -Np*pow(Np+Nf,-2)
        return sqrt(pow(dedNp*eNp,2) + pow(dedNf*eNf,2)) 


#____________________________________________________________
def create_mean_hist(sample):
    h = ROOT.TH1F("h_%s_mean"%sample,";N %s Fit;Trials"%sample,100,999999,-999999)
    h.SetBit(ROOT.TH1.kCanRebin)
    return h

#____________________________________________________________
def create_error_hist(sample):
    h = ROOT.TH1F("h_%s_error"%sample,";Error %s Fit;Trials"%sample,100,999999,-999999)
    h.SetBit(ROOT.TH1.kCanRebin)
    return h

#____________________________________________________________
def create_pull_hist(sample):
    #h = ROOT.TH1F("h_%s_pull"%sample,";Pull %s Fit;Trials"%sample,100,999999,-999999)
    h = ROOT.TH1F("h_%s_pull"%sample,";Pull %s Fit;Trials"%sample,100,-5.,5.)
    #h.SetBit(ROOT.TH1.kCanRebin)
    return h


#____________________________________________________________
def create_2d_mean_hist(sample1,sample2):
    h = ROOT.TH2F("h2_%s_%s_mean"%(sample1,sample2),";N %s Fit; N %s Fit;Trials"%(sample1,sample2),100,999999,-999999,100,999999,-999999)
    h.SetBit(ROOT.TH1.kCanRebin)
    return h


#____________________________________________________________
def scale_frame_text( fr, scale ):
    xaxis = fr.GetXaxis()
    yaxis = fr.GetYaxis()
    yaxis.SetTitleSize( yaxis.GetTitleSize() * scale )
    yaxis.SetLabelSize( yaxis.GetLabelSize() * scale )
    yaxis.SetTitleOffset( 1.1* yaxis.GetTitleOffset() / scale  )
    yaxis.SetLabelOffset( yaxis.GetLabelOffset() * scale )
    xaxis.SetTitleSize( xaxis.GetTitleSize() * scale )
    xaxis.SetLabelSize( xaxis.GetLabelSize() * scale )
    xaxis.SetTickLength( xaxis.GetTickLength() * scale )
    xaxis.SetTitleOffset( 2.5* xaxis.GetTitleOffset() / scale  )
    xaxis.SetLabelOffset( 2.5* xaxis.GetLabelOffset() / scale )


#____________________________________________________________
def get_kin_bin(pt_bin=None,eta_bin=None):
    if pt_bin is None: pt_bin = 'ptAll'
    if eta_bin is None: eta_bin = 'etaAll'
    return '%s_%s'%(pt_bin,eta_bin)


## EOF
