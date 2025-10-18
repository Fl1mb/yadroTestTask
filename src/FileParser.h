#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "Event.h"

class FileParser {
public:
	FileParser(const std::string& filename);

	void ParseFile();

	int getTablesCount() const { return tableCount; }
	Time getStartTime() const { return startTime; }
	Time getEndTime() const { return endTime; }
	int getHourlyRate() const { return hourlyRate; }
	std::vector<Event> getEvents() const { return events; }

private:
	std::string file_name;

	int tableCount;
	Time startTime;
	Time endTime;
	int hourlyRate;
	std::vector<Event> events;
};