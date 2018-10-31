```
#UMD Radiation Damage Recovery Project
```
##Current Status
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
##Prerequisites
In order to able to run this code you should have already installed Python 2 and ROOT.
Warning: Do not use python 3!

##Instructions
I will provide more info on how to run the scripts later.

##############################################################################################
##FREADME file contents

Save your data txt files in alphaSource/data.

###ALPHA SOURCE ANALYSIS

First, make a txt file that lists all the data txt files that you want to process. Usually you take all your measurements for a set of samples on the same day; in the example command below, the date is 20180223, but you should replace that with the date of your measurements of interest. Then, from alphaSource directory run

```bash
ls -l data | grep -ir 20180223 | awk '{print substr($10,0,length($10))}' > runLists/List-AlphaSource-20180223.txt
```

Then, you want to run makeTree.py on this list of files to convert them into ROOT histograms. So, you do the following with your list file from the scripts directory:

```bash
python makeTree.py - -i ../runLists/List-AlphaSource-20180223.txt -d ../data
```

Finally, choose one of the more recent compareSimpleAlpha_????.py files in scripts directory and edit it to run over your new root files, which will be saved in alphaSource/data
In this python script, create an alias for each root file that you want to run over. Decide which histograms you want to compare on the same canvas, and then create a vector of canvases "plotSets" (at the end of the script); for each canvas, specify the set of histograms (the aliases you defined earlier) that you want to plot together for comparison.
Then, run it:

```bash
python compareSimpleAlpha_????.py
```
