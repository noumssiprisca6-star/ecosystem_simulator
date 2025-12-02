#include "../../include/Core/Entity.h" 
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cmath>
namespace Ecosystem {
    namespace Core {
        // CONSTRUCTEUR PRINCIPAL
        Entity::Entity(EntityType type, Vector2D pos, std::string entityName)
        : mType(type), position(pos), name(entityName), 
        mRandomGenerator(std::random_device{}()) // Initialisation du générateur alé
        {
            // 🔧 INITIALISATION SELON LE TYPE
            switch(mType) {
                case EntityType::HERBIVORE:
                    mEnergy = 80.0f;
                    mMaxEnergy = 150.0f;
                    mMaxAge = 200;
                    maxspeed=1.5f;
                    color = Color::Blue();
                    size = 8.0f;
                break;
                
                case EntityType::CARNIVORE:
                    mEnergy = 100.0f;
                    mMaxEnergy = 200.0f;
                    mMaxAge = 150;
                    maxspeed =2.0f;
                    color = Color::Red();
                    size = 12.0f;
                break;
                
                case EntityType::PLANT:
                    mEnergy = 50.0f;
                    mMaxEnergy = 100.0f;
                    mMaxAge = 300;
                    maxspeed=0.0f;
                    color = Color::Green();
                    size = 6.0f;
                break;
            }
            const float dangerDist = 30.0f;
            mAge = 0;
            mIsAlive = true;
            mVelocity = GenerateRandomDirection();
            
            std::cout << "🌱 Entité créée: " << name << " à (" << position.x << ", " << position.y << ")" << std::endl;
        }
        // CONSTRUCTEUR DE COPIE
        Entity::Entity(const Entity& other)
        : mType(other.mType), position(other.position), name(other.name + "_copy"),
        mEnergy(other.mEnergy * 0.7f), // Enfant a moins d'énergie
        mMaxEnergy(other.mMaxEnergy),
        mAge(0), // Nouvelle entité, âge remis à 0
        mMaxAge(other.mMaxAge),
        mIsAlive(true),
        mVelocity(other.mVelocity),
        color(other.color),
        size(other.size * 0.8f), // Enfant plus petit
        mRandomGenerator(std::random_device{}())
        {
            std::cout << "👶 Copie d'entité créée: " << name << std::endl;
            
        }
        
