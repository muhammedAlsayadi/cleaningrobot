#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>


#include <lidarLite.h>
#include <time.h>

#include <std_msgs/Int16.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "laserScanNode");
    ros::NodeHandle nodeHandle;

    ros::Publisher laserScanPublisher = nodeHandle.advertise <sensor_msgs::LaserScan> ("base_scan", 10);

	int readingsNo = 400;

	sensor_msgs::LaserScan scan;
	scan.ranges.resize(readingsNo+1);
	scan.intensities.resize(readingsNo+1);

	int fd;

	 fd = lidar_init(false);
	
	if (fd == -1)
	{
		ROS_INFO("Lidar initialization error");

	} // end if

    ros::Rate r(10.0);

    while(nodeHandle.ok())
    {

	//double laserFrequency = 100 * 1000;
	ros::Time scanTime = ros::Time::now();
	ROS_INFO("start: \n");
	double minAngle = -3.14;
	double maxAngle = 3.14;
	double angleIncrement = 6.28 / readingsNo;
	double timeIncrement = 0.8 / readingsNo;
	double minRange = 0.0;
	double maxRange = 40.0;

	//populate the LaserScan message
	scan.header.stamp = scanTime;
	scan.header.frame_id = "base_laser";
	scan.angle_min = minAngle;
	scan.angle_max = maxAngle;
	scan.angle_increment = angleIncrement;
	scan.time_increment = timeIncrement;
	scan.range_min = minRange;
	scan.range_max = maxRange;

	for(int i = 0; i <= readingsNo; i++)
	{
		scan.ranges[i] = lidar_read(fd);

		scan.intensities[i] = 135;
		sleep(0.00001);
		ros::spinOnce();

	} // end for


	laserScanPublisher.publish(scan);


	ros::spinOnce();

    } // end while

	r.sleep();

} // end main
