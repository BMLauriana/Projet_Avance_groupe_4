CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm

#EXECUTABLES
EXECS=main0 main1

#MODULES COMMUNS
COMMON_OBJS=lecture_donnees.o lire_fichier.o

#MODULE SPECIFIQUES
MAIN0_OBJS=main0.o $(COMMON_OBJS)
MAIN1_OBJS=main1.o $(COMMON_OBJS) force_brute.o

#REGLE PAR DEFAUT POUR FAIRE TOUS LES EXECUTABLES
all: $(EXECS)

#EXECUTABLES
#($@ donne le nom de la cible et 
#$^ la liste de toutes les dependances)
main0: $(MAIN0_OBJS)
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

main1: $(MAIN1_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

#RGLE SPECIFIQUE POUR lire_fichier.c
lire_fichier.o: lire_fichier.c lecture_donnees.h
	$(CC) $(CFLAGS) -c $< -o $@

#REGLE POUR TOUS LES .o
#($< la premiere dependance par exemple
#lorsque l'on construit lecture_donnees.o $<
#donne lecture_donnees.c
#ET % remplace par le nom de chaque fichier)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

#REGLE POUR LES .o QUI N'ONT PAS DE .h
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#(-o $@ garantit que le .o produit correspond exactement
#a la cible de la regle)

#(si la regle n'est pas generique on ecrirait :
#main1.o: main1.c
#	$(CC) $(CFLAGS) -c $< -o $@
#main2.o: main2.c
#	$(CC) $(CFLAGS) -c $< -o $@

#lire_fichier.o: lire_fichier.c lecture_donnees.h
#	$(CC) $(CFLAGS) -c $< -o $@

#force_brute.o: force_brute.c force_brute.h
#	$(CC) $(CFLAGS) -c $< -o $@)

#COMMANDE MAKE CLEAN
clean:
	rm -f *.o $(EXECS)

#(on supprime tous les .o)