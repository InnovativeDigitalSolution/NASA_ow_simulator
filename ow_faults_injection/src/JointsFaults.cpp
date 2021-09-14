// The Notices and Disclaimers for Ocean Worlds Autonomy Testbed for Exploration
// Research and Simulation can be found in README.md in the root directory of
// this repository.

#include "ow_faults_injection/JointsFaults.h"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/rendering/RenderingIface.hh>
#include <gazebo/rendering/Scene.hh>
#include <ros/ros.h>

using namespace gazebo;
using namespace std;

constexpr double JointsFaults::MAX_FRICTION;

GZ_REGISTER_MODEL_PLUGIN(JointsFaults)

JointsFaults::JointsFaults() :
  ModelPlugin()
{
  m_JointsFaultsMap = {
    {"j_shou_yaw", JointFaultInfo("shou_yaw_effort_failure")},
    {"j_shou_pitch", JointFaultInfo("shou_pitch_effort_failure")},
    {"j_prox_pitch", JointFaultInfo("prox_pitch_effort_failure")},
    {"j_dist_pitch", JointFaultInfo("dist_pitch_effort_failure")},
    {"j_hand_yaw", JointFaultInfo("hand_yaw_effort_failure")},
    {"j_scoop_yaw", JointFaultInfo("scoop_yaw_effort_failure")},
    {"j_ant_pan", JointFaultInfo("ant_pan_effort_failure")},
    {"j_ant_tilt", JointFaultInfo("ant_tilt_effort_failure")} };
}

JointsFaults::~JointsFaults()
{
}

void JointsFaults::Load(physics::ModelPtr model, sdf::ElementPtr /* sdf */)
{
  m_model = model;

  // Listen to the update event. This event is broadcast every sim iteration.
  // If result goes out of scope updates will stop, so it is assigned to a member variable.
  m_updateConnection = event::Events::ConnectBeforePhysicsUpdate(std::bind(&JointsFaults::onUpdate, this));

  gzlog << "JointsFaultsPlugin - successfully loaded!" << endl;
}

void JointsFaults::onUpdate()
{
//   injectFault("ant_tilt_effort_failure", m_antennaTiltEffortFaultActivated, m_antennaTiltEncFaultActivated, "j_ant_tilt",
//             m_antennaTiltLowerLimit, m_antennaTiltUpperLimit);

//   injectFault("ant_pan_effort_failure", m_antennaPanEffortFaultActivated, m_antennaPanEncFaultActivated, "j_ant_pan",
//             m_antennaPanLowerLimit, m_antennaPanUpperLimit);

//   injectFault("ant_tilt_encoder_failure", m_antennaTiltEncFaultActivated, m_antennaTiltEffortFaultActivated, "j_ant_tilt",
//             m_antennaTiltLowerLimit, m_antennaTiltUpperLimit);

//   injectFault("ant_pan_encoder_failure", m_antennaPanEncFaultActivated, m_antennaPanEffortFaultActivated, "j_ant_pan",
//             m_antennaPanLowerLimit, m_antennaPanUpperLimit);
// }

// void JointsFaults::injectFault(const std::string& joint_fault, bool& fault_activated, bool& other_active, 
//                                const std::string& joint_name, double lower_limit, double upper_limit)
// {
//   bool fault_enabled;
//   ros::param::param("/faults/" + joint_fault, fault_enabled, false);
  
//   if (fault_enabled)
//   {
//     if (!fault_activated && !other_active){
//       ROS_INFO_STREAM(joint_fault << " activated!");
//       fault_activated = true;
//       // lock the joint to current position
//       auto j = m_model->GetJoint(joint_name);
//       auto p = j->Position(0);
//       j->SetLowerLimit(0, p);
//       j->SetUpperLimit(0, p);
//     }
//   }
//   else if (!fault_enabled)
//   {
//     if (fault_activated && !other_active) {
//       fault_activated = false;
//       ROS_INFO_STREAM(joint_fault << " de-activated!");
//       // restore the joint limits
//       auto j = m_model->GetJoint(joint_name);
//       j->SetLowerLimit(0, lower_limit);
//       j->SetUpperLimit(0, upper_limit); 
//     }
  for (auto& kv : m_JointsFaultsMap)
    injectFault(kv.first, kv.second);
}

void JointsFaults::injectFault(const std::string& joint_name, JointFaultInfo& jfi)
{
  bool fault_enabled;
  ros::param::param("/faults/" + jfi.fault, fault_enabled, false);
  if (!jfi.activated && fault_enabled)
  {
    ROS_INFO_STREAM(jfi.fault << " activated!");
    jfi.activated = true;
    // lock the joint to current position
    auto j = m_model->GetJoint(joint_name);
    jfi.friction = j->GetParam("friction", 0);
    j->SetParam("friction", 0, MAX_FRICTION);
  }
  else if (jfi.activated && !fault_enabled)
  {
    ROS_INFO_STREAM(jfi.fault << " de-activated!");
    jfi.activated = false;
    // restore the joint limits
    auto j = m_model->GetJoint(joint_name);
    j->SetParam("friction", 0, jfi.friction);
  }
}