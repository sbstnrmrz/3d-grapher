#ifndef FUNC_H
#define FUNC_H

#include "defs.h"

void mat_mul_3x3(pointf3d_t *p, f64 matrix[3][3]);
void mat_mul_4x4(pointf4d_t *p, f64 matrix[4][4]);
pointf3d_t normalize_point(pointf3d_t point);
void _normalize_point(pointf3d_t *point);
void project_line(pointf3d_t line[2]);
void project_triangle(triangle_t *triangle);
void project_cube(cube_t *cube);
void project_circle(circle_t *circle);
void project_point(pointf3d_t *point);
void rotate_line(pointf3d_t line[2], f64 angle, axis axis);
void rotate_triangle(triangle_t *triangle, f64 angle, axis axis);
void rotate_cube(cube_t *cube, f64 angle, axis axis);
void rotate_circle(circle_t *circle, f64 angle, axis axis);
void rotate_line(pointf3d_t line[2], f64 angle, axis axis);
void rotate(void *data, shape shape, f64 angle, axis axis);
void render_triangle(SDL_Renderer *renderer, triangle_t triangle);
void render_cube(SDL_Renderer *renderer, cube_t cube);
void render_circle(SDL_Renderer *renderer, circle_t circle, f64 angle);

#endif
