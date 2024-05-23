#include "func.h"
#include "defs.h"

void mat3x3_mul_vec3(vec3f *vec, f64 mat[3][3]) {
    vec3f tmp = {0};
    tmp.x = (vec->x*mat[0][0]) + (vec->y*mat[1][0]) + (vec->z*mat[2][0]);
    tmp.y = (vec->x*mat[0][1]) + (vec->y*mat[1][1]) + (vec->z*mat[2][1]);
    tmp.z = (vec->x*mat[0][2]) + (vec->y*mat[1][2]) + (vec->z*mat[2][2]);

    vec->x = tmp.x;
    vec->y = tmp.y;
    vec->z = tmp.z;
}

void mat4x4_mul_vec4(vec4f *vec, f64 mat[4][4]) {
    vec4f tmp = {0};
    tmp.x = (vec->x*mat[0][0]) + (vec->y*mat[1][0]) + (vec->z*mat[2][0]) + (vec->w*mat[3][0]);
    tmp.y = (vec->x*mat[0][1]) + (vec->y*mat[1][1]) + (vec->z*mat[2][1]) + (vec->w*mat[3][1]);
    tmp.z = (vec->x*mat[0][2]) + (vec->y*mat[1][2]) + (vec->z*mat[2][2]) + (vec->w*mat[3][2]);
    tmp.w = (vec->x*mat[0][3]) + (vec->y*mat[1][3]) + (vec->z*mat[2][3]) + (vec->w*mat[3][3]);

    vec->x = tmp.x;
    vec->y = tmp.y;
    vec->z = tmp.z;
    vec->w = tmp.w;
}

void rotate_shape(void *shape, shape_type shape_type, f64 angle, axis axis) {
    f64 cosang = cos(angle);
    f64 sinang = sin(angle);

    const f64 x_rotation_mat[3][3] = {
        {1, 0, 0},
        {0, cosang, -sinang},
        {0, sinang, cosang},
    };
    const f64 y_rotation_mat[3][3] = {
        {cosang, 0, sinang},
        {0, 1, 0},
        {-sinang, 0, cosang},
    };
    const f64 z_rotation_mat[3][3] = {
        {cosang, -sinang, 0},
        {sinang, cosang, 0},
        {0, 0, 1},
    };

    size_t vec_size = 0;
    switch (shape_type) {
    case POINT:
        vec_size = 1;
        break;
    case LINE:
        vec_size = 2;
        break;
    case TRIANGLE:
        vec_size = 3;
        break;
    case RECTANGLE:
        vec_size = 4;
        break;
    case CUBE:
        vec_size = 8;
        break;
    case CIRCLE:
//      p_size = 1;
//      p = (vf3*)malloc(p_size * sizeof(pointf3d_t));
//      for (size_t i = 0; i < p_size; i++) {
//          p[i] = ((*)shape)[i];
//      }
        break;
    default:
        break;
    }
    vec3f vec[vec_size];
    for (size_t i = 0; i < vec_size; i++) {
        vec[i] = ((vec3f*)shape)[i];
    }

    for (size_t i = 0; i < vec_size; i++) {
        vec3f tmp = {0};
        switch (axis) {
        case AXIS_X:
            tmp.x = vec[i].x;
            tmp.y = vec[i].y * cosang + vec[i].z * sinang;
            tmp.z = vec[i].z * cosang - vec[i].y * sinang;
            break;
        case AXIS_Y:
            tmp.x = vec[i].x * cosang - vec[i].z * sinang;
            tmp.y = vec[i].y;
            tmp.z = vec[i].x * sinang + vec[i].z * cosang;
            break;
        case AXIS_Z: 
            tmp.x = (vec[i].x * cosang + vec[i].y * sinang);
            tmp.y = (vec[i].y * cosang - vec[i].x * sinang);
            tmp.z = vec[i].z;
            break;
        default:
            printf("Invalid axis\n");
            break;
        }
        ((vec3f*)shape)[i] = tmp;
    }
}

