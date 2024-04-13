#include "func.h"
#include "defs.h"

void mat_mul_4x4(pointf4d_t *p, f64 matrix[4][4]) {
    pointf4d_t tmp_point = {0};
    tmp_point.x = ((p->x*matrix[0][0]) + (p->y*matrix[1][0]) + (p->z*matrix[2][0]) + (p->w*matrix[3][0]));
    tmp_point.y = ((p->x*matrix[0][1]) + (p->y*matrix[1][1]) + (p->z*matrix[2][1]) + (p->w*matrix[3][1]));
    tmp_point.z = ((p->x*matrix[0][2]) + (p->y*matrix[1][2]) + (p->z*matrix[2][2]) + (p->w*matrix[3][2]));
    tmp_point.w = ((p->x*matrix[0][3]) + (p->y*matrix[1][3]) + (p->z*matrix[2][3]) + (p->w*matrix[3][3]));

    p->x = tmp_point.x;
    p->y = tmp_point.y;
    p->z = tmp_point.z;
    p->w = tmp_point.w;
} 

void mat_mul_3x3(pointf3d_t *p, f64 (*matrix)[3]) {
    pointf3d_t tmp_point = *p;
    tmp_point.x = (p->x*matrix[0][0]) + (p->y*matrix[1][0]) + (p->z*matrix[2][0]);
    tmp_point.y = (p->x*matrix[0][1]) + (p->y*matrix[1][1]) + (p->z*matrix[2][1]);
    tmp_point.z = (p->x*matrix[0][2]) + (p->y*matrix[1][2]) + (p->z*matrix[2][2]);

    p->x = tmp_point.x;
    p->y = tmp_point.y;
    p->z = tmp_point.z;
} 

void rotate_line(pointf3d_t *line, f64 angle, axis axis) {
    f64 cosang = cos(angle);
    f64 sinang = sin(angle);

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

    for (size_t i = 0; i < 2; i++) {
        pointf3d_t tmp = {0};
        switch (axis) {
        case AXIS_X:
            tmp.y = line[i].y * cosang + line[i].z * sinang;
            tmp.z = line[i].z * cosang - line[i].y * sinang;
            line[i].y = tmp.y;
            line[i].z = tmp.z;
            break;
        case AXIS_Y:
            tmp.x = line[i].x * cosang - line[i].z * sinang;
            tmp.z = line[i].x * sinang + line[i].z * cosang;
            line[i].x = tmp.x;
            line[i].z = tmp.z;
            break;
        case AXIS_Z:
            tmp.x = line[i].x * cosang + line[i].y * sinang;
            tmp.y = line[i].y * cosang - line[i].x * sinang;
            line[i].x = tmp.x;
            line[i].y = tmp.y;
            break;
        default:
            printf("Invalid axis\n");
            break;
        }
    }
}

void rotate_triangle(triangle_t *triangle, f64 angle, axis axis) {
    f64 cosang = cos(angle);
    f64 sinang = sin(angle);

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

    for (size_t i = 0; i < 3; i++) {
        pointf3d_t tmp = {0};
        switch (axis) {
        case AXIS_X:
            tmp.y = triangle->point[i].y * cosang + triangle->point[i].z * sinang;
            tmp.z = triangle->point[i].z * cosang - triangle->point[i].y * sinang;
            triangle->point[i].y = tmp.y;
            triangle->point[i].z = tmp.z;
            break;
        case AXIS_Y:
            tmp.x = triangle->point[i].x * cosang - triangle->point[i].z * sinang;
            tmp.z = triangle->point[i].x * sinang + triangle->point[i].z * cosang;
            triangle->point[i].x = tmp.x;
            triangle->point[i].z = tmp.z;
            break;
        case AXIS_Z:
            tmp.x = triangle->point[i].x * cosang + triangle->point[i].y * sinang;
            tmp.y = triangle->point[i].y * cosang - triangle->point[i].x * sinang;
            triangle->point[i].x = tmp.x;
            triangle->point[i].y = tmp.y;
            break;
        default:
            printf("Invalid axis\n");
            break;
        }
    }

}

