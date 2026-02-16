#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <limits>

// ===== CLASSE POSITION =====
class Position {
public:
    double x, y;
    
    Position(double x = 0, double y = 0) : x(x), y(y) {}
    
    // Distance euclidienne vers une autre position
    double distanceTo(const Position& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    // Vecteur directionnel normalisé vers une autre position
    Position directionTo(const Position& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        double distance = distanceTo(other);
        
        if (distance < 0.001) {
            return Position(0, 0);
        }
        
        return Position(dx / distance, dy / distance);
    }
    
    // Crée une nouvelle position en se déplaçant vers une cible
    Position moveTowards(const Position& target, double speed) const {
        Position direction = directionTo(target);
        double newX = x + direction.x * speed;
        double newY = y + direction.y * speed;
        
        // Clamp dans les limites de la carte (0 à 10000)
        newX = std::max(0.0, std::min(10000.0, newX));
        newY = std::max(0.0, std::min(10000.0, newY));
        
        return Position(newX, newY);
    }
    
    // Opérateurs pour faciliter les calculs
    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }
    
    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }
    
    Position operator*(double scalar) const {
        return Position(x * scalar, y * scalar);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

// ===== CLASSE DE BASE ENTITY =====
class Entity {
protected:
    int id;
    Position position;
    bool alive;
    
public:
    Entity(int id, const Position& pos) 
        : id(id), position(pos), alive(true) {}
    
    virtual ~Entity() = default;
    
    int getId() const { return id; }
    Position getPosition() const { return position; }
    bool isAlive() const { return alive; }
    
    void setPosition(const Position& pos) { position = pos; }
    
    virtual void update(const std::vector<std::shared_ptr<Entity>>& entities) = 0;
    virtual std::string getType() const = 0;
};

// ===== CLASSE ZOMBIE =====
class Zombie : public Entity {
private:
    double speed;
    int damage;
    double attackRange;
    
public:
    Zombie(int id, const Position& pos, double speed = 30.0) 
        : Entity(id, pos), speed(speed), damage(20), attackRange(50.0) {}
    
    std::string getType() const override { return "Zombie"; }
    
    double getSpeed() const { return speed; }
    int getDamage() const { return damage; }
    double getAttackRange() const { return attackRange; }
    
    void update(const std::vector<std::shared_ptr<Entity>>& entities) override {
        if (!alive) return;
        
        // Trouve l'humain le plus proche
        std::shared_ptr<Entity> nearestHuman = findNearestHuman(entities);
        
        if (nearestHuman) {
            double distance = position.distanceTo(nearestHuman->getPosition());
            
            // Si assez proche pour attaquer
            if (distance <= attackRange) {
                attack(nearestHuman);
            } else {
                // Sinon, se déplace vers l'humain
                position = position.moveTowards(nearestHuman->getPosition(), speed);
            }
        }
    }
    
private:
    std::shared_ptr<Entity> findNearestHuman(const std::vector<std::shared_ptr<Entity>>& entities) {
        std::shared_ptr<Entity> nearest = nullptr;
        double minDistance = std::numeric_limits<double>::max();
        
        for (const auto& entity : entities) {
            if (!entity->isAlive() || entity->getType() != "Human") continue;
            
            double distance = position.distanceTo(entity->getPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearest = entity;
            }
        }
        
        return nearest;
    }
    
    void attack(std::shared_ptr<Entity> target);  // Déclaration forward
};

// ===== CLASSE HUMAN =====
class Human : public Entity {
private:
    int health;
    double speed;
    double detectionRange;
    
public:
    Human(int id, const Position& pos, int health = 100) 
        : Entity(id, pos), health(health), speed(50.0), detectionRange(2000.0) {}
    
    std::string getType() const override { return "Human"; }
    
    int getHealth() const { return health; }
    double getSpeed() const { return speed; }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            alive = false;
            std::cout << "Human #" << id << " est mort !" << std::endl;
        } else {
            std::cout << "Human #" << id << " prend " << damage 
                      << " dégâts (santé: " << health << ")" << std::endl;
        }
    }
    
    void update(const std::vector<std::shared_ptr<Entity>>& entities) override {
        if (!alive) return;
        
        // Trouve le zombie le plus proche
        std::shared_ptr<Entity> nearestZombie = findNearestZombie(entities);
        
        if (nearestZombie) {
            double distance = position.distanceTo(nearestZombie->getPosition());
            
            // Si le zombie est dans la portée de détection, fuir
            if (distance <= detectionRange) {
                fleeFrom(nearestZombie->getPosition());
            }
        }
    }
    
