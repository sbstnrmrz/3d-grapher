#ifndef FUNC_H
#define FUNC_H

#include "defs.h"

void mat3x3_mul_vec3(vec3f *vec3f, f64 mat[3][3]);
void mat4x4_mul_vec4(vec4f *vec4f, f64 mat[4][4]);

vec3f normalize_point(vec3f point, f32 scale);
void normalize_point2d(pointf2d_t *p, f32 scale);
void _normalize_point(vec3f *vec, f32 scale);
void normalize_vec3(vec3f *vec, f32 scale, coord_origin origin);
void normalize_vec2(vec2f *vec, f32 scale, coord_origin origin);
void project_shape(void *shape, shape_type type, camera_t camera, projection_type projection_type);
void project_line(line_t *line);
void project_triangle(triangle_t *triangle);
void project_rect(rect_t *rect);
void project_cube(cube_t *cube);
void _project_cube(cube_t *cube, f32 zfar, f32 znear, f32 fov);
void project_circle(circle_t *circle);
void project_point(vec3f *point);
void rotate_line(pointf3d_t line[2], f64 angle, axis axis);
void rotate_triangle(triangle_t *triangle, f64 angle, axis axis);
void rotate_cube(cube_t *cube, f64 angle, axis axis);
void rotate_circle(circle_t *circle, f64 angle, axis axis);
void rotate_line(line_t line, f64 angle, axis axis);
void rotate_shape(void *shape, shape_type shape_type, f64 angle, axis axis);

void render_triangle(SDL_Renderer *renderer, triangle_t triangle, f32 scale);
void render_rect(SDL_Renderer *renderer, rect_t rect, f32 scale);
void render_cube(SDL_Renderer *renderer, cube_t cube, f32 scale);
void render_circle(SDL_Renderer *renderer, circle_t circle, f64 angle);

#endif
