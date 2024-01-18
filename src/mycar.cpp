#include <limits>
#include "mycar.hpp"
#include "mymap.hpp"

namespace mycar{
bool MyCar::process_command(std::optional<myutils::Command> command){
    if(!command.has_value())
        return false;

    // 1. check if the command is valid for the current car state.
    // As long as the command is selected from the prompt list, this should succeed. 
    switch(command.value()){
        case myutils::Command::TurnLeft:
            if(!mymap::MyMap::can_turn_left(position_, heading_))
                return false;
            break;
        case myutils::Command::TurnRight:
            if(!mymap::MyMap::can_turn_right(position_, heading_))
                return false;
            break;
        case myutils::Command::ContinueStraight:
            if(!mymap::MyMap::can_continue_straight(position_, heading_) || (speed_ <= 0))
                return false;
            break;
        case myutils::Command::Accelerate:
            if(!mymap::MyMap::can_continue_straight(position_, heading_) || (speed_ >= std::numeric_limits<decltype(speed_)>::max()))
                return false;
            break;
        case myutils::Command::Decelerate:
        case myutils::Command::Stop:
            if(speed_ <=0)
                return false;
            break;
        case myutils::Command::UTurn:
            if(!mymap::MyMap::can_u_turn(position_, heading_))
                return false;
            break;
    }

    // 2. update heading and speed
    switch(command.value()){
        case myutils::Command::TurnLeft:
            heading_ = rotate_heading(heading_, 3U);
            break;
        case myutils::Command::TurnRight:
            heading_ = rotate_heading(heading_, 1U);
            break;
        case myutils::Command::UTurn:
            heading_ = rotate_heading(heading_, 2U);
            break;
        case myutils::Command::Accelerate:
            speed_++;
            break;
        case myutils::Command::Decelerate:
            speed_--;
            break;
        case myutils::Command::Stop:
            speed_ = 0;
            break;
        case myutils::Command::ContinueStraight:
            break;
        }

    // 3. procceed ahead
    for (size_t i = 0; i < speed_; ++i){
        if(!mymap::MyMap::can_continue_straight(position_, heading_))
            break;

        position_ = mymap::MyMap::get_position_ahead(position_, heading_);
        fuel_--;

        if(mymap::MyMap::is_gas_station(position_))
            put_fuel();
        
        if(fuel_ == 0){
            speed_ = 0;
            break;
        }
    }

    return true;
};

void MyCar::print_info()const {
    std::cout << "<Car info>" << std::endl;
    std::cout << "Position: (" << position_.x << ", " << position_.y << ")" << std::endl;
    std::cout << "Speed: " << speed_ << std::endl;
    std::cout << "Fuel: " << fuel_ << std::endl
              << std::endl;
}

} // namespace mycar
