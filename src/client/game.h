#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "common/world/area.h"
#include "common/google.h"

#include "client/view/graphics.h"

class Game {
public:
  Game();
  void Init();
  void Run();

private:
  DISALLOW_COPY_AND_ASSIGN(Game);

  void UpdateVertices();

  Area area_;
  float vertices_[12];
  Graphics graphics_; // FIXME
};

#endif

