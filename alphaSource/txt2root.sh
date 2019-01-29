#!/bin/sh
ls -l Data/AlphaSource | grep -ir $1 | \
awk '{print substr($10,0,length($10))}' > RunLists/List-AlphaSource-$1.txt
python makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource
