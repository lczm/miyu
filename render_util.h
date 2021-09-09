#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "def.h"

// Some documentation on how to use this:
// - Render texture to screen
// draw_texture(g_renderer, g_texture);
// - Render red filled quad
// set_render_color(g_renderer, 255, 0, 0);
// draw_rect(g_renderer, {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2});
// - Render green outlined quad
// set_render_color(g_renderer, 0, 255, 0);
// draw_rect_outline(g_renderer, {SCREEN_WIDTH/6,
//                                SCREEN_HEIGHT/6,
//                                SCREEN_WIDTH*2/3,
//                                SCREEN_HEIGHT*2/3});
// - Set render color
// set_render_color(g_renderer, 255, 255, 0);
// - Draw some points
// for (int i = 0; i < SCREEN_HEIGHT; i+=4) {
//     draw_point(g_renderer, SCREEN_WIDTH/2, i);
// }
// vector<SDL_Point> points;
// for (int i = 0; i < SCREEN_WIDTH; i++) {
//     points.push_back({i, 50});
// }
// - Clear screen
// clear_renderer(g_renderer);
// - Update screen
// flush_renderer(g_renderer);


inline void clear_renderer(SDL_Renderer* r) {
    SDL_RenderClear(r);
}

inline void flush_renderer(SDL_Renderer* r) {
    SDL_RenderPresent(r);
}

inline void set_render_color(SDL_Renderer* r, int cr, int cg, int cb, int ca) {
    SDL_SetRenderDrawColor(r, cr, cg, cb, ca);
}

inline void set_render_color(SDL_Renderer* r, int cr, int cg, int cb) {
    SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
}

inline void set_viewport(SDL_Renderer* r, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderSetViewport(r, &rect);
}

inline void set_viewport(SDL_Renderer* r, SDL_Rect rect) {
    SDL_RenderSetViewport(r, &rect);
}

inline void draw_point(SDL_Renderer* r, int x, int y) {
    SDL_RenderDrawPoint(r, x, y);
}

inline void draw_points(SDL_Renderer* r, SDL_Point* points, int points_count) {
    SDL_RenderDrawPoints(r, points, points_count);
}

inline void draw_line(SDL_Renderer* r, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
}

// create a vector/array, pass the first element in as points
inline void draw_lines(SDL_Renderer* r, SDL_Point* points, int points_count) {
    SDL_RenderDrawLines(r, points, points_count);
}

inline void draw_rect(SDL_Renderer* r, SDL_Rect rect) {
    SDL_RenderFillRect(r, &rect);
}

inline void draw_rect(SDL_Renderer* r, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(r, &rect);
}

inline void draw_rect_outline(SDL_Renderer* r, SDL_Rect rect) {
    SDL_RenderDrawRect(r, &rect);
}

inline void draw_rect_outline(SDL_Renderer* r, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(r, &rect);
}

inline void draw_texture(SDL_Renderer* r, SDL_Texture* t) {
    SDL_RenderCopy(r, t, nullptr, nullptr);
}

inline void draw_texture(SDL_Renderer* r, TextureAtlas* ta, AnimatedSprite* as) {
    ta->src.x = as->animation_coordinates[as->index].y * ta->per_y
        + as->animation_offsets[as->index].y;
    ta->src.y = as->animation_coordinates[as->index].x * ta->per_x
        + as->animation_offsets[as->index].x;
    ta->src.w = ta->per_y;
    ta->src.h = ta->per_x;

    as->dest.x = as->pos.x + as->animation_offsets[as->index].y;
    as->dest.y = as->pos.y + as->animation_offsets[as->index].x;
    as->dest.w = (ta->per_x * as->scale);
    as->dest.h = (ta->per_y * as->scale);

    SDL_RenderCopy(r, ta->texture, &ta->src, &as->dest);

    // To debug textures
    // set_render_color(r, 0, 0, 255);
    // draw_rect_outline(r, as->dest);
}

void draw_texture_atlas(SDL_Renderer* r, vector<TextureAtlas> atlass) {
    for (TextureAtlas ta : atlass) {
        for (Entity* e : ta.entities) {
            draw_texture(r, &ta, &e->as);
        }
    }
}
