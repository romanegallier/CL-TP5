#!/bin/bash

CHEMIN=/home/g/gallierr/Documents/CL/tp5/Corpus_RPM2_resumes/Corpus_RPM2_documents;
cd $CHEMIN;
ls ..;

for x in {01..20}; 
	do     
	for y in {1..2};
		do 
			
			cat T"$x"_C"$y"_*.txt |  perl ../../lowercase.perl | perl /home/p/portetf/Tools/moses-2015-11-20/scripts-moses/tokenizer/tokenizer.perl -l fr >> ../Corpus_RPM2__documents_mono/T"$x"_C"$y"_mono.txt;	
	done;
done;