        // DESTRUCTEUR
        Entity::~Entity() {
            std::cout << "💀 Entité détruite: " << name << " (Âge: " << mAge << ")" << std::endl;
        }
        // ⚙️ MISE À JOUR PRINCIPALE
        void Entity::Update(float deltaTime) {
            if (!mIsAlive) return;
            
            // 🔄 PROCESSUS DE VIE
            ConsumeEnergy(deltaTime);
            Age(deltaTime);
            Move(deltaTime);
            CheckVitality();
        }
        // 🚶 MOUVEMENT
        void Entity::Move(float deltaTime) {
            if (mType == EntityType::PLANT) return; // Les plantes ne bougent pas
            
            // 🎲 Comportement aléatoire occasionnel
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            if (chance(mRandomGenerator) < 0.02f) {
                mVelocity = GenerateRandomDirection();
            }

           position = StayInBounds(1200.0f, 600.0f);
            // 📐 Application du mouvement
            position = position + mVelocity * deltaTime * 20.0f;
            
            // 🔄 Consommation d'énergie due au mouvement
            mEnergy -= mVelocity.Distance(Vector2D(0, 0)) * deltaTime * 0.1f;
            
        }
        // MANGER
        void Entity::Eat(float energy) {
            mEnergy += energy;
            if (mEnergy > mMaxEnergy) {
                mEnergy = mMaxEnergy;
            }
            std::cout << " " << name << " mange et gagne " << energy << " énergie" << std::endl;
        }
        // 🔄 CONSOMMATION D'ÉNERGIE
        void Entity::ConsumeEnergy(float deltaTime) {
            float baseConsumption = 0.0f;
            
            switch(mType) {
            case EntityType::HERBIVORE:
            baseConsumption = 1.5f;
            break;
            case EntityType::CARNIVORE:
            baseConsumption = 2.0f;
            break;
            case EntityType::PLANT:
            baseConsumption = -0.5f; // Les plantes génèrent de l'énergie !
            break;
            }
            
            mEnergy -= baseConsumption * deltaTime;
        }
        // 🎂 VIEILLISSEMENT
        void Entity::Age(float deltaTime) {
            mAge += static_cast<int>(deltaTime * 10.0f); // Accéléré pour la simulation
        }
        // ❤️ VÉRIFICATION DE LA SANTÉ
        void Entity::CheckVitality() {
            if (mEnergy <= 0.0f || mAge >= mMaxAge) {
            mIsAlive = false;
            std::cout << "💀 " << name << " meurt - ";
            if (mEnergy <= 0) std::cout << "Faim";
            else std::cout << "Vieillesse";
            std::cout << std::endl;
            }
        }
        // 👶 REPRODUCTION
        bool Entity::CanReproduce() const {
            return mIsAlive && mEnergy > mMaxEnergy * 0.8f && mAge > 20;
        }
        std::unique_ptr<Entity> Entity::Reproduce() {
            if (!CanReproduce()) return nullptr;
            
            // 🎲 Chance de reproduction
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            if (chance(mRandomGenerator) < 0.3f) {
                mEnergy *= 0.6f; // Coût énergétique de la reproduction
                return std::make_unique<Entity>(*this); // Utilise le constructeur de copie
            }
            
            return nullptr;
        }
        // 🎲 GÉNÉRATION DE DIRECTION ALÉATOIRE
        Vector2D Entity::GenerateRandomDirection() {
            std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
            return Vector2D(dist(mRandomGenerator), dist(mRandomGenerator));
        }
        // 🎨 CALCUL DE LA COULEUR BASÉE SUR L'ÉTAT
        Color Entity::CalculateColorBasedOnState() const {
            float energyRatio = GetEnergyPercentage();
            
            Color baseColor = color;
            
            // 🔴 Rouge si faible énergie
            if (energyRatio < 0.3f) {
                baseColor.r = 255;
                baseColor.g = static_cast<uint8_t>(baseColor.g * energyRatio);
                baseColor.b = static_cast<uint8_t>(baseColor.b * energyRatio);
            }
            
            return baseColor;
        }
        // 🎨 RENDU GRAPHIQUE
        void Entity::Render(SDL_Renderer* renderer) const {
            if (!mIsAlive) return;
            
            Color renderColor = CalculateColorBasedOnState();
            
            SDL_FRect rect = {
                position.x - size / 2.0f,
                position.y - size / 2.0f,
                size,
                size
            };
            
            SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
            SDL_RenderFillRect(renderer, &rect);
            
            // 🔵 Indicateur d'énergie (barre de vie)
            if (mType != EntityType::PLANT) {
            float energyBarWidth = size * GetEnergyPercentage();
            SDL_FRect energyBar = {
                position.x - size / 2.0f,
                position.y - size / 2.0f - 3.0f,
                energyBarWidth,
                2.0f
            };
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &energyBar);
            }
        }
        //implementation de la fonction StayInBounds
       Vector2D Entity::StayInBounds(float worldWidth, float worldHeight) const {
        Vector2D pos= position ;
        //distance avant de corriger la trajectoire 
          // pour les herbivores et les carnivores , les plantes sont statiques 
            if(pos.x < 8.0f){
            pos.x = 8.0f;
            }
            if (pos.x > worldWidth - 8.0f ){ 
            pos.x = worldWidth - 8.0f;
        // je cherche à ramener l'entité dans le cadre  chaque fois qu'il essaira de s'en eloigner ils doivent rester dans l'enclos
            }
            if(pos.y < 8.0f){
                pos.y = 8.0f;
            }
            if(pos.y >worldHeight - 8.0f){
                pos.y = worldHeight - 8.0f;
            }
            

            
              return pos ;
       }
       //implementation de la fonction SeeKFood 
     Vector2D Entity::SeekFood(const std::vector<Food>& foodSources) const {
        Vector2D herbipos = position; // initialisation d'une position temporaire
        float distMin = 6000.0f;
        float distance;
        float energyh;
        Vector2D foodPos; //stocke la position de la nourriture la plus proche
        for(auto& food : foodSources) {
            distance = herbipos.Distance(food.position);
            if(distMin > distance && distance < 300.0f) {
                distMin = distance ;
                foodPos = food.position;
            }   
        }
        herbipos.x = foodPos.x - herbipos.x;
        herbipos.y = foodPos.y - herbipos.y;
      float length = sqrt(herbipos.x*herbipos.x + herbipos.y*herbipos.y); 
      //normalisation du vecteur de la position des herbivores
       herbipos.x /= length;
       herbipos.y /= length;
        return herbipos;
     }

     Vector2D Entity::SeekFood(const std::vector<std::unique_ptr<Entity>>& EntityFood) const {
        Vector2D predatpos = position ;
        float distance;
        float distMinim = 7000.0f;
        Vector2D herbipos ;
        for(auto& foodp : EntityFood){
            if(foodp->GetType() == EntityType::HERBIVORE){
                distance = predatpos.Distance(foodp->position);
             if(distMinim > distance && distance < 450.0f  ){
                    distMinim = distance ;
                    herbipos = foodp->position;
                }

            }
            
        }
        //normalisation du vecteurs 
        predatpos.x = herbipos.x - predatpos.x ;
        predatpos.y =  herbipos.y -  predatpos.y ;
        float length = sqrt(predatpos.x*predatpos.x  +  predatpos.y*predatpos.y);
        predatpos.x /= length ;
        predatpos.y /= length;
         return predatpos;
      }

     void Entity::ApplyForce(Vector2D force) {
        mVelocity = mVelocity + force ;
        //normalisation du vecteur 
        float length = sqrt(mVelocity.x*mVelocity.x + mVelocity.y*mVelocity.y);
        mVelocity.x /=length;
        mVelocity.y /=length;
     }
    
     void Entity::Skill(){
        mEnergy = 0.0f ;
        mAge = mMaxAge + 10;
     }
} // namespace Core
} // namespace Ecosystem

