#include "Validator.h"

Validator::Validator(const Time& start, const Time& end, int count):
    startTime(start), endTime(end), tablesCount(count)
{}

bool Validator::validateTime(const Time& time) const{
    return startTime < time && time < endTime && time >= currentTime;
}

void Validator::setCurrentTime(const Time& time){
    currentTime = time;
}


bool Validator::validateNick(const std::string& nick)const{
    for(const auto& sm : nick){
        if(!IsAvailable(sm)){
            return false;
        }
    }
    return true;
}

bool Validator::IsAvailable(char ch)const {
    for(auto i = 0; i < AV_SYM_SIZE; ++i){
        if(availableSymbols[i] == ch)return true;
    }return false;
}

bool Validator::validateTable(int table)const{
    return table > 0 && table <= tablesCount;
}

bool Validator::validateGlobalData(int tables, const Time& start, const Time& end, int hourlyRate){
    return tables > 0 && start < end && hourlyRate > 0;
}