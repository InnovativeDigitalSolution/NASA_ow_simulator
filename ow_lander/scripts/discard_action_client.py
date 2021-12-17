#!/usr/bin/env python3

# The Notices and Disclaimers for Ocean Worlds Autonomy Testbed for Exploration
# Research and Simulation can be found in README.md in the root directory of
# this repository.

import rospy
import actionlib
import ow_lander.msg
import argparse
from guarded_move_action_client import print_arguments


def discard_client():

    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('x_start', type=float,
                        help='x coordinates of sample discard location', nargs='?', default=1.5, const=0)
    parser.add_argument('y_start', type=float,
                        help='y coordinates of sample discard location', nargs='?', default=0.8, const=0)
    parser.add_argument('z_start', type=float,
                        help='z coordinates of sample discard location', nargs='?', default=0.65, const=0)
    args = parser.parse_args()
    print_arguments(args)

    client = actionlib.SimpleActionClient(
        'Discard', ow_lander.msg.DiscardAction)

    client.wait_for_server()

    goal = ow_lander.msg.DiscardGoal()

    goal.discard.x = args.x_start
    goal.discard.y = args.y_start
    goal.discard.z = args.z_start

    # Sends the goal to the action server.
    client.send_goal(goal)

    # Waits for the server to finish performing the action.
    client.wait_for_result()

    # Prints out the result of executing the action
    return client.get_result()


if __name__ == '__main__':
    try:
        # Initializes a rospy node so that the discard client can
        # publish and subscribe over ROS.
        rospy.init_node('discard_client_py')
        result = discard_client()
        rospy.loginfo("Result: %s", result)
    except rospy.ROSInterruptException:
        rospy.logerror("program interrupted before completion")
