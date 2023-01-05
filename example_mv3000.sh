#!/bin/bash


EXE=./GenParGunPileUp.exe
OUTFILE=out_GenParGunPileUp_mv3000.root
INFILE=MV_nfsp2_small.csv
SEED=6759
NumVtx=2

$EXE -o $OUTFILE\
     -i $INFILE\
     -s $SEED\
     --MaxVtx $NumVtx
