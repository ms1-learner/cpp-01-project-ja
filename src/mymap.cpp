#include <cassert>
#include "mymap.hpp"

namespace mymap{
std::unordered_map<uint, std::string> MyMap::landmark_lookup_ = {
{2, "Mountain"}, {3, "Restaurant"}, {4, "Museum"},{5, "Gas Station"},};

void MyMap::print_car_on_map(const myutils::Vector2D& position, const myutils::Heading heading)
{
    if(!is_drivable(position)){
        std::cerr << "invalid position. aborting." << std::endl;
        return;
    }

    std::cout << "<Map>"<< std::endl;
    std::cout << "  ";
    for (size_t j = 0; j < kNumCol; j++)
        std::cout << j;
    std::cout << std::endl;

    for (size_t i = 0; i < kNumRow; ++i)
    {
        std::cout << i << " ";
        for (size_t j = 0; j < kNumCol; j++)
        {
            // print car
            if (position.y == i && position.x == j)
            {
                switch (heading)
                {
                case myutils::Heading::North:
                    std::cout << "↑";
                        break;
                case myutils::Heading::East:
                    std::cout << "→";
                        break;
                case myutils::Heading::South:
                    std::cout << "↓";
                        break;
                case myutils::Heading::West:
                    std::cout << "←";
                        break;
                }
                continue;
            }

            // print map
            const auto & value = map_.at(i).at(j);
            switch (value)
            {
            case 0:
                std::cout << ' '; // drivable area
                break;
            case 1:
                std::cout << '#'; // boundary
                break;
            case 5:
                std::cout << 'G'; // gas station
                break;
            default:
                std::cout << 'L'; // landmark
            }
        }
        std::cout << std::endl;
    }
    std::cout << "#: boundary, L: landmark, G: gas station" << std::endl;
    std::cout << std::endl;
};

void MyMap::print_nearby_landmark_info(const myutils::Vector2D& position, const myutils::Heading heading)
{
    if(!is_drivable(position)){
        std::cerr << "invalid position. aborting." << std::endl;
        return;
    }


    const auto maybe_front_info= get_landmark_info_on(position, heading, 0U);
    const auto maybe_right_info = get_landmark_info_on(position, heading, 1U);
    const auto maybe_back_info = get_landmark_info_on(position, heading, 2U);
    const auto maybe_left_info = get_landmark_info_on(position, heading, 3U);
    if(maybe_front_info|| maybe_right_info || maybe_back_info || maybe_left_info){
        std::cout << "<Landmark info>" << std::endl;
    }
    if(maybe_front_info.has_value())
        std::cout << "You can see a " << maybe_front_info.value() << " ahead." << std::endl;
    if(maybe_right_info.has_value())
        std::cout << "You can see a " << maybe_right_info.value() << " on your right." << std::endl;
    if(maybe_back_info.has_value())
        std::cout << "You can see a " << maybe_back_info.value() << " on your back." << std::endl;
    if(maybe_left_info.has_value())
        std::cout << "You can see a " << maybe_left_info.value() << " on your left." << std::endl;

    std::cout << std::endl;
};

bool MyMap::can_turn_right(const myutils::Vector2D& position, const myutils::Heading heading){
    return can_turn_on(position, heading, 1U);
};

bool MyMap::can_turn_left(const myutils::Vector2D& position, const myutils::Heading heading){
    return can_turn_on(position, heading, 3U);
};

bool MyMap::can_continue_straight(const myutils::Vector2D& position, const myutils::Heading heading){
    return can_turn_on(position, heading, 0U);
};

bool MyMap::can_u_turn(const myutils::Vector2D& position, const myutils::Heading heading){
    return can_turn_on(position, heading, 2U);
};

myutils::Vector2D MyMap::get_position_ahead(const myutils::Vector2D& position, const myutils::Heading heading){
    assert(can_continue_straight(position, heading));
    return position + to_unit_vector(heading);
};

bool MyMap::is_gas_station(const myutils::Vector2D &position){
    if(!is_drivable(position)){
        std::cerr << "invalid position provided to is_gas_station()." << std::endl;
        return false;
    }
    const auto value = map_.at(position.y).at(position.x);
    if(landmark_lookup_.contains(value) && landmark_lookup_.at(value) == "Gas Station")
        return true;

    return false;
}

bool MyMap::can_turn_on(const myutils::Vector2D& position, const myutils::Heading heading, uint rotation_cnt){
    if(!is_drivable(position)){
        std::cerr << "invalid position provided to can_turn_on()." << std::endl;
        return false;
    }
    const auto target_direction = rotate_heading(heading, rotation_cnt);
    const auto target_position = position + to_unit_vector(target_direction);
    return is_drivable(target_position);
};

std::optional<std::string> MyMap::get_landmark_info_on(const myutils::Vector2D &position, const myutils::Heading heading, uint rotation_cnt){
    if(!is_drivable(position)){
        std::cerr << "invalid position provided to get_landmark_info_on()." << std::endl;
        return std::nullopt;
    }
    const auto target_direction = rotate_heading(heading, rotation_cnt);
    const auto target_position = position + to_unit_vector(target_direction);
    if(is_landmark(target_position)){
        const auto key = map_.at(target_position.y).at(target_position.x);
        assert(landmark_lookup_.contains(key));
        return landmark_lookup_.at(key);
    }
    return std::nullopt;
};

bool MyMap::is_drivable(const myutils::Vector2D &pos)
{
    const auto value = map_.at(pos.y).at(pos.x);
    return pos.x >= 0 && pos.x < kNumCol && pos.y >= 0 && pos.y < kNumRow && (value == 0 || value == 5);
};

bool MyMap::is_landmark(const myutils::Vector2D &pos)
{
    return map_.at(pos.y).at(pos.x) > 1;
};

myutils::Vector2D MyMap::to_unit_vector(const myutils::Heading &heading)
{
    switch(heading){
        case myutils::Heading::North:
            return {0, -1};
        case myutils::Heading::East:
            return {1, 0};
        case myutils::Heading::South:
            return {0, 1};
        case myutils::Heading::West:
            return {-1, 0};
    }
};
}