#pragma once

#include <string>
#include "MyTime.h"
#include <stdexcept>
#include "Event.h"

class Validator{
public:

    explicit Validator(const Time& start, const Time& end, int count);

    ~Validator() = default;

    void setCurrentTime(const Time& time);

    bool validateTime(const Time& time) const;
    bool validateNick(const std::string& nick) const;
    bool validateTable(int table) const;

    static bool validateGlobalData(int tables, const Time& start, const Time& end, int hourlyRate);

private:

    Time startTime;
    Time endTime;
    Time currentTime;
    int tablesCount;

    const char*  availableSymbols = "zxcvbnmasdfghjklqwertyuiop1234567890_-";
    static const int AV_SYM_SIZE = 39;

    bool IsAvailable(char ch)const;

};