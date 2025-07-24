#!/bin/sh

# txt2root.sh
# This script processes files in the AlphaSource directory, generating a list of files
# that match a given pattern and then creating a ROOT tree from those files.
# Usage: ./txt2root.sh <pattern>

# Check if output directory exists, if not create it
if [ ! -d "root/AlphaSource" ]; then
    mkdir -p root/AlphaSource
fi

if [ ! -d "RunLists" ]; then
    mkdir RunLists
fi

output_name="List-AlphaSource.txt"
if [ ! -z "$1" ]; then
    output_name="List-AlphaSource-$1.txt"
fi

for ff in Data/AlphaSource/*$1*; do
    basename $ff >> RunLists/$output_name
done
python makeTree.py - -i RunLists/$output_name -d Data/AlphaSource

# Standardize file names by replacing hyphens with underscores
find root/AlphaSource -name "*-*" -execdir bash -c 'mv -- "$1" "${1//-/_}"' bash {} \;
