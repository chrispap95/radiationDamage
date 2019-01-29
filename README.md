
# UMD Radiation Damage Recovery Project

## Current Status
The project contains scripts/data/results only for measurements with the alpha source for now.
Most of the code has been written by Geng-Yuan Jeng.

```bash
.
└── alphaSource                 # Main directory
    ├── Data                    # Here you put your .txt files
    ├── Results                 # Plots and results go here
    │   └── 20181027            # Various date named folders
    ├── root                    # Here you put your .root files
    └── Plotter                 # Routines for plotting, fitting and style configuration
```
## Prerequisites
In order to able to run this code you should have already installed Python 2 and ROOT.
**Warning: Do not use python 3!**

## Instructions
In order to run any of the scripts, you can issue
```bash
python scriptName.py
```

You can fit a single measurement by issuing
```bash
python compareSimpleAlpha_fitRoutine.py -i root/AlphaSource/<Filename>.root
```
This is going to output the fit results on the command line and also create a PDF file with a plot under Results/<Date of Execution>/<Filename>.pdf.

Now follows a brief description of the scripts:
#### compareSimpleAlpha_oldRoutine.py
Plots a number of measurements under the same histogram. You have to put manually the measurements inside the plot.
#### compareSimpleAlpha_newRoutine.py
The same as the oldRoutine, but this time things are being handled automatically. You just need to put the filenames that you want to plot inside a list.
#### compareSimpleAlpha_fitRoutine.py
Takes just one measurement, fits a gaussian curve and outputs its peak with the systematic error.
#### compareSimpleAlpha_fitDrawRoutine.py
This script needs some work. It fits many measurements and and overlays their histograms with their fits. It also outputs a simple .dat file with 2 rows, the first being the peak of each curve and the second being the peak error.
#### makeTree.py
Takes a .txt file with raw measurement numbers and converts that into a histogram stored under a TTree inside a .root file.
#### makeTree_new.py
Just a newer version of the previous script which is not yet compatible with our code yet.
#### operator_comparison.cpp
Takes as input the .dat file from compareSimpleAlpha_fitDrawRoutine.py and plots the peaks numbers vs the operator name. The vertical axis has alphanumeric bins corresponding to the operator name.
#### alnum_example.cpp
Legacy version of the previous routine. Might be reserved for future use.
#### doseConstant.py
Script that calculates the Dose constant. You need to just give a list of the files and a lists containing the material types and the irradiation details.


## FREADME file contents
(This is how to create the ROOT files)
Save your data txt files in alphaSource/data.

### ALPHA SOURCE ANALYSIS

First, make a txt file that lists all the data txt files that you want to process. Usually you take all your measurements for a set of samples on the same day; in the example command below, the date is 20180223, but you should replace that with the date of your measurements of interest. Then, from alphaSource directory run
```bash
ls -l Data/AlphaSource | grep -ir 20180223 | awk '{print substr($10,0,length($10))}' > RunLists/List-AlphaSource-20180223.txt
```
At this point you should check if the list file has been created correctly. issue
```bash
cat RunLists/List-AlphaSource-20180223.txt
```
You should see a list containing all the filenames of measurements done on 20180223. If you don't see that then you might want to check if $10 is the correct number in the previous command. (Sometimes you need to set it to $9 or $11.)

Then, you want to run makeTree.py on this list of files to convert them into ROOT histograms. So, you do the following with your list file from the scripts directory:
```bash
python makeTree.py - -i RunLists/List-AlphaSource-20180223.txt -d Data/AlphaSource
```
The files are being written inside the root/alphaSource directory. You might want to check if they were created properly.

Finally, choose one of the more recent compareSimpleAlpha_????.py files in scripts directory and edit it to run over your new root files, which will be saved in alphaSource/data
In this python script, create an alias for each root file that you want to run over. Decide which histograms you want to compare on the same canvas, and then create a vector of canvases "plotSets" (at the end of the script); for each canvas, specify the set of histograms (the aliases you defined earlier) that you want to plot together for comparison.
Then, run it:

```bash
python compareSimpleAlpha_????.py
```