void project_shape(void *shape, shape_type type, camera_t camera, projection_type projection_type) {
    f32 znear = camera.znear;
    f32 zfar = camera.zfar;
    f32 fov = camera.fov;
    f32 itan = 1.f/(tan(DEG2RAD(fov/2.f)));
    f32 itana = 1.f/(ASPECT_RATIO * tan(DEG2RAD(fov/2.f)));

    f64 ortho4x4[4][4] = ORTHO_PROJECTION_MAT4x4;
    f64 ortho3x3[3][3] = ORTHO_PROJECTION_MAT3x3;

    f64 perspective_proj_mat[4][4] {
        {itana, 0, 0, 0},
        {0, itan, 0, 0},
        {0, 0, -(zfar+znear)/(zfar-znear), (-2*zfar*znear)/(zfar-znear)},
        {0, 0, -1.f}
    };

    f64 _____perspective_proj_mat[4][4] = {
        {1.f/(ASPECT_RATIO*tan(DEG2RAD(fov/2))), 0, 0, 0},
        {0, 1.f/tan(DEG2RAD(fov/2)), 0, 0},
        {0, 0, (-zfar-znear)/(zfar-znear), (2*zfar*znear)/(znear-zfar)},
        {0, 0, 1.f, 0},
    };

    f64 __perspective_proj_mat[4][4] = {
        {ASPECT_RATIO*itan, 0, 0, 0},
        {0, itan, 0, 0},
        {0, 0, zfar/(zfar-znear), -(zfar*znear)/(zfar-znear)},
        {0, 0, 1, 0},
    };

    size_t p_size = 0;
    if (type == POINT) {
        p_size = 1;
    }
    if (type == LINE) {
        p_size = 2;
    }
    if (type == TRIANGLE) {
        p_size = 3;
    }
    if (type == RECTANGLE) {
        p_size = 4;
    }
    if (type == CUBE) {
        p_size = 8;
    }

    for (size_t i = 0; i < p_size; i++) {
        vec4f p = {0};
        p.x = ((vec3f*)shape)[i].x;
        p.y = ((vec3f*)shape)[i].y;
        p.z = ((vec3f*)shape)[i].z + 4.0f;
        p.w = 1;

        switch (projection_type) {
        case ORTHOGRAPHIC_PROJ:
            mat3x3_mul_vec3((vec3f*)&p, ortho3x3);
            break;
        case PERSPECTIVE_PROJ:
            mat4x4_mul_vec4(&p, _____perspective_proj_mat);
            if (p.w != 0) {
                ((vec3f*)shape)[i].x = p.x/p.w;
                ((vec3f*)shape)[i].y = p.y/p.w;
                ((vec3f*)shape)[i].z = p.z/p.w;
            }
            break;
        }
//      if (projection_type == ORTHOGRAPHIC_PROJ) {
//          mat3x3_mul_vec3((vec3f*)&p, ortho3x3);
//      }
//      if (projection_type == PERSPECTIVE_PROJ) {
//          mat4x4_mul_vec4(&p, _____perspective_proj_mat);
//          if (p.w != 0) {
//              ((vec3f*)shape)[i].x = p.x/p.w;
//              ((vec3f*)shape)[i].y = p.y/p.w;
//              ((vec3f*)shape)[i].z = p.z/p.w;
//          }
//      }
    }
}

void project_line(line_t *line) {
    f64 ortho[3][3] = ORTHO_PROJECTION_MAT3x3;
    for (size_t i = 0; i < 2; i++) {
        mat3x3_mul_vec3(&line->p[i], ortho);
    }
}

void project_triangle(triangle_t *triangle) {
    f64 ortho[3][3] = ORTHO_PROJECTION_MAT3x3;

    for (size_t i = 0; i < 3; i++) {
        f64 tanang = tan((f32)90/2);
        const f64 _perspective_projection_matrix[3][3] = {
            {tanang, 0, 0},
            {0, tanang, 0},
            {0, 0, 1},
        };
        mat3x3_mul_vec3(&triangle->p[i], ortho);
    }
}

