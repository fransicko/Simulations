#include "crosswalk.h"

#include <iostream>
#include <queue>

using namespace std;

// Constants
const int B = 330; // ft width of block
const int w = 24; // ft width of crosswalk
const int S = 46; // ft width of str
const int RED = 18; // red signal, s
const int YELLOW = 8; // s
const int GREEN = 35; // s, minimal green time
const double rp = 1/20; // minute arriaval of peds per minute
const double ra = 1/15; // s arrial of cars per minute
const int L = 9; // ft length of car
const double vj = 0; //mph speed of car coming towards light
const double acc = 10; // ft/s/s acceleration of car
const double vk = 0; // ft/s pedestrian speed

const double car_start = 1288; // ft, this is the distance the car starts out when they enter
const double ped_start = 376; // ft, the location that peds spawn at

// ques and variables
int N = 0; // the total number of cars and pedstrain travels
int cur_light_time = 0;
bool button_pressed = false;
queue<Ped> peds;
queue<Car> cars;


// takes length of car, distance of car from light
bool auto_can_make(double distance) {

}

bool ped_can_make(Ped p) {
	// total distance to the other streee
	double td = S + p.distance;
	return ((p.speed * cur_light_time) <= td);
}


int main() {
	// TODO priority que for time
	// TODO priority que for distance
	// set up the stuff
	// This is the simulatin
	// while (N != 0) 
	// 	
}
