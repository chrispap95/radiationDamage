#!/usr/bin/python

###################################################################################################
##                                                                                               ##
##   This routine is going to fit multiple measurements and write the results in a file          ##
##                                                                                               ##
###################################################################################################

import os, sys, math, datetime, numpy

sys.path.append(os.path.abspath(os.path.curdir))
from ROOT import gROOT, gStyle, TFile, TTree, TH1F, TH1D, TCanvas, TPad, TMath, TF1, TLegend, gPad, gDirectory
from ROOT import kRed, kBlue, kGreen, kWhite, TLatex

from Plotter import parseLYAnaInputArgs
options = parseLYAnaInputArgs()

gStyle.SetOptStat(0)

def fitFile(dirname,fname):
    print(dirname)
    ifname = dirname.rstrip("/")+ "/" + fname + ".txt"

    if len(dirname.split("/"))>1:
        outdir = "root/"+dirname.split('/')[1]
    else:
        outdir = "root"
    try:
        os.makedirs(outdir)
    except:
        pass

    ofname = outdir + "/" + fname + ".root"
    print(ofname)
    #output file and tree
    outfile = TFile(ofname,"RECREATE")
    tree = TTree("tree","tree of "+fname)

    #create tree from text file

    # Raw formats:
    desc = "time:amplitude:area:areaFromScope"

    tree.ReadFile(ifname,desc) # delimiter = space
    #tree.ReadFile(ifname,desc,",") # delimiter = comma

    #save output tree
    outfile.cd()
    tree.Write()
    outfile.Close()

    #print message
    print("Created tree in root file: %s"%ofname)

if __name__ == '__main__':
    print(options)
    DataDir = options.inputdir
    list_input = open("%s"%options.input)

    for line in list_input:
        print(line)
        if line.find("#") != -1:
            print('Ignore file: '+line.strip('#'))
            continue

        #check for file suffix to be removed
        if len(line)>=4 and line[len(line)-4:].find("txt") != -1:
            line=line[:len(line)-5]

        fitFile(DataDir,line)
