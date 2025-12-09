"""
Soft ad hoc pour tester les programme C de résolution du tsp
VD 8/2025
"""
import random,math
import subprocess
import time
#import os
#import sys
import tsplib95 # fonctions pour lire les fichiers TSPLIB et faire des calculs
import pyproj # pour les coordonnées géométriques
import matplotlib.pyplot as plt # pour les graphiques
from tsp_tools import *

def extract_data(path,code,filename,method,distance_fct,coord,graphique):
    # interaction avec C avec subprocess
    if method == "-c": # canonical
        command = command = [path+code, "-f",filename,"-c"]
    else: # autres balises
        command = [path+code, "-f",filename,"-m"]+method.split()
    #print("Commande = ",command) # debug
    txt = subprocess.run(command,capture_output=True,text=True)
    lst = txt.stdout.split() # retour du C c'est une chaîne on en fait une liste de chaînes
    lst = [i for i in lst if i != ":"] # on fait le ménage
    lst = [i for i in lst if i != ";"]
    #print("lst = ",lst) # vérif debug
    try: # analyse de la chaîne renvoyé par subprocess. A adapter à votre cas
    # Notre code C affiche: "Instance ; Methode ; Temps CPU ; longueur ; Tour"
    # Exemple: ['att15', 'none', 'temps', 'CPU', '?', '13095', '13095.000000;', '[1,', '2,', ...]
        ind = lst.index('Tour')
        nom = lst[ind+1]
        algo = lst[ind+2]
        #tps = eval(lst[ind+3]) # float
        #length = int(eval(lst[ind+4])) # int

        tps = float(lst[ind+3]) 
        length = int(float(lst[ind+4])) 
        # extraire manuellement le tour
        # chercher l'élément qui commence par '[' et reconstruire la liste
        tour_start_index = None
        for i in range(ind+5, len(lst)):
            if lst[i].startswith('['):
                tour_start_index = i
                break
        if tour_start_index is None:
            print("ERREURRRRRR")
            return
        tour_str = ""
        for i in range(tour_start_index, len(lst)):
            tour_str+= lst[i]
            if lst[i].endswith(']'):
                break
        # nettoyer et convertir en liste
        tour_str_clean = tour_str.strip('[]').replace(' ',' ')
        tour = [int(x) for x in tour_str_clean.split(',')]

        #tour = eval(lst[ind+5]) # liste
        l = fitness(tour,distance_fct,coord)
        v = valid(tour)
        print(f"{nom} ; {algo} ; {length} ; {l} ; {tps} ; {tour} ; {v==0} ; {l==length}")
        graphique(tour,algo,coord)
    except Exception as e:
        print(f"Erreur (Python) e = {e} lst = {lst}")#

def load_instance(filename):
    # utilise fcts tsplib95
    instance = load_tsp_file(filename)
    cities = list(instance.get_nodes())
    instance_dict = instance.as_name_dict()
    print(instance)
    coord = instance_dict['node_coords']
    edge_type = instance_dict['edge_weight_type']
    return instance,coord,edge_type

def test_instance(filename,methods):
    # affiche la banière des résultats et appelle les programme C
    instance,coord,edge_type = load_instance(filename)
    distance_fct,graphique = select_fct(edge_type)

    print("Longueur du tour canonique (tsplib95 built in) =",instance.trace_canonical_tour())
    print("\n")

    print("Instance ; algo ; long (C) ; long (Python) ; temps ; tour ; valid ; mêmes longueurs")

    for m in methods:
        extract_data(path,code,filename,m,distance_fct,coord,graphique) # l'exécutable C s'appelle tsp dans mon cas.

def tests_instances_list(instances_file,methods):
    # lance le programme C avec toutes les options de methods
    # et tous les fichiers d'instance de instance_file 
    with open(instances_file, 'r') as file:
        #for instance in file:
        #   test_instance(instance.strip(),methods)

        for line in file:
            line = line.strip()
            # ignore les lignes vides
            if not line:
                continue
            #extraire le nom du fichier avant :
            if ':' in line:
                instance_name = line.split(':')[0].strip()
            else:
                instance_name = line.strip()
            
            #ajouter extension .tsp
            if not instance_name.endswith('.tsp'):
                instance_name += '.tsp'
            
            #chemin complet
            instance_path = f"../jeu_de_donnees/{instance_name}"
            #erreur au début c'était psk j'avais laissé avec instance_name dcp il trouvait pas
            test_instance(instance_path,methods)

# Liste de paramètres.
# opt = tsplib95.load_solution(filename+'.opt.tour') pour charger un fichier solution dans python
#filename = "burma14.tsp"
filename = "../jeu_de_donnees/att15.tsp" #
path = "../C/" # adaptez à votre cas
code = "main" # nom du programe C partie 1
methods = ["-gadpx 30 1000 0.10f"]#["ga 30 1000 0.10"]#["-c"],"nn","rw","2optnn","2optrw","ga 10000 0.10 100"]#"bf","gadpx"

test_instance(filename,methods) # pour un seul appel
#tests_instances_list("../jeu_de_donnees/instances.txt",methods) # marche si le C marche
