
# ############################################################################ #
#                                  MAKEFILE                                    #
#                                   projet                                     #
#                                  STELLODY                                    #
# ############################################################################ #


# Définition des cibles particulières
.PHONY: clean mrproper help mac lin


# Définition de variables :
	# Compilateur
CC =			gcc
	# Fichiers nécessaires à  GTK+
GTK_CFLAGS = 	`pkg-config --cflags gtk+-2.0`
GTK_LDFLAGS = 	`pkg-config --libs gtk+-2.0`
	# Fichiers nécessaires à  OpenGL (pour Ubuntu)
GL_CFLAGS = 	`pkg-config --cflags gl glu`
GL_LDFLAGS = 	`pkg-config --libs gl glu`

	# Dossier contenant les sources
SRC_DIR = 		src
	# Dossier où créer les fichiers objets
OBJ_DIR = 		obj
	# Dossier où créer l'exécutable
BIN_DIR = 		bin
	# Nom de l'exécutable
EXEC = 			stellody
	# Noms des fichiers sources
SRCS = 			stellody.c \
			analyzed_track.c \
			analyzed_tracks.c \
			preferences.c \
			opengl_drawing.c \
			files.c
	# Noms des fichiers objets (génération dynamique)
OBJS_LIN =		$(SRCS:%.c=$(OBJ_DIR)/%_lin.o)
OBJS_MAC = 		$(SRCS:%.c=$(OBJ_DIR)/%_mac.o)

	# Options nécessaires à  la compilation sous linux
LIN_CFLAGS = 		-Wall -pedantic -ansi -g 	$(GTK_CFLAGS)	$(GL_CFLAGS)
LIN_LDFLAGS =		-export-dynamic -lglut 		$(GTK_LDFLAGS)	$(GL_LDFLAGS)
	# Options nécessaires à  la compilation sous MAC
MAC_CFLAGS = 		-Wall -pedantic -ansi -m32 -g \
				-I/Developer/FMOD\ Programmers\ API\ Mac/api/inc \
				$(GTK_CFLAGS)
MAC_LDFLAGS =		-m32 -framework GLUT -framework OpenGL -framework Cocoa \
				$(GTK_LDFLAGS)
				
# Fin de définition de variables


# ############################################################################ #
# ############################################################################ #


# Début de l'énumération des cibles


help :
	@echo
	@echo "                    <-=  AIDE  =->"
	@echo
	@echo "Voici les cibles que vous pouvez appeler :"
	@echo "    - make mac        Construit l'exécutable pour MAC OS X"
	@echo "    - make lin        Construit l'exécutable pour Ubuntu"
	@echo "    - make clean      Détruit les fichiers objets"
	@echo "    - make mrproper   Détruit tout ce qui peut être reconstruit"
	@echo "    - make [help]     Affiche cette page d'aide"
	@echo
		
check :
	@echo
	@if [ ! -e bin ]; then mkdir bin; fi
	@if [ ! -e obj ]; then mkdir obj; fi
	@if [ ! -e data ]; then	echo "Dossiers de données inexistants..."; exit 1;fi
	@if [ ! -e .stellody_data ]; then echo "# Fichier de données" >.stellody_data; fi
	@if [ ! -e .stellody_config ]; then echo "# Fichier de configuration" >.stellody_data; fi
	
		
mac : check $(OBJS_MAC)
	@echo
	@echo "Fin des messages compilateur (syntaxe)."
	@echo
	@echo "======================================="
	@echo
	@echo "Début des messages linker."
	@echo
	@$(CC) $(MAC_LDFLAGS) $(OBJS_MAC) -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin des messages linker."
	@echo
	@echo "======================================="
	@echo
	@echo "Stellody construit avec sucès !"
	@echo
	
lin : check $(OBJS_LIN)
	@echo
	@echo "Fin des messages compilateur (syntaxe)."
	@echo
	@echo "======================================="
	@echo
	@echo "Début des messages linker."
	@echo
	@$(CC) $(LIN_LDFLAGS) $(OBJS_LIN) -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin des messages linker."
	@echo
	@echo "======================================="
	@echo
	@echo "Stellody construit avec sucès !"
	@echo

$(OBJ_DIR)/%_mac.o : $(SRC_DIR)/%.c
	@echo "  $<\n"
	@$(CC) -c $(MAC_CFLAGS) $< -o $@
	@echo "------------------------"

$(OBJ_DIR)/%_lin.o : $(SRC_DIR)/%.c
	@echo "  $<\n"
	@$(CC) -c $(LIN_CFLAGS) $< -o $@
	@echo "------------------------"

clean :
	@echo
	@rm -fv $(OBJ_DIR)/*.o
	@echo

mrproper :
	@echo
	@rm -fv $(OBJ_DIR)/*.o
	@rm -fv $(BIN_DIR)/$(EXEC)
	@echo
	
