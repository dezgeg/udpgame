#include "common/world/components/client_handler.h"
#include "common/world/world.h"
#include "client/controller/input/input.h"

#include <algorithm>

using namespace std;

void ClientHandler::tick(float, World& w) {
  for (auto it = mComponents.begin(); it != mComponents.end();) {
    ClientData& c = *it;
    FrameInput* ic = w.input().getByClient(c.client());
    if (ic != nullptr) {
      ic->set_eid(c.eid());
      if (ic->actions() & ContinousAction::SPAWN_MONSTER)
        w.spawn_monster();
      if (ic->actions() & ContinousAction::PLAYER_MODE)
        c.set_mode(c.mode() == ClientMode::PLAYER ?
            ClientMode::PLAYER : ClientMode::OBSERVER);
    } else {
      w.mDeleteList.insert(c.eid());
      it = mComponents.erase(it);
      continue;
    }

    if (c.mode() == ClientMode::PLAYER and c.dead()) {
      w.spawn_player(c.client());
    } else if (c.mode() == ClientMode::OBSERVER and !c.dead()) {
      w.mDeleteList.insert(c.eid());
    }
    ++it;
  }
}

void ClientHandler::handleDead(const std::set<EntityId>& ids) {
  for (EntityId eid : ids)
    markDead(eid);
}

void ClientHandler::markDead(EntityId eid) {
  auto r = find_if(mComponents.begin(), mComponents.end(),
      [&](ClientData& c) { return c.eid() == eid; });
  if (r != mComponents.end())
    r->set_dead(1);
}

ClientData* ClientHandler::getByClient(int cid) {
  for (ClientData& c : mComponents)
    if (c.client() == cid)
      return &c;
  return nullptr;
}

void ClientHandler::removeByClient(int cid) {
  mComponents.erase(std::remove_if(std::begin(mComponents),
    std::end(mComponents), [&](ClientData& c) { return c.client() == cid; }),
    std::end(mComponents));
}

void ClientHandler::remove(EntityId) {}

void ClientHandler::serialize(
    google::protobuf::RepeatedPtrField<ClientData>* r)
{
  for (const ClientData& c : mComponents)
    r->Add()->CopyFrom(c);
}

void ClientHandler::deserialize(
    const google::protobuf::RepeatedPtrField<ClientData>& r)
{
  mComponents.clear();
  for (const ClientData& c : r)
    mComponents.push_back(c);
}
