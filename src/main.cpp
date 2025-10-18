#include "ComputerClub.h"

int main(int argc, char* argv[]){
    auto computerClub = std::make_shared<ComputerClub>(argc, argv);
    computerClub->Work();
    return 0;
}