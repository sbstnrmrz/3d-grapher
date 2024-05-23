#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_render.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlrenderer3.h"
using namespace ImGui;

#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#elif __unix__
#define CLEAR_SCREEN system("clear")
#endif

#define array_len(x) sizeof(x)/sizeof(x[0])
#define PI atan2f(1, 1) * 4
#define DEG2RAD(x) x * (PI / 180.0f)
#define RAD2DEG(x) x * (180.0f / PI)
#define sgn(x) (x < 0) ? -1 : (x > 0)// ? 1 : 0
#define WIN_WIDTH  1280 
#define WIN_HEIGHT 720
#define ASPECT_RATIO (f32)WIN_WIDTH/WIN_HEIGHT
#define ORTHO_PROJECTION_MAT3x3 {{1.0, 0.0, 0.0}, \
                                 {0.0, 1.0, 0.0}, \
                                 {0.0, 0.0, 0.0}}

#define ORTHO_PROJECTION_MAT4x4 {{1.0, 0.0, 0.0, 0.0}, \
                                 {0.0, 1.0, 0.0, 0.0}, \
                                 {0.0, 0.0, 1.0, 0.0}, \
                                 {0.0, 0.0, 0.0, 0.0}}

#define COLOR_RED    (SDL_Color) {255, 0, 0, 255}
#define COLOR_GREEN  (SDL_Color) {0, 255, 0, 255}
#define COLOR_BLUE   (SDL_Color) {0, 0, 255, 255}
#define COLOR_CYAN   (SDL_Color) {0, 255, 255, 255}
#define COLOR_YELLOW (SDL_Color) {255, 255, 0, 255}
#define COLOR_ORANGE (SDL_Color) {255, 80, 0, 255}
#define COLOR_PURPLE (SDL_Color) {255, 0, 255, 255}
#define COLOR_WHITE  (SDL_Color) {255, 255, 255, 255}
#define COLOR_BLACK  (SDL_Color) {0, 0, 0, 255}

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

typedef enum {
    ZERO,
    MID_SCREEN,
} coord_origin;

typedef enum {
    AXIS_X,
    AXIS_Y,
    AXIS_Z
} axis;

typedef enum {
    ORTHOGRAPHIC_PROJ,
    PERSPECTIVE_PROJ,
} projection_type;

typedef enum {
    POINT,
    LINE,
    TRIANGLE,
    RECTANGLE,
    CUBE,
    CIRCLE
} shape_type;

typedef struct {
    i32 x;
    i32 y;
} vec2;

typedef struct {
    i32 x;
    i32 y;
    i32 z;
} vec3;

typedef struct {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
} vec4;

typedef struct {
    f32 x;
    f32 y;
} vec2f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} vec3f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} vec4f;

typedef struct {
    f32 x;
    f32 y;
} pointf2d_t;

typedef struct {
    i32 x;
    i32 y;
    i32 z;
} point3d_t;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} pointf3d_t;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} pointf4d_t;

typedef struct { 
    vec3f p[3];
} triangle_t;

typedef struct {
    vec3f p[8];
} cube_t;

typedef struct {
    vec3f p[4];
} rect_t;

typedef struct {
    vec3f p[2];
} line_t;

typedef struct {
    f32 radius;
    pointf3d_t center;
} circle_t;

typedef struct {
    vec4f pos;
    f32 fov;
    f32 znear;
    f32 zfar;
} camera_t;

#endif // DEFS_H

