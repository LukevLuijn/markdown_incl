#!/bin/bash

source="$PWD/$1"
output="$PWD/$2"

# change this to your actual path
program=$HOME/path/to/build/folder/markdown_incl

while getopts "hs:o:" option; do
   case $option in
      h) $program -h && exit;;
      s) source="$PWD/$OPTARG";;
      o) output="$PWD/$OPTARG";;
     \?) $program -h && exit;;
   esac
done

$program -s="$source" -o="$output"
