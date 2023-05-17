#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "stb-master/stb_easy_font.h"
#include "mEnu.h"
#include "tExtur.h"

using namespace std;

GLuint texture;
GLuint sprite;
GLuint background;
//////
GLuint block;
//////

int width = 1080;
int height = 720;
float gravity = 0.6;
float speed = 3;
float currentframe=2;
float chSiX;
float chSiY;
bool directional;
bool gameState=0;
int n=0;

string TileMap[32] = {
    /*12345678901234567890123456789012*/
    "           BBBBBBBB             ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "    BBBBBBB                     ",
    "                  BBBBBBBBBB    ",
    "                                ",
    "                                ",
    "                                ",
    "      BBBBBBBB                  ",
    "                                ",
    "                  BBBBB         ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
};

typedef struct
{
    float left, right, top, buttom, textX, textY;
} sritCoord;

typedef struct
{
    float x, y, dx, dy;
    //////////
    const float HERO_HEIGHT = 95.0f;
    const float HERO_WIDTH = 100.0f;
    /////////
} Hero;

Hero pers;
sritCoord vrtcoord;
sritCoord BackGroundCoord;

void Hero_Init(Hero *obj, float x1, float y1, float dx1, float dy1)
{ // character position spawn set
    obj->x=x1;
    obj->y=y1;
    obj->dx=dx1;
    obj->dy=dy1;
}

void Reflect (float *da, float *a, BOOL cond, float wall)
{
    if (!cond) return;
    *da*=-0;
    *a=wall;
}

void Hero_Move(Hero *obj)
{
    obj->y+=obj->dy;
    obj->x+=obj->dx;
    Reflect(&obj->dy, &obj->y, obj->y<0,0); // anti sunk
    Reflect(&obj->dx,&obj->x, obj->x<0, 0); // left border bump
    Reflect(&obj->dx,&obj->x, obj->x>width-chSiX-0, width-chSiX-0);// right border bump
    obj->dy-=gravity;
    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx-=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=7;
        directional=1;

    }

    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx+=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=7;
        directional=0;


    }
    if (GetKeyState(VK_UP)<0 && (obj->y<500) && gameState==1)
    {
        obj->dy =speed*1.2;
        obj->y+=obj->dy;
    }
}



void Sprite_animation(GLuint texture, int n, Hero *obj)
{

    static float svertix[]= {0,0, 105,0, 105,105, 0,105};
    static float TexCord[]= {0,0, 0.12,0, 0.12,1, 0,1};
    static float line[]= {0,0, 100,0, 100,95, 0,95};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.7);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static float spriteXsize=800;
    static float spriteYsize=300;
    static float charsizey=100;
    static float charsizex=100;
    vrtcoord.left=(charsizex*n)/spriteXsize;
    vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
    vrtcoord.top=(charsizey*0)/spriteYsize;
    vrtcoord.buttom=vrtcoord.top+(charsizey/spriteYsize);

    TexCord[1] = TexCord[3] = vrtcoord.buttom;
    TexCord[5] = TexCord[7] = vrtcoord.top;

    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }
    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }




    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_ALPHA_TEST);

    glLineWidth(3);
    glVertexPointer(2,GL_FLOAT,0,line);


    glDrawArrays(GL_LINE_LOOP, 0,4); // hit box lines draw


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    chSiX=charsizex;
    chSiY=charsizey;

}

void Main_Init()
{
    Hero_Init(&pers, width/2.0,height/2.0,0,0);
    Load_Texture("spritesheet_HQ.png", &sprite, GL_CLAMP, GL_CLAMP, GL_NEAREST);
    Load_Texture("background.png", &background, GL_REPEAT, GL_CLAMP, GL_NEAREST);
    //Load_Texture("block.jpg", &block, GL_REPEAT, GL_CLAMP, GL_NEAREST);
}

void Menu_Init(){ // buttons initialization
    Menu_AddButton("Play", 400.0, 10.0, 130.0, 90.0, 4);
    Menu_AddButton("Pause", 540.0, 10.0, 130.0, 40.0, 4);
    Menu_AddButton("Quit", 540.0, 60.0, 130.0, 40.0, 4);
}


