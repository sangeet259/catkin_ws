#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include "turtlesim/Spawn.h"
#include "blackbox.hpp"
#include "turtlesim/SetPen.h"
#include <cstdlib>
#include <unistd.h>
#include <fstream>
ofstream outf("NAMES.txt");

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

double PI =3.141592654;

void move(double speed, double distance , bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double deg2rad(double angle_in_degrees);
void setDesiredOrientation(double desired_angle_in_radians);
void poseCallback(const turtlesim::Pose::ConstPtr& pose_message);

using namespace std ;
int searchFor=1;
int Floor =0;

struct Coordinate{
 	int x;
 	int y;
 };

double side =1.108888912;
double diagonal =1.568205739;


int visited[10][10][10];

Coordinate pos;

//a matrix to store the states of boxes we have already found
int matrix [3][3];

//function to check within bounds or not ?
int within_bounds(Coordinate box){
	if(box.x<0 || box.x>9 || box.y <0 || box.y>9)
		return 0;
	else 
		return 1;
}

void poseCallback(const turtlesim::Pose::ConstPtr& pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}

void move (double speed, double distance , bool isForward){

	geometry_msgs::Twist vel_msg;
	//distance =speed/time
	if(isForward)
		vel_msg.linear.x=abs(speed);
	else
		vel_msg.linear.x=-abs(speed);
	vel_msg.linear.y=0;
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
	vel_msg.linear.x=0;
	velocity_publisher.publish(vel_msg);
	//loop
	//publish the velocity
	//estimate the distance by multiplying speed of robot t1-t0; t1 is the current time , t0 is time before entering the loop
}


void rotate(double angular_speed, double angle, bool clockwise){
	outf<<"Inside rotate function\n";
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
	outf<<"omegas to be published"<<vel_msg.angular.z<<"\n";
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

void setDesiredOrientation(double desired_angle_in_radians)
{
	static double current_angle=0.000000;
	outf<<"Inside Orientation setter\n";
	outf<<"My current Orientation "<<current_angle<<endl;

	double relative_angle_radians=desired_angle_in_radians-current_angle;
	outf<<"relative_angle_radians= "<<relative_angle_radians<<endl;
	bool clockwise =((relative_angle_radians<0)?true:false);
	outf<<"Its clockwise ="<<clockwise<<endl;
	//outf<<desired_angle_in_radians<<","<<turtlesim_pose.theta<<","<<rientation 1.57
	outf<<"About to enter rotate function"<<endl;
	if(relative_angle_radians!=0) rotate(abs(relative_angle_radians)/4,abs(relative_angle_radians),clockwise);
			
	current_angle+=relative_angle_radians;
	current_angle=fmod(current_angle,2*PI);
	outf<<"Now I have returned from rotate function and";
	outf<<"My current Orientation "<<current_angle<<endl;

}




void DFS(int m, int n, int o,int parent_x,int parent_y){
	
	if (Floor ==10) {
		outf<<"The search is completed!!!";
		exit(0);
	}
	outf<<"The turtle is at "<<m<<","<<n<<" and is searing for :: "<<o+1<<endl;
	
	if (visited[m][n][o]==1)
	{
		outf<<"I have visited this room ,Damnn!\n";
		outf<<"So now I have to move back to the block from where I came !\n";
		outf<<"I came here from "<<parent_x<<","<<parent_y<<endl;
		//Write the Code to move back to the parent::
		if (parent_y<n){
			if (parent_x<m){
				//move to south west
				//set Orientation to 5*PI/4
				setDesiredOrientation(5*PI/4);
				//ros::Rate loop_rate(0.5);
				//move it by side root 2
				move(1,diagonal,1);
			}
			if (parent_x=m){
				//move to south
				//set Orientation to 3*PI/2
				setDesiredOrientation(3*PI/2);
				//ros::Rate loop_rate(0.5);
				//move it by a side
				move(1,side,1);
				
			}
			if (parent_x>m){
				//move to south east
				//set Orientation to 7*PI/4
				setDesiredOrientation(7*PI/4);
				//ros::Rate loop_rate(0.5);
				
				//move it by side root 2
				move(1,diagonal,1);
				
			}
		}
		if (parent_y>n){
			if (parent_x<m){
				//move to north west
				//set Orientation to 3*PI/4
				setDesiredOrientation(3*PI/4);
				//ros::Rate loop_rate(0.5);
				
				//move it by side root 2
				move(1,diagonal,1);
				
			}
			if (parent_x=m){
				//move to north
				//move to pos.x , pos.y
				//set Orientation to PI/2
				setDesiredOrientation(PI/2);
				//ros::Rate loop_rate(0.5);
				
				//move it by side
				move(1,side,1);
				
			}
			if (parent_x>m){
				//move to north east
				//set Orientation to PI/4
				setDesiredOrientation(PI/4);
				//ros::Rate loop_rate(0.5);
				
				//move it by side root 2
				move(1,diagonal,1);
				
			}
		}
		if (parent_y=n){
			if (parent_x<m){
				//move to west
				//set Orientation to PI
				setDesiredOrientation(PI);
				//ros::Rate loop_rate(0.5);
				
				//move it by a side
				move(1,side,1);
				
			}
	
			if (parent_x>m){
				//move to east
				//set Orientation to 0
				setDesiredOrientation(0);
				//ros::Rate loop_rate(0.5);
				//move it by a side
				move(1,side,1);
				
			}
		
		}
	outf<<"Now I am at "<<parent_x<<","<<parent_y<<endl;
	return;
	}

	visited[m][n][o]=1;

	//whenever where_to_go function is called it will have the current coordinates of the turtle
	// east,west,north,south, north east, north west, south east, south west
	// these will be Coordinate structures with suitable values
	////////////////////////////////////////////////////////////////////////////////////
	Coordinate east;
	Coordinate west;
	Coordinate north;
	Coordinate south;
	Coordinate north_east;
	Coordinate north_west;
	Coordinate south_east;
	Coordinate south_west;
	//////////////////////////////////////////////////////////////////////////////////////
	east.x = pos.x+1;
	east.y = pos.y ;
	west.x = pos.x-1;
	west.y = pos.y;
	north.x = pos.x;
	north.y = pos.y+1;
	south.x = pos.x;
	south.y = pos.y-1;
	north_east.x=pos.x+1;
	north_east.y=pos.y+1;
	north_west.x=pos.x-1;
	north_west.y=pos.y+1;
	south_west.x=pos.x-1;
	south_west.y=pos.y-1;
	south_east.x=pos.x+1;
	south_east.y=pos.y-1;
	//now function call to blackbox i.e. decode will be made with adj coordinates if they fall within bounds
	///////////////////////////////////////////////east/////////////////////////////////////////////////////////////////////////
	if(within_bounds(east)){
		matrix[2][1]=decode(east.x,east.y);
	}

	//////////////////////////////////////////////////west//////////////////////////////////////////////////////////////////
	if(within_bounds(west)){
		matrix[0][1]=decode(west.x,west.y);
	}
	/////////////////////////////////////////////////north///////////////////////////////////////////////////////////////////
	if(within_bounds(north)){
		matrix[1][2]=decode(north.x,north.y);
	}
	//south
	if(within_bounds(south)){
		matrix[1][0]=decode(south.x,south.y);
	}
	//north_east
	if(within_bounds(north_east)){
		matrix[2][2]=decode(north_east.x,north_east.y);
	}
	//north_west
	if(within_bounds(north_west)){
		matrix[0][2]=decode(north_west.x,north_west.y);
	}
	//south_east
	if(within_bounds(south_east)){
		matrix[2][0]=decode(south_east.x,south_east.y);
	}
	//south_west
	if(within_bounds(south_west)){
		matrix[0][0]=decode(south_west.x,south_west.y);
	}



	//////////////////////////////////////8888888888888888888888888888888888888888////////////////////////////
	/////// if not within bounds then set the values to anyhtin apart from 0 1 or 2   say we set it at 8 /////
	if(!visited[9][8][1]){
			if(!within_bounds(east)){
				matrix[2][1]=8;
			}

			
			if(!within_bounds(west)){
				matrix[0][1]=8;
			}
			if(!within_bounds(north)){
				matrix[1][2]=8;
			}
			//south
			if(!within_bounds(south)){
				matrix[1][0]=8;
			}
			//north_east
			if(!within_bounds(north_east)){
				matrix[2][2]=8;
			}
			//north_west
			if(!within_bounds(north_west)){
				matrix[0][2]=8;
			}
			//south_east
			if(!within_bounds(south_east)){
				matrix[2][0]=8;
			}
			//south_west
			if(!within_bounds(south_west)){
				matrix[0][0]=8;
			}

		}
	///////////////////////////////////////////////88888888888888888888888888888888888888////////////////////////
	

	outf<<endl;
	//print the state matrix
	for (int j=2;j>=0;j--){
		for(int i=0;i<3;i++){
			outf<<matrix[i][j]<<" ";
		}
		outf<<endl;
	}
	/////////////////////////////////////// IMPLEMENT DFS recursive search /////////////////////////////////////

	if (
		matrix[2][1] 	==2 ||
		matrix[0][1] 	==2 ||
		matrix[1][2] 	==2 ||
		matrix[1][0] 	==2 ||
		matrix[2][2] 	==2 ||
		matrix[0][2] 	==2 ||
		matrix[2][0] 	==2 ||
		matrix[0][0] 	==2
	)
	{
		outf<<"Oh Yeah I found "<<searchFor<<"\n";
		searchFor++;
		Floor++;
		outf<<"Now searching for "<<searchFor;
		// No movement here
		DFS(pos.x,pos.y,Floor,m,n);
	}

	else{
		outf<<"In the else";

		
		
		if (within_bounds(north) && matrix[1][2]==0){
			outf<<"Moving to "<<north.x<<","<<north.y<<endl;
			pos.x=north.x;
			pos.y=north.y;
			//move to pos.x , pos.y
			//set Orientation to PI/2
			setDesiredOrientation(PI/2);
			//ros::Rate loop_rate(0.5);
			//move it by a side
			move(1,side,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(east) && matrix[2][1]==0){
			outf<<"Moving to "<<east.x<<","<<east.y<<endl;
			pos.x=east.x;
			pos.y=east.y;
			//move to pos.x , pos.y
			//set Orientation to 0
			setDesiredOrientation(0);
			////ros::Rate loop_rate(0.5);
			//move it by a side
			move(1,side,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(west) && matrix[0][1]==0){
			outf<<"Moving to "<<west.x<<","<<west.y<<endl;
			pos.x=west.x;
			pos.y=west.y;
			//move to pos.x , pos.y
			//set Orientation to PI
			setDesiredOrientation(PI);
			//ros::Rate loop_rate(0.5);
			//move it by a side
			move(1,side,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(south) && matrix[1][0]==0){
			outf<<"Moving to "<<south.x<<","<<south.y<<endl;
			pos.x=south.x;
			pos.y=south.y;
			//move to pos.x , pos.y
			//set Orientation to 3*PI/2
			setDesiredOrientation(3*PI/2);
			//ros::Rate loop_rate(0.5);
			//move it by a side
			move(1,side,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(north_east) && matrix[2][2]==0){
			outf<<"Moving to "<<north_east.x<<","<<north_east.y<<endl;
			pos.x=north_east.x;
			pos.y=north_east.y;
			//move to pos.x , pos.y
			//set Orientation to PI/4
			setDesiredOrientation(PI/4);
			//ros::Rate loop_rate(0.5);
			//move it by side root 2
			move(1,diagonal,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(north_west) && matrix[0][2]==0){
			outf<<"Moving to "<<north_west.x<<","<<north_west.y<<endl;
			pos.x=north_west.x;
			pos.y=north_west.y;
			//move to pos.x , pos.y
			//set Orientation to 3*PI/4
			setDesiredOrientation(3*PI/4);
			//ros::Rate loop_rate(0.5);
			//move it by side root 2
			move(1,diagonal,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(south_east) && matrix[2][0]==0){
			outf<<"Moving to "<<south_east.x<<","<<south_east.y<<endl;
			pos.x=south_east.x;
			pos.y=south_east.y;
			//move to pos.x , pos.y
			//set Orientation to 7*PI/4
			setDesiredOrientation(7*PI/4);
			//ros::Rate loop_rate(0.5);
			//move it by side root 2
			move(1,diagonal,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		if (within_bounds(south_west) && matrix[0][0]==0){
			outf<<"Moving to "<<south_west.x<<","<<south_west.y<<endl;
			pos.x=south_west.x;
			pos.y=south_west.y;
			//move to pos.x , pos.y
			//set Orientation to 5*PI/4
			setDesiredOrientation(5*PI/4);
			//ros::Rate loop_rate(0.5);
			//move it by side root 2
			move(1,diagonal,1);
			DFS(pos.x,pos.y,Floor,m,n);
		}
		outf<<"Looks like I have exhausted this room\n";
		outf<<"Lets go back to "<<parent_x<<","<<parent_y<<endl;
		//Write the Code to move back to the parent::
		if (parent_y<n){
			if (parent_x<m){
				//move to south west
				//set Orientation to 5*PI/4
				setDesiredOrientation(5*PI/4);
				//ros::Rate loop_rate(0.5);
				//move it by side root 2
				move(1,diagonal,1);
			}
			if (parent_x=m){
				//move to south
				//set Orientation to 3*PI/2
				setDesiredOrientation(3*PI/2);
				//ros::Rate loop_rate(0.5);
				//move it by a side
				move(1,side,1);
			}
			if (parent_x>m){
				//move to south east
				//set Orientation to 7*PI/4
				setDesiredOrientation(7*PI/4);
				//ros::Rate loop_rate(0.5);
				//move it by side root 2
				move(1,diagonal,1);
			}
		}
		if (parent_y>n){
			if (parent_x<m){
				//move to north west
				//set Orientation to 3*PI/4
				setDesiredOrientation(3*PI/4);
				//ros::Rate loop_rate(0.5);
				//move it by side root 2
				move(1,diagonal,1);
			}
			if (parent_x=m){
				//move to north
				//move to pos.x , pos.y
				//set Orientation to PI/2
				setDesiredOrientation(PI/2);
				//ros::Rate loop_rate(0.5);
				//move it by side
				move(1,side,1);
			}
			if (parent_x>m){
				//move to north east
				//set Orientation to PI/4
				setDesiredOrientation(PI/4);
				//ros::Rate loop_rate(0.5);
				//move it by side root 2
				move(1,diagonal,1);
			}
		}
		if (parent_y=n){
			if (parent_x<m){
				//move to west
				//set Orientation to PI
				setDesiredOrientation(PI);
				//ros::Rate loop_rate(0.5);
				//move it by a side
				move(1,side,1);
			}
	
			if (parent_x>m){
				//move to east
				//set Orientation to 0
				setDesiredOrientation(0);
				//ros::Rate loop_rate(0.5);
				//move it by a side
				move(1,side,1);
			}
		}

		outf<<"Now I am at "<<parent_x<<","<<parent_y<<endl;

		
	}//eslse bracket

}
////////////////////////    END OF DFS    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////





int main(int argc, char **argv)
{
	ros::init(argc, argv,"turtle_bot");   //create a node called turtle_bot
	ros::NodeHandle n;					// Handle for the node


	/*/////////////////// Set the color of pen as background color to avoid leaving lines on the arena
	ros::ServiceClient pen_client =n.serviceClient<turtlesim::SetPen>("set_pen");
	turtlesim::SetPen::Request reques;
	turtlesim::SetPen::Response respons ;
	reques.r = 69;
	reques.g = 86;
	reques.b = 255;
	reques.width=0;
	reques.off =1;
	bool succes = pen_client.call(reques,respons);
	if(succes)
	{
		ROS_INFO_STREAM("Pen's color set to background color");
	}
	else
	{
		ROS_ERROR_STREAM("Failed to set pen's color");
	}
*/
	//////////////////////////////////////////////     OUR PEN IS SET    //////////////////////////////////////////////////////

	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle_bot/cmd_vel",10);
	//news_publisher =n.advertise<krssg::news>("newsfeed",1000);
	pose_subscriber =n.subscribe("/turtle_bot/pose",10,poseCallback);
	//create an object of type ros::ServiceClient , whose job is to actually carry out the service call.
	outf<<"Upto this done\n";
	ros::ServiceClient client =n.serviceClient<turtlesim::Spawn>("spawn");
	turtlesim::Spawn::Request req;
	turtlesim::Spawn::Response resp ;
	req.x = 1.10399043/2;
	req.y = 1.10399043/2;
	req.theta =0;
	req.name="turtle_bot";
	bool success = client.call(req,resp);
	if(success){
		ROS_INFO_STREAM("Spawned_a_turtle_named\n"<<resp.name);
	}
	else
	{
		ROS_ERROR_STREAM("Failed_to_spawn.");
	}
	//////////////////////////////////////////////     SPAWNING DONE    //////////////////////////////////////////////////////


	outf<<"//initialise all visited rooms to zero\n";

	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			for(int k=0;k<10;k++){
				visited[i][j][k]=0;

			}
		}
	} 	

	outf<<"//initialise all elements in state matrix to 9\n";

	for (int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			matrix[i][j]=9;
		}
	}


	//initialise turtle's position to (0,0)
	pos.x=0;
	pos.y=0;


	outf<<"//but make 0,0 of matrix =0\n";
	matrix[1][1]=0;
	outf<<"//Lets start the search\n";
	DFS(pos.x,pos.y,Floor,0,0);	
}
double deg2rad(double angle_in_degrees){
	return angle_in_degrees*PI/180;
}