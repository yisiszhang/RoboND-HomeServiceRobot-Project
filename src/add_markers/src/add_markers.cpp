#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>

class AddMarker {
private:
	ros::NodeHandle n;
	ros::Publisher marker_pub; 
	ros::Subscriber odom_sub;
	visualization_msgs::Marker marker;

	double pickup_loc[2] = {3.0, -1.0};
	double deliver_loc[2] = {5.5, 4.0};
	double threshold;

public:
	AddMarker()
	{
		marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
		odom_sub = n.subscribe("/odom", 1, &AddMarker::odom_callback, this);

		marker.header.frame_id = "map";
	  	marker.header.stamp = ros::Time::now();	
  		marker.ns = "add_markers";
	  	marker.id = 0;
		marker.type = visualization_msgs::Marker::CUBE;
		marker.action = visualization_msgs::Marker::ADD;
		
		// pick up location
		setPosition(pickup_loc[0], pickup_loc[1]);

		// size
  		marker.scale.x = 0.25;
  		marker.scale.y = 0.25;
  		marker.scale.z = 0.25;

		//color
		marker.color.r = 0.0f;
		marker.color.g = 1.0f;
		marker.color.b = 0.0f;
		marker.color.a = 1.0;
		
		marker.lifetime = ros::Duration();

		threshold = 0.3;

  		while (marker_pub.getNumSubscribers() < 1)
  		{
   			ROS_WARN_ONCE("Please create a subscriber to the marker");
   			sleep(1);
  		}
   		marker_pub.publish(marker);

	}

	void odom_callback(const nav_msgs::Odometry::ConstPtr &msg) {
		float x = msg->pose.pose.position.x;
		float y = msg->pose.pose.position.y;

		if ((x > pickup_loc[0] - threshold && x < pickup_loc[0] + threshold) && (y > pickup_loc[1] - threshold && y < pickup_loc[1] + threshold))
		{
			ROS_INFO("Picking...");				
			ros::Duration(5).sleep();
			// set marker to delivery location
			setPosition(deliver_loc[0], deliver_loc[1]);
			// hide the marker
			marker.color.a = 0.0;			
		}
		else if ((x > deliver_loc[0] - threshold && x < deliver_loc[0] + threshold) && (y > deliver_loc[1] - threshold && y < deliver_loc[1] + threshold))	
		{
			ROS_INFO("Delivering...");
			// reveal the marker
			marker.color.a = 1.0;
		}
    		while (marker_pub.getNumSubscribers() < 1)
    		{
      			ROS_WARN_ONCE("Please create a subscriber to the marker");
      			sleep(1);
    		}
    		marker_pub.publish(marker);		
	}
	void setPosition(double x, double y) {
		marker.pose.position.x = x;
		marker.pose.position.y = y;
	}
};

int main( int argc, char** argv )
{
	ros::init(argc, argv, "add_markers");
	
	AddMarker addmarker;
	ros::spin();
  	return 0;
}
