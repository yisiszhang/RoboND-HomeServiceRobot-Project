#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = 3.0;
  goal.target_pose.pose.position.y = -1.0;
  goal.target_pose.pose.orientation.w = 0.2;

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Picking up...");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Object picked.");
  else
    ROS_INFO("The robot failed to reach the goal.");
  
  // wait for 5 seconds
  ros::Duration(5.0).sleep();
  
  // Define a second goal
  goal.target_pose.pose.position.x = 5.5;
  goal.target_pose.pose.position.y = 4.0;
  goal.target_pose.pose.orientation.w = 0.3;
  
  ROS_INFO("Delivering...");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();  
  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Object delivered.");
  else
    ROS_INFO("The object failed to read the goal.");
  
  ros::Duration(5.0).sleep();
  
  return 0;
}
