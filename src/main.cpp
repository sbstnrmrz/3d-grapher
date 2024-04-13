#include "defs.h"
#include "func.h"

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} sdl;

bool spin = true;
f64 angle = 0.00f;
f64 x_ang = 0.00f;
f64 y_ang = 0.00f;
f64 z_ang = 0.00f;
ImGuiIO io;
bool show_axis = false;
bool ax = false;
bool ay = false;
bool az = false;

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
    pointf2d_t last_pos;
    bool pressed;
    bool moving;
    
} mouse;

void render_axis(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    static f64 ang = 0;
    pointf3d_t x1 = {
        .x = 0,
        .y = WIN_HEIGHT/2,
        .z = 0
    };
    pointf3d_t x2 = {
        .x = WIN_WIDTH,
        .y = WIN_HEIGHT/2,
        .z = 0
    };
    pointf3d_t y1 = {
        .x = WIN_WIDTH/2,
        .y = 0,
        .z = 0
    };
    pointf3d_t y2 = {
        .x = WIN_WIDTH/2,
        .y = WIN_HEIGHT,
        .z = 0
    };
    pointf3d_t z1 = {
        .x = 0,
        .y = 0,
        .z = -100
    };
    pointf3d_t z2 = {
        .x = 0,
        .y = 0,
        .z = 100
    };
    pointf3d_t x[2] = {
        x1,
        x2
    };
//    rotate(x, LINE, ang, AXIS_Z);
//  rotate_line(x, ang, AXIS_Z);
    project_line(x);
//    _normalize_point(x);

    SDL_RenderLine(renderer, x[0].x, x[0].y, x[1].x, x[1].y);
    SDL_RenderLine(renderer, y1.x, y1.y, y2.x, y2.y);
    SDL_RenderLine(renderer, z1.x, z1.y, z2.x, z2.y);

//    ang += 0.02;

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
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

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

void init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(sdl.window, sdl.renderer);
    ImGui_ImplSDLRenderer3_Init(sdl.renderer);

}

void update() {
    SDL_Event event;
    SDL_PollEvent(&event);
    const u8 *state = SDL_GetKeyboardState(NULL);



    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
        sdl.running = false;
    }

    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        SDL_GetMouseState(&mouse.pos.x, &mouse.pos.y);
        mouse.moving = true;
//        printf("Pos = %.2f | %.2f\n", mouse.pos.x, mouse.pos.y);

    } else {
        mouse.moving = false;
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        mouse.last_pos.x = mouse.pos.x; 
        mouse.last_pos.y = mouse.pos.y; 
        printf("Last pos = %.2f | %.2f\n", mouse.last_pos.x, mouse.last_pos.y);
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

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Rotation");
    ImGui::Checkbox("Show axis", &show_axis);
    ImGui::Checkbox("X axis", &ax);
    ImGui::Checkbox("Y axis", &ay);
    ImGui::Checkbox("Z axis", &az);
    ImGui::End();


}

void render() {
    ImGui::Render();

    SDL_SetRenderDrawColor(sdl.renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(sdl.renderer);
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);

    if (show_axis) {
        render_axis(sdl.renderer);
    }


    
    pointf2d_t px;
    if (!mouse.pressed) {
        mouse.last_pos.x = mouse.last_pos.y = 0;
    }
    f32 co = -(mouse.pos.y - (WIN_HEIGHT/2));
    f32 ca = mouse.pos.x - (WIN_WIDTH/2);
    f32 hyp = tan(co/ca);
    f32 ang = atan2f(co, ca);
    int conv = RAD_TO_DEG(ang);

//  if (mouse.pressed && mouse.moving) {
//      angle = ang;    
//  }
      printf("ang: %f\n", ang);

    printf("Pos = %.2f | %.2f\n", ca, co);

    SDL_RenderLine(sdl.renderer, WIN_WIDTH/2, WIN_HEIGHT/2, mouse.pos.x, mouse.pos.y);
    SDL_RenderLine(sdl.renderer, mouse.pos.x, WIN_HEIGHT/2, mouse.pos.x, mouse.pos.y);

    cube_t c1 = cube1;
    if (ax) {
        rotate(&c1, CUBE, x_ang, AXIS_X);
        x_ang += 0.02;
    } else {
        x_ang = 0;
    }
    if (ay) {
        rotate(&c1, CUBE, y_ang, AXIS_Y);
        y_ang += 0.02;
    } else {
        y_ang = 0;
    }
    if (az) {
        rotate(&c1, CUBE, z_ang, AXIS_Z);
        z_ang += 0.02;
    } else {
        z_ang = 0;
    }

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

//  rotate_line(l, angle, AXIS_Z);
//  project_line(l);
//  
//  for (size_t i = 0; i < 2; i++) {
//      l[i] = normalize_point(l[i]);
//  }

//  SDL_RenderLine(sdl.renderer, l[0].x, l[0].y, l[1].x, l[1].y);

    if (spin) {
        angle += 0.02;
    }
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(sdl.renderer);
}

void uninit_sdl() {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window); 
    SDL_Quit();

}

void uninit_imgui() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

}

int main() {
    init_sdl("WINDOW", WIN_WIDTH, WIN_HEIGHT, 0);
    init_imgui();

    while (sdl.running) {
        update();
        render();
    }

    uninit_imgui();
    uninit_sdl();

    return 0;
}
