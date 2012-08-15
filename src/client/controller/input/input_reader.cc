#include "client/controller/input/input_reader.h"

#include <iostream>
#include "client/controller/input/input.h"
#include "client/view/window.h"
#include "client/controller/screen_stack.h"

InputReader::InputReader()
{
  key_state_ = SDL_GetKeyState(&num_keys_);
  //SDL_EnableKeyRepeat(0,0);
  init_actions();
}

InputReader::~InputReader()
{
  delete[] actions_;
}

void InputReader::init_actions() {
  actions_ = new unsigned[num_keys_];
  memset(actions_, 0, sizeof(*actions_) * (unsigned)num_keys_);
  actions_[SDLK_ESCAPE] = Action::ESCAPE;
  actions_[SDLK_w] = Action::MOVE_FORWARD;
  actions_[SDLK_s] = Action::MOVE_BACK;
  actions_[SDLK_d] = Action::MOVE_RIGHT;
  actions_[SDLK_a] = Action::MOVE_LEFT;
}

void InputReader::CheckMouseState(Input& input)
{
  Uint8 mouse_buttons = SDL_GetMouseState(&input.mouse_x_, &input.mouse_y_);
  input.mouse_buttons_ = SDL_BUTTON(1) & mouse_buttons;
  input.gl_mouse_x_ = gl_pos_x(input.mouse_x_);
  input.gl_mouse_y_ = gl_pos_y(input.mouse_y_);
}

void InputReader::ReadInput(Input& i)
{
  memset(&i, 0, sizeof(Input));
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        g_screen_stack.clear();
        break;
      case SDL_KEYDOWN:
        i.actions_ |= actions_[e.key.keysym.sym];
        break;
    }
  }
  check_keyboard(i);
  CheckMouseState(i);
}

void InputReader::check_keyboard(Input& i)
{
  if (key_state_[SDLK_w]) {
    i.actions_ |= actions_[SDLK_w];
  } else if (key_state_[SDLK_s]) {
    i.actions_ |= actions_[SDLK_s];
  }
  if (key_state_[SDLK_a]) {
    i.actions_ |= actions_[SDLK_a];
  } else if (key_state_[SDLK_d]) {
    i.actions_ |= actions_[SDLK_d];
  }
}

