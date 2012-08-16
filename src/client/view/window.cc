#include <SDL.h>
#include <GL/glew.h>

#include "client/view/window.h"

void open_window()
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);
  SDL_WM_SetCaption("udpgame", 0);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

float gl_pos_x(int x)
{
  return ((float)x / (WINDOW_WIDTH/2)) - 1.0f;
}

float gl_pos_y(int y)
{
  return 1.0f - (float)y / (WINDOW_HEIGHT/2);
}

void grab_mouse()
{
  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(SDL_DISABLE);
}

void release_mouse()
{
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(SDL_ENABLE);
}

