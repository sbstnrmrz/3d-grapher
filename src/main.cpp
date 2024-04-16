#include "defs.h"
#include "func.h"
#include <SDL3/SDL_render.h>

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} sdl;

struct {
    line_t ax;
    line_t ay;
    line_t az;
} axis_lines;

ImGuiIO io;
f32 ip[2] = {0};
f32 scale = 30;
f64 angle = 0;
f64 x_ang = 0.00f;
f64 y_ang = 0.00f;
f64 z_ang = 0.00f;
bool show_axis = true;
bool show_cube = false;
bool show_coord_lines = true;
bool ax = false;
bool ay = false;
bool az = false;
line_t line = {
    -3, 0, 0,
    3, -3, 0
};

pointf3d_t camera = {
    0, 0, 0  
};

const f64 ortho[3][3] = ORTHO_PROJECTION_MAT;

cube_t cube1 = {
    {-2, -2, -2,
      2, -2, -2,
     -2,  2, -2,
      2,  2, -2,
     -2, -2,  2,
      2, -2,  2,
     -2,  2,  2,
      2,  2,  2},
};
cube_t cube2 = {
    {2, -2, -2,
     6, -2, -2,
     2,  2, -2,
     6,  2, -2,
     2, -2,  2,
     6, -2,  2,
     2,  2,  2,
     6,  2,  2},
};
cube_t cube3 = {
    { 2, -2, -2,
     -6, -2, -2,
      2,  2, -2,
     -6,  2, -2,
      2, -2,  2,
     -6, -2,  2,
      2,  2,  2,
     -6,  2,  2},
};

