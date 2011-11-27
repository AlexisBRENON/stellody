
# ############################################################################ #
#                                  MAKEFILE                                    #
#                                   projet                                     #
#                                  STELLODY                                    #
# ############################################################################ #


# Définition des cibles particulières
.PHONY: flush clean mrproper help mac lin


# Définition de variables :
	# Compilateur
CC =			gcc
	# Fichiers nécessaires à  GTK+
GTK_CFLAGS = 	`pkg-config --cflags gtk+-2.0`
GTK_LDFLAGS = 	`pkg-config --libs gtk+-2.0`
	# Fichiers nécessaires à  OpenGL (pour Ubuntu)
GL_CFLAGS = 	`pkg-config --cflags gl glu`
GL_LDFLAGS = 	`pkg-config --libs gl glu`
	# Fichiers nécessaires à  GtkGLExt
GTKGL_CFLAGS =	`pkg-config --cflags gtkglext-x11-1.0`
GTKGL_LDFLAGS =	`pkg-config --libs gtkglext-x11-1.0`

	# Dossier contenant les sources
SRC_DIR = 		src
	# Dossier où créer les fichiers objets
OBJ_DIR = 		obj
	# Dossier où créer l'exécutable
BIN_DIR = 		bin
	# Nom de l'exécutable
EXEC = 			stellody
	# Noms des fichiers sources
SRCS = 			files.c \
			analyzed_track.c \
			preferences.c \
			analyzed_tracks.c \
			player.c \
			gui.c \
			analysis.c \
			image.c \
			star.c \
			stellarium.c \
			opengl_drawing.c \
			dynamic_array.c \
			linked_list.c \
			gui_data.c \
			player_data.c \
			stellody.c 

	# Noms des fichiers objets (génération dynamique)
OBJS_LIN =		$(SRCS:%.c=$(OBJ_DIR)/%_lin.o)
OBJS_MAC = 		$(SRCS:%.c=$(OBJ_DIR)/%_mac.o)

	# Options nécessaires à  la compilation sous linux
LIN_CFLAGS = 		-Wall -pedantic -ansi -g  \
					$(GTK_CFLAGS)	$(GL_CFLAGS)	$(GTKGL_CFLAGS)
LIN_LDFLAGS =		-L/usr/lib64 -export-dynamic \
					-lfmodex64-4.36.05 -lm\
					$(GTKGL_LDFLAGS) \
					$(GTK_LDFLAGS)	$(GL_LDFLAGS)

	# Options nécessaires à  la compilation sous MAC
MAC_CFLAGS = 		-Wall -pedantic -ansi -m32 -g \
				-I/Developer/sw/include/gtkglext-1.0 \
				-I/Developer/sw/lib/gtkglext-1.0/include \
				-I/Developer/FMOD\ Programmers\ API\ Mac/api/inc \
				$(GTK_CFLAGS) $(GL_CFLAGS)
MAC_LDFLAGS =		-m32 -lfmodex -lgtkglext-x11-1.0.0 -lgdkglext-x11-1.0.0 \
				-framework OpenGL -framework Cocoa \
				-L/Developer/FMOD\ Programmers\ API\ Mac/api/lib \
				-L/Developer/sw/ \
				-L/Developer/sw/lib \
				$(GTK_LDFLAGS) 	$(GL_LDFLAGS)
				
# Fin de définition de variables


# ############################################################################ #
# ############################################################################ #


# Début de l'énumération des cibles


help :
	@echo
	@echo "                    <-=  AIDE  =->"
	@echo
	@echo "Voici les cibles que vous pouvez appeler :"
	@echo "    - make \033[1mmac\033[0m        Construit l'exécutable pour MAC OS X"
	@echo "    - make \033[1mlin\033[0m        Construit l'exécutable pour Ubuntu"
	@echo "    - make \033[1mclean\033[0m      Détruit les fichiers objets"
	@echo "    - make \033[1mflush\033[0m      Vide les fichiers de données et de configuration"
	@echo "    - make \033[1mmrproper\033[0m   Détruit tout ce qui peut être reconstruit"
	@echo "    - make \033[1m[help]\033[0m     Affiche cette page d'aide"
	@echo
		
check :
	@echo
	@echo "\033[1;33m  Vérification de l'arborescence\n"
	@if [ ! -e bin ]; then mkdir bin; fi
	@if [ ! -e obj ]; then mkdir obj; fi
	@if [ ! -e data ]; then	echo "\033[1;31mDossiers de données inexistants...\033[0m"; exit 1;fi
	@if [ ! -e data/.stellody_data ]; then echo "# Fichier de données" >data/.stellody_data; fi
	@if [ ! -e data/.stellody_config ]; then echo "# Fichier de configuration" >data/.stellody_config; fi
	@echo "\033[1;32m======================================="
	@echo "\033[1;33m"
	@echo "Début des messages compilateur (syntaxe)."
	@echo
	
	
		
mac : check $(OBJS_MAC)
	@echo "\033[1;32m"
	@echo "Fin des messages compilateur (syntaxe)."
	@echo
	@echo "======================================="
	@echo "\033[1;33m"
	@echo "Début des messages linker."
	@echo "\033[1;31m"
	@$(CC) $(MAC_LDFLAGS) $(OBJS_MAC) -o $(BIN_DIR)/$(EXEC)
	@echo "\033[1;32m"
	@echo "Fin des messages linker."
	@echo
	@echo "======================================="
	@echo
	@echo "Stellody construit avec succès !"
	@echo "\033[0m"
	
lin : check $(OBJS_LIN)
	@echo "\033[1;32m"
	@echo "Fin des messages compilateur (syntaxe)."
	@echo
	@echo "======================================="
	@echo "\033[1;33m"
	@echo "Début des messages linker."
	@echo "\033[1;31m"
	@$(CC) $(OBJS_LIN) $(LIN_LDFLAGS) -o $(BIN_DIR)/$(EXEC)
	@echo "\033[1;32m"
	@echo "Fin des messages linker."
	@echo
	@echo "======================================="
	@echo 
	@echo "Stellody construit avec succès !"
	@echo "\033[0m"

$(OBJ_DIR)/%_mac.o : $(SRC_DIR)/%.c
	@echo "\033[1;33m  $<\n\033[1;31m"
	@$(CC) -c $(MAC_CFLAGS) $< -o $@
	@echo "\033[1;32m------------------------"

$(OBJ_DIR)/%_lin.o : $(SRC_DIR)/%.c
	@echo "\033[1;33m  $<\n\033[1;31m"
	@$(CC) -c $(LIN_CFLAGS) $< -o $@
	@echo "\033[1;32m------------------------"

clean :
	@echo "\033[1;33m"
	@rm -fv $(OBJ_DIR)/*.o
	@echo "\033[0m"
	
flush :
	@echo "\033[1;33mVidage des fichiers...\n"
	@echo "# Fichier de données de l'application Stellody" >data/.stellody_data
	@echo "# Fichier de configuration\n\n[DEFAULT]\niAnalysisRate=1\niWindowXSize=0\niWindowYSize=0\ni3DQuality=25\niTIDMax=0\niTIDMin=0\niMoveCam=1\niDispGrid=1\npstrPath=\n" >data/.stellody_config
	@echo "=======================================\033[0m"

mrproper :
	@echo "\033[1;33m"
	@rm -fv $(OBJ_DIR)/*.o
	@rm -fv $(BIN_DIR)/$(EXEC)
	@echo "\033[0m"
	
