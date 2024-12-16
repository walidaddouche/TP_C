# Étape 1 : Choisir une image de base avec un compilateur C
FROM gcc:latest

# Étape 2 : Définir un répertoire de travail
WORKDIR /app

# Étape 3 : Copier les fichiers du projet dans le conteneur
COPY . /app

# Étape 4 : Compiler le code
RUN gcc -o app main.c

# Étape 5 : Spécifier la commande par défaut pour exécuter l'application
CMD ["./app"]
