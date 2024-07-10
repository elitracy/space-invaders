#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "raylib.h"
#include <ostream>
#include <string>

class Entity {
  public:
    Texture2D sprite_sheet;
    Rectangle src, pos;
    float width, height;
    std::string name;

    Entity(Rectangle src, Rectangle dest, std::string name)
        : src(src), pos(dest), width(dest.height), height(dest.height),
          name(name){};

    Entity(Rectangle src, Rectangle dest, Texture2D sprite_sheet,
           std::string name)
        : sprite_sheet(sprite_sheet), src(src), pos(dest), width(dest.width),
          height(dest.height), name(name){};

    Entity(Entity &e, std::string name)
        : sprite_sheet(e.sprite_sheet), src(e.src), pos(e.pos),
          width(e.pos.width), height(e.pos.height), name(name){};

    void draw() { DrawTexturePro(sprite_sheet, src, pos, {0, 0}, 0.0f, WHITE); }

    friend std::ostream &operator<<(std::ostream &os, Entity const &e) {
        os << e.name << ": ";
        os << "[" << e.pos.width << "x" << e.pos.height << "] ";
        os << "(" << e.pos.x << "," << e.pos.y << ")";
        return os;
    }
};

#endif // !ENTITY_HPP
