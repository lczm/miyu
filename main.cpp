#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>
#include <vector>

#include "render_utils.h"

using std::vector;
using std::string;

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 800;
const string MAIN_ATLAS = "./0x72_DungeonTilesetII_v1.4.png";

// Window that to be rendered to
SDL_Window*  g_window = nullptr;
// Surface contained by the window
SDL_Texture* g_texture = nullptr;
SDL_Renderer* g_renderer = nullptr;

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

SDL_Texture* load_texture(string path) {
    // Final texture
    SDL_Texture* new_texture = nullptr;

    // Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());

    if (loaded_surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }

    // Create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
    if (new_texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return nullptr;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
    return new_texture;
}

void close_window() {
    // Free loaded image
    SDL_DestroyTexture(g_texture);
    g_texture = nullptr;

    // Destroy window
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_renderer = nullptr;
    g_window = nullptr;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
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

    // Load media
    g_texture = load_texture(MAIN_ATLAS);

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
                    case SDLK_UP:
                        printf("Up key\n");
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }

        // Clear screen
        clear_renderer(g_renderer);
        // Render texture to screen
        draw_texture(g_renderer, g_texture);

        // Render red filled quad
        set_render_color(g_renderer, 255, 0, 0);
        draw_rect(g_renderer, {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2});

        // Render green outlined quad
        set_render_color(g_renderer, 0, 255, 0);
        draw_rect_outline(g_renderer, {SCREEN_WIDTH/6,
                                       SCREEN_HEIGHT/6,
                                       SCREEN_WIDTH*2/3,
                                       SCREEN_HEIGHT*2/3});

        set_render_color(g_renderer, 255, 255, 0);
        for (int i = 0; i < SCREEN_HEIGHT; i+=4) {
            draw_point(g_renderer, SCREEN_WIDTH/2, i);
        }

        vector<SDL_Point> points;
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            points.push_back({i, 50});
        }

        set_render_color(g_renderer, 0, 0, 0);
        draw_points(g_renderer, &points[0], points.size());

        // Update screen
        flush_renderer(g_renderer);
    }

    close_window();

    return 0;
}
