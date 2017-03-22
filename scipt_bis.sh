#!/bin/bash

CHEMIN=/home/g/gallierr/Documents/CL/tp5/Corpus_RPM2_resumes/Corpus_RPM2_references;
cd $CHEMIN;
ls ..;

for x in {01..20}; 
	do     
	for y in {1..2};
		do 
		for z in {1..4};
			do
			cat T"$x"_C"$y"_A"$z".txt > ../Corpus_RPM2_references2/T"$x"_C"$y"_A"$z".txt;	
			cat T"$x"_C"$y"_A"$z".txt |  perl ../../lowercase.perl | perl /home/p/portetf/Tools/moses-2015-11-20/scripts-moses/tokenizer/tokenizer.perl -l fr > T"$x"_C"$y"_A"$z".txt;	
			done;
	done;
done;


