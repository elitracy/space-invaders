#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "raylib.h"

class Entity {
public:
  Texture2D sprite_sheet;
  Rectangle src, pos;
  float width, height;

  Entity(Rectangle &src, Rectangle &dest)
      : src(src), pos(dest), width(dest.height), height(dest.height){};

  Entity(Rectangle &src, Rectangle &dest, Texture2D &sprite_sheet)
      : src(src), pos(dest), height(dest.height), width(dest.width),
        sprite_sheet(sprite_sheet){};

  void draw() { DrawTexturePro(sprite_sheet, src, pos, {0, 0}, 0.0f, WHITE); }
};

#endif // !ENTITY_HPP
