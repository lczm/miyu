#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <chrono>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

using i32 = int32_t;
using u32 = uint32_t;
using f32 = float_t;

static const int SCREEN_WIDTH  = 1280;
static const int SCREEN_HEIGHT = 800;
static const string MAIN_ATLAS = "./0x72_DungeonTilesetII_v1.4.png";

struct Position {
    f32 x = 0;
    f32 y = 0;
};

struct AnimatedSprite {
    // Rectangle to render onto the screen with
    SDL_Rect dest;
    // Index of the animation_coordinate to draw
    i32 index = 0;
    f32 scale = 1.0f;
    i32 max_index = 0;
    // Change index every timer seconds
    f32 cumu_dt  = 0.0f;
    f32 interval = 0.1f;
    // 2D X/Y coordinates
    Position pos;
    // X/Y coordinates of the animated sprites
    vector<Position> animation_coordinates;
    vector<Position> animation_offsets;
};

struct Entity {
    AnimatedSprite as;

    virtual void update(f32 dt) = 0;
};

struct TextureAtlas {
    // Rectangle to clip the atlas with
    SDL_Rect src;

    // Width and height of the atlas
    i32 w, h = 0;
    i32 access = 0;
    u32 format = 0;
    // Pixels per frame - needs to be manually inputted
    i32 per_x, per_y = 0;
    // Total rows and columns - needs to be manually inputted
    i32 total_x, total_y = 0;
    // z-index for the future, if needed
    i32 z = 0;

    // For every texture that uses each TextureAtlas, add them to this vector
    // This is to make rendering more efficient
    vector<Entity*> entities;
    SDL_Texture* texture = nullptr;
};

struct State {
};

struct Princess : Entity {
    void update(f32 dt) {
    };
};
