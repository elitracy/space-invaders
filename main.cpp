#include "enemy.hpp"
#include "entity.hpp"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float SPRITE_SCALE = 3.0;
const float ENEMY_SPEED = 750;
const float LASER_DELAY = 150;
const int NUM_ENEMIES = 50;

int main() {
    // Game Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(60);

    Image image = LoadImage("./resources/sprites/entity_sprites.png");
    Texture2D entity_sprites = LoadTextureFromImage(image);
    UnloadImage(image);

    // create character
    Rectangle character_src{114, 114, 13, 8}; // sprite sheet loc
    float character_init_x = SCREEN_WIDTH / 2.0 - character_src.width / 2;
    float character_init_y = SCREEN_HEIGHT - 8;
    Rectangle character_pos{
        character_init_x - (character_src.width * SPRITE_SCALE / 2),
        character_init_y - (character_src.height * SPRITE_SCALE / 2),
        character_src.width * SPRITE_SCALE,
        character_src.height * SPRITE_SCALE};
    Entity character(character_src, character_pos, entity_sprites, "MC");

    Entity laser({120, 106, 1, 4}, {0, 0, 3, 8}, entity_sprites, "laser");

    EnemyGroup enemies;
    enemies.num_enemies = NUM_ENEMIES;
    enemies.enemies = new Enemy *[enemies.num_enemies];

    float start_pos_x = 10;
    float start_pos_y = 50;

    float enemy_margin_x = 10;
    float enemy_margin_y = 20;

    for (int i = 0; i < NUM_ENEMIES; i++) {
        Rectangle enemy_src{35, 13, 12, 10}; // sprite sheet loc
                                             //
        if (start_pos_x + enemy_src.width * SPRITE_SCALE * 2 > SCREEN_WIDTH) {
            start_pos_x = 100;
            start_pos_y += enemy_src.height * SPRITE_SCALE + enemy_margin_y;
        } else {
            start_pos_x += enemy_src.width * SPRITE_SCALE + enemy_margin_x;
        }

        Rectangle enemy_pos{start_pos_x, start_pos_y,
                            enemy_src.width * SPRITE_SCALE,
                            enemy_src.height * SPRITE_SCALE};
        Enemy *enemy = new Enemy(enemy_src, enemy_pos, entity_sprites,
                                 "e" + std::to_string(i));
        enemies.enemies[i] = enemy;
        enemies.width += enemy_pos.width;
        enemies.height = std::max(enemy_pos.height, enemies.height);
    }
    // create enemies
    float enemy_timer = 0;
    float laser_timer = 0;
    bool reset_timer = false;

    Entity *lasers[100];
    int num_lasers = 0;
    int score = 0;

    // Game Loop
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        enemy_timer += dt * 1000; // ms
        laser_timer += dt * 1000; // ms

        if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT))
            character.pos.x -= 500 * dt;
        if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT))
            character.pos.x += 500 * dt;
        if (IsKeyDown(KEY_SPACE)) {
            if (laser_timer > LASER_DELAY) {
                laser_timer = 0;
                // allocate new laser
                Entity *l =
                    new Entity(laser, "laser" + std::to_string(num_lasers));
                l->pos.x = character.pos.x + character.width / 2.0 - 1;
                l->pos.y = character.pos.y;
                lasers[num_lasers] = l;
                num_lasers++;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("Score: %i", score), 20, 20, 20, RED);

        character.draw();
        enemies.drawAll();

        bool removeLaser = 0;

        for (int i = 0; i < num_lasers; i++) {
            lasers[i]->draw();
            lasers[i]->pos.y -= 500 * dt;

            if (lasers[i]->pos.y <= 0) { removeLaser = 1; }
            if (enemies.hitEnemy(lasers[i]->pos.x, lasers[i]->pos.y)) {
                score++;
            }

            if (removeLaser) {
                num_lasers--;
                lasers[i] = lasers[i + 1];
            }
        }

        if (enemy_timer > ENEMY_SPEED) {
            reset_timer = true;

            Entity *last_enemy = enemies.enemies[enemies.num_enemies - 1];
            Entity *first_enemy = enemies.enemies[0];

            if (last_enemy->pos.x + last_enemy->width * 2 >= SCREEN_WIDTH) {
                enemies.moveAll(-(last_enemy->width / 2.0), last_enemy->height);
                enemies.direction = -1;
            }

            if (first_enemy->pos.x - first_enemy->width <= 0) {
                enemies.moveAll(first_enemy->width / 2.0, first_enemy->height);
                enemies.direction = 1;
            }

            enemies.moveAll(enemies.direction * SCREEN_WIDTH / 20.0);
        }

        if (reset_timer) {
            enemy_timer = 0;
            reset_timer = false;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
