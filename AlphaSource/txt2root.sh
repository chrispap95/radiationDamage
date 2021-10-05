#!/bin/sh
ls -l Data/AlphaSource.nosync | grep -i $1 | \
awk '{print substr($NF,0,length($NF))}' > RunLists/List-AlphaSource-$1.txt
python2 makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource.nosync

find root/AlphaSource.nosync -name "*-*" -execdir bash -c 'mv -- "$1" "${1//-/_}"' bash {} \;
