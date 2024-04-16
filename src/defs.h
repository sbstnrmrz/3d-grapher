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
#define DEG_TO_RAD(x) (x * PI) / 180.0f
#define RAD_TO_DEG(x) x * (180.0f / PI)
#define sgn(x) (x < 0) ? -1 : (x > 0)// ? 1 : 0
#define WIN_WIDTH  1280 
#define WIN_HEIGHT 720
#define ASPECT_RATIO (f32)WIN_WIDTH/WIN_HEIGHT
#define ORTHO_PROJECTION_MAT {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}}

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
typedef size_t   usize;
typedef struct dirent de;

typedef enum {
    AXIS_X,
    AXIS_Y,
    AXIS_Z
} axis;

typedef enum {
    POINT2D,
    POINT3D,
    LINE,
    TRIANGLE,
    CUBE,
    CIRCLE
} shape;

typedef struct {
    i32 x;
    i32 y;
} v2;

typedef struct {
    i32 x;
    i32 y;
    i32 z;
} v3;

typedef struct {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
} v4;

typedef struct {
    f32 x;
    f32 y;
} vf2;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} vf3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} vf4;

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
    pointf3d_t point[3];
} triangle_t;

typedef struct {
    pointf3d_t point[8];
} cube_t;

typedef struct {
    pointf3d_t p[2];
} line_t;

typedef struct {
    f32 radius;
    pointf3d_t center;
} circle_t;

#endif // DEFS_H

