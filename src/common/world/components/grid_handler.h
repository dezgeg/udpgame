#pragma once

#include <glm/glm.hpp>

#include "common/proto/udpgame.pb.h"
#include "common/world/components/physics.h"
#include "common/world/components/grid_array.h"

struct GridHandler {
  void defaultGrid();

  bool handle_grid_collisions(PhysicsC&, float) const;
  bool check_collision(PhysicsC&, float) const;
  bool correct_one_hit(PhysicsC& p) const;

  void overlapping_indices(const PhysicsC&, int[3][2]) const;
  void range_indices(const glm::vec3&, int[3][2]) const;
  void raycast_range(const glm::vec3&, const glm::vec3&, int[3][2]) const;

  bool raycast(const glm::vec3&, const glm::vec3&, float&, char**, char**);

  bool belowBottom(const glm::vec3&) const;
  glm::vec3 spawn_pos();

  void serialize(InitialState&) const;
  void deserialize(const InitialState&);

  GridArray mArr;
  glm::vec3 mSpawn;
};
