
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
	# Fichiers nescessaires à GTK+
GTK_CFLAGS = 	`pkg-config --cflags gtk+-2.0`
GTK_LDFLAGS = 	`pkg-config --libs gtk+-2.0`
	# Fichiers nescessaires à OpenGL (pour Ubuntu)
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

	# Options nescessaires à la compilation sous linux
LIN_CFLAGS = 		-Wall -pedantic -ansi -g 	$(GTK_CFLAGS)	$(GL_CFLAGS)
LIN_LDFLAGS =		-export-dynamic -lglut 		$(GTK_LDFLAGS)	$(GL_LDFLAGS)
	# Options nescessaires à la compilation sous MAC
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
	
	
$(EXEC) : $(OBJS)
	@echo ""
	$(CC) $(LDFLAGS) $(GTK_LDFLAGS) $(GL_LDFLAGS) $^ -o $(BIN_DIR)/$@
		
mac : $(OBJS_MAC)
	@echo
	@echo "Ci-dessus : erreurs compilateur (syntaxe)"
	@echo "Ci_dessous : erreurs linker"
	@echo
	@$(CC) $(MAC_LDFLAGS) $^ -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin de compilation"
	@echo

lin : $(OBJS_LIN)
	@echo
	@echo "Ci-dessus : erreurs compilateur (syntaxe)"
	@echo "Ci_dessous : erreurs linker"
	@echo
	@$(CC) $(LIN_LDFLAGS) $^ -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin de compilation"
	@echo

$(OBJ_DIR)/%_mac.o : $(SRC_DIR)/%.c
	@echo " ."
	@$(CC) -c $(MAC_CFLAGS) $< -o $@

$(OBJ_DIR)/%_lin.o : $(SRC_DIR)/%.c
	@echo " ."
	@$(CC) -c $(LIN_CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) -c $(CFLAGS) $(GTK_CFLAGS) $(GL_CFLAGS) $< -o $@

clean :
	@echo
	@rm -fv $(OBJ_DIR)/*.o $(OBJ_DIR)/Cube*
	@echo

mrproper :
	@echo
	@rm -fv $(OBJ_DIR)/*.o
	@rm -fv $(BIN_DIR)/$(EXEC)
	@echo
	