cube_t cube4 = {
    {-2, -6, -2,
      2, -6, -2,
     -2, -2, -2,
      2, -2, -2,
     -2, -6,  2,
      2, -6,  2,
     -2, -2,  2,
      2, -2,  2}
};
cube_t cube5 = {
    {-2, 2, -2,
      2, 2, -2,
     -2, 6, -2,
      2, 6, -2,
     -2, 2,  2,
      2, 2,  2,
     -2, 6,  2,
      2, 6,  2,}
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

struct {
    pointf2d_t pos;
    pointf2d_t last_pos;
    bool pressed;
    bool moving;
    
} mouse;

void render_axis(SDL_Renderer *renderer, f32 scale) {
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    static f64 ang = 0;
    
    axis_lines.ax.p[0] = (pointf3d_t) {
        .x = (f32)-WIN_WIDTH/scale,
        .y = 0,
        .z = 0
    };
    axis_lines.ax.p[1] = (pointf3d_t) {
        .x = (f32)WIN_WIDTH/scale,
        .y = 0,
        .z = 0
    };
    axis_lines.ay.p[0] = (pointf3d_t) {
        .x = 0,
        .y = (f32)-WIN_HEIGHT/scale,
        .z = 0
    };
    axis_lines.ay.p[1] = (pointf3d_t) {
        .x = 0, 
        .y = (f32)WIN_HEIGHT/scale,
        .z = 0
    };
    axis_lines.az.p[0] = (pointf3d_t) {
        .x = 0,
        .y = 0,
        .z = -100
    };
    axis_lines.az.p[1] = (pointf3d_t) {
        .x = 0, 
        .y = 0,
        .z = 100
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
    static f64 a = 0;
    f64 an = DEG_TO_RAD(45);
//  rotate(&axis_lines.ax, LINE, a, AXIS_Z);
//  rotate(&axis_lines.ay, LINE, a, AXIS_Z);
//  rotate(&axis_lines.ax, LINE, a, AXIS_Y);
//  rotate(&axis_lines.ay, LINE, a, AXIS_Y);
    rotate(&axis_lines.ax, LINE, an, AXIS_Z);
    rotate(&axis_lines.ay, LINE, 0, AXIS_Z);
    rotate(&axis_lines.az, LINE, an, AXIS_Y);
    project_line(axis_lines.ax);
    project_line(axis_lines.ay);
    project_line(axis_lines.az);
    _normalize_point(&axis_lines.ax.p[0], scale);
    _normalize_point(&axis_lines.ax.p[1], scale);
    _normalize_point(&axis_lines.ay.p[0], scale);
    _normalize_point(&axis_lines.ay.p[1], scale);
    _normalize_point(&axis_lines.az.p[0], scale);
    _normalize_point(&axis_lines.az.p[1], scale);

//  SDL_RenderLine(renderer, x[0].x, x[0].y, x[1].x, x[1].y);
//  SDL_RenderLine(renderer, y1.x, y1.y, y2.x, y2.y);
//  SDL_RenderLine(renderer, z1.x, z1.y, z2.x, z2.y);

    SDL_RenderLine(
        renderer, 
        axis_lines.ax.p[0].x, 
        axis_lines.ax.p[0].y, 
        axis_lines.ax.p[1].x, 
        axis_lines.ax.p[1].y
    );
    SDL_RenderLine(
        renderer, 
        axis_lines.ay.p[0].x, 
        axis_lines.ay.p[0].y, 
        axis_lines.ay.p[1].x, 
        axis_lines.ay.p[1].y
    );
    SDL_RenderLine(
        renderer, 
        axis_lines.az.p[0].x, 
        axis_lines.az.p[0].y, 
        axis_lines.az.p[1].x, 
        axis_lines.az.p[1].y
    );
//  SDL_RenderLine(renderer, y1.x, y1.y, y2.x, y2.y);
//  SDL_RenderLine(renderer, z1.x, z1.y, z2.x, z2.y);
    a += 0.02;

}

void print_point(pointf3d_t point) {
    printf("(X, Y, Z) %f, %f, %f\n", point.x, point.y, point.z);
}

void render_coord_lines(SDL_Renderer *renderer, f32 scale) {
    pointf3d_t coord_line[2] = {
        {1, 0, 0},
        {1, 0, 0}
    };

    project_point(&coord_line[0]);
    project_point(&coord_line[1]);

    while (normalize_point(coord_line[0], scale).x <= WIN_WIDTH) {
        pointf3d_t p1 = normalize_point(coord_line[0], scale);
        pointf3d_t p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x, p1.y-5, p2.x, p2.y+5);
        coord_line[0].x++;
        coord_line[1].x++;
    }

    coord_line[0].x = -1;
    coord_line[1].x = -1;
    while (normalize_point(coord_line[0], scale).x >= 0) {
        pointf3d_t p1 = normalize_point(coord_line[0], scale);
        pointf3d_t p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x, p1.y-5, p2.x, p2.y+5);
        coord_line[0].x--;
        coord_line[1].x--;
    }

    coord_line[0].x = 0;
    coord_line[1].x = 0;
    coord_line[0].y = 1;
    coord_line[1].y = 1;
    while (normalize_point(coord_line[0], scale).y <= WIN_HEIGHT) {
        pointf3d_t p1 = normalize_point(coord_line[0], scale);
        pointf3d_t p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x-5, p1.y, p2.x+5, p2.y);
        coord_line[0].y++;
        coord_line[1].y++;
    }

    coord_line[0].x = 0;
    coord_line[1].x = 0;
    coord_line[0].y = -1;
    coord_line[1].y = -1;
    while (normalize_point(coord_line[0], scale).y >= 0) {
        pointf3d_t p1 = normalize_point(coord_line[0], scale);
        pointf3d_t p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x-5, p1.y, p2.x+5, p2.y);
        coord_line[0].y--;
        coord_line[1].y--;
    }


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

    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        if (event.wheel.y > 0) {
            scale += 1;
        }
        if (event.wheel.y < 0 && scale > 15) {
            scale -= 1;
        }
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.keysym.sym == SDLK_z) {

        }

    } else if (event.type == SDL_EVENT_KEY_UP) {

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
    Checkbox("Show axis", &show_axis);
    Checkbox("Show coord lines", &show_coord_lines);
    Checkbox("Show cube", &show_cube);
    if (show_cube) {
        Checkbox("X axis", &ax);
        Checkbox("Y axis", &ay);
        Checkbox("Z axis", &az);
    } else {
        ax = ay = az = false;
    }
    Text("Scale: %.2f", scale);
    Text("Angle X: (RAD)%.2f | (DEG)%d", x_ang, (int)(RAD_TO_DEG(x_ang)/200) % 360);
    Text("Angle Y: (RAD)%.2f | (DEG)%d", y_ang, (int)(RAD_TO_DEG(y_ang)/200) % 360);
    Text("Angle Z: (RAD)%.2f | (DEG)%d", z_ang, (int)(RAD_TO_DEG(z_ang)/200) % 360);

    ImGui::End();

    ImGui::Begin("Mouse info");
    pointf2d_t m = {
        .x = (mouse.pos.x - ((f32)WIN_WIDTH/2))/scale,
        .y = (-mouse.pos.y + ((f32)WIN_HEIGHT/2))/scale,
    };
    Text("Mouse X: %.2f", m.x);
    Text("Mouse Y: %.2f", m.y);

    ImGui::End();

    ImGui::Begin("Cube info");
    ImGui::Text("Cube vertices:");
    static char str[128] = {0};
    static f32 vf3[8][3] = {0};
    f32 v[3];


    for (size_t i = 0; i < 8; i++) {
        vf3[i][0] = cube1.point[i].x;
        vf3[i][1] = cube1.point[i].y;
        vf3[i][2] = cube1.point[i].z;
    }

    for (size_t i = 0; i < 8; i++) {
        char buf[32] = {0};
        snprintf(buf, 32, "V%zu", i);
        InputFloat3(buf, vf3[i]);
        cube1.point[i].x = vf3[i][0];
        cube1.point[i].y = vf3[i][1];
        cube1.point[i].z = vf3[i][2];

    }
    Text("%p, %p", vf3[0], vf3[1]);

//  ImGui::InputFloat3("V", vf3[2]);
//  ImGui::InputFloat3("V", vf3[3]);
//  ImGui::InputFloat3("V", vf3[4]);
//  ImGui::InputFloat3("V", vf3[5]);
//  ImGui::InputFloat3("V", vf3[6]);
//  ImGui::InputFloat3("V", vf3[7]);
//      cube1.point[i].x = vf3[i][0];
//      cube1.point[i].y = vf3[i][1];
//      cube1.point[i].z = vf3[i][2];
    ImGui::End();
}

