#ifndef MYMAP_HPP_
#define MYMAP_HPP_ 

#include <iostream>
#include <array>
#include <unordered_map>
#include <optional>
#include "myutils.hpp"

namespace mymap{
class MyMap
{
public:
    MyMap() = delete;

    static void print_car_on_map(const myutils::Vector2D & position, const myutils::Heading  heading);
    static void print_nearby_landmark_info(const myutils::Vector2D & position, const myutils::Heading  heading);
    static bool can_turn_right(const  myutils::Vector2D& position, const myutils::Heading heading);
    static bool can_turn_left(const myutils::Vector2D& position, const myutils::Heading heading);
    static bool can_continue_straight(const myutils::Vector2D& position, const myutils::Heading heading);
    static bool can_u_turn(const myutils::Vector2D& position, const myutils::Heading heading);
    static myutils::Vector2D get_position_ahead(const myutils::Vector2D &position, const myutils::Heading heading);
    static bool is_gas_station(const myutils::Vector2D &position);

private:
    static bool can_turn_on(const myutils::Vector2D &position, const myutils::Heading heading, uint rotation_cnt);
    static bool is_drivable(const myutils::Vector2D &pos);
    static myutils::Vector2D to_unit_vector(const myutils::Heading &heading);
    static std::optional<std::string> get_landmark_info_on(const myutils::Vector2D &position, const myutils::Heading heading, uint rotation_cnt);
    static bool is_landmark(const myutils::Vector2D &pos);

    static constexpr uint kNumCol = 9;
    static constexpr uint kNumRow = 5;
    using Map_t = std::array<std::array<uint, kNumCol>, kNumRow>;
    static constexpr Map_t map_{{{1, 1, 1, 1, 1, 1, 1, 1, 1},
                                 {1, 0, 0, 0, 0, 0, 0, 0, 4},
                                 {1, 0, 1, 2, 1, 0, 1, 0, 1},
                                 {1, 5, 0, 0, 0, 0, 0, 0, 1},
                                 {1, 1, 1, 1, 1, 1, 3, 1, 1}}};
    static std::unordered_map<uint, std::string> landmark_lookup_;
};
} // namespace mymap

#endif