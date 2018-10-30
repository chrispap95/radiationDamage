Radiation Damage Recovery Project UMD

The project contains scripts/data/results only for measurements with the alpha source for now. 
Most of the code has been written by Geng-Yuan Jeng.

```bash
.
└── alphaSource                 # Main directory
    ├── data                    # Here  you put your .root files
    ├── results                 # Plots and results go here
    │   └── 20181027
    └── scripts                 # All the scripts and the routines that we are going to use
        └── Plotter             # Routines for plotting, fitting and style configuration
```

In order to able to run this code you should have already installed Python and ROOT.

I will provide more info on how to run the scripts later.

##############################################################################################
FREADME file contents

Assume you are working in the folder cosmicstandana.

If there isn't already one, create a folder called Data for storing the data txt files saved from the lab.
Under Data, create two subfolders: AlphaSource and Cosmics, where you will save respectively the data taken from alpha source measurements or cosmics measurements.
Save your data txt files in AlphaSource or Cosmics.


ALPHA SOURCE ANALYSIS

First, make a txt file that lists all the data txt files that you want to process. Usually you take all your measurements for a set of samples on the same day; in the example command below, the date is 20180223, but you should replace that with the date of your measurements of interest.

ls -l Data/AlphaSource | grep -ir 20180223 | awk '{print substr($10,0,length($10))}' > RunLists/List-AlphaSource-20180223.txt



Then, you want to run makeTree.py on this list of files to convert them into ROOT histograms. So, you do the following with your list file:

python makeTree.py - -i RunLists/List-AlphaSource-20180223.txt -d Data/AlphaSource


Finally, choose one of the more recent compareSimpleAlpha_????.py files and edit it to run over your new root files, which will be saved in root/AlphaSource/
In this python script, create an alias for each root file that you want to run over. Decide which histograms you want to compare on the same canvas, and then create a vector of canvases "plotSets" (at the end of the script); for each canvas, specify the set of histograms (the aliases you defined earlier) that you want to plot together for comparison.
Then, run it:

python compareSimpleAlpha_????.py


COSMICS ANALYSIS

First, make a txt file that lists all the data txt files that you want to process. In RunLists/, create a .txt file with a name like List-Cosmics-DESCRIPTIVENAME.txt, where you replace DESCRIPTIVENAME with some descriptive name that characterizes the set of runs that you are processing, so that you remember what they correspond to. (For example, if you are analyzing a series of cosmic runs taken on a specific EJ200 tile with ID number N, you could call the list List-Cosmics-EJ200N.txt.)



Then, you want to run makeTree.py on this list of files to convert them into ROOT histograms. In the text file with the list of filenames, remember to leave an empty line at the end of the list, so that all the filenames will be processed, otherwise makeTree.py will jam on the last file. Anyway, back in cosmicstandana/, do the following with your list file:

python makeTree.py - -i RunLists/List-cosmics-DESCRIPTIVENAME.txt -d Data/Cosmics


This will convert the contents of the .txt files into histograms saved in root files, which are stored in the folder root/Data/Cosmics/.

Finally, choose one of the more recent compareSimpleAlpha_????.py files and edit it to run over your new root files, which will be saved in root/AlphaSource/. Once again, replace ???? with some descriptive name that characterizes the run or set of runs that you are studying.
In this python script, create an alias for each root file that you want to run over. Decide which histograms you want to compare on the same canvas, and then create a vector of canvases "plotSets" (at the end of the script); for each canvas, specify the set of histograms (the aliases you defined earlier) that you want to plot together for comparison.
N.B.: unlike alpha source data runs, which measure each signal as positive, the cosmic runs measure the signal as negative. Therefore, you will need to multiply the histogram for each cosmic run by -1 before plotting it. Open one of the old compare???.py files for cosmics to get a template for how this is done.
When you are done, run the python script:

python compareSimpleCosmics_????.py



FITTING THE RESULTS OF A COSMICS ANALYSIS

In general, for each run, you should find the pedestal yourself and choose the best values for fitRng and fitRngPed. Below is a series of example commands to show how to analyze a hypothetical file called filename.root (replace it with the real filename of interest). You may need to change the pedestal fit range (-0.4 to -0.1 divided into 8 bins in my example) and the signal peak fit range (-1 to 10 divided into 10 bins in my example) depending on what your own data looks like. For instance, if the pedestal peak seems to go from -0.8 to -0.3, use those limits instead. All comments (i.e., not commands to run in the terminal) start with "//".

// Open your file in a ROOT environment
root -l root/Cosmics/filename.root

//Pedestal fitting:
//First, draw the pedestal region of the signal histogram
tree->Draw("areaFromScope*-1*pow(10,9)>>h0(20,-1,0)","")
//Look at the plot, decide the pedestal range, and fit a gaussian to it
h0->Fit("gaus","R","",-0.4,-0.1)
//Set up the pedestal-subtracted signal peak fitter
.L Plotter/langaus.C
PyLandGaus *fobj=new PyLandGaus()
//Range of signal histogram (you may want to expand the range if the example below is too small)
double fitRng[] = {10,-1,10}
//Range of pedestal fit (enter the range used in the gaussian fit you did before)
double fitRngPed[] = {8,-0.4,-0.1}
//Fit the pedestal-subtracted peak!
fobj->langaus("root/Cosmics/filename.root",fitRng,true,fitRngPed)

Look at the output canvas and check that the pedestal is now centered over zero and everything looks reasonable. The most probable value (MPV) on the legend is the fitted signal mean.
