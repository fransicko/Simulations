#pragma once
#include <string>

using namespace std;

struct Event {
	enum Type {
		AUTO_ARRIVAL, PED_ARRIVAL, PED_AT_BUTTON, PED_IMPATIENT, GREEN_EXPIRES, 
		YELLO_EXPIRES, RED_EXPIRES, AUTO_EXIT, PED_EXIT
	}
	Type type;
	string types[9];
  Event(Type type, int time, Thread* thread) {
      this->type = type;
      this->time = time;
      this->thread = thread;

      types[0] = "AUTO_ARRIVAL";
      types[1] = "PED_ARRIVAL";
      types[2] = "PED_AT_BUTTON";
      types[3] = "PED_IMPATIENT";
      types[4] = "GREEN_EXPIRES";
      types[5] = "YELLO_EXPIRES";
      types[6] = "RED_EXPIRES";
      types[7] = "AUTO_EXIT";
      types[8] = "PED_EXIT";
   }
    
};

struct Ped {
	double arrive_time;
	char direction; // left or right
	double speed;	
	double distance_away;
};

struct Car {
	double arrive_time;
	char direction;
	double speed;
	double distance_away;
}
