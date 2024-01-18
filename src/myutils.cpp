#include "myutils.hpp"

namespace myutils{
Heading rotate_heading(const Heading heading, uint steps, bool is_cw){
    steps %= 4;
    if(!is_cw){
        if(steps == 1)
            steps = 3;
        else if(steps == 3)
            steps = 1;
    }

    if(steps == 0)
        return heading;
    if(steps == 1){
        switch(heading){
            case Heading::North:
                return Heading::East;
            case Heading::East:
                return Heading::South;
            case Heading::South:
                return Heading::West;
            case Heading::West:
                return Heading::North;
        }
    }
    if(steps == 2){
        switch(heading){
            case Heading::North:
                return Heading::South;
            case Heading::East:
                return Heading::West;
            case Heading::South:
                return Heading::North;
            case Heading::West:
                return Heading::East;
        }
    }
    if(steps == 3){
        switch(heading){
            case Heading::North:
                return Heading::West;
            case Heading::East:
                return Heading::North;
            case Heading::South:
                return Heading::East;
            case Heading::West:
                return Heading::South;
        }
    }

    std::cerr << "unreachable reached in rotate_heading()." << std::endl;
    return heading;
}

std::optional<Command> maybe_to_enum(const std::string & command){
    if(command == "turn-left")
        return Command::TurnLeft;
    if (command == "turn-right")
        return Command::TurnRight;
    if (command == "continue-straight")
        return Command::ContinueStraight;
    if (command == "accelerate")
        return Command::Accelerate;
    if (command == "decelerate")
        return Command::Decelerate;
    if (command == "stop")
        return Command::Stop;
    if (command == "u-turn")
        return Command::UTurn;

    return std::nullopt;
}

} // namespace myutils
