#include "defs.h"
#include "func.h"

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} sdl;

bool spin = true;

pointf3d_t camera = {
    0, 0, 0  
};

const f64 ortho[3][3] = ORTHO_PROJECTION_MAT;

cube_t cube1 = {
    {-1, -1, -1,
     1, -1, -1,
     -1, 1, -1,
     1, 1, -1,
     -1, -1, 1,
     1, -1, 1,
     -1, 1, 1,
     1, 1, 1},
};
cube_t cube2 = {
    {1, -1, -1,
     3, -1, -1,
     1, 1, -1,
     3, 1, -1,
     1, -1, 1,
     3, -1, 1,
     1, 1, 1,
     3, 1, 1},
};
cube_t cube3 = {
    {1, -1, -1,
     -3, -1, -1,
     1, 1, -1,
     -3, 1, -1,
     1, -1, 1,
     -3, -1, 1,
     1, 1, 1,
     -3, 1, 1},
};

cube_t cube4 = {
    {-1, -3, -1,
     1, -3, -1,
     -1, -1, -1,
     1, -1, -1,
    -1, -3, 1,
     1, -3, 1,
     -1, -1, 1,
     1, -1, 1}
};
cube_t cube5 = {
    {-1, 1, -1,
     1, 1, -1,
     -1, 3, -1,
     1, 3, -1,
     -1, 1, 1,
     1, 1, 1,
     -1, 3, 1,
     1, 3, 1,}
};

triangle_t tri1 = {
    -4, 0, 0,
    0, -4, 0,
    0, 0, 0
};

circle_t circle = {
    .center = {0, 0, 0},
    .radius = 2
};

static f64 dist = 2.5;

pointf3d_t line[2] = {
    {0, 0, 0},
    {10, 0, 0}
};

struct {
    pointf2d_t pos;
    bool pressed;
} mouse;

void render_axis(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderLine(renderer, 0, WIN_HEIGHT/2, WIN_WIDTH, WIN_HEIGHT/2);
    SDL_RenderLine(renderer, WIN_WIDTH/2, 0, WIN_WIDTH/2, WIN_HEIGHT);

}

void print_point(pointf3d_t point) {
    printf("(X, Y, Z) %f, %f, %f\n", point.x, point.y, point.z);
}

void print_matrix(const f64 matrix[][3]) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            printf("%.2f |", matrix[i][j]);
        }
        printf("\n");
    }

}

