#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

#include "def.h"
#include "render_util.h"

// Window that to be rendered to
SDL_Window*  g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

high_resolution_clock::time_point timer;
f32 dt        = 0;
f32 second_dt = 0;
i32 frames    = 0;

// Using a set to store the key values is definitely not the most
// efficient way to go about doing this. But it is very convenient
// Performance will definitely not be bottle-necked here so this should
// be fine.
static set<SDL_Keycode>     keys;
static vector<TextureAtlas> atlass;

struct State {
};

struct Princess : Entity {
    void update(f32 dt) {
        if (keys.find(SDLK_DOWN) != keys.end()) {
            as.index = 0;
        }
    };
};

bool init_window() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Create window
    g_window = SDL_CreateWindow("Miyu",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (g_window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    } 
    // Create renderer for window
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize PNG loading
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    } 

    // Initialize renderer color
    set_render_color(g_renderer, 255, 255, 255);

    return true;
}

TextureAtlas load_texture(string path) {
    // Final texture
    TextureAtlas texture;

    // Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());

    if (loaded_surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return texture;
    }

    // Create texture from surface pixels
    texture.texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
    if (texture.texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return texture;
    }

    // Store the information of the new 
    SDL_QueryTexture(texture.texture,
                     &texture.format,
                     &texture.access,
                     &texture.w,
                     &texture.h);

    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);

    return texture;
}

void close_window() {
    // Free loaded image
    for (TextureAtlas ta : atlass) {
        SDL_DestroyTexture(ta.texture);
        // Reset the pointer, just in case this gets used again after this
        ta.texture = nullptr;
    }

    // Destroy window
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_renderer = nullptr;
    g_window = nullptr;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void update_entities(f32 dt, vector<TextureAtlas> atlass) {
    for (TextureAtlas ta : atlass) {
        for (Entity* e : ta.entities) {
            // Update the entity
            e->update(dt);
            // Update the frame data
            e->as.cumu_dt += dt;
            if (e->as.cumu_dt >= e->as.interval) {
                e->as.index++;
                if (e->as.index == e->as.max_index) {
                    e->as.index -= e->as.max_index;
                }
                e->as.cumu_dt -= e->as.interval;
            }
        }
    }
}

int main(int argv, char** args) {
    bool quit = false;
    // Event handler
    SDL_Event e;

    // Start up SDL and create window
    if (!init_window()) {
        printf("Failed to initialize\n");
        return 0;
    }

    // Init the timer
    timer = high_resolution_clock::now();

    // Load assets
    TextureAtlas atlas = load_texture(MAIN_ATLAS);
    atlas.per_x = 16;
    atlas.per_y = 16;
    atlas.total_x = 0;
    atlas.total_y = 0;

    // Create AnimationSprite
    Princess princess;
    princess.as.pos = {100.0f, 100.0f};
    princess.as.scale = 10.0f;
    princess.as.animation_coordinates.push_back({1.0f, 8.0f});
    princess.as.animation_coordinates.push_back({1.0f, 9.0f});
    princess.as.animation_coordinates.push_back({1.0f, 10.0f});
    princess.as.animation_coordinates.push_back({1.0f, 11.0f});
    princess.as.animation_coordinates.push_back({1.0f, 12.0f});
    princess.as.animation_coordinates.push_back({1.0f, 13.0f});
    princess.as.animation_coordinates.push_back({1.0f, 14.0f});
    princess.as.animation_coordinates.push_back({1.0f, 15.0f});
    princess.as.animation_coordinates.push_back({1.0f, 16.0f});
    princess.as.max_index = 
        princess.as.animation_coordinates.size();
    // Create an offset vector that is the same size as the animation_coordinates
    princess.as.animation_offsets = 
        vector<Position>(princess.as.animation_coordinates.size(),
                         Position());
    // Add the offsets
    princess.as.animation_offsets[5].x = -1;
    princess.as.animation_offsets[8].x = -5;

    // Add AnimatedSprite into the TextureAtlas
    atlas.entities.push_back(&princess);

    // Add TextureAtlas into global list of TextureAtlas'
    atlass.push_back(atlas);

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
                continue;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        auto key = e.key.keysym.sym;
                        keys.insert(key);
                        break;
                }
            }
        }

        set_render_color(g_renderer, 255, 255, 255);

        auto now = high_resolution_clock::now();
        duration<float> elapsed = now - timer;
        dt = elapsed.count();
        timer = now;
        second_dt += dt;

        if (second_dt >= 1.0f) {
            second_dt -= 1.0f;
            frames = 0;
        }

        // Update all textures
        update_entities(dt, atlass);

        // Clear screen
        clear_renderer(g_renderer);

        // Draw all the textures
        draw_texture_atlas(g_renderer, atlass);

        // Update screen
        flush_renderer(g_renderer);

        // Clear set
        keys.clear();
    }

    close_window();

    return 0;
}
