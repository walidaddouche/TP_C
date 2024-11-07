# Nom de l'exécutable
TARGET = fsh

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline

# Répertoires pour les fichiers sources
SRC_DIR = src
CMD_DIR = $(SRC_DIR)/Commandes_internes

# Fichiers sources
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/prompt.c $(CMD_DIR)/exit.c $(CMD_DIR)/pwd.c $(CMD_DIR)/ftype.c $(CMD_DIR)/cd.c

# Fichiers objets (générés dans le répertoire src ou Commandes_internes)
OBJS = $(SRCS:.c=.o)

# Répertoires où les objets seront générés
OBJ_DIR = obj

# Règle par défaut pour construire l'exécutable
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Création du répertoire obj s'il n'existe pas
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# Compilation des fichiers objets pour src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers objets pour Commandes_internes/
$(OBJ_DIR)/%.o: $(CMD_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJ_DIR)

# Exécuter le shell
run: $(TARGET)
	./$(TARGET)

# Phony targets pour éviter les conflits avec des fichiers
.PHONY: all clean run
