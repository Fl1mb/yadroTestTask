#pragma once
#include <string>
#include <stdexcept>
#include <sstream>
#include <memory>
#include "FileParser.h"
#include "Validator.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <algorithm>

class ComputerClub {
public:
	explicit ComputerClub(int argc, char* argv[]);

	void Work();

private:
	struct Table
	{
		int number;
		int revenue;
		int occupiedMinutes = 0;
		std::string currentClient;
		Time startTime;
		bool isOccupied;
		int totalSum = 0;
	};

	struct Client
	{
		std::string name;
		int tableNumber = -1;
		Time arrivalTime;	
	};

	std::string filename;

	std::vector<Event> events;
	int tablesCount;
	Time startTime;
	Time endTime;
	int hourlyRate;

	std::vector<Table> tables;
	std::unordered_map<std::string, Client> clients;
	std::queue<std::string> waitingQueue;
	std::shared_ptr<Validator> validator;

	void initializeTables();
    void processEvents();
    void processEvent(const Event& event);
    void handleClientArrival(const Event& event);
    void handleClientSit(const Event& event);
    void handleClientWait(const Event& event);
    void handleClientLeave(const Event& event);
    void generateOutgoingEvent(const Time& time, int id, const std::string& body);
    void generateError(const Time& time, const std::string& error);
    void clientSitAtTable(const std::string& clientName, int tableNumber, const Time& time);
    void clientLeaveTable(const std::string& clientName, const Time& time);
    void processEndOfDay();
    void calculateTableRevenue();
    void outputResults();
    int calculateCost(int minutes);
};