void rotate_cube(cube_t *cube, f64 angle, axis axis) {
    f64 cosang = cos(angle);
    f64 sinang = sin(angle);

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

    for (usize i = 0; i < 8; i++) {
        pointf3d_t tmp = {0};
        switch (axis) {
        case AXIS_X:
            tmp.y = cube->point[i].y * cosang + cube->point[i].z * sinang;
            tmp.z = cube->point[i].z * cosang - cube->point[i].y * sinang;
            cube->point[i].y = tmp.y;
            cube->point[i].z = tmp.z;
            break;
        case AXIS_Y:
            tmp.x = cube->point[i].x * cosang - cube->point[i].z * sinang;
            tmp.z = cube->point[i].x * sinang + cube->point[i].z * cosang;
            cube->point[i].x = tmp.x;
            cube->point[i].z = tmp.z;
            break;
        case AXIS_Z:
            tmp.x = cube->point[i].x * cosang + cube->point[i].y * sinang;
            tmp.y = cube->point[i].y * cosang - cube->point[i].x * sinang;
            cube->point[i].x = tmp.x;
            cube->point[i].y = tmp.y;
            break;
        default:
            printf("Invalid axis\n");
            break;
        }
        printf("point[%zu] rotate_cube: %.2f | %.2f | %.2f\n", i, cube->point[i].x, cube->point[i].y, cube->point[i].z);
    }
}

void rotate(void *data, shape shape, f64 angle, axis axis) {
    f64 cosang = cos(angle);
    f64 sinang = sin(angle);

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

    size_t p_size = 0;
    pointf3d_t *p = NULL;
    switch (shape) {
    case POINT3D:
        p_size = 1;
        p = (pointf3d_t*)malloc(p_size * sizeof(pointf3d_t));
        for (size_t i = 0; i < p_size; i++) {
            p[i] = ((pointf3d_t*)data)[i];
        }
        break;
    case LINE:
        p_size = 2;
        p = (pointf3d_t*)malloc(p_size * sizeof(pointf3d_t));
        for (size_t i = 0; i < p_size; i++) {
            p[i] = ((pointf3d_t*)data)[i];
        }
        break;
    case TRIANGLE:
        p_size = 3;
        p = (pointf3d_t*)malloc(p_size * sizeof(triangle_t));
        for (size_t i = 0; i < p_size; i++) {
            p[i] = ((triangle_t*)data)->point[i];
        }
        break;
    case CUBE:
        p_size = 8;
        p = (pointf3d_t*)malloc(p_size * sizeof(cube_t));
        for (size_t i = 0; i < p_size; i++) {
            p[i] = ((cube_t*)data)->point[i];
        }
        break;
    case CIRCLE:
        p_size = 1;
        p = (pointf3d_t*)malloc(p_size * sizeof(pointf3d_t));
        for (size_t i = 0; i < p_size; i++) {
            p[i] = ((pointf3d_t*)data)[i];
        }
        break;
    default:
        break;
    }

    for (size_t i = 0; i < p_size; i++) {
        pointf3d_t tmp = {0};
        switch (axis) {
        case AXIS_X:
            tmp.x = ((pointf3d_t*)data)[i].x;
            tmp.y = p[i].y * cosang + p[i].z * sinang;
            tmp.z = p[i].z * cosang - p[i].y * sinang;
            break;
        case AXIS_Y:
            tmp.x = p[i].x * cosang - p[i].z * sinang;
            tmp.y = ((pointf3d_t*)data)[i].y;
            tmp.z = p[i].x * sinang + p[i].z * cosang;
            break;
        case AXIS_Z: 
            tmp.x = (p[i].x * cosang + p[i].y * sinang);
            tmp.y = (p[i].y * cosang - p[i].x * sinang);
            tmp.z = ((pointf3d_t*)data)[i].z;
            break;
        default:
            printf("Invalid axis\n");
            break;
        }
        if (shape == TRIANGLE) {
            ((triangle_t*)data)->point[i] = tmp;
        }
        if (shape == CUBE) {
            ((cube_t*)data)->point[i] = tmp;
        }
        if (shape == CIRCLE) {
            ((pointf3d_t*)data)[i] = tmp;
        }
    }
    free(p);
}

void project_line(pointf3d_t line[2]) {
    f64 ortho[3][3] = ORTHO_PROJECTION_MAT;
    for (size_t i = 0; i < 2; i++) {
        mat_mul_3x3(&line[i], ortho);
    }

}

void project_triangle(triangle_t *triangle) {
    f64 ortho[3][3] = ORTHO_PROJECTION_MAT;

    for (usize i = 0; i < 3; i++) {
        f64 tanang = tan((f32)90/2);
        const f64 _perspective_projection_matrix[3][3] = {
            {tanang, 0, 0},
            {0, tanang, 0},
            {0, 0, 1},
        };
        mat_mul_3x3(&triangle->point[i], ortho);
    }
}

