#!/usr/bin/python
import os, sys, math

sys.path.append(os.path.abspath(os.path.curdir))
from ROOT import TTree, TFile

from Plotter import parseLYAnaInputArgs
options = parseLYAnaInputArgs()

#makes a tree from a tab-delimited text file with columns of data
def makeEMSTree(dirname,fname):
    print dirname
    ifname = dirname.rstrip("/")+ "/" + fname + ".txt"

    loutdir = (dirname.rstrip("/")).split('/')
    if len(loutdir)>1:
        outdir = "."
        for idx in range(1,len(loutdir)):
            outdir+="%s/"%(loutdir[idx])
    else:
        outdir = "."
    try:
        os.makedirs(outdir)
    except:
        pass

    ofname = outdir.rstrip("/") + "/" + fname + ".root"
    print ofname
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
    print "Created tree in root file: %s"%ofname

if __name__ == '__main__':
    print options
    DataDir = options.inputdir
    list_input = open("%s"%options.input)

    for line in list_input:
        print line
        if line.find("#") != -1:
            print 'Ignore file: '+line.strip('#')
            continue

        #check for file suffix to be removed
        if len(line)>=4 and line[len(line)-4:].find("txt") != -1:
            line=line[:len(line)-5]

        makeEMSTree(DataDir,line)
