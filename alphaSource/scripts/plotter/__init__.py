"""
Authors : Geng-Yuan Jeng <Geng-Yuan.Jeng@cern.ch>

"""
import optparse

def parseLYAnaInputArgs():
    parser = optparse.OptionParser(description='Scintillator light yield input configuration.')
    parser.add_option('-i', '--input', default=None, help="input (list) file")
    parser.add_option('-p', '--isPlastic', default=False, help="Plastic scintillator? False (default).", action="store_true")
    parser.add_option('--polarity', default=True, help="Use default amplitude polarity (-1 for PMT)? True (default).", action="store_false")
    parser.add_option('-e', '--publish', default=False, help="Publish the results? False (default).", action="store_true")
    parser.add_option('-d', '--inputdir', default=None, help="input dir")
    parser.add_option('-t', '--outtag', default="Test", help="tag for output files")
    parser.add_option('-c', '--myPedCut', default=None, help="Set the pedestal cut. 0.005 is a good start.")
    parser.add_option('-n', '--normalize', default=False, help="Normalizing pedestal entries of plots? False (default).", action="store_true")
    parser.add_option('-u', '--unitarea', default=False, help="Normalizing plot area to 1? False (default).", action="store_true")
    parser.add_option('-s', '--savefiles', default=False, help="Save plots? False (default).", action="store_true")
    parser.add_option('-b', '--batch', default=False, help=" Batch mode? False (default).", action="store_true")
    parser.add_option('--timeCut', default=0, help="Set the time delay cut. 0 seconds is default.")
    parser.add_option('--temps', default="24.7,24.7", help="Set the temperatures for exp. mean calculation.")
    parser.add_option('-v', '--verbose', default=False, help="Print debugging info", action="store_true")
    parser.add_option('--sampleSet', default="Test", help="Sample set name, e.g., EJ260_2p1s")

## to be added or not
##    get_amp  = False
##    get_ped  = False
##    get_sig  = False

    (options, args) = parser.parse_args()

    return options

def parseInputArgs():
    usage = "usage: python %prog -k <Sample Name>,<excitation wavelength> -x <xmin>,<xmax> -i <run files config> -t <results plot tag>\n"
    usage += "Eg., python PlotEMS.py - -k EJ-200_2.0X,350 -x 360,600 -i Data/Runfiles/EJ200_2.0X_30d_compareDose_exct350nm_20150728.txt -t CompDose-EJ200_2.0X_30degree_exct350_20150728"

    parser = optparse.OptionParser(description='Input configuration.',usage=usage)
    parser.add_option('-i', '--input', default=None, help="input (list) file")
    parser.add_option('-d', '--inputdir', default=None, help="input dir")
    parser.add_option('-t', '--outtag', default="Test", help="tag for output files")
    parser.add_option('-x', '--xrange', default="405,600", help="emission wavelength range [nm]")
    parser.add_option('-y', '--yrange', default="0.2,1.8", help="ratio plot y-range")
    parser.add_option('-k', '--keywords', default="EJ-200,350", help="labels/titles for the plot; separated by comma")
    parser.add_option('--NFfile', default="RefNF_EJ200_2X_Sample03_exct400.txt", help="File containing all normalization factors. Default is RefNF_EJ200_2X_Sample03_exct400.txt")
    parser.add_option('--noNorm', default=False, help="Do not apply normalization factors.", action="store_true")

    parser.add_option('-a', '--abs', default=True, help="Absorption [default] or Transmittance", action="store_false")
    parser.add_option('-c', '--myPedCut', default=0, help="Set the pedestal cut. 0.005 is a good start.")
    parser.add_option('-r', '--showratio', default=False, help="Show ratio plot", action="store_true")
    parser.add_option('--nosyst', default=False, help="Not making systematics plots", action="store_true")
    parser.add_option('--timeCut', default=-1.0, help="Set the time delay cut. No cut is the default.")

    parser.add_option('-v', '--verbose', default=False, help="Print debugging info", action="store_true")
    ## specific for makeEMSTree
    parser.add_option('-s', '--skimmed', default=False, help="Skimmed formats or not", action="store_true")

    (options, args) = parser.parse_args()

    return options
