#!/bin/sh
ls -l Data/AlphaSource | grep -ir $1 | \
awk '{print substr($11,0,length($11))}' > RunLists/List-AlphaSource-$1.txt
if [ -z RunLists/List-AlphaSource-$1.txt ]
then
        python makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource
else
        ls -l Data/AlphaSource | grep -ir $1 | \
        awk '{print substr($10,0,length($10))}' > RunLists/List-AlphaSource-$1.txt
        python makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource
fi
