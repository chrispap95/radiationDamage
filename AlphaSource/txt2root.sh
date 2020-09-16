#!/bin/sh
ls -l Data/AlphaSource.nosync | grep -i $1 | \
awk '{print substr($NF,0,length($NF))}' > RunLists/List-AlphaSource-$1.txt
python makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource.nosync
