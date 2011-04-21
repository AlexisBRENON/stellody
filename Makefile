
# ############################################################################ #
#                                  MAKEFILE                                    #
#                                   projet                                     #
#                                  STELLODY                                    #
# ############################################################################ #


# D�finition des cibles particuli�res
.PHONY: clean mrproper help mac lin


# D�finition de variables :
	# Compilateur
CC =			gcc
	# Fichiers n�cessaires à GTK+
GTK_CFLAGS = 	`pkg-config --cflags gtk+-2.0`
GTK_LDFLAGS = 	`pkg-config --libs gtk+-2.0`
	# Fichiers nescessaires à OpenGL (pour Ubuntu)
GL_CFLAGS = 	`pkg-config --cflags gl glu`
GL_LDFLAGS = 	`pkg-config --libs gl glu`

	# Dossier contenant les sources
SRC_DIR = 		src
	# Dossier o� cr�er les fichiers objets
OBJ_DIR = 		obj
	# Dossier o� cr�er l'ex�cutable
BIN_DIR = 		bin
	# Nom de l'ex�cutable
EXEC = 			stellody
	# Noms des fichiers sources
SRCS = 			stellody.c \
			analyzed_track.c \
			analyzed_tracks.c \
			preferences.c \
			opengl_drawing.c \
			files.c
	# Noms des fichiers objets (g�n�ration dynamique)
OBJS_LIN =		$(SRCS:%.c=$(OBJ_DIR)/%_lin.o)
OBJS_MAC = 		$(SRCS:%.c=$(OBJ_DIR)/%_mac.o)

	# Options n�cessaires à la compilation sous linux
LIN_CFLAGS = 		-Wall -pedantic -ansi -g 	$(GTK_CFLAGS)	$(GL_CFLAGS)
LIN_LDFLAGS =		-export-dynamic -lglut 		$(GTK_LDFLAGS)	$(GL_LDFLAGS)
	# Options n�cessaires à la compilation sous MAC
MAC_CFLAGS = 		-Wall -pedantic -ansi -m32 -g \
				-I/Developer/FMOD\ Programmers\ API\ Mac/api/inc \
				$(GTK_CFLAGS)
MAC_LDFLAGS =		-m32 -framework GLUT -framework OpenGL -framework Cocoa \
				$(GTK_LDFLAGS)
				
# Fin de d�finition de variables


# ############################################################################ #
# ############################################################################ #


# D�but de l'�num�ration des cibles


help :
	@echo
	@echo "                    <-=  AIDE  =->"
	@echo
	@echo "Voici les cibles que vous pouvez appeler :"
	@echo "    - make mac        Construit l'ex�cutable pour MAC OS X"
	@echo "    - make lin        Construit l'ex�cutable pour Ubuntu"
	@echo "    - make clean      D�truit les fichiers objets"
	@echo "    - make mrproper   D�truit tout ce qui peut �tre reconstruit"
	@echo "    - make [help]     Affiche cette page d'aide"
	@echo
	
	
$(EXEC) : $(OBJS)
	@echo ""
	$(CC) $(LDFLAGS) $(GTK_LDFLAGS) $(GL_LDFLAGS) $^ -o $(BIN_DIR)/$@
		
mac : $(OBJS_MAC)
	@echo
	@echo "Fin des erreurs compilateur (syntaxe)."
	@echo "D�but des erreurs linker."
	@echo
	@$(CC) $(MAC_LDFLAGS) $^ -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin de compilation."
	@echo

lin : $(OBJS_LIN)
	@echo
	@echo "Fin des erreurs compilateur (syntaxe)."
	@echo "D�but des erreurs linker."
	@echo
	@$(CC) $(LIN_LDFLAGS) $^ -o $(BIN_DIR)/$(EXEC)
	@echo
	@echo "Fin de compilation."
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
	
