#include "defs.h"
#include "func.h"

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} sdl;

struct {
    line_t ax;
    line_t ay;
    line_t az;
    bool x_spin;
    bool y_spin;
    bool z_spin;
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
projection_type curr_proj = ORTHOGRAPHIC_PROJ;
line_t line = {
    -3, 0, 0,
    3, -3, 0
};

camera_t cam = {
    .pos = {0, 0, 0, 0},
    .fov = 90.f,
    .znear = -1.f,
    .zfar = 1.f,
};

const f64 ortho[3][3] = ORTHO_PROJECTION_MAT3x3;

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
    SDL_SetRenderDrawColor(sdl.renderer, 
                           COLOR_WHITE.r, 
                           COLOR_WHITE.g, 
                           COLOR_WHITE.b, 
                           COLOR_WHITE.a);
//  project_line(&axis_lines.ax);
    project_shape(&axis_lines.ax, LINE, cam, ORTHOGRAPHIC_PROJ);
    project_shape(&axis_lines.ay, LINE, cam, ORTHOGRAPHIC_PROJ);
    project_shape(&axis_lines.az, LINE, cam, ORTHOGRAPHIC_PROJ);
//  project_line(&axis_lines.ay);
//  project_line(&axis_lines.az);

    normalize_vec3(&axis_lines.ax.p[0], scale, MID_SCREEN);
    normalize_vec3(&axis_lines.ax.p[1], scale, MID_SCREEN);
    normalize_vec3(&axis_lines.ay.p[0], scale, MID_SCREEN);
    normalize_vec3(&axis_lines.ay.p[1], scale, MID_SCREEN);
    normalize_vec3(&axis_lines.az.p[0], scale, MID_SCREEN);
    normalize_vec3(&axis_lines.az.p[1], scale, MID_SCREEN);

//  _normalize_point(&axis_lines.ax.p[0], scale);
//  _normalize_point(&axis_lines.ax.p[1], scale);
//  _normalize_point(&axis_lines.ay.p[0], scale);
//  _normalize_point(&axis_lines.ay.p[1], scale);
//  _normalize_point(&axis_lines.az.p[0], scale);
//  _normalize_point(&axis_lines.az.p[1], scale);

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

}

void print_point(pointf3d_t point) {
    printf("(X, Y, Z) %f, %f, %f\n", point.x, point.y, point.z);
}

void render_coord_lines(SDL_Renderer *renderer, f32 scale) {
    vec3f coord_line[2] = {
        {1, 0, 0},
        {1, 0, 0}
    };


//  project_point(&coord_line[0]);
//  project_point(&coord_line[1]);

    while (normalize_point(coord_line[0], scale).x <= WIN_WIDTH) {
        vec3f p1 = normalize_point(coord_line[0], scale);
        vec3f p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x, p1.y-5, p2.x, p2.y+5);
        coord_line[0].x++;
        coord_line[1].x++;
    }

    coord_line[0].x = -1;
    coord_line[1].x = -1;
    while (normalize_point(coord_line[0], scale).x >= 0) {
        vec3f p1 = normalize_point(coord_line[0], scale);
        vec3f p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x, p1.y-5, p2.x, p2.y+5);
        coord_line[0].x--;
        coord_line[1].x--;
    }

    coord_line[0].x = 0;
    coord_line[1].x = 0;
    coord_line[0].y = 1;
    coord_line[1].y = 1;
    while (normalize_point(coord_line[0], scale).y <= WIN_HEIGHT) {
        vec3f p1 = normalize_point(coord_line[0], scale);
        vec3f p2 = normalize_point(coord_line[1], scale);

        SDL_RenderLine(renderer, p1.x-5, p1.y, p2.x+5, p2.y);
        coord_line[0].y++;
        coord_line[1].y++;
    }

    coord_line[0].x = 0;
    coord_line[1].x = 0;
    coord_line[0].y = -1;
    coord_line[1].y = -1;
    while (normalize_point(coord_line[0], scale).y >= 0) {
        vec3f p1 = normalize_point(coord_line[0], scale);
        vec3f p2 = normalize_point(coord_line[1], scale);

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
            scale += 2;
        }
        if (event.wheel.y < 0 && scale > 15) {
            scale -= 2;
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
    
    static int e = 0;
    Text("Projection type");
    RadioButton("Orthographic", &e, 0);
    SameLine();
    RadioButton("Perspective", &e, 1);
    if (e == 0) {
        curr_proj = ORTHOGRAPHIC_PROJ;
    } else {
        curr_proj = PERSPECTIVE_PROJ;
    }
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
    Text("Angle X: (RAD)%.2f | (DEG)%d", x_ang, (int)(RAD2DEG(x_ang)/200) % 360);
    Text("Angle Y: (RAD)%.2f | (DEG)%d", y_ang, (int)(RAD2DEG(y_ang)/200) % 360);
    Text("Angle Z: (RAD)%.2f | (DEG)%d", z_ang, (int)(RAD2DEG(z_ang)/200) % 360);

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
        vf3[i][0] = cube1.p[i].x;
        vf3[i][1] = cube1.p[i].y;
        vf3[i][2] = cube1.p[i].z;
    }

    for (size_t i = 0; i < 8; i++) {
        char buf[32] = {0};
        snprintf(buf, 32, "V%zu", i);
        InputFloat3(buf, vf3[i]);
        cube1.p[i].x = vf3[i][0];
        cube1.p[i].y = vf3[i][1];
        cube1.p[i].z = vf3[i][2];
    }
    Text("%p, %p", vf3[0], vf3[1]);
    ImGui::End();
}

