#  VIRTUEL ECOSYSTEM SIMULATOR



## DESCRIPTION  
un simulateur d'écosysteme virtuel ou des entités se déplacent , cherchent de la nourriture , se reproduisent et fuient les predateurs et interagissent entres elles 



## 🌹 Fonctionalités 

-Deplacemeent autonome des entités 
-Systeme de nourriture et de prédateurs 
-Physique simple ( forces , vitesse , limites )
-Interface graphique avec SDL3



## Restrictions 

-il m'a fallu modifier les chemins d'acces dans chaque fichier afin de pouvoir observer l'affichage de la fenetre de l'ecosysteme apres la compilation reussit


## Installation 

1.creer un document qui recevra le clonage de mon projet et mentionner son chemin d'acces dans l'invite de commande 

2.Cloner le projet :

'''bash
git clone le nom de mon repository  et juste avant github le nom de mon  tokens 
et ensuite le clonage est lancer . 
pour la compilation c'est g++ -std=c++17 -Include -o ecosystem src/*.cpp src/Core/*.cpp src/Graphics/*.cpp -o ecosysteme.exe -lSDL3 



## 🎇Utilisation 
lancez : ./ECOSYSTEM
Pour l'execution 


## 📒Architecture du projet
� Structure des Fichiers
ecosystem_simulator/
├── include/
│ ├── Core/
│ │ ├── Structs. hpp
│ │ ├── Entity. hpp
│ │ └── Ecosystem. hpp
│ └── Graphics/
│ ├── Window. hpp
│ └── Renderer. hpp
├── src/
│ ├── Core/
│ │ ├── Entity. cpp
│ │ └── Ecosystem. cpp
│ ├── Graphics/
│ │ ├── Window. cpp
│ │ └── Renderer. cpp
│ └── main. cpp
├── assets/
│ └── ( futures textures)
└── README. md

un apercu de quelques  lignes de code du fichier entity.h

## implementations importantes

            
            // 🎯 MÉTHODES DE COMPORTEMENT
            Vector2D SeekFood(const std::vector<Food>& foodSources) const;
            Vector2D AvoidPredators(const std::vector<Entity>& predators) const;
            Vector2D StayInBounds(float worldWidth, float worldHeight) const;
            
        s
  
  les fonctions  à implémenter dans entity.cpp 
  - Vector2D SeekFodd () const 
  -Vector2D AvoidPredators()const
  -Vector2D StayInBouds() const
  -Vector2D  apllyForce() 

  les fonctions à implementer dans Ecosystem .h
  - Void addentity()
  -void addFood()

 ## 🎀Explication


🧠 1. seekFood() — Chercher la nourriture

📌 But :

Calculer la direction dans laquelle l’entité doit se déplacer pour aller vers la nourriture la plus proche.

📘 Ce que fait la fonction :

Parcourt toutes les sources de nourriture

Trouve la plus proche

Calcule un vecteur directionnel :
direction = position_nourriture - position_entité

Retourne ce vecteur (à normaliser selon ton code)


🎯 Pourquoi c’est utile :

Sans cette fonction, l’entité ne saurait pas où aller.
C’est son comportement “chercher de la nourriture”.


---

🧱 2. stayInBounds() — Rester dans les limites

📌 But :

Empêcher les entités de sortir de l’écran / fenêtre.

📘 Ce que fait la fonction :

Vérifie si l’entité approche des bords (x < 0, x > largeur, y < 0, y > hauteur)

Si oui, elle renvoie une force opposée pour la ramener à l’intérieur


🎯 Pourquoi c’est utile :

Sans ça, tes entités sortiraient de la fenêtre et disparaîtraient.


---

🦊 3. avoidPredators() — Éviter les prédateurs

📌 But :

Faire fuir l’entité lorsqu’un prédateur s’approche.

📘 Ce que fait la fonction :

Parcourt les prédateurs

Trouve s’ils sont trop proches

Calcule une force de fuite :
fuite = position_entité - position_predateur

Retourne un vecteur de “répulsion”


🎯 Pourquoi c’est utile :

Permet aux entités d'avoir un comportement biologique crédible : se sauver.


---

➕ 4. addEntity() — Ajouter une entité dans l’écosystème

📌 But :

Ajouter dynamiquement une nouvelle entité dans la simulation.

📘 Ce que fait la fonction :

Crée une nouvelle entité

L’ajoute dans le std::vector<Entity> de l’écosystème

Peut lui donner une position aléatoire ou définie


🎯 Pourquoi c’est utile :

Pour générer plusieurs entités au début

Pour ajouter de nouveaux agents pendant la simulation

Pour garder un système flexible




⚙ 5. applyForce() — Appliquer une force à une entité

📌 But :

Modifier son mouvement en ajoutant une force à son accélération.

📘 Ce que fait la fonction :

Reçoit un vecteur force

Ajoute cette force à l’accélération de l’entité :
acceleration += force

La vitesse et la position seront mises à jour ensuite

 ## ✨🎊 apercu graphique
 ![alt text](image-1.png)


 - la fonction main.cpp
  '''cpp
#include "../include/Core/GameEngine.h" 
#include "../include/Graphics/Window.h"
#include "../include/Core/Ecosystem.h" 
#include "../include/Core/Structs.h"   
#include <iostream>
#include <SDL3/SDL.h>
#include <iostream> 
#include <cstdlib> 
#include <ctime> 
#include <windows.h>
#include<iostream>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8); 
    // Initialisation de l'aléatoire 
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
     
    std::cout << "🎮Démarrage du Simulateur d'Écosystème" << std::endl; 
    std::cout << "=======================================" << std::endl; 
     
    // 🏗 Création du moteur de jeu 
    Ecosystem::Core::GameEngine engine("Simulateur d'Écosystème Intelligent", 1200.0f, 600.0f);
     
    // Initialisation 
    if (!engine.Initialize()) { 
        std::cerr << "❌Erreur: Impossible d'initialiser le moteur de jeu" << std::endl;
        return -1; 
    }
     
    std::cout << "✅Moteur initialisé avec succès" << std::endl; 
    std::cout << "🎯Lancement de la simulation..." << std::endl; 
    std::cout << "=== CONTRÔLES ===" << std::endl; 
    std::cout << "ESPACE: Pause/Reprise" << std::endl; 
    std::cout << "R: Reset simulation" << std::endl; 
    std::cout << "F: Ajouter nourriture" << std::endl; 
    std::cout << "FLÈCHES: Vitesse simulation" << std::endl; 
    std::cout << "ÉCHAP: Quitter" << std::endl; 
     
    // Boucle principale 
    engine.Run(); 
     
    // Arrêt propre 
    engine.Shutdown(); 
     
    std::cout << "👋Simulation terminée. Au revoir !" << std::endl; 
    return 0;
    }

 
 # REMARQUES 
- Cette exercice est un excellent moyen de rassembler les notions abordées jusqu'ici concernant les fonctions , les procedures et mettre en exergue le fameux SDL3 ❤❤
-  l'execution de ces fichiers en passant par le main.cpp est extremement beau . MERCI pour cet exercice etant un moyen de travail intellectuel et de divertissement visuel   

     ## REALISATEUR :
     -NOM : NOUMMSI TIATSAP 
     -PRENOM : VANESSA  PRISCA
     -FILIERE : ART NUMERIQUE ING
           - PROPOSE PAR : M. TEGUIA 
           #   🌹❤ GAME PROGRAMMING 
           -MERCI POUR VOTRE ATTENTION , A UNE PROCHAINE POUR UNE AVENTURE NOUVELLE 🎊✨



