#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <time.h>
#include <sys\timeb.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"glfw-3.3.7.bin.WIN64/lib-vc2019/glfw3_mt.lib")

const float pi=3.141592653589793;

float itime(){
    struct timeb now;
    ftime(&now);
    return (float)(now.time%(60*60*24))+now.millitm/1e3;
}


void draw_square(){
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-.5,-.5,0);
    glVertex3f(-.5,+.5,0);
    glVertex3f(+.5,-.5,0);
    glVertex3f(+.5,+.5,0);
    glEnd();
}

inline float pos(bool p){return p?1.0f:-1.0f;}

void draw_test_cube(){
    for(int k=0;k<3;k++){
        float rgb[3]={k==0,k==1,k==2};
        for(int j=0;j<2;j++){
            if(j==0) glColor3fv(rgb);
            else glColor3f(1-rgb[0],1-rgb[1],1-rgb[2]);
            glBegin(GL_TRIANGLE_STRIP);
            float vtx[3];
            vtx[k]=.5*pos(j);
            for(int i=0;i<4;i++){
                vtx[(k+1)%3]=+.5*pos(i/2);
                vtx[(k+2)%3]=+.5*pos(i%2);
                glVertex3fv(vtx);
            }
            glEnd();
        }
    }
}

void draw_test_octa(){
    for(int i=0;i<8;i++){
        glColor3f(i&1,i&2,i&4);
        glBegin(GL_TRIANGLES);
        glVertex3f(.5*pos(i&1),0,0);
        glVertex3f(0,.5*pos(i&2),0);
        glVertex3f(0,0,.5*pos(i&4));
        glEnd();
    }
}

void draw_test_tetra(){
    const float a=sqrtf(2)/4;
    float points[4][3]={
        {+a,+.25,+0},
        {-a,+.25,+0},
        {+0,-.25,+a},
        {+0,-.25,-a},
    };
    for(int i=0;i<4;i++){
        glColor3f(points[i][0]+.5,points[i][2]+.5,points[i][2]+.5);
        glBegin(GL_TRIANGLES);
        for(int j=0;j<4;j++) if(i!=j){
            glVertex3fv(points[j]);
        }
        glEnd();
    }
}

inline float map(float t,float t0,float t1,float s0,float s1){
    return s0+(s1-s0)*(t-t0)/(t1-t0);
}

void draw_test_sphere(int res){
    float inc=pi/res;
    float rho=.5;
    for(float phi=0;phi<pi;phi+=inc){
        for(float theta=0;theta<2*pi;theta+=inc){
            glBegin(GL_TRIANGLE_STRIP);
            glColor3f(
                .5+.5*cosf(theta+inc/2)*sinf(phi+inc/2),
                .5+.5*                  cosf(phi+inc/2),
                .5+.5*sinf(theta+inc/2)*sinf(phi+inc/2)
            );
            for(int i=0;i<4;i++){
                float t=theta+inc*(i&1);
                float p=phi+.5*inc*(i&2);
                glVertex3f(
                    rho*cosf(t)*sinf(p),
                    rho*cosf(p),
                    rho*sinf(t)*sinf(p)
                );
            }
            glEnd();
        }
    }
}

typedef struct{
    float x,y;
}vec2;
typedef struct{
    float x,y,z;
}vec3;
typedef struct{
    float x,y,z,w;
}vec4;



void draw_surface(vec3(*f)(vec2,float),vec2 uv0,vec2 uv1,int res){
    float du=(uv1.x-uv0.x)/res;
    float dv=(uv1.y-uv0.y)/res;
    for(float u=uv0.x;u<=uv1.x;u+=du){
        glBegin(GL_TRIANGLE_STRIP);
        for(float v=uv0.y;v<=uv1.y;v+=dv){
            float theta=map(u+du/2,uv0.x,uv1.x,0,pi/2);
            float phi  =map(v+dv/2,uv0.y,uv1.y,0,pi/2);
            glColor3f(
                .5+.5*cosf(theta)*sinf(phi),
                .5+.5*            cosf(phi),
                .5+.5*sinf(theta)*sinf(phi)
            );
            for(int i=0;i<2;i++){
                vec3 p=f((vec2){u+du*i,v},itime());
                glVertex3fv((float*)&p);
            }
        }
        glEnd();
    }
}



inline float deg(float rad){return rad*180/pi;}

void plot_surface(vec3(*f)(vec2,float),vec2 uv0,vec2 uv1,int res){
    assert(glfwInit());
    float width=640,height=480;
    GLFWwindow* window=glfwCreateWindow(width,height, "._____.", NULL, NULL);
    assert(window);
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){

        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        float theta=map(xpos,0,width,pi,-pi);
        float phi=map(ypos,0,height,pi/2,-pi/2);
        int iwidth,iheight;
        glfwGetFramebufferSize(window,&iwidth,&iheight);
        width=iwidth;
        height=iheight;

        glViewport(0,0,iwidth,iheight);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDepthMask(GL_TRUE);

        glPushMatrix();
        glScalef(height/width,1,1);
        float sc=1;
        glScalef(sc,sc,sc);
        glRotatef(deg(phi),1,0,0);
        glRotatef(deg(theta),0,1,0);
        draw_surface(f,uv0,uv1,res);
        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}