void project_rect(rect_t *rect) {
    pointf4d_t p[8] = {0};
    const f32 fov = 50.0f;
    f32 x = (1.0f/(ASPECT_RATIO * tan(DEG2RAD(fov/2.0f)))); 
    f32 y = 1.0f/tan(DEG2RAD(fov/2.0f));
    f32 znear = -30.0f;
    f32 zfar = 30.0f;

    f64 ortho[3][3] = ORTHO_PROJECTION_MAT3x3;

    for (size_t i = 0; i < 4; i++) {
        p[i].x = rect->p[i].x;
        p[i].y = rect->p[i].y;
        p[i].z = rect->p[i].z + 5.f;
        p[i].w = 1;

        f64 _perspective_projection_matrix[4][4] = {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, (-zfar-znear)/(znear-zfar), (2*zfar*znear)/(znear-zfar)},
            {0, 0, 1, 0},
        };
        f64 __perspective_projection_matrix[4][4] = {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, (zfar)/(zfar-znear), 1},
            {0, 0, (-zfar*znear)/(zfar-znear), 0},
        };

//      multiply_matrix(&cube->point[i], perspective_projection_matrix);
        
        mat3x3_mul_vec3(&rect->p[i], ortho);
//      mat_mul_4x4(&p[i], _perspective_projection_matrix);
//      cube->point[i].x = p[i].x;
//      cube->point[i].y = p[i].y;
//      cube->point[i].z = p[i].z;
    }
}

void _project_cube(cube_t *cube, f32 zfar, f32 znear, f32 fov) {
    f32 x = (1.0f/(ASPECT_RATIO * tan(DEG2RAD(fov/2.0f)))); 
    f32 y = 1.0f/tan(DEG2RAD(fov/2.0f));

    f32 top = znear*tan(DEG2RAD(90.0f/2.0f));
    f32 bottom = -top; 
    f32 right = top * ASPECT_RATIO;
    f32 left = -right;

    f32 rl = right - left;
    f32 tb = top - bottom;
    f32 fn = zfar - znear;

    f32 t = 1.0f/tan(DEG2RAD(fov/2));

    f64 ortho[3][3] = ORTHO_PROJECTION_MAT3x3;

        f64 perspective_proj_mat[4][4] = {
            {znear/(znear*tan(DEG2RAD(fov/2.0f))), 0, 0, 0},
            {0, znear/(znear*tan(DEG2RAD(fov/2.0f))), 0, 0},
            {0, 0, -((zfar + znear))/(zfar-znear), -(2*zfar*znear)/(zfar - znear)},
            {0, 0, -1.0f, 0},
        };

        f64 _perspective_proj_mat[4][4] = {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, (-zfar-znear)/(zfar-znear), (2*zfar*znear)/(zfar-znear)},
            {0, 0, 1, 0},
        };
        f64 ___perspective_proj_mat[4][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, (zfar)/(zfar-znear), -1},
            {0, 0, -(zfar*znear)/(zfar-znear), 0},
        };
        f64 _____perspective_proj_mat[4][4] = {
            {1.f/(ASPECT_RATIO*tan(DEG2RAD(fov/2))), 0, 0, 0},
            {0, 1.f/tan(DEG2RAD(fov/2)), 0, 0},
            {0, 0, (-zfar-znear)/(zfar-znear), (2*zfar*znear)/(znear-zfar)},
            {0, 0, 1.f, 0},
        };
        f64 ____perspective_proj_mat[4][4] = {
            {1.f/(ASPECT_RATIO*tan(DEG2RAD(fov/2))), 0, 0, 0},
            {0, 1.f/tan(DEG2RAD(fov/2)), 0, 0},
            {0, 0, (zfar)/(zfar-znear), -1},
            {0, 0, (-zfar*znear)/(zfar-znear), 0},
        };
        f64 __perspective_proj_mat[4][4] = {
            {ASPECT_RATIO*t, 0, 0, 0},
            {0, t, 0, 0},
            {0, 0, zfar/(zfar-znear), -(zfar*znear)/(zfar-znear)},
            {0, 0, 1, 0},
        };

    for (size_t i = 0; i < 8; i++) {
        vec4f p = {0};
        p.x = cube->p[i].x;
        p.y = cube->p[i].y;
        p.z = cube->p[i].z + 5.0f;
        p.w = 1;


//      multiply_matrix(&cube->point[i], perspective_projection_matrix);
        
 //     mat_mul_3x3(&cube->p[i], ortho);
        mat4x4_mul_vec4(&p, _____perspective_proj_mat);
        if (p.w != 0) {
            cube->p[i].x = p.x/p.w;
            cube->p[i].y = p.y/p.w;
            cube->p[i].z = p.z/p.w;
        }
    }
}

