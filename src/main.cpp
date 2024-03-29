#include <iostream>
#include <vector>
#include <ros/ros.h>

extern "C" {
#include "extApi.h"
}

const float PI = 3.1415;

//using namespace std;


int main(int argv, char** argc) {
	//cout << "hello" <<endl;
	simxChar* simIP = "127.0.0.1";
	int PortNumber = 19999;
	int ClientID = -1;
	while (ClientID == -1) { ClientID = simxStart(simIP, PortNumber, true, true, 2000, 5); }

	ros::init(argv, argc, "main");
	ros::NodeHandle nh;

	int I = 40;
	int J = I * 2;
	float d = 0.05;
	
	std::vector<std::vector<int>> map = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0},
	{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
	{0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
	{0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0, 0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0, 0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	
	std::cout << "map size " <<map.size()<<" "<<map[0].size()<< std::endl;

	std::vector<std::vector<int>> mori = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,4,4,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,4,0,  0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,   0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,4,4,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,4,0,  0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,   0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,0,4,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,0,0,  0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,   0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,0,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,0,0,  0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,4,4,4,4,4,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,0,0,  0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,0,4,4,4,4,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,4,4,4,0,0,4,4,4,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,0,4,4,4,4,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,4,4,4,0,0,4,4,4,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,4,4,4,4,4,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,4,4,4,4,4,4,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,4,4,4,4,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,4,4,4,4,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},

	{0,0,4,4,4,0,0,0,4,4,4,4,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,4,4,4,4,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,4,4,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,4,4,4,4,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,4,4,4,4,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,4,4,4,4,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,4,4,4,4,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,0,0,   0,0,0,4,4,4,0,0,0,0,0,4,4,4,4,0,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,4,4,4,4,0,0,0,0,  0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0},
	{0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,  0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,   0,0,0,4,4,4,0,0,0,0,0,0,0,4,4,4,4,0,0,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,4,4,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,4,0,  0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,   0,0,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,4,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,4,4,4,4,4,0,0,0,0,0,0,0,0,4,4,4,4,4,0,  0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,   0,0,4,4,4,4,4,0,0,0,0,0,0,4,4,4,4,4,4,0,  0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	int t = -20;
	int time[] = { 50,100,150,200,250,300,350,400, 450, 500, 550, 600, 650, 700, 750 };
	ros::Rate rate(10);
	std::vector<int> color(3 * I * J, 0);
	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			if (map[i][j] == 1) {
				color[3 * i * J + 3 * j] = 0;
				color[3 * i * J + 3 * j + 1] = 0;
				color[3 * i * J + 3 * j + 2] = 255;
			}
		}
	}

	//std::cout << "2" << std::endl;
	std::vector<float> points0;//��������
	for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
		points0.push_back(-0.5 * J * d + 1.0 * j * d);
		points0.push_back(+0.5 * d * I - 1.0 * i * d);
		points0.push_back(0.0);
	}
	std::vector<float> points1;//�󒆕���
	for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
		points1.push_back(-0.5 * J * d + 1.0 * j * d);
		points1.push_back(+0.5 * d * I - 1.0 * i * d);
		points1.push_back(0.01*(time[1] - time[0]));
	}

	float oc[3] = { 0.0, 0.0, 0.5 };
	float r = 0.4;
	std::vector<float> points2;//������
	for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
		points2.push_back(oc[0] + r * sin(PI * i / I) * cos(2 * PI * j / J));
		points2.push_back(oc[1] + r * sin(PI * i / I) * sin(2 * PI * j / J));
		points2.push_back(oc[2] + r * cos(PI * i / I));
	}
	float alpha = PI * 20 / 180;
	std::vector<float> points3;//�X����
	for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
		points3.push_back(oc[0] + r * sin(PI * i / I ) * cos(2 * PI * j / J) * cos(alpha) + r * cos(PI * i / I) * sin(alpha));
		points3.push_back(oc[1] + r * sin(PI * i / I) * sin(2 * PI * j / J));
		points3.push_back(oc[2] - r * sin(PI * i / I ) * cos(2 * PI * j / J) * sin(alpha) + r * cos(PI * i / I));
	}

	float Ra = 0.1;
	int L = 3;
	int n = 80;
	int R[5] = { 0, 0, 255, 255, 0 };
	int G[5] = { 0, 0, 0, 255, 255 };
	int B[5] = { 0, 255, 0, 0, 0 };
	float x[5] = { 0.0, -0.25, 0.25, -0.125, 0.125 };
	float y[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	float z[5] = { 0.5, 0.5, 0.5, 0.4, 0.4 };

	std::vector<float> points;
	points.push_back(0.0);
	while (ros::ok()) {
		points.clear();
		if (0) {

		}
		else if (t < time[0]) {
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				points.push_back(-0.5 * J * d + 1.0 * j * d);
				points.push_back(+0.5 * d * I - 1.0 * i * d);
				points.push_back(0.0);
			}
		}
		else if (t < time[1]) {
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				points.push_back(-0.5 * J * d + 1.0 * j * d);
				points.push_back(+0.5 * d * I - 1.0 * i * d);
				points.push_back(0.01 * (t - time[0]));
			}
		}
		else if (t < time[2]) {//����->��
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				int it = 3 * i * J + 3 * j;
				points.push_back(points1[it] + (points2[it] - points1[it]) * (t - time[1]) / (time[2] - time[1])); it++;
				points.push_back(points1[it] + (points2[it] - points1[it]) * (t - time[1]) / (time[2] - time[1])); it++;
				points.push_back(points1[it] + (points2[it] - points1[it]) * (t - time[1]) / (time[2] - time[1]));
			}
			t++;
		}
		else if (t < time[3]) {//������->�X����
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				int it = 3 * i * J + 3 * j;
				points.push_back(points2[it] + (points3[it] - points2[it]) * (t - time[2]) / (time[3] - time[2])); it++;
				points.push_back(points2[it] + (points3[it] - points2[it]) * (t - time[2]) / (time[3] - time[2])); it++;
				points.push_back(points2[it] + (points3[it] - points2[it]) * (t - time[2]) / (time[3] - time[2]));
			}
			t++;
		}
		else if (t < time[4]) {//��]
			float T = 0.1 * (t - time[3]);
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				points.push_back(oc[0] + r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * cos(alpha) + r * cos(PI * i / I) * sin(alpha));
				points.push_back(oc[1] + r * sin(PI * i / I) * sin(T + 2 * PI * j / J));
				points.push_back(oc[2] - r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * sin(alpha) + r * cos(PI * i / I));
			}
		}
		else if (t < time[5]) {//���n�ω�
			r = 0.3 + 0.15 * sin(0.1 * (t - time[4]));
			float T = 0.1 * (t - time[3]);
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				points.push_back(oc[0] + r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * cos(alpha) + r * cos(PI * i / I) * sin(alpha));
				points.push_back(oc[1] + r * sin(PI * i / I) * sin(T + 2 * PI * j / J));
				points.push_back(oc[2] - r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * sin(alpha) + r * cos(PI * i / I));
			}
		}
		else if (t<time[6]){//���k
			r = (0.3 + 0.15 * sin(0.1 * (t - time[4])))*(50-t+time[5])/50;
			float T = 0.1 * (t - time[3]);
			for (int i = 0; i < I; i++)for (int j = 0; j < J; j++) {
				points.push_back(oc[0] + r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * cos(alpha) + r * cos(PI * i / I) * sin(alpha));
				points.push_back(oc[1] + r * sin(PI * i / I) * sin(T + 2 * PI * j / J));
				points.push_back(oc[2] - r * sin(PI * i / I) * cos(T + 2 * PI * j / J) * sin(alpha) + r * cos(PI * i / I));
			}
		}
		else if (t == time[6]) {//�ܗ֏�����
			color.clear();
			r == 0.0;
			for (int k = 0; k < 5; k++) {
				for (int i = 0; i < L; i++) for (int j = 0; j < n; j++) {
					points.push_back( r * cos(2 * PI * j / n));
					points.push_back(0.0);
					points.push_back( oc[2] + r * sin(2 * PI * j / n));

					color.push_back(R[k]);
					color.push_back(G[k]);
					color.push_back(B[k]);
				}
			}
		}
		else if (t < time[7]) { //�~�@���S
			r = Ra * (t - time[6]) / (time[7] - time[6]);
			for (int k = 0; k < 5; k++) {
				for (int i = 0; i < L; i++) for (int j = 0; j < n; j++) {
					points.push_back((r + 0.005 * i) * cos(2 * PI * j / n));
					points.push_back(0.0);
					points.push_back(oc[2] + (r + 0.01 * i) * sin(2 * PI * j / n));
				}
			}
			t++;
		}
		else if (t < time[8]) { //�~�@�ړ�
			for (int k = 0; k < 5; k++) {
				for (int i = 0; i < L; i++) for (int j = 0; j < n; j++) {
					r = Ra + 0.002 * i;
					points.push_back(r * cos(2 * PI * j / n) + x[k] * (t - time[7]) / (time[8] - time[7]));
					points.push_back(0.0 + y[k] * (t - time[7]) / (time[8] - time[7]));
					points.push_back(oc[2] + r * sin(2 * PI * j / n) + z[k] * (t - time[7]) / (time[8] - time[7]));
				}
			}
			t++;
		}
		else if(t<time[9]) {//��]
			for (int k = 0; k < 5; k++) {
				for (int i = 0; i < L; i++) for (int j = 0; j < n; j++) {
					r = Ra + 0.002 * i;
					float X = r * cos(2 * PI * j / n) + x[k];
					float Y = 0.0 + y[k];
					float Z = oc[2] + r * sin(2 * PI * j / n) + z[k];
					float T = (t - time[8]) * 0.1;
					points.push_back(X * cos(T) + Y * sin(T));
					points.push_back(-X * sin(T) + Y * cos(T));
					points.push_back(Z + 0.1 * cos(5 * T));
				}
			}
		}
		else if(t<time[10]){
			if (t == time[9]) {
				color.clear();
				R[1] = 0; R[2] = 255; R[3] = 0; R[4] = 255;
				G[1] = 0; G[2] = 255; G[3] = 255; G[4] = 0;
				B[1] = 255; B[2] = 0; B[3] = 0; B[4] = 0;
			}
			for (int i = 0; i < I; i++) for (int j = 0; j < J; j++) if (mori[i][j] > 0) {
				points.push_back(-0.5 * J * d + 1.0 * j * d);
				points.push_back(45.0-(t-time[9]));
				points.push_back(1.0 * d * I - 1.0 * i * d);
				if (t == time[9]) {
					color.push_back(R[mori[i][j]]);
					color.push_back(G[mori[i][j]]);
					color.push_back(B[mori[i][j]]);
				}
			}

		}
		else {
			for (int i = 0; i < I; i++) for (int j = 0; j < J; j++) if (mori[i][j] > 0) {
				points.push_back(-0.5 * J * d + 1.0 * j * d - 8.0 + 0.1*((t - time[10])%160));
				points.push_back(5.0);
				points.push_back(1.0 * d * I - 1.0 * i * d);
			}
		}

		std::cout << t<<"  "<<points.size() <<"   "<< color.size() << std::endl;
		float* ptr = &points[0];
		simxCallScriptFunction(ClientID, "PointClouds", sim_scripttype_childscript, "ShowPointCloud", color.size(), &color[0], points.size(), &points[0], 0, NULL, 0, NULL, 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot);

		//ros::spinOnce();
		rate.sleep();
		t++;
	}

	std::cout << "hi" << std::endl;
	return 0;
}