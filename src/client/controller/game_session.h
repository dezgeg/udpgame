#pragma once

#include "common/net/connection.h"
#include "common/world/world.h"
#include "client/controller/perspective.h"

#include <string>

struct GameSession {
  GameSession(const std::string&);
  void tick(Input&);
  void sendFrameInput(Input&);

  Connection mConnection;
  Perspective mPerspective;
  World mWorld;
};

extern GameSession* g_game_session;
