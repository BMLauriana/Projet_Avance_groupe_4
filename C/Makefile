CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm

#EXECUTABLES
EXEC=main0 main1

#MODULES COMMUNS
COMMON_OBJS=lecture_donnees.o lire_fichier.o 

ECHO = @
ifeq ($(VERBOSE),1)
	ECHO=
endif

#PERMET DE DEBUGGER LE PROGRAME GRACE A GDB
#il faut utilise la commande make DEBUG=yes
ifeq ($(DEBUG),yes)
	CFLAGS += -g
endif

#MODULE SPECIFIQUES
MAIN_OBJS=main.o $(OBJS)

#REGLE PAR DEFAUT POUR FAIRE L'EXECUTABLE
all: $(EXEC)

#EXECUTABLES
#($@ donne le nom de la cible et 
#$^ la liste de toutes les dependances)
main0: main0.c $(COMMON_OBJS)
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

main1: main1.c $(COMMON_OBJS) force_brute.o ctrl_c.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

#REGLE SPECIFIQUE POUR lire_fichier.c
lire_fichier.o: lire_fichier.c lecture_donnees.h
	$(CC) $(LDFLAGS) -c $< -o $@

#REGLE SPECIFIQUE POUR force_brute
force_brute.o: force_brute.c force_brute.h lecture_donnees.h ctrl_c.h
	$(CC) $(LDFLAGS) -c $< -o $@

#REGLE POUR TOUS LES .o
#($< la premiere dependance par exemple
#lorsque l'on construit lecture_donnees.o $<
#donne lecture_donnees.c
#ET % remplace par le nom de chaque fichier)
%.o: %.c %.h
	$(CC) $(LDFLAGS) -c $< -o $@

#REGLE POUR LES .o QUI N'ONT PAS DE .h
%.o: %.c
	$(CC) $(LDFLAGS) -c $< -o $@

#(-o $@ garantit que le .o produit correspond exactement
#a la cible de la regle)

#COMMANDE MAKE CLEAN
clean:
	rm -f *.o $(EXEC)

#(on supprime tous les .o)