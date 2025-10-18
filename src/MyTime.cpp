#include "MyTime.h"

#include <iostream>
#include <iomanip>
#include <sstream>

Time::Time(int h, int m) : hours(h), minutes(m) {}

Time Time::fromString(const std::string& timeStr)
{
	Time t;
	char colon;
	std::istringstream iss(timeStr);

	iss >> t.hours >> colon >> t.minutes;
	
	return t;
}

int Time::toMinutes() const {
	return hours * 60 + minutes;
}


bool Time::operator<(const Time& other) const {
	return toMinutes() < other.toMinutes();
}

bool Time::operator>(const Time& other) const {
	return toMinutes() > other.toMinutes();
}

bool Time::operator>=(const Time& other) const {
	return toMinutes() >= other.toMinutes();
}

bool Time::operator<=(const Time& other) const {
	return toMinutes() <= other.toMinutes();
}

bool Time::operator==(const Time& other) const {
	return toMinutes() == other.toMinutes();
}

bool Time::operator!=(const Time& other) const {
	return toMinutes() != other.toMinutes();
}

std::ostream& operator<<(std::ostream& os, const Time& time){
	os << std::setw(2) << std::setfill('0') << time.hours << ":"
		<< std::setw(2) << std::setfill('0') << time.minutes;
	return os;
}