void Show_Background(GLuint texture)
{ // BG show settings
    static float svertix[]= {-1080,0, -1080,700, 1080*2,700, 1080*2,0};
    static float TexCord[]= {-1,1, -1,0, 2,0, 2,1 }; // define coord of 1st element array

    glClearColor(0,0,0,0); // clear values for color buffers

    glEnable(GL_TEXTURE_2D); // on/off opengl (ability)
    glBindTexture(GL_TEXTURE_2D, texture); // create named texture, connected to tex obj
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord); // array for tex coords
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY); // off vertex array
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // cords array for texture
}


void MouseDown()
{ // mouse actions processing
    int buttonId = Menu_MouseDown();
    if(buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    printf("%s\n",name, &buttonId);
    switch (buttonId)
    {
    case 0:
        if (gameState==0) gameState=1;
        break;
    case 1:
        if (gameState==1) gameState=0;
        break;
    case 2:
        PostQuitMessage(0);
        break;

    }
}

void Collision(Hero& hero)
{ // collision check function
    const float tile_size = 31.5f;

    // character edges coords set
    float left   = hero.x;
    float right  = hero.x + hero.HERO_WIDTH;
    float top    = hero.y;
    float bottom = hero.y + hero.HERO_HEIGHT;

    // interacted tiles' numbers
    int left_tile   = (int)(left / tile_size);
    int right_tile  = (int)(right / tile_size);
    int top_tile    = (int)(top / tile_size);
    int bottom_tile = (int)(bottom / tile_size);

    // every character-tile interaction check
    for (int i = top_tile; i <= bottom_tile; i++) {
        for (int j = left_tile; j <= right_tile; j++) {

            // calculate overlap between character and tile
            float overlapLeft = right - j * tile_size;
            float overlapRight = (j + 1) * tile_size - left;
            float overlapTop = bottom - i * tile_size;
            float overlapBottom = (i + 1) * tile_size - top;
            // find the minimum overlap direction
            float minOverlapX = std::min(overlapLeft, overlapRight);
            float minOverlapY = std::min(overlapTop, overlapBottom);

            if (TileMap[i][j] == 'B') {
                // calculating tiles' edges coords
                float tile_left   = j * tile_size;
                float tile_right  = tile_left + tile_size;
                float tile_top    = i * tile_size;
                float tile_bottom = tile_top + tile_size;

                // vertical interaction check
                if (bottom > tile_top && top < tile_bottom) {
                    // if character hits upper tile's edge
                    if (hero.dy > 0 && bottom - hero.dy >= tile_top) {
                        hero.dy = 0;
                        hero.y = tile_top - hero.HERO_HEIGHT;
                    }
                    // if character hits bottom tile's edge
                    else if (hero.dy < 0 && top - hero.dy >= tile_bottom) {
                        hero.dy = 0;
                        hero.y = tile_bottom;
                    }
                }

                // horizontal interaction check
                 if (minOverlapX < minOverlapY) {
                    if (overlapLeft < overlapRight) {
                        hero.x -= minOverlapX;
                    }
                    else {
                        hero.x += minOverlapX;
                    }
                }
            }
        }
    }
}


void draw_map()
{ // map draw
    const float tsize = 31.5f;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < TileMap[i].length(); j++)
        {
            if (TileMap[i][j] == 'B')
            {
                // tile's coords
                float x1 = j * tsize + tsize;
                float y1 = i * tsize + tsize;
                float x2 = j * tsize + tsize;
                float y2 = i * tsize;
                float x3 = j * tsize;
                float y3 = i * tsize;
                float x4 = j * tsize;
                float y4 = i * tsize + tsize;

                // tiles' coords array
                float tile[] = { x1, y1, x2, y2, x3, y3, x4, y4 };


                glVertexPointer(2, GL_FLOAT, 0, tile);
                glEnableClientState(GL_VERTEX_ARRAY);

                glColor3f(0.95f, 0.0f, 0.92f);

                glDrawArrays(GL_QUADS, 0, 4);
                glDisableClientState(GL_VERTEX_ARRAY);


            }
            else if (TileMap[i][j] == ' ')
            {
                continue;
            }
        }
    }
}

void Hero_Show(Hero *obj)
{
    glPushMatrix();
    glTranslatef(obj->x,obj->y,0);
    Sprite_animation(sprite, currentframe, &pers);
    glPopMatrix();
    ///////
    Collision(*obj);
    draw_map();
    //////
}

#endif // MAIN_H_INCLUDED