void init_sdl(const char *win_title, int win_width, int win_height, u32 flags) {
    sdl.running = false;

    if (SDL_Init(SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "Failed to initialize SDL. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("Events initialized\n");

    sdl.window = SDL_CreateWindow(win_title, win_width, win_height, 0);
    if (sdl.window == NULL) {
        fprintf(stderr, "Failed to create SDL_Window. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("Window created\n");

    sdl.renderer = SDL_CreateRenderer(sdl.window, NULL, SDL_RENDERER_PRESENTVSYNC);
    if (sdl.renderer == NULL) {
        fprintf(stderr, "Failed to create SDL_Renderer. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("Renderer created\n");

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize TTF. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("TTF initialized\n");
    sdl.running = true;

}

void update() {
    SDL_Event event;
    SDL_PollEvent(&event);
    const u8 *state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_EVENT_QUIT) {
        sdl.running = false;
    }

    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        SDL_GetMouseState(&mouse.pos.x, &mouse.pos.y);
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        mouse.pressed = true;
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        mouse.pressed = false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.keysym.sym == SDLK_z) {

        }

    } else if (event.type == SDL_EVENT_KEY_UP) {
        if (event.key.keysym.sym == SDLK_SPACE) {
            spin = !spin;
        }
    }
    if (state[SDL_SCANCODE_W]) {
        dist -= 0.02;
    }
    if (state[SDL_SCANCODE_S]) {
        dist += 0.02;
    }

    if (state[SDL_SCANCODE_RIGHT]) {

    }
    if (state[SDL_SCANCODE_LEFT]) {

    }
    if (state[SDL_SCANCODE_UP]) {

    }
    if (state[SDL_SCANCODE_DOWN]) {

    }
}

void render() {
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(sdl.renderer);

//    render_axis(sdl.renderer);

    static f64 angle = 0.00f;
    cube_t c1 = cube1;
    rotate(&c1, CUBE, angle, AXIS_X);
    project_cube(&c1);
    render_cube(sdl.renderer, c1);
    
    f32 sinang = sin(angle);
    f32 cosang = cos(angle);
    const f64 x_rotation_matrix[3][3] = {
        {1, 0, 0},
        {0, cosang, -sinang},
        {0, sinang, cosang},
    };
    const f64 y_rotation_matrix[3][3] = {
        {cosang, 0, sinang},
        {0, 1, 0},
        {-sinang, 0, cosang},
    };
    const f64 z_rotation_matrix[3][3] = {
        {cosang, -sinang, 0},
        {sinang, cosang, 0},
        {0, 0, 1},
    };

    for (f32 i = 0; i < 100; i += 0.10) {
        pointf3d_t p = {
//            .x = sqrt(4 - pow(i, 2)),
            .x = 2,
            .y = 0,
            .z = 0
        };
        pointf3d_t p2 = {
//            .x = -sqrt(4 - pow(i, 2)),
            .x = -2,
            .y = 0,
            .z = 0
        };
        pointf3d_t p3 = {
            .x = 0,
//            .y = sqrt(4 - pow(i, 2)),
            .y = 2,
            .z = 0
        };
        pointf3d_t p4 = {
            .x = 0,
//            .y = -sqrt(4 - pow(i, 2)),
            .y = -2,
            .z = 0
        };
        pointf3d_t p5 = {
            .x = 0,
            .y = 0,
            .z = 2
        };
        pointf3d_t p6 = {
            .x = 0,
            .y = 0,
            .z = -2
        };
        pointf3d_t proj = p;
        pointf3d_t proj2 = p2;
        pointf3d_t proj3 = p3;
        pointf3d_t proj4 = p4;
        pointf3d_t proj5 = p5;
        pointf3d_t proj6 = p6;

        mat_mul_3x3(&proj, z_rotation_matrix);
        mat_mul_3x3(&proj2, z_rotation_matrix);
        mat_mul_3x3(&proj3, z_rotation_matrix);
        mat_mul_3x3(&proj4, z_rotation_matrix);
        mat_mul_3x3(&proj5, y_rotation_matrix);
        mat_mul_3x3(&proj6, y_rotation_matrix);
        mat_mul_3x3(&proj5, x_rotation_matrix);
        mat_mul_3x3(&proj6, x_rotation_matrix);
        mat_mul_3x3(&proj5, z_rotation_matrix);
        mat_mul_3x3(&proj6, z_rotation_matrix);

        mat_mul_3x3(&proj, ortho);
        mat_mul_3x3(&proj2, ortho);
        mat_mul_3x3(&proj3, ortho);
        mat_mul_3x3(&proj4, ortho);
        mat_mul_3x3(&proj5, ortho);
        mat_mul_3x3(&proj6, ortho);
        
//      SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + proj.x*30, (WIN_HEIGHT/2) - proj.y*30);
//      SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + proj2.x*30, (WIN_HEIGHT/2) - proj2.y*30);
//      SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + proj3.x*30, (WIN_HEIGHT/2) - proj3.y*30);
//      SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + proj4.x*30, (WIN_HEIGHT/2) - proj4.y*30);

        SDL_RenderLine(sdl.renderer, (WIN_WIDTH/2) + proj.x*30, (WIN_HEIGHT/2) - proj.y*30, 
                       (WIN_WIDTH/2) + proj2.x*30, (WIN_HEIGHT/2) - proj2.y*30);
        SDL_RenderLine(sdl.renderer, (WIN_WIDTH/2) + proj3.x*30, (WIN_HEIGHT/2) - proj3.y*30, 
                       (WIN_WIDTH/2) + proj4.x*30, (WIN_HEIGHT/2) - proj4.y*30);
        SDL_RenderLine(sdl.renderer, (WIN_WIDTH/2) + proj5.x*30, (WIN_HEIGHT/2) - proj5.y*30, 
                       (WIN_WIDTH/2) + proj6.x*30, (WIN_HEIGHT/2) - proj6.y*30);
    }


 // for (f32 i = 0; i < 100; i += 0.10) {
 //     pointf3d_t p = {
 //         .x = i,
 //         .y = cos(i),
 //         .z = 0
 //     };
 //     pointf3d_t proj = p;

 //     mat_mul_3x3(&proj, z_rotation_matrix);
 //     mat_mul_3x3(&proj, ortho_projection_matrix);
 //     
 //     SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + proj.x*30, (WIN_HEIGHT/2) - proj.y*30);
 // }

// LINE
//  SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
//  pointf3d_t l[2];
//  l[0] = line[0];
//  l[1] = line[1];

//  rotate_line(l, angle, AXIS_X);
//  rotate_line(l, angle, AXIS_Y);
//  rotate_line(l, angle, AXIS_Z);
//  project_line(l);
//  
//  for (size_t i = 0; i < 2; i++) {
//      l[i] = normalize_point(l[i]);
//  }

//  SDL_RenderLine(sdl.renderer, l[0].x, l[0].y, l[1].x, l[1].y);
    circle_t c = circle;
//    project_circle(circle);
    render_circle(sdl.renderer, circle);
    if (spin) {
        angle += 0.02;
    }
    SDL_RenderPresent(sdl.renderer);
}

void uninit_sdl() {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window); 
    SDL_Quit();

}

int main() {
    init_sdl("WINDOW", WIN_WIDTH, WIN_HEIGHT, 0);

    while (sdl.running) {
        update();
        render();
    }

    uninit_sdl();

    return 0;
}