void normalize_vec3(vec3f *vec, f32 scale, coord_origin origin) {
    vec2f off = {0};
    if (origin == ZERO) {
        off.x = off.y = 0;
    }
    if (origin == MID_SCREEN) {
        off.x = (f32)WIN_WIDTH/2; 
        off.y = (f32)WIN_HEIGHT/2; 
    }
    vec->x = (vec->x * scale) + off.x; 
    vec->y = (vec->y * scale) + off.y;
    vec->z = (vec->z * scale) + 0;
}

void normalize_vec2(vec2f *vec, f32 scale, coord_origin origin) {
    vec2f off = {0};
    if (origin == ZERO) {
        off.x = off.y = 0;
    }
    if (origin == MID_SCREEN) {
        off.x = (f32)WIN_WIDTH/2; 
        off.y = (f32)WIN_HEIGHT/2; 
    }
    vec->x = (vec->x * scale) + off.x; 
    vec->y = (vec->y * scale) + off.y;
}

vec3f normalize_point(vec3f point, f32 scale) {
    return (vec3f) {
               .x = (point.x * scale) + (f32)WIN_WIDTH/2, 
               .y = (point.y * scale) + (f32)WIN_HEIGHT/2,
               .z = (point.z * scale) + 0
           };
}

void _normalize_point(vec3f *vec, f32 scale) {
    vec->x = (vec->x * scale) + (f32)WIN_WIDTH/2; 
    vec->y = (vec->y * scale) + (f32)WIN_HEIGHT/2;
    vec->z = (vec->z * scale);
}

void normalize_point2d(pointf2d_t *p, f32 scale) {
    p->x = p->x * scale + (f32)WIN_WIDTH/2; 
    p->y = p->y * scale + (f32)WIN_HEIGHT/2;
}

void render_triangle(SDL_Renderer *renderer, triangle_t triangle, f32 scale) {
    vec3f p[3] = {triangle.p[0], triangle.p[1], triangle.p[2]};
    for (size_t i = 0; i < 3; i++) {
        _normalize_point(&p[i], scale);
    }
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderLine(renderer, p[0].x, p[0].y, p[1].x, p[1].y);
    SDL_RenderLine(renderer, p[0].x, p[0].y, p[2].x, p[2].y);
    SDL_RenderLine(renderer, p[1].x, p[1].y, p[2].x, p[2].y);
}

