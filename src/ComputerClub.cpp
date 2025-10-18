#include "ComputerClub.h"

ComputerClub::ComputerClub(int argc, char* argv[]):
	events(), tablesCount(0), startTime(), endTime(),
	tables(), clients(), waitingQueue(), validator(nullptr),
	filename()
{
	if (argc != 2) {
		throw std::runtime_error("Ошибка получения аргументов...");
	}

	filename = argv[1];
}

void ComputerClub::Work(){
	auto parser = std::make_shared<FileParser>(filename);

	try{
		
		parser->ParseFile();

		tablesCount = parser->getTablesCount();
		startTime = parser->getStartTime();
		endTime = parser->getEndTime();
		events = parser->getEvents();
		hourlyRate = parser->getHourlyRate();
		
		if(!Validator::validateGlobalData(tablesCount, startTime, endTime, hourlyRate)){
			std::cout << "Входные данные не валидные!" << std::endl;
			return;
		}

		validator = std::make_shared<Validator>(startTime, endTime, tablesCount);

		std::cout << startTime << std::endl;

		initializeTables();
		processEvents();
		processEndOfDay();
		calculateTableRevenue();
		outputResults();

	}catch(const std::exception& ex){
		std::cout << ex.what() << std::endl;
	}
}

void ComputerClub::initializeTables(){
	tables.resize(tablesCount);
	for(int i = 0; i < tablesCount; ++i){
		tables[i].number = i + 1;
	}
}

void ComputerClub::processEvents(){
	std::sort(events.begin(), events.end(), 
				[](const Event& a, const Event& b){return a.time < b.time;});

	for(const auto& event : events){
		processEvent(event);
	}
}

void ComputerClub::processEvent(const Event& event){
	validator->setCurrentTime(event.time);

	std::cout << event.time << " " << event.id << " " << event.body << std::endl;

	switch (event.id)
	{
	case Event::EVENTS_ID::CLIENT_ARRIVAL:
		handleClientArrival(event);
		break;
	case Event::EVENTS_ID::CLIENT_SIT:
		handleClientSit(event);
		break;
	case Event::EVENTS_ID::CLIENT_WAIT:
		handleClientWait(event);
		break;
	case Event::EVENTS_ID::CLIENT_LEAVE:
		handleClientLeave(event);
		break;
	default:
		break;
	}
}

void ComputerClub::handleClientArrival(const Event& event){
	std::istringstream iss(event.body);
	std::string clientName;

	iss >> clientName;

	if(!validator->validateTime(event.time)){
		generateError(event.time, "NotOpenYet");
		return;
	}

	if(clients.find(clientName) != clients.end()){
		generateError(event.time, "YouShallNotPass");
		return;
	}

	if(!validator->validateNick(clientName)){
		generateError(event.time, "ClientUnknown");
		return;
	}

	clients[clientName] = {clientName, -1, event.time};
}

void ComputerClub::handleClientSit(const Event& event){
	std::istringstream iss(event.body);
    std::string clientName;
    int tableNumber;
    iss >> clientName >> tableNumber;

	if (clients.find(clientName) == clients.end()) {
        generateError(event.time, "ClientUnknown");
        return;
    }

	if (!validator->validateTable(tableNumber)) {
        generateError(event.time, "PlaceIsBusy");
        return;
    }

	Table& table = tables[tableNumber - 1];
    if (table.isOccupied && table.currentClient != clientName) {
        generateError(event.time, "PlaceIsBusy");
        return;
    }

	if (clients[clientName].tableNumber != -1 && clients[clientName].tableNumber != tableNumber) {
        clientLeaveTable(clientName, event.time);
    }

	clientSitAtTable(clientName, tableNumber, event.time);
}

void ComputerClub::handleClientWait(const Event& event){
	std::istringstream iss(event.body);
    std::string clientName;
    iss >> clientName;

	if (clients.find(clientName) == clients.end()) {
        generateError(event.time, "ClientUnknown");
        return;
    }
 	bool hasFreeTable = false;
    
	for (const auto& table : tables) {
        if (!table.isOccupied) {
            hasFreeTable = true;
            break;
        }
    }

	if (hasFreeTable) {
        generateError(event.time, "ICanWaitNoLonger!");
        return;
    }

	if (waitingQueue.size() >= tablesCount) {
        generateOutgoingEvent(event.time, 11, clientName);
        clients.erase(clientName);
        return;
    }

	waitingQueue.push(clientName);
}

void ComputerClub::handleClientLeave(const Event& event){
	std::istringstream iss(event.body);
	std::string clientName;
	iss >> clientName;

	if(clients.find(clientName) == clients.end()){
		generateError(event.time, "ClientUnknown");
		return;
	}

	int tableNumber = clients[clientName].tableNumber;

	clientLeaveTable(clientName, event.time);

	if(!waitingQueue.empty()){
		std::string nextClient = waitingQueue.front();
		waitingQueue.pop();

		generateOutgoingEvent(event.time, 12, nextClient + " " + std::to_string(tableNumber));
		clientSitAtTable(nextClient, tableNumber, event.time);
	}
}

void ComputerClub::clientSitAtTable(const std::string& clientName, int tableNumber, const Time& time){
	Client& client = clients[clientName];
	Table& table = tables[tableNumber - 1];

	client.tableNumber = tableNumber;
	table.isOccupied = true;
	table.currentClient = clientName;
	table.startTime = time;
}

void ComputerClub::clientLeaveTable(const std::string& clientName, const Time& time){
	Client& client = clients[clientName];

	if(client.tableNumber != -1){
		Table& table = tables[client.tableNumber - 1];
		table.isOccupied = false;

		int occupiedMinutes = time.toMinutes() - table.startTime.toMinutes();
		table.occupiedMinutes += occupiedMinutes;

		table.totalSum += calculateCost(occupiedMinutes);

		table.currentClient = std::string();
		client.tableNumber = -1;
	}

	clients.erase(clientName);
}

void ComputerClub::processEndOfDay(){
	std::vector<std::string> remainingCliens;

	for(const auto& [name, client] : clients){
		remainingCliens.push_back(name);
	}

	std::sort(remainingCliens.begin(), remainingCliens.end());

	for(const auto& clientName : remainingCliens){
		generateOutgoingEvent(endTime, 11, clientName);
		clientLeaveTable(clientName, endTime);
	}
}

void ComputerClub::calculateTableRevenue(){
	for(auto& table : tables){
		if(table.occupiedMinutes > 0){
			table.revenue = table.totalSum;
		}
	}
}

int ComputerClub::calculateCost(int minutes){
	int hours = (minutes + 59) / 60;
	return hourlyRate * hours;
}

void ComputerClub::generateOutgoingEvent(const Time& time, int id, const std::string& body){
	std::cout << time << " " << id << " " << body << std::endl;
}

void ComputerClub::generateError(const Time& time, const std::string& error){
	std::cout << time << " 13 " << error << std::endl;
}

void ComputerClub::outputResults() {
    std::cout << endTime << std::endl;
    
    for (const auto& table : tables) {
        Time occupiedTime(0, 0);
        if (table.occupiedMinutes > 0) {
            occupiedTime = Time(table.occupiedMinutes / 60, table.occupiedMinutes % 60);
        }
        std::cout << table.number << " " << table.revenue << " " << occupiedTime << std::endl;
    }
}
