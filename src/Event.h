#pragma once

#include "MyTime.h"

struct Event {
	enum EVENTS_ID{
		CLIENT_ARRIVAL = 1,
		CLIENT_SIT,
		CLIENT_WAIT,
		CLIENT_LEAVE,
		NONE
	};
	
	Time time;
	EVENTS_ID id;
	std::string	body;

	Event() : id(NONE) {}
	Event(const Time& t, EVENTS_ID i, const std::string& b) : time(t), id(i), body(b) {}

	
};