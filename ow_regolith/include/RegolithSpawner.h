// The Notices and Disclaimers for Ocean Worlds Autonomy Testbed for Exploration
// Research and Simulation can be found in README.md in the root directory of
// this repository.

#ifndef REGOLITH_SPAWNER_H
#define REGOLITH_SPAWNER_H

#include <ros/ros.h>
#include <tf/transform_datatypes.h>

#include "ow_dynamic_terrain/modified_terrain_diff.h"

class RegolithSpawner
{
public:
  RegolithSpawner() = delete;

  RegolithSpawner(ros::NodeHandle* nh);

  bool initializeRegolith(void);

  bool spawnRegolithInScoop(tf::Vector3 pushback_direction);

  void terrainVisualModCb(
    const ow_dynamic_terrain::modified_terrain_diff::ConstPtr& msg
  );

private:
  // sum of volume displaced since last call to spawnRegolithInScoop
  double m_volume_displaced;
  // volume threshold for spawning regolith
  double m_spawn_threshold;
  // where terrain modification previously occurred
  tf::Vector3 m_previous_center;

  // regolith model that spawns in the scoop when digging occurs
  std::string m_model_uri;
  std::string m_model_sdf;
  // magnitude of the force that pushes each model into the back of the scoop.
  float m_psuedo_force_mag;

  // ROS interfaces
  std::unique_ptr<ros::NodeHandle> m_node_handle;
  ros::ServiceClient m_gz_spawn;
  ros::ServiceClient m_gz_wrench;
};

#endif // REGOLITH_SPAWNER_H