void render_cube(SDL_Renderer *renderer, cube_t cube, f32 scale) {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    triangle_t trig[12] = {0}; 

    trig[0].p[0].x = cube.p[0].x;
    trig[0].p[0].y = cube.p[0].y;
    trig[0].p[0].z = cube.p[0].z;
    trig[0].p[1].x = cube.p[1].x;
    trig[0].p[1].y = cube.p[1].y;
    trig[0].p[1].z = cube.p[1].z;
    trig[0].p[2].x = cube.p[2].x;
    trig[0].p[2].y = cube.p[2].y;
    trig[0].p[2].z = cube.p[2].z;

    trig[1].p[0].x = cube.p[2].x;
    trig[1].p[0].y = cube.p[2].y;
    trig[1].p[0].z = cube.p[2].z;
    trig[1].p[1].x = cube.p[1].x;
    trig[1].p[1].y = cube.p[1].y;
    trig[1].p[1].z = cube.p[1].z;
    trig[1].p[2].x = cube.p[3].x;
    trig[1].p[2].y = cube.p[3].y;
    trig[1].p[2].z = cube.p[3].z;

    trig[2].p[0].x = cube.p[4].x;
    trig[2].p[0].y = cube.p[4].y;
    trig[2].p[0].z = cube.p[4].z;
    trig[2].p[1].x = cube.p[5].x;
    trig[2].p[1].y = cube.p[5].y;
    trig[2].p[1].z = cube.p[5].z;
    trig[2].p[2].x = cube.p[6].x;
    trig[2].p[2].y = cube.p[6].y;
    trig[2].p[2].z = cube.p[6].z;

    trig[3].p[0].x = cube.p[6].x;
    trig[3].p[0].y = cube.p[6].y;
    trig[3].p[0].z = cube.p[6].z;
    trig[3].p[1].x = cube.p[5].x;
    trig[3].p[1].y = cube.p[5].y;
    trig[3].p[1].z = cube.p[5].z;
    trig[3].p[2].x = cube.p[7].x;
    trig[3].p[2].y = cube.p[7].y;
    trig[3].p[2].z = cube.p[7].z;
  
    trig[4].p[0].x = cube.p[4].x;
    trig[4].p[0].y = cube.p[4].y;
    trig[4].p[0].z = cube.p[4].z;
    trig[4].p[1].x = cube.p[0].x;
    trig[4].p[1].y = cube.p[0].y;
    trig[4].p[1].z = cube.p[0].z;
    trig[4].p[2].x = cube.p[6].x;
    trig[4].p[2].y = cube.p[6].y;
    trig[4].p[2].z = cube.p[6].z;
  
    trig[5].p[0].x = cube.p[6].x;
    trig[5].p[0].y = cube.p[6].y;
    trig[5].p[0].z = cube.p[6].z;
    trig[5].p[1].x = cube.p[0].x;
    trig[5].p[1].y = cube.p[0].y;
    trig[5].p[1].z = cube.p[0].z;
    trig[5].p[2].x = cube.p[2].x;
    trig[5].p[2].y = cube.p[2].y;
    trig[5].p[2].z = cube.p[2].z;

    trig[6].p[0].x = cube.p[5].x;
    trig[6].p[0].y = cube.p[5].y;
    trig[6].p[0].z = cube.p[5].z;
    trig[6].p[1].x = cube.p[1].x;
    trig[6].p[1].y = cube.p[1].y;
    trig[6].p[1].z = cube.p[1].z;
    trig[6].p[2].x = cube.p[7].x;
    trig[6].p[2].y = cube.p[7].y;
    trig[6].p[2].z = cube.p[7].z;
  
    trig[7].p[0].x = cube.p[7].x;
    trig[7].p[0].y = cube.p[7].y;
    trig[7].p[0].z = cube.p[7].z;
    trig[7].p[1].x = cube.p[1].x;
    trig[7].p[1].y = cube.p[1].y;
    trig[7].p[1].z = cube.p[1].z;
    trig[7].p[2].x = cube.p[3].x;
    trig[7].p[2].y = cube.p[3].y;
    trig[7].p[2].z = cube.p[3].z;

    trig[8].p[0].x = cube.p[4].x;
    trig[8].p[0].y = cube.p[4].y;
    trig[8].p[0].z = cube.p[4].z;
    trig[8].p[1].x = cube.p[5].x;
    trig[8].p[1].y = cube.p[5].y;
    trig[8].p[1].z = cube.p[5].z;
    trig[8].p[2].x = cube.p[0].x;
    trig[8].p[2].y = cube.p[0].y;
    trig[8].p[2].z = cube.p[0].z;
  
    trig[9].p[0].x = cube.p[0].x;
    trig[9].p[0].y = cube.p[0].y;
    trig[9].p[0].z = cube.p[0].z;
    trig[9].p[1].x = cube.p[5].x;
    trig[9].p[1].y = cube.p[5].y;
    trig[9].p[1].z = cube.p[5].z;
    trig[9].p[2].x = cube.p[1].x;
    trig[9].p[2].y = cube.p[1].y;
    trig[9].p[2].z = cube.p[1].z;

    trig[10].p[0].x = cube.p[6].x;
    trig[10].p[0].y = cube.p[6].y;
    trig[10].p[0].z = cube.p[6].z;
    trig[10].p[1].x = cube.p[7].x;
    trig[10].p[1].y = cube.p[7].y;
    trig[10].p[1].z = cube.p[7].z;
    trig[10].p[2].x = cube.p[2].x;
    trig[10].p[2].y = cube.p[2].y;
    trig[10].p[2].z = cube.p[2].z;
  
    trig[11].p[0].x = cube.p[2].x;
    trig[11].p[0].y = cube.p[2].y;
    trig[11].p[0].z = cube.p[2].z;
    trig[11].p[1].x = cube.p[7].x;
    trig[11].p[1].y = cube.p[7].y;
    trig[11].p[1].z = cube.p[7].z;
    trig[11].p[2].x = cube.p[3].x;
    trig[11].p[2].y = cube.p[3].y;
    trig[11].p[2].z = cube.p[3].z;

    for (size_t i = 0; i < 12; i++) {
        render_triangle(renderer, trig[i], scale);
    }
}

