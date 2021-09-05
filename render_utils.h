#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

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
