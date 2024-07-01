#include "entity.hpp"
#include "raylib.h"
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float SPRITE_SCALE = 3.0;
const float ENEMY_SPEED = 750;

using namespace std;

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
      character_src.width * SPRITE_SCALE, character_src.height * SPRITE_SCALE};
  Entity character(character_src, character_pos, entity_sprites);

  Entity *enemies[5];

  for (int i = 0; i < 10; i++) {
    Rectangle enemy_src{35, 13, 12, 10}; // sprite sheet loc
    Rectangle enemy_pos{100 + enemy_src.width * float(3.5) * i, 12,
                        enemy_src.width * SPRITE_SCALE,
                        enemy_src.height * SPRITE_SCALE};
    Entity *enemy = new Entity(enemy_src, enemy_pos, entity_sprites);
    enemies[i] = enemy;
  }
  // create enemies
  int enemy_direction = 1;
  float timer_ms = 0;
  bool reset_timer = false;

  // Game Loop
  while (!WindowShouldClose()) {

    float dt = GetFrameTime();
    timer_ms += dt * 1000; // ms

    if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT))
      character.pos.x -= 500 * dt;
    if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT))
      character.pos.x += 500 * dt;

    BeginDrawing();
    ClearBackground(BLACK);

    character.draw();
    for (auto e : enemies) {
      e->draw();

      if (timer_ms > ENEMY_SPEED) {
        reset_timer = true;

        if (e->pos.x + (e->pos.width * 2) >= SCREEN_WIDTH) {
          e->pos.x -= e->pos.width;
          e->pos.y += e->height;
          enemy_direction = -1;
        }
        if (e->pos.x - (e->pos.width) <= 0) {
          e->pos.x += e->pos.width;
          e->pos.y += e->height;
          enemy_direction = 1;
        }

        e->pos.x += enemy_direction * SCREEN_WIDTH / 20.0;
      }
    }

    if (reset_timer) {
      timer_ms = 0;
      reset_timer = false;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