void render() {
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
    SDL_RenderClear(sdl.renderer);
    SDL_SetRenderDrawColor(sdl.renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);

    Begin("Camera info");
    InputFloat("znear", &cam.znear);
    InputFloat("zfar", &cam.zfar);
    InputFloat("fov", &cam.fov);
    End();

    axis_lines.ax.p[0] = (vec3f) {
        .x = (f32)-WIN_WIDTH/scale,
        .y = 0,
        .z = 0
    };
    axis_lines.ax.p[1] = (vec3f) {
        .x = (f32)WIN_WIDTH/scale,
        .y = 0,
        .z = 0
    };
    axis_lines.ay.p[0] = (vec3f) {
        .x = 0,
        .y = (f32)-WIN_HEIGHT/scale,
        .z = 0
    };
    axis_lines.ay.p[1] = (vec3f) {
        .x = 0, 
        .y = (f32)WIN_HEIGHT/scale,
        .z = 0
    };
    axis_lines.az.p[0] = (vec3f) {
        .x = 0,
        .y = 0,
        .z = -100
    };
    axis_lines.az.p[1] = (vec3f) {
        .x = 0, 
        .y = 0,
        .z = 100
    };

    pointf2d_t px;
    if (!mouse.pressed) {
        mouse.last_pos.x = mouse.last_pos.y = 0;
    }
    f32 co = -(mouse.pos.y - (WIN_HEIGHT/2));
    f32 ca = mouse.pos.x - (WIN_WIDTH/2);
    f32 hyp = tan(co/ca);
    f32 ang = atan2f(co, ca);
    int conv = RAD2DEG(ang);

    if (mouse.pressed) {
        if (mouse.pos.x < mouse.last_pos.x || mouse.pos.x > mouse.last_pos.x) {
            rotate_shape(&axis_lines.ax, LINE, ang, AXIS_X);
        }
        rotate_shape(&axis_lines.ax, LINE, ang, AXIS_Y);
        rotate_shape(&axis_lines.ay, LINE, ang, AXIS_Z);
    }

    if (show_axis) {
        render_axis(sdl.renderer, scale);
        if (show_coord_lines) {
            render_coord_lines(sdl.renderer, scale);
        }
    }
    

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
        }
        if (ay) {
            y_ang += 0.02;
        }
        if (az) {
            z_ang += 0.02;
        }
        rotate_shape(&c1, CUBE, x_ang, AXIS_X);
        rotate_shape(&c1, CUBE, y_ang, AXIS_Y);
        rotate_shape(&c1, CUBE, z_ang, AXIS_Z);

        project_shape(&c1, CUBE, cam, curr_proj);
        render_cube(sdl.renderer, c1, scale);
    }

    i32 a = sqrt(2);
    static f32 inc = 1;
    static f32 r = 3;
    if (r >= 3) {
        inc *= -1;
    } else if (r < 0) {
        inc *= -1;
    }
//  r += (0.05 * inc);
    
    for (f32 i = 0; i < 2*PI; i += 0.01) {
        for (i32 j = 0; j < 1; j++) {
            vec3f p1 = {0}; 
            p1.x = ((r-j)*cos(i));
            p1.y = ((r-j)*sin(i));
            p1.z = 0;
            vec3f p2 = {0}; 
            p2.x = 0;
            p2.y = ((r-j)*sin(i));
            p2.z = ((r-j)*cos(i));
            vec3f p3 = {0}; 
            p3.x = ((r-j)*cos(i));
            p3.y = 0;
            p3.z = ((r-j)*sin(i));

            rotate_shape(&p1, POINT, angle, AXIS_Y);
            project_shape(&p1, POINT, cam, curr_proj);
            normalize_vec3(&p1, scale, MID_SCREEN);
            rotate_shape(&p2, POINT, angle, AXIS_Y);
            project_shape(&p2, POINT, cam, curr_proj);
            normalize_vec3(&p2, scale, MID_SCREEN);
            rotate_shape(&p3, POINT, angle, AXIS_X);
            project_shape(&p3, POINT, cam, curr_proj);
            normalize_vec3(&p3, scale, MID_SCREEN);

            SDL_RenderPoint(sdl.renderer, p1.x, p1.y);
            SDL_RenderPoint(sdl.renderer, p2.x, p2.y);
            SDL_RenderPoint(sdl.renderer, p3.x, p3.y);
        }
    }

    vec3f p = {0};
    p.x = (3*cos(angle));
    p.y = (3*sin(angle));
    normalize_vec3(&p, scale, MID_SCREEN);
    project_shape(&p, CIRCLE, cam, PERSPECTIVE_PROJ);

    SDL_RenderPoint(sdl.renderer, p.x, p.y);


    angle += 0.02;

    ImGui::Render();
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
