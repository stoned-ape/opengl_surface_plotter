#include <math.h>
#include "plot_surface.h"

vec3 surf_sphere(vec2 uv,float t){
    return (vec3){
        .5*cosf(uv.x)*sinf(uv.y),
        .5*           cosf(uv.y),
        .5*sinf(uv.x)*sinf(uv.y)
    };
}

vec3 surf_parabola(vec2 uv,float t){
    return (vec3){
        uv.x,
        uv.x*uv.x+uv.y*uv.y-.7,
        uv.y
    };
}

vec3 surf_hyperbola(vec2 uv,float t){
    return (vec3){
        uv.x,
        uv.x*uv.y,
        uv.y
    };
}

vec3 surf_wave(vec2 uv,float t){
    return (vec3){
        uv.x,
        .4*sinf(5*(uv.x*uv.x+uv.y*uv.y)-t),
        uv.y
    };
}

vec3 surf_torus(vec2 uv,float t){
    float R=.7,r=.2;
    return (vec3){
        cosf(uv.x)*(R+r*sinf(uv.y)),
        r*cosf(uv.y),
        sinf(uv.x)*(R+r*sinf(uv.y)),
    };
}

int main(void){
    // plot_surface(surf_sphere,(vec2){0,0},(vec2){2*pi,pi},20);
    // plot_surface(surf_parabola,(vec2){-1,-1},(vec2){1,1},20);
    // plot_surface(surf_hyperbola,(vec2){-1,-1},(vec2){1,1},20);
    // plot_surface(surf_wave,(vec2){-1,-1},(vec2){1,1},20);
    plot_surface(surf_torus,(vec2){0,0},(vec2){2*pi,2*pi},20);

    
}

