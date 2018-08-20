#!/bin/sh

xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/workspace/catkin_ws/src/World/u_world" &
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/workspace/catkin_ws/src/World/slamMap.yaml" &
sleep 5
xterm -e " rosrun rviz rviz -d /home/workspace/catkin_ws/src/rviz_config/marker_config.rviz" &
sleep 5
xterm -e " rosrun add_markers add_markers" &
sleep 5
xterm -e " rosrun pick_objects pick_objects"