void project_cube(cube_t *cube) {
    pointf4d_t p[8] = {0};
    const f32 fov = 50.0f;
    f32 x = (1.0f/(ASPECT_RATIO * tan(DEG_TO_RAD(fov/2.0f)))); 
    f32 y = 1.0f/tan(DEG_TO_RAD(fov/2.0f));
    f32 znear = -30.0f;
    f32 zfar = 30.0f;

    const f64 ortho[3][3] = ORTHO_PROJECTION_MAT;

    for (size_t i = 0; i < 8; i++) {
        p[i].x = cube->point[i].x;
        p[i].y = cube->point[i].y;
        p[i].z = cube->point[i].z;
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
        
        mat_mul_4x4(&p[i], _perspective_projection_matrix);
        cube->point[i].x = p[i].x;
        cube->point[i].y = p[i].y;
        cube->point[i].z = p[i].z;
    }
}

void project_circle(circle_t *circle) {
    for (size_t i = 0; i < 1; i++) {
//        mat_mul_3x3(circle->radius, ortho_projection_mat);
    }

}

void project_point(pointf3d_t *point) {
    f64 ortho[3][3] = ORTHO_PROJECTION_MAT;
    for (size_t i = 0; i < 1; i++) {
        mat_mul_3x3(point, ortho);
    }

}





pointf3d_t normalize_point(pointf3d_t point) {
    return (pointf3d_t) {
               .x = ((point.x*30) + (f32)WIN_WIDTH/2), 
               .y = ((point.y*30) + (f32)WIN_HEIGHT/2),
               .z = (point.z*30 + 0)
           };
}

void _normalize_point(pointf3d_t *point) {
    point->x = point->x*30 + (f32)WIN_WIDTH/2; 
    point->y = point->y*30 + (f32)WIN_HEIGHT/2;
    point->z = point->z*30 + 0;
}

void render_triangle(SDL_Renderer *renderer, triangle_t triangle) {
    pointf3d_t p[3] = {0};
    for (size_t i = 0; i < 3; i++) {
        p[i] = normalize_point(triangle.point[i]);
    }
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderLine(renderer, p[0].x, p[0].y, p[1].x, p[1].y);
    SDL_RenderLine(renderer, p[0].x, p[0].y, p[2].x, p[2].y);
    SDL_RenderLine(renderer, p[1].x, p[1].y, p[2].x, p[2].y);
}