void render() {
    ImGui::Render();

    SDL_SetRenderDrawColor(sdl.renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(sdl.renderer);
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);

    if (show_axis) {
        render_axis(sdl.renderer, scale);
        if (show_coord_lines) {
            render_coord_lines(sdl.renderer, scale);
        }
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

    SDL_RenderLine(sdl.renderer, WIN_WIDTH/2, WIN_HEIGHT/2, mouse.pos.x, mouse.pos.y);
    SDL_RenderLine(sdl.renderer, mouse.pos.x, WIN_HEIGHT/2, mouse.pos.x, mouse.pos.y);
    SDL_RenderPoint(sdl.renderer, (WIN_WIDTH/2) + ip[0] * scale, (WIN_HEIGHT/2) - ip[1] * scale);

    if (show_cube) {
        cube_t c1 = cube1;
        if (ax) {
            x_ang += 0.02;
        } else {
            //x_ang = 0;
        }
        if (ay) {
            y_ang += 0.02;
        } else {
//        y_ang = 0;
        }
        if (az) {
            z_ang += 0.02;
        } else {
//        z_ang = 0;
        }
        rotate(&c1, CUBE, x_ang, AXIS_X);
        rotate(&c1, CUBE, y_ang, AXIS_Y);
        rotate(&c1, CUBE, z_ang, AXIS_Z);

        project_cube(&c1);
        render_cube(sdl.renderer, c1, scale);
    }
    
//  f32 sinang = sin(angle);
//  f32 cosang = cos(angle);
//  const f64 x_rotation_matrix[3][3] = {
//      {1, 0, 0},
//      {0, cosang, -sinang},
//      {0, sinang, cosang},
//  };
//  const f64 y_rotation_matrix[3][3] = {
//      {cosang, 0, sinang},
//      {0, 1, 0},
//      {-sinang, 0, cosang},
//  };
//  const f64 z_rotation_matrix[3][3] = {
//      {cosang, -sinang, 0},
//      {sinang, cosang, 0},
//      {0, 0, 1},
//  };
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
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    line_t l = line;

//    rotate(&l, LINE, angle, AXIS_X);
    rotate(&l, LINE, angle, AXIS_Y);
    project_line(l);
    
    for (size_t i = 0; i < 2; i++) {
        _normalize_point(&l.p[i], scale);
    };

    SDL_RenderLine(sdl.renderer, l.p[0].x, l.p[0].y, l.p[1].x, l.p[1].y);

    for (f32 i = -100; i < 100; i += 0.20) {
        pointf2d_t p {
            .x = i,
            .y = i * i * i,
        };
        normalize_point2d(&p, scale);
        SDL_RenderPoint(sdl.renderer, p.x, p.y);
    }
    printf("ang: %f\n", angle);
    angle += 0.02;
    

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
