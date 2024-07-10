#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"
#include <iostream>

class Enemy : public Entity {
  public:
    Enemy(Rectangle src, Rectangle dest, std::string name)
        : Entity(src, dest, name){};

    Enemy(Rectangle src, Rectangle dest, Texture2D sprite_sheet,
          std::string name)
        : Entity(src, dest, sprite_sheet, name){};

    Enemy(Entity &e, std::string name) : Entity(e, name){};

    bool hit = false;
};

struct EnemyGroup {
    Enemy **enemies;

    int num_enemies = 0;
    float width;
    float height;
    Rectangle pos;
    int direction = 1; // 1 right, 0 stop, -1 left

    void moveAll(float x = 0, float y = 0) {
        for (int i = 0; i < num_enemies; i++) {
            Enemy *e = enemies[i];
            e->pos.x += x;
            e->pos.y += y;
        }
    }

    void drawAll() {
        for (int i = 0; i < num_enemies; i++) {
            Enemy *e = enemies[i];
            e->draw();
        }
    }

    bool hitEnemy(float x, float y) {
        for (int i = 0; i < num_enemies; i++) {
            Enemy *current = enemies[i];

            if (!current->hit && current->pos.x <= x &&
                current->pos.x + current->width >= x && current->pos.y <= y &&
                current->pos.y + current->height >= y) {

                enemies[i]->src = {0, 0, 10, 12};
                enemies[i]->draw();
                current->hit = true;
                return true;
            }
        }

        return false;
    }
};

#endif
