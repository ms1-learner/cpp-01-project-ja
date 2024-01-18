#ifndef MYCAR_HPP_
#define MYCAR_HPP_ 

#include <iostream>
#include "myutils.hpp"

namespace mycar{
class MyCar
{
public:
    MyCar() : position_{1, 1}, heading_{myutils::Heading::East} {};
    const myutils::Vector2D &get_position() const & { return position_; };
    uint get_speed() const { return speed_; };
    myutils::Heading get_heading() const & { return heading_; };
    bool is_out_of_gas() const { return fuel_ == 0; };
    void put_fuel() { fuel_ = 100; };
    bool process_command(std::optional<myutils::Command> command);
    void print_info() const;

private:
    myutils::Vector2D position_;
    uint speed_{0};
    myutils::Heading heading_;
    uint fuel_{20};
};

} // namespace mycar

#endif