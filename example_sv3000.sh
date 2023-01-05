#!/bin/bash


EXE=./GenParGunPileUp.exe
OUTFILE=out_GenParGunPileUp_sv3000.root
INFILE=SV_nfsp2_small.csv
SEED=6759
NumVtx=1

$EXE -o $OUTFILE\
     -i $INFILE\
     -s $SEED\
     --MaxVtx $NumVtx
