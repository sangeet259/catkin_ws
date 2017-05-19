/*
 * robot_cleaner.cpp
 *
 *  Created on: 26-Feb-2017
 *      Author: sangeet
 */


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;
using namespace std;
const double PI = 3.14159265359;


//void move(double speed, double distance , bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double deg2rad(double angle_in_degrees);
void setDesiredOrientation(double desired_angle_in_radians);
void poseCallback(const turtlesim::Pose::ConstPtr& pose_message);


int main(int argc, char **argv)
{
	/*double speed;
	double distance;
	bool isForward;*/
	ros::init(argc,argv,"robot_cleaner"); //Initiate new rose node named "robot_cleaner"
	ros::NodeHandle n;
	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
	pose_subscriber =n.subscribe("/turtle1/pose",10,poseCallback);
	/*speed=3;
	distance=1.108889122;
	isForward=1;
	move(speed,distance,isForward);*/
	/*double angular_speed;
	double angle;
	bool clockwise;
	cout<<"Enter angular velocity(degrees/sec):: ";
	cin>>angular_speed;
	cout<<"Enter angle in degrees ::-  ";
	cin>>angle;
	cout<<"Clockwise ? ::  ";
	cin>>clockwise;
	rotate(deg2rad(angular_speed),deg2rad(angle),clockwise);*/
	double ang;
	cout<<"Enter the desired orientation in degrees \n";
	cin>>ang;
	setDesiredOrientation(deg2rad(ang));
	ros::Rate loop_rate(0.5); //sleep for 2 second.
	loop_rate.sleep();
	setDesiredOrientation(deg2rad(-60));
	loop_rate.sleep();
	setDesiredOrientation(deg2rad(0));



}
/*make the robot move a certain distance with a certain linear velocity
 * in a certain direction backward or forward
 * */
/*void move (double speed, double distance , bool isForward){

	geometry_msgs::Twist vel_msg;
	//distance =speed/time
	if(isForward)
		vel_msg.linear.y=abs(speed);
	else
		vel_msg.linear.y=-abs(speed);
	vel_msg.linear.x=0;
	vel_msg.linear.z=0;

	//set random angular velocity in y-axis

	vel_msg.angular.x =0;
	vel_msg.angular.y =0;
	vel_msg.angular.z =0;
	//t0:current time
	double t0=ros::Time::now().toSec();
	double current_distance=0;
	ros::Rate loop_rate(500000);
	do{
		velocity_publisher.publish(vel_msg);
		double t1=ros::Time::now().toSec();
		current_distance=speed*(t1-t0);
		ros::spinOnce();
		loop_rate.sleep();

	}while(current_distance<distance);
	vel_msg.linear.y=0;
	velocity_publisher.publish(vel_msg);
	//loop
	//publish the velocity
	//estimate the distance by multiplying speed of robot t1-t0; t1 is the current time , t0 is time before entering the loop
	//current_distance_moved_by_robot=



}*/

void rotate(double angular_speed, double angle, bool clockwise){
	geometry_msgs::Twist vel_msg;
	//set liner velocity to zero
	vel_msg.linear.x=0;
	vel_msg.linear.y=0;
	vel_msg.linear.z=0;
	//set a random angular velocity in the y axis
	vel_msg.angular.x=0;
	vel_msg.angular.y=0;
	if (clockwise)
		vel_msg.angular.z=-abs(angular_speed);
	else
		vel_msg.angular.z=abs(angular_speed);
	double t0=ros::Time::now().toSec();
	double current_angle=0.0;
	ros::Rate loop_rate(500);
	do{
		velocity_publisher.publish(vel_msg);
		double t1=ros::Time::now().toSec();
		current_angle=angular_speed*(t1-t0);
		ros::spinOnce();
		loop_rate.sleep();

	}while(current_angle<=angle);
	vel_msg.angular.z=0;
	velocity_publisher.publish(vel_msg);
}

double deg2rad(double angle_in_degrees){
	return angle_in_degrees*PI/180;
}

void setDesiredOrientation(double desired_angle_in_radians){
	double relative_angle_radians=desired_angle_in_radians - turtlesim_pose.theta;
	bool clockwise =((relative_angle_radians<0)?true:false);
	//cout<<desired_angle_in_radians<<","<<turtlesim_pose.theta<<","<<
	rotate(abs(relative_angle_radians)/4,abs(relative_angle_radians),clockwise);

}

void poseCallback(const turtlesim::Pose::ConstPtr& pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}