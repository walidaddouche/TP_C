# Nom de l'exécutable
TARGET = fsh

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline

# Fichiers sources et objets
SRCS = main.c prompt.c
OBJS = $(SRCS:.c=.o)

# Règle par défaut pour construire l'exécutable
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)

# Exécuter le shell
run: $(TARGET)
	./$(TARGET)

# Phony targets pour éviter les conflits avec des fichiers
.PHONY: all clean run