void render_rect(SDL_Renderer *renderer, rect_t rect, f32 scale) {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);

    triangle_t trig[2] = {0}; 
    trig[0].p[0].x = rect.p[0].x;
    trig[0].p[0].y = rect.p[0].y;
    trig[0].p[0].z = rect.p[0].z;
    trig[0].p[1].x = rect.p[1].x;
    trig[0].p[1].y = rect.p[1].y;
    trig[0].p[1].z = rect.p[1].z;
    trig[0].p[2].x = rect.p[2].x;
    trig[0].p[2].y = rect.p[2].y;
    trig[0].p[2].z = rect.p[2].z;

    trig[1].p[0].x = rect.p[1].x;
    trig[1].p[0].y = rect.p[1].y;
    trig[1].p[0].z = rect.p[1].z;
    trig[1].p[1].x = rect.p[2].x;
    trig[1].p[1].y = rect.p[2].y;
    trig[1].p[1].z = rect.p[2].z;
    trig[1].p[2].x = rect.p[3].x;
    trig[1].p[2].y = rect.p[3].y;
    trig[1].p[2].z = rect.p[3].z;

    for (size_t i = 0; i < 2; i++) {
        render_triangle(renderer, trig[i], scale);
    }
}

void render_circle(SDL_Renderer *renderer, circle_t circle, f64 angle) {
    f32 x = circle.radius * 30;
    f32 y = 0;
    f32 err = 0;

    pointf3d_t pc = {
        .x = circle.center.x, 
        .y = circle.center.y,
        .z = 0
    };

    pointf3d_t *result = NULL;
    size_t cnt = 0;

    while (x >= y) {
        pointf3d_t r = {x*30, y, 0};
//      rotate(&r, POINT3D, angle, AXIS_X);
//      project_point(&r);

        pointf3d_t p[8] = {0};
        p[0].x = pc.x + x;
        p[1].x = pc.x + y;
        p[2].x = pc.x - y;
        p[3].x = pc.x - x;
        p[4].x = pc.x - x;
        p[5].x = pc.x - y;
        p[6].x = pc.x + y;
        p[7].x = pc.x + x;

        p[0].y = pc.y + y;
        p[1].y = pc.y + x;
        p[2].y = pc.y + x;
        p[3].y = pc.y + y;
        p[4].y = pc.y - y;
        p[5].y = pc.y - x;
        p[6].y = pc.y - x;
        p[7].y = pc.y - y;

        for (size_t i = 0; i < 8; i++) {
            SDL_RenderPoint(renderer, (f32)WIN_WIDTH/2 + p[i].x, (f32)WIN_HEIGHT/2 - p[i].y);
            printf("p[%zu] = x: %.2f | y: %.2f\n", i, p[i].x, p[i].y);
        }

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
        for (size_t i = 0; i < 8; i++) {
            result = (pointf3d_t*)realloc(result, sizeof(pointf3d_t) * (cnt+1));
            result[cnt] = p[i];
        }
        cnt++;
    }


    free(result);
}



