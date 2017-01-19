#include "ros/ros.h"

#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"

#include <sstream>



geometry_msgs::PoseStamped current_cartesian_position;
std::string input_topic, output_topic;
double ros_loop_rate;


void inputCallback(const geometry_msgs::PoseStamped &ps)
{
    current_cartesian_position = ps;
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "filter");

    ros::NodeHandle n;


    // ROS spinner.
    ros::AsyncSpinner spinner(1);//1 thread
    spinner.start();

    // Dynamic parameters. Last arg is the default value. You can assign these from a launch file.
    n.param("input_topic", input_topic, std::string("/iiwa/state/CartesianPose"));
    n.param("output_topic", output_topic, std::string("/iiwa/state/CartesianPoseFiltered"));
    n.param("ros_loop_rate", ros_loop_rate, 33.0);
   
	ros::Rate loop_rate(ros_loop_rate);
	ROS_INFO("Output topic has a rate of %3.3f Hz",
	ros_loop_rate);

    ros::Subscriber sub_input = n.subscribe(input_topic, 1, inputCallback);
    ros::Publisher pub_output = n.advertise<geometry_msgs::PoseStamped>(output_topic, 1);


    while (ros::ok())
    {
    

	ros::spinOnce();
    pub_output.publish(current_cartesian_position);
	loop_rate.sleep();
    }

    std::cerr << "Stopping spinner..." << std::endl;
    spinner.stop();

    return 0;
}
