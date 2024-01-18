#ifndef MYUTILS_HPP_
#define MYUTILS_HPP_ 

#include <iostream>
#include <optional>

namespace myutils{
template <typename T>
struct Vector2D_
{
    T x, y;
    Vector2D_() : x(0), y(0){}
    Vector2D_(T x, T y){
        this->x = x;
        this->y = y;
    }
    Vector2D_(const Vector2D_<T> & v){
        this->x = v.x;
        this->y = v.y;
    }
    Vector2D_ operator+(Vector2D_<T> && v)const {
        return Vector2D_<T>(x + v.x, y + v.y);
    }
};
using Vector2D = Vector2D_<int>;

enum class Heading
{
    North,
    East,
    South,
    West
};

enum class Command
{
    TurnLeft,
    TurnRight,
    ContinueStraight,
    Accelerate,
    Decelerate,
    Stop,
    UTurn
};

Heading rotate_heading(const Heading heading, uint steps, bool is_cw = true);

std::optional<Command> maybe_to_enum(const std::string &command);

} // namespace myutils

#endif