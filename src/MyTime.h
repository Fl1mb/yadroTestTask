#pragma once
 
#include <string>

struct Time {
    int hours;
    int minutes;

    Time() = default;
    Time(int h, int m);
    ~Time() = default;

    int toMinutes() const;

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    
    static Time fromString(const std::string& timeStr);

    friend std::ostream& operator<<(std::ostream& os, const Time& time);
};