#include <iostream>
#include "common/proto/udpgame.pb.h"
#include "client/controller/game_session.h"
#include "common/world/components/inputc.h"

using namespace std;

GameSession::GameSession(const std::string& addr):
  mConnection(addr),
  mPerspective(),
  mWorld()
{
  while (!mWorld.mInit) {
    mConnection.checkMessages(*this);
  }
}

void GameSession::tick(Input& input) {
  mConnection.checkMessages(*this);
  mPerspective.handle_input(input);
  mPerspective.tick();
  sendFrameInput(input);
}

bool GameSession::handleAMessage(const AMessage& a, int) {
  switch (a.type()) {
    case Type::WORLD_STATE:
      mWorld.setState(a.world_state());
      mPerspective.m_follow_id = a.world_state().owned_id();
      return true;
    default:
      return false;
  }
}

void GameSession::sendFrameInput(Input& i) {
  InputC ic(i);
  ClientInput ci;
  ci.set_tick_number(mWorld.mTickNumber + 1);
  ci.set_actions(ic.actions);
  ci.set_verical_delta(ic.vertical_angle_delta);
  ci.set_horizontal_delta(ic.horizontal_angle_delta);
  AMessage a;
  a.set_type(Type::CLIENT_INPUT);
  a.mutable_input()->CopyFrom(ci);
  mConnection.sendMessage(a);
}

GameSession* g_game_session = nullptr;
