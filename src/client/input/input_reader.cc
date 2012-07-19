#include "client/input/input_reader.h"

#include <iostream>

InputReader::InputReader() {}

void InputReader::Init()
{
  key_state_ = SDL_GetKeyState(&num_keys_);
  SDL_EnableKeyRepeat(0,0);
}

bool InputReader::ReadInput(Actions& a) {
  memset(&a, 0, sizeof(Actions));
  bool quit = false;
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      quit = true;
    }
  }
  CheckKeyboard(a);
  return quit;
}

void InputReader::CheckKeyboard(Actions& a)
{
  if (key_state_[SDLK_a])
    a.commands |= Action::MOVE_LEFT;
  if (key_state_[SDLK_d])
    a.commands |= Action::MOVE_RIGHT;
  if (key_state_[SDLK_w])
    a.commands |= Action::MOVE_FORWARD;
  if (key_state_[SDLK_s])
    a.commands |= Action::MOVE_BACK;
}

