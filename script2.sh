#!/bin/bash

CHEMIN=/home/g/gallierr/Documents/CL/tp5/Corpus_RPM2_resumes/Corpus_RPM2__documents_mono;
cd $CHEMIN;


for x in {01..20}; 
	do     
	for y in {1..2};
		do 
		head -n 5 T"$x"_C"$y"_mono.txt > ../baseline_head/res_T"$x"_C"$y"_head.txt;
		
	done;
done;
