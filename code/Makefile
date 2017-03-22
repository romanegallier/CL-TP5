CFLAGS=-c -g 
LDFLAGS= -lm


resume: resume.o dictionnaire.o document.o liste.o 
	gcc  -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< 
		
clean: 
	rm -f *.o *.orig
	
archive : 
	make clean 
	tar czvf ../$(notdir $(PWD) )-`whoami`-`date +%d-%m-%H-%M`.tar.gz ../$(notdir $(PWD) )
	echo "Fichier archive ../code-`whoami`-`date +%d-%m-%H-%M`.tar.gz généré"

