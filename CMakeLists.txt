cmake_minimum_required(VERSION 3.10)

#nom du projet
project(projet_labyrinthe)

#standard de c++ utilisé
set(CMAKE_CXX_STANDARD 17)

#spécifie le chemin d'accès 
set(SFML_DIR "/usr/lib/cmake/SFML")

# Trouve SFML et ses composants dans l'ordi
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Ajoute l'exécutable "projet_labyrinthe" en compilant le src/main.cpp
add_executable(projet_labyrinthe src/main.cpp src/etats_jeu.cpp)


# Lie l'executable aux bibliothèques nécessaires
target_link_libraries(projet_labyrinthe PRIVATE sfml-graphics sfml-window sfml-system)