private:
    std::shared_ptr<Entity> findNearestZombie(const std::vector<std::shared_ptr<Entity>>& entities) {
        std::shared_ptr<Entity> nearest = nullptr;
        double minDistance = std::numeric_limits<double>::max();
        
        for (const auto& entity : entities) {
            if (!entity->isAlive() || entity->getType() != "Zombie") continue;
            
            double distance = position.distanceTo(entity->getPosition());
            if (distance < minDistance && distance <= detectionRange) {
                minDistance = distance;
                nearest = entity;
            }
        }
        
        return nearest;
    }
    
    void fleeFrom(const Position& threat) {
        // Calcule la direction opposée au zombie
        Position direction = position - threat;
        double distance = position.distanceTo(threat);
        
        if (distance < 0.001) {
            // Si on est exactement sur le zombie, direction aléatoire
            direction = Position(1, 1);
            distance = std::sqrt(2);
        }
        
        // Normalise
        direction = direction * (1.0 / distance);
        
        // Applique la vitesse de fuite
        double newX = position.x + direction.x * speed;
        double newY = position.y + direction.y * speed;
        
        // Clamp dans les limites
        newX = std::max(0.0, std::min(10000.0, newX));
        newY = std::max(0.0, std::min(10000.0, newY));
        
        position = Position(newX, newY);
    }
};

// Implémentation de l'attaque zombie (après la définition de Human)
void Zombie::attack(std::shared_ptr<Entity> target) {
    if (auto human = std::dynamic_pointer_cast<Human>(target)) {
        std::cout << "Zombie #" << id << " attaque Human #" << target->getId() << std::endl;
        human->takeDamage(damage);
    }
}

// ===== CLASSE GAME =====
class Game {
private:
    std::vector<std::shared_ptr<Entity>> entities;
    int nextId;
    int turn;
    
public:
    Game() : nextId(0), turn(0) {}
    
    void addHuman(const Position& pos, int health = 100) {
        entities.push_back(std::make_shared<Human>(nextId++, pos, health));
    }
    
    void addZombie(const Position& pos, double speed = 30.0) {
        entities.push_back(std::make_shared<Zombie>(nextId++, pos, speed));
    }
    
    void update() {
        turn++;
        std::cout << "\n===== TOUR " << turn << " =====" << std::endl;
        
        // Mise à jour de toutes les entités
        for (auto& entity : entities) {
            entity->update(entities);
        }
        
        // Affiche l'état
        displayState();
        
        // Vérifie les conditions de fin
        if (isGameOver()) {
            std::cout << "\n===== FIN DU JEU =====" << std::endl;
            displayWinner();
        }
    }
    
    void displayState() const {
        std::cout << "\nÉtat actuel:" << std::endl;
        
        for (const auto& entity : entities) {
            std::cout << entity->getType() << " #" << entity->getId() 
                      << " - Position: " << entity->getPosition()
                      << " - Vivant: " << (entity->isAlive() ? "Oui" : "Non");
            
            if (entity->getType() == "Human") {
                auto human = std::dynamic_pointer_cast<Human>(entity);
                std::cout << " - Santé: " << human->getHealth();
            }
            
            std::cout << std::endl;
        }
    }
    
    bool isGameOver() const {
        bool hasAliveHuman = false;
        bool hasAliveZombie = false;
        
        for (const auto& entity : entities) {
            if (!entity->isAlive()) continue;
            
            if (entity->getType() == "Human") hasAliveHuman = true;
            if (entity->getType() == "Zombie") hasAliveZombie = true;
        }
        
        return !hasAliveHuman || !hasAliveZombie;
    }
    
    void displayWinner() const {
        bool humansWin = false;
        
        for (const auto& entity : entities) {
            if (entity->isAlive() && entity->getType() == "Human") {
                humansWin = true;
                break;
            }
        }
        
        if (humansWin) {
            std::cout << "Les humains ont survécu !" << std::endl;
        } else {
            std::cout << "Les zombies ont gagné !" << std::endl;
        }
    }
    
    int getTurn() const { return turn; }
};

// ===== FONCTION MAIN - EXEMPLE D'UTILISATION =====
int main() {
    Game game;
    
    // Créer quelques humains à différentes positions
    game.addHuman(Position(1000, 1000), 100);
    game.addHuman(Position(3000, 2000), 100);
    game.addHuman(Position(5000, 5000), 100);
    
    // Créer quelques zombies
    game.addZombie(Position(500, 500), 30.0);
    game.addZombie(Position(8000, 8000), 25.0);
    
    std::cout << "Simulation d'un jeu de zombies" << std::endl;
    std::cout << "Carte: 10000 x 10000 unités" << std::endl;
    
    // Simulation de 50 tours maximum
    for (int i = 0; i < 50 && !game.isGameOver(); ++i) {
        game.update();
    }
    
    std::cout << "\nSimulation terminée après " << game.getTurn() << " tours." << std::endl;
    
    return 0;
}