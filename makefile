TP_Reseau: TP_Reseau.o BAL.o FonctionsMessage.o 
	
	gcc TP_Reseau.o BAL.o FonctionsMessage.o -o TP_Reseau

TP_Reseau.o: TP_Reseau.c
	
	gcc -c TP_Reseau.c

 

BAL.o: BAL.c
	
	gcc -c BAL.c


FonctionsMessage.o: FonctionsMessage.c
	
	gcc -c FonctionsMessage.c