void render_cube(SDL_Renderer *renderer, cube_t cube) {
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
    cube_t norm_cube = {0};
    for (size_t i = 0; i < 8; i++) {
        norm_cube.point[i] = normalize_point(cube.point[i]);
    }
    triangle_t trig[12] = {0}; 

    trig[0].point[0].x = cube.point[0].x;
    trig[0].point[0].y = cube.point[0].y;
    trig[0].point[0].z = cube.point[0].z;
    trig[0].point[1].x = cube.point[1].x;
    trig[0].point[1].y = cube.point[1].y;
    trig[0].point[1].z = cube.point[1].z;
    trig[0].point[2].x = cube.point[2].x;
    trig[0].point[2].y = cube.point[2].y;
    trig[0].point[2].z = cube.point[2].z;

    trig[1].point[0].x = cube.point[2].x;
    trig[1].point[0].y = cube.point[2].y;
    trig[1].point[0].z = cube.point[2].z;
    trig[1].point[1].x = cube.point[1].x;
    trig[1].point[1].y = cube.point[1].y;
    trig[1].point[1].z = cube.point[1].z;
    trig[1].point[2].x = cube.point[3].x;
    trig[1].point[2].y = cube.point[3].y;
    trig[1].point[2].z = cube.point[3].z;

    trig[2].point[0].x = cube.point[4].x;
    trig[2].point[0].y = cube.point[4].y;
    trig[2].point[0].z = cube.point[4].z;
    trig[2].point[1].x = cube.point[5].x;
    trig[2].point[1].y = cube.point[5].y;
    trig[2].point[1].z = cube.point[5].z;
    trig[2].point[2].x = cube.point[6].x;
    trig[2].point[2].y = cube.point[6].y;
    trig[2].point[2].z = cube.point[6].z;

    trig[3].point[0].x = cube.point[6].x;
    trig[3].point[0].y = cube.point[6].y;
    trig[3].point[0].z = cube.point[6].z;
    trig[3].point[1].x = cube.point[5].x;
    trig[3].point[1].y = cube.point[5].y;
    trig[3].point[1].z = cube.point[5].z;
    trig[3].point[2].x = cube.point[7].x;
    trig[3].point[2].y = cube.point[7].y;
    trig[3].point[2].z = cube.point[7].z;
  
    trig[4].point[0].x = cube.point[4].x;
    trig[4].point[0].y = cube.point[4].y;
    trig[4].point[0].z = cube.point[4].z;
    trig[4].point[1].x = cube.point[0].x;
    trig[4].point[1].y = cube.point[0].y;
    trig[4].point[1].z = cube.point[0].z;
    trig[4].point[2].x = cube.point[6].x;
    trig[4].point[2].y = cube.point[6].y;
    trig[4].point[2].z = cube.point[6].z;
  
    trig[5].point[0].x = cube.point[6].x;
    trig[5].point[0].y = cube.point[6].y;
    trig[5].point[0].z = cube.point[6].z;
    trig[5].point[1].x = cube.point[0].x;
    trig[5].point[1].y = cube.point[0].y;
    trig[5].point[1].z = cube.point[0].z;
    trig[5].point[2].x = cube.point[2].x;
    trig[5].point[2].y = cube.point[2].y;
    trig[5].point[2].z = cube.point[2].z;

    trig[6].point[0].x = cube.point[5].x;
    trig[6].point[0].y = cube.point[5].y;
    trig[6].point[0].z = cube.point[5].z;
    trig[6].point[1].x = cube.point[1].x;
    trig[6].point[1].y = cube.point[1].y;
    trig[6].point[1].z = cube.point[1].z;
    trig[6].point[2].x = cube.point[7].x;
    trig[6].point[2].y = cube.point[7].y;
    trig[6].point[2].z = cube.point[7].z;
  
    trig[7].point[0].x = cube.point[7].x;
    trig[7].point[0].y = cube.point[7].y;
    trig[7].point[0].z = cube.point[7].z;
    trig[7].point[1].x = cube.point[1].x;
    trig[7].point[1].y = cube.point[1].y;
    trig[7].point[1].z = cube.point[1].z;
    trig[7].point[2].x = cube.point[3].x;
    trig[7].point[2].y = cube.point[3].y;
    trig[7].point[2].z = cube.point[3].z;

    trig[8].point[0].x = cube.point[4].x;
    trig[8].point[0].y = cube.point[4].y;
    trig[8].point[0].z = cube.point[4].z;
    trig[8].point[1].x = cube.point[5].x;
    trig[8].point[1].y = cube.point[5].y;
    trig[8].point[1].z = cube.point[5].z;
    trig[8].point[2].x = cube.point[0].x;
    trig[8].point[2].y = cube.point[0].y;
    trig[8].point[2].z = cube.point[0].z;
  
    trig[9].point[0].x = cube.point[0].x;
    trig[9].point[0].y = cube.point[0].y;
    trig[9].point[0].z = cube.point[0].z;
    trig[9].point[1].x = cube.point[5].x;
    trig[9].point[1].y = cube.point[5].y;
    trig[9].point[1].z = cube.point[5].z;
    trig[9].point[2].x = cube.point[1].x;
    trig[9].point[2].y = cube.point[1].y;
    trig[9].point[2].z = cube.point[1].z;

    trig[10].point[0].x = cube.point[6].x;
    trig[10].point[0].y = cube.point[6].y;
    trig[10].point[0].z = cube.point[6].z;
    trig[10].point[1].x = cube.point[7].x;
    trig[10].point[1].y = cube.point[7].y;
    trig[10].point[1].z = cube.point[7].z;
    trig[10].point[2].x = cube.point[2].x;
    trig[10].point[2].y = cube.point[2].y;
    trig[10].point[2].z = cube.point[2].z;
  
    trig[11].point[0].x = cube.point[2].x;
    trig[11].point[0].y = cube.point[2].y;
    trig[11].point[0].z = cube.point[2].z;
    trig[11].point[1].x = cube.point[7].x;
    trig[11].point[1].y = cube.point[7].y;
    trig[11].point[1].z = cube.point[7].z;
    trig[11].point[2].x = cube.point[3].x;
    trig[11].point[2].y = cube.point[3].y;
    trig[11].point[2].z = cube.point[3].z;

    for (size_t i = 0; i < 12; i++) {
        render_triangle(renderer, trig[i]);
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
            SDL_RenderPoint(renderer, WIN_WIDTH/2 + p[i].x, WIN_HEIGHT/2 - p[i].y);
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

    for (size_t i = 0; i < cnt; i++) {
//      rotate(&result[i], POINT3D, angle, AXIS_X);
        project_point(&result[i]);

//        pointf3d_t po = normalize_point(result[i]);
        SDL_RenderPoint(renderer, (WIN_WIDTH/2) + result[i].x, (WIN_HEIGHT/2) - result[i].y);
    }

    free(result);
}



