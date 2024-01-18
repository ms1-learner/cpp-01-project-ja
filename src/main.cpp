#include <iostream>
#include <string>
#include <limits>

#include "myutils.hpp"
#include "mymap.hpp"
#include "mycar.hpp"


namespace{
void prompt_for_command(const mycar::MyCar & car){
    const auto & position = car.get_position();
    const auto heading = car.get_heading();
    std::cout << "<Command>" << std::endl;
    std::cout << "Please select and enter a command from the below list."<< std::endl;
    if(mymap::MyMap::can_turn_left(position, heading)){
        std::cout << "- turn-left" << std::endl;
    }
    if(mymap::MyMap::can_turn_right(position, heading)){
        std::cout << "- turn-right" << std::endl;
    }
    if(mymap::MyMap::can_continue_straight(position, heading)){
        const auto speed = car.get_speed();
        if (speed > 0)
            std::cout << "- continue-straight" << std::endl;
        if(speed < std::numeric_limits<decltype(speed)>::max())
            std::cout << "- accelerate" << std::endl;
    }
    if(mymap::MyMap::can_u_turn(position, heading)){
        std::cout << "- u-turn" << std::endl;
    }
    if(car.get_speed() > 0){
        std::cout << "- decelerate" << std::endl;
        std::cout << "- stop" << std::endl;
    }
    std::cout << std::endl;
}
}

int main()
{
    mycar::MyCar car;
    long long cnt{0};

    while(true){
        std::cout << "-----------" << cnt << "th iteration -------------" << std::endl;

        mymap::MyMap::print_car_on_map(car.get_position(), car.get_heading());
        mymap::MyMap::print_nearby_landmark_info(car.get_position(), car.get_heading());
        car.print_info();

        prompt_for_command(car);
        std::string command;
        std::cin >> command;
        while (!car.process_command(myutils::maybe_to_enum(command)))
        {
            std::cout << "invalid command. please select again from the above list." << std::endl;
            std::cin >> command;
        }

        if(car.is_out_of_gas()){
            std::cerr << "out of gas. aborting." << std::endl;
            break;
        }

        std::cout << std::endl;
        cnt++;
    }

    return 0;
}
