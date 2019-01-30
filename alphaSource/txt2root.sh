#!/bin/sh
ls -l Data/AlphaSource | grep -i $1 | \
awk '{print substr($15,0,length($15))}' > RunLists/List-AlphaSource-$1.txt
((y=15))
while [ ! -z RunLists/List-AlphaSource-$1.txt ] && [ $y -gt 9 ]
do
        ((y=$y-1))
        ls -l Data/AlphaSource | grep -i $1 | \
        awk -v x=$y '{print substr($(x),0,length($(x)))}' > RunLists/List-AlphaSource-$1.txt
done
python makeTree.py - -i RunLists/List-AlphaSource-$1.txt -d Data/AlphaSource
