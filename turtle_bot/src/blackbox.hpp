#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

#define TOTAL_POINTS 10


struct co_ordinates {
	int x;
	int y;
};


int decode(int x, int y){		

/*
Input  :	x: x co-ordinate
  			y: y co-ordinate
 
Returns: 	0: if the (x,y) co-ordinate doesn't have an obstacle.
			1: if the (x,y) co-ordinate has an obstacle and that's not what you are 
 				searching for.
			2: if the (x,y) co-ordinate has an obstacle and that's what you are 
				searching for.
*/

	static int seq_count=0, redo=0;
	int rand_x, rand_y, count = 0;
	static vector<co_ordinates> vec;
	co_ordinates temp;

	if(!redo){
		while (count != TOTAL_POINTS){

			// generate a random number between [1, 10]
			while(1){
				int flag=0;
				rand_x = rand()%10;
				rand_y = rand()%10;
				if((rand_x == 0 || rand_x == 9) && (rand_y == 0 || rand_y == 9))
					continue;
				else{
					for(int i=0;i<vec.size(); ++i){
						if(((rand_x == vec[i].x   ) && (rand_y == vec[i].y   )) ||
						   ((rand_x == vec[i].x -1) && (rand_y == vec[i].y   )) ||
						   ((rand_x == vec[i].x +1) && (rand_y == vec[i].y   )) ||
						   ((rand_x == vec[i].x   ) && (rand_y == vec[i].y -1)) ||
						   ((rand_x == vec[i].x   ) && (rand_y == vec[i].y +1)) ||
						   ((rand_x == vec[i].x -1) && (rand_y == vec[i].y -1)) ||
						   ((rand_x == vec[i].x +1) && (rand_y == vec[i].y +1)) ||
						   ((rand_x == vec[i].x -1) && (rand_y == vec[i].y +1)) ||
						   ((rand_x == vec[i].x +1) && (rand_y == vec[i].y -1))){
							flag=1;
							break;
						}
					}
					if(!flag){
						temp.x = rand_x;
						temp.y = rand_y;
						vec.push_back(temp);
						break;
					}
					else{
						continue;
					}
				}
			} // generating one random co-ordinate
			++count;
		} // generate 'TOTAL_POINTS' random numbers
		++redo;
	}

	for(int i=0;i<vec.size();++i){
		if(vec[i].x==x && vec[i].y==y){
			if(i==seq_count){
				++seq_count;
				return 2;
			}
			else
				return 1;
		}
	}
	return 0;
}