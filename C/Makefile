CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm

#EXECUTABLES
EXEC=main test_0 test_2 

#MODULES COMMUNS
COMMON_OBJS=lecture_donnees.o lire_fichier.o heuristiques.o

#PERMET DE DECIDER SI LES COMMANDES SONT AFFICHEES
#VERVOSE=1 les affiches 
ECHO = @
ifeq ($(VERBOSE),1)
	ECHO=
endif

#PERMET DE DEBUGGER LE PROGRAME GRACE A GDB
#il faut utilise la commande make DEBUG=yes
ifeq ($(DEBUG),yes)
	CFLAGS += -g
endif


#REGLE PAR DEFAUT POUR FAIRE L'EXECUTABLE
all: $(EXEC)

#EXECUTABLES
#($@ donne le nom de la cible et 
#$^ la liste de toutes les dependances)
test_0: test_0.c $(COMMON_OBJS)
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

test_2: test_2.c $(COMMON_OBJS)
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

main: main.c $(COMMON_OBJS) force_brute.o ctrl_c.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDLIBS)

#REGLE SPECIFIQUE POUR lire_fichier.c
lire_fichier.o: lire_fichier.c lecture_donnees.h
	$(ECHO)$(CC) $(LDFLAGS) -c $< -o $@

#REGLE SPECIFIQUE POUR force_brute
force_brute.o: force_brute.c force_brute.h lecture_donnees.h ctrl_c.h
	$(ECHO)$(CC) $(LDFLAGS) -c $< -o $@

heuristiques.o: plus_proche_voisin.c marche_aleatoire.c deux_opt.c lecture_donnees.h
	$(ECHO)$(CC) $(LDFLAGS) -c $< -o $@

#REGLE POUR TOUS LES .o
#($< la premiere dependance par exemple
#lorsque l'on construit lecture_donnees.o $<
#donne lecture_donnees.c
#ET % remplace par le nom de chaque fichier)
%.o: %.c %.h
	$(ECHO)$(CC) $(LDFLAGS) -c $< -o $@

#REGLE POUR LES .o QUI N'ONT PAS DE .h
%.o: %.c
	$(ECHO)$(CC) $(LDFLAGS) -c $< -o $@

#(-o $@ garantit que le .o produit correspond exactement
#a la cible de la regle)

#COMMANDE MAKE CLEAN
clean:
	rm -f *.o $(EXEC)

#(on supprime tous les .o)