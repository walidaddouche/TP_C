# Nom de l'exécutable
TARGET = fsh

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline

# Répertoires pour les fichiers sources
SRC_DIR = src
CMD_DIR = $(SRC_DIR)/gestionCommande
CMD_SMP_DIR = $(CMD_DIR)/commandeSimple
CMD_STR_DIR = $(CMD_DIR)/commandeStructure
CMD_INT_DIR = $(CMD_SMP_DIR)/Commandes_internes

# Fichiers sources
SRCS = $(CMD_STR_DIR)/commande-for.c \
	   $(CMD_STR_DIR)/commande_consecutive.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/trait-ligne-commande.c \
       $(CMD_SMP_DIR)/cmd-externes.c \
       $(SRC_DIR)/prompt.c \
       $(CMD_INT_DIR)/exit.c \
       $(CMD_INT_DIR)/pwd.c \
       $(CMD_INT_DIR)/ftype.c \
       $(CMD_INT_DIR)/cd.c

# Fichiers objets (dans obj/)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Répertoire des fichiers objets
OBJ_DIR = obj

# Règle par défaut pour construire l'exécutable
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)  # Crée les sous-répertoires nécessaires dans obj/
	$(CC) $(CFLAGS) -c $< -o $@

# Création du répertoire obj
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Exécuter le shell
run: $(TARGET)
	./$(TARGET)

# Phony targets pour éviter les conflits avec des fichiers
.PHONY: all clean run
