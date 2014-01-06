#include "common/world/world.h"
#include "common/world/components/physics.h"
#include "common/world/components/grid_handler.h"

#include <SDL.h>
#include <GL/glew.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <chrono>
#include <set>

using namespace std;

World::World():
  mInit(false),
  mTickNumber(0)
{
}

void World::defaultWorld() {
  mGrid.defaultGrid();
}

void World::spawn_monster() {
  PhysicsC p;
  memset(&p, 0, sizeof(p));
  p.id = m_idgen.NextId();
  p.position = spawn_position;
  p.dimensions = glm::vec3(0.4f, 0.2f, 0.4f);
  p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  mPhysicsHandler.add(p);
}

void World::spawn_player(int fd) {
  PhysicsC p;
  memset(&p, 0, sizeof(p));
  p.id = m_idgen.NextId();
  p.position = spawn_position;
  p.dimensions = glm::vec3(0.4f, 0.9f, 0.4f);
  p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  mPhysicsHandler.add(p);

  Inventory i;
  i.id = p.id;
  i.mWielding = 1;
  mInventory.add(i);

  ClientData* cd = mClient.getByClient(fd);
  assert(cd != nullptr);
  cd->dead = false;
  cd->id = p.id;
  cd->mode = PLAYER_MODE;
  cd->connected = true;
  cd->client = fd;
}

void World::connected(int fd) {
  ClientData cd;
  cd.dead = true;
  cd.mode = PLAYER_MODE;
  cd.connected = true;
  cd.client = fd;
  mClient.add(cd);
}

void World::disconnected(int fd) {
  ClientData* cd = mClient.getByClient(fd);
  if (cd != nullptr) cd->connected = false;
}

int World::tick(float dt, const std::vector<InputC>& inputs) {
  mDeleteList.clear();
  mInputHandler.setInputs(inputs);
  mClient.tick(dt, *this);
  mInputHandler.tick(dt, *this);
  mPhysicsHandler.tick(dt, *this);
  removeDead();
  return ++mTickNumber;
}

void World::removeDead() {
  mPhysicsHandler.handleDead(mDeleteList);
  mClient.handleDead(mDeleteList);
}

InventoryHandler& World::inventory() {
  return mInventory;
}

PhysicsHandler& World::physics() {
  return mPhysicsHandler;
}

GridHandler& World::grid() {
  return mGrid;
}

ClientHandler& World::client() {
  return mClient;
}

InputHandler& World::input() {
  return mInputHandler;
}

void World::setState(const WorldState& w) {
  mPhysicsHandler.setObjects(w);
  mGrid.setGrid(w);
  mTickNumber = w.tick_number();
  mInit = true;
}

WorldState World::getState() {
  WorldState w;
  mPhysicsHandler.getObjects(w);
  mGrid.getGrid(w);
  w.set_tick_number(mTickNumber);
  return w;
}
