################################################################################
#                                              _            ____               #
#                                             | \          |    \              #
#                                             |  \         |___  \             #
#                                             |   )        |   \ |             #
#        ______                               |  /         |    \|             #
#       /      \  |              |    |       | /          |     |             #
#      /        \ |              |    |       |/           |     |             #
#      \          |___           |    |       /            |                   #
#       \______   |        ____  |    |      /|__     _____|  \        /       #
#              \  |       /    \ |    |     //|  \   /     |   \      /        #
#               \ |      |_____/ |    |    /| |   \ |      |    \    /         #
#      \        / \      \       |    |    \  |   / |      |     \  /          #
#       \______/   \___/  \____/ |__/ |__/  \_|__/   \_____/      \/           #
#                                             |                   /            #
#                                             |                  /             #
#                                             |                 /              #
#                                          /  |             /  /               #
#                                          \__/             \_/                #
#                                                                              #
################################################################################

********************************************************************************
********************************************************************************
  **                                                                        **  
  **                               README.TXT                               **
  **                                                                        **  
********************************************************************************
********************************************************************************

                             <-------------->
                             <-- Sommaire -->
                             <--    --    -->
                             <-- Contents -->
                             <-------------->

Français
    - Description
    - Compilation
    - Exécution
    - Documentation
    - Contacts
    - Remerciements
    
English
    - Description
    - Compilation
    - Execution
    - Documentation
    - Contacts
    - Thanks
    
    
********************************************************************************
  **                                FRANCAIS                                **
********************************************************************************

~~ Description ~~

Stellody est un logiciel réalisé dans le cadre du projet de "LIF7 : Conception
et Réalisation d'une Application", une Unité d'Enseignement dispensée à l'Univ-
ersité Claude Bernard Lyon1.
Ce logiciel a pour but de vous faire voir la musique autrement. Chacun des 
morceaux de votre bibliothèque peut être analysé par notre (puissant :p) 
algorithme d'analyse pour être ajouté au Stellarium. Déambulez au travers de 
centaines d'étoiles tout en écoutant vos morceaux favoris.


~~ Compilation ~~

Pour compiler, vous avoir installé les librairies GTK, OpenGL, FModEx ainsi que
GtkGLExt.
Une fois toutes les librairies correctement installées :
    ~$ cd stellody
    ~/stellody$ make {lin|mac} (selon si vous voulez un exécutable pour ubuntu
ou pour MacOSX).

Il se peut qu'un 'Warning' apparaisse à la compilation sur le fichier
gdkgltokens.h. Cet avertissement (indépendant de notre volonté) ne pose aucun
problème pour l'exécution.


~~ Execution ~~

Pour exécuter, une fois la compilation réussie sans erreur (c'est-à-dire que 
vous possédez tous les fichiers nescessaires et que l'exécutable a été
construit), il vous suffit de taper la commande suivante :
    ~/stellody$ bin/stellody

Si tous les fichier d'interfaces sont présents et que vous lancez le programme
depuis le dossier stellody, tout se charge normalement et l'application est dès
à présent fonctionnelle, il ne vous reste plus qu'à ajouter des morceaux via le
menu Fichier->Ajouter un morceau ou Fichier->Ajouter un répertoire.


~~ Documentation ~~

Un fichier de génération de documentation est fournis dans le dossier 
stellody/doc.
Après avoir installé doxygen, entrez la commande suivante :
    ~/stellody$ doxygen doc/doxyfile
    
Pour afficher la documentation résultante :
    ~/stellody$ {firefox|opera} doc/html/index.html


~~ Contacts ~~

Pour tout renseignements, encouragements, (demandes en mariage :p), merci de
contacter :
	Alexis BRENON : brenon.alexis@gmail.com
	Matthieu VIDAL : matthieu.vidal.69@gmail.com


~~ Remerciement ~~

Alexis :
J'aimerais remercier :
    - nos professeurs - Alexandre MEYER, Julien MILLE et Quentin COLOMBET -
pour leur aide et leur soutient tout au long du semestre;
    - mon coéquipié sans qui la représentation 3D n'aurait jamais été possible;
    - mes frères - Lucas et Basile - qui ont réalisé le logo de l'application;
    - ma chérie - Alison - pour m'avoir laisser programmer sans (trop) me
déconcentrer ;
    - et enfin, tous ceux qui ont servis de béta-testeurs afin de 
rendre cette application la plus intuitive.

Matthieu :
Je souhaiterais � mon tour remercier :
    - nos professeurs de LIF7 - Alexandre MEYER, Julien MILLE et Quentin COLOMBET - pour leurs connaissances, leus soutien et surtout leur patience !
    - Alexis sans qui tout ce projet n'aurait jamais pu voir le jour. D�s le d�but, il savait o� nous allions et commentons y allions alors que je me demandais ce que nous allions faire...
    - H�l�ne, ma ch�re et tendre, pour sa pr�sence quotidienne, sa patience, pour m'avoir laiss� programmer de longues heures et pour son soutien et ses r�ponses � mes questions tout au long du projet.


********************************************************************************
  **                                ENGLISH                                 **
********************************************************************************

TODO