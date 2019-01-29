#!/bin/sh
ls -l Data/AlphaSource | grep -ir 20181024 | \
awk '{print substr($10,0,length($10))}' > RunLists/List-AlphaSource-20181024.txt
python makeTree.py - -i RunLists/List-AlphaSource-20181024.txt -d Data/AlphaSource
