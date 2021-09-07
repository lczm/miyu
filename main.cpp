#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "render_utils.h"

using std::vector;
using std::string;

using i32 = int32_t;
using u32 = uint32_t;
using f32 = float_t;

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 800;
const string MAIN_ATLAS = "./0x72_DungeonTilesetII_v1.4.png";

// Window that to be rendered to
SDL_Window*  g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

struct TextureAtlas;

struct TextureGrid {
    f32 x = 0;
    f32 y = 0;
    f32 w = 0;
    f32 h = 0;
    f32 scale = 1.0f;
    TextureAtlas* atlas = nullptr;
};

struct TextureAtlas {
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
    vector<TextureGrid*> grids;
    SDL_Texture* texture = nullptr;
};

struct Player {
};

static vector<TextureAtlas> textures;

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
    for (TextureAtlas t : textures) {
        SDL_DestroyTexture(t.texture);
        // Reset the pointer, just in case this gets used again after this
        t.texture = nullptr;
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

inline void draw_texture(SDL_Renderer* r, TextureAtlas* t, TextureGrid* tg) {
    // TODO : SDL_RenderCopy takes in a pointer to SDL_Rect, there is no need
    // to keep constructing this.
    SDL_Rect src;
    SDL_Rect dest;
    src.x = tg->x;
    src.y = tg->y;
    src.w = tg->w * tg->scale;
    src.h = tg->h * tg->scale;

    dest.x = tg->x;
    dest.y = tg->y;
    dest.w = tg->w * tg->scale;
    dest.h = tg->h * tg->scale;

    SDL_RenderCopy(r, t->texture, &src, &dest);
    // SDL_RenderCopy(r, t->texture, {tg->x, tg->y, // source
    //                                tg->w * tg->scale, tg->h * tg->scale},
    //                               {tg->x, tg->y, // destination
    //                                tg->w * tg->scale, tg->h * tg->scale});
}

void draw_texture_atlas(SDL_Renderer* r, vector<TextureAtlas> textures) {
    for (TextureAtlas t : textures) {
        for (TextureGrid* tg : t.grids) {
            // set_viewport(r, tg.x, tg.y, tg.w, tg.h);
            draw_texture(r, &t, tg);
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

    // Load assets
    TextureAtlas atlas = load_texture(MAIN_ATLAS);
    atlas.per_x = 16;
    atlas.per_y = 16;
    atlas.total_x = 0;
    atlas.total_y = 0;

    textures.push_back(atlas);

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
        for (TextureAtlas t : textures) {
            // TODO : Check if i need to set viewport in the first place
            // set_viewport(g_renderer, 0, 0, t.w, t.h);
            draw_texture(g_renderer, t.texture);
        }

        // Update screen
        flush_renderer(g_renderer);
    }

    close_window();

    return 0;
}
