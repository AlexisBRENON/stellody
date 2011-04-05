CC =			gcc

CFLAGS = 		-Wall -pedantic -ansi -g
LDFLAGS =		-export-dynamic

GTK_CFLAGS = 	`pkg-config --cflags gtk+-2.0`
GTK_LDFLAGS = 	`pkg-config --libs gtk+-2.0`		

SRC_DIR = 		src
OBJ_DIR = 		obj
BIN_DIR = 		bin
EXEC = 			Stellody
SRCS = 			stellody.c analyzed_tracks.c
OBJS = 			$(SRCS:%.c=$(OBJ_DIR)/%.o)

all : $(EXEC) clean

$(EXEC) : $(OBJS)
	@echo ""
	$(CC) $(LDFLAGS) $(GTK_LDFLAGS) $^ -o $(BIN_DIR)/$@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(GTK_CFLAGS) $< -o $@

.PHONY: clean mrproper help

clean :
	@rm -fv $(OBJ_DIR)/*.o

mrproper :
	@rm -fv $(OBJ_DIR)/*.o
	@rm -fv $(BIN_DIR)/$(EXEC)

help :
	@echo
	@echo "                    <-=  AIDE  =->"
	@echo
	@echo "Voici les cibles que vous pouvez appeler :"
	@echo "    - make [all]      Construit l'exécutable et détruit les fichiers objets"
	@echo "    - make Stellody   Construit l'exécutable"
	@echo "    - make clean      Détruit les fichiers objets"
	@echo "    - make mrproper   Détruit tout ce qui peut être reconstruit"
	@echo "    - make help       Affiche cette page d'aide"
	@echo

