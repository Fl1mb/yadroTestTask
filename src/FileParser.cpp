#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream> 

FileParser::FileParser(const std::string& filename) : file_name(filename)
{
}

void FileParser::ParseFile() {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + file_name);
    }

    std::string line;

    if (!std::getline(file, line)) {
        throw std::runtime_error("Файл пуст");
    }
    tableCount = std::stoi(line);

    if (!std::getline(file, line)) {
        throw std::runtime_error("Ошибка: отсутствует время работы");
    }

    size_t spacePos = line.find(' ');
    if (spacePos == std::string::npos) {
        throw std::runtime_error("Неверный формат времени работы");
    }
    
    std::string startStr = line.substr(0, spacePos);
    std::string endStr = line.substr(spacePos + 1);
    
    startTime = Time::fromString(startStr);
    endTime = Time::fromString(endStr);

    if (!std::getline(file, line)) {
        throw std::runtime_error("Ошибка: отсутствует стоимость часа");
    }
    hourlyRate = std::stoi(line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t firstSpace = line.find(' ');
        if (firstSpace == std::string::npos) continue;
        
        size_t secondSpace = line.find(' ', firstSpace + 1);
        if (secondSpace == std::string::npos) continue;

        std::string timeStr = line.substr(0, firstSpace);
        std::string eventIdStr = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        std::string body = line.substr(secondSpace + 1);

        int eventId = std::stoi(eventIdStr);
        Event event(Time::fromString(timeStr), static_cast<Event::EVENTS_ID>(eventId), body);
        events.push_back(event);
    }

    file.close();
}