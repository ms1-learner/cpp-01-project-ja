#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <string>
#include <typeinfo>
#include <tuple>

const std::vector<std::string> VALID_COMMANDS = {"tl", "tr", "gs", "su", "sd", "stop", "end"};
constexpr int GAS_FULL_AMOUNT = 30;
constexpr int EGO_INITIAL_ROW = 5;
constexpr int EGO_INITIAL_COL = 5;
constexpr int EGO_INITIAL_VEL = 1;
constexpr int MAX_TOWN_MAP_ROW = 11;
constexpr int MAX_TOWN_MAP_COL = 11;
constexpr int MAX_SPEED_LIMIT = 3;
constexpr int MIN_SPEED_LIMIT = 0;

enum Map_object_type
{
    Ego,
    Road,
    Tokyo_Tower,
    Woven_Office,
    Gas_Station
};

enum Direction
{
    West,
    East,
    South,
    North
};

struct Position
{
    int row = 0; // Horizontal axis. East directions is positive.
    int col = 0; // Vertical axis. South directions is positive.
};

struct Display_Contents
{
    Direction direction;
    Position position;
    Map_object_type current_map_type;
    std::string closest_landmark_name;
    Position closest_landmark_pos;
};

struct Car_State
{
    int velocity = EGO_INITIAL_VEL;
    Direction direction = North;
    Position position = {EGO_INITIAL_ROW, EGO_INITIAL_COL};
    float gas_amount = GAS_FULL_AMOUNT;
};

std::vector<std::vector<Map_object_type>> init_map()
{
    const int town_rows = MAX_TOWN_MAP_ROW;
    const int town_cols = MAX_TOWN_MAP_COL;
    // Put everything as Road as initialization.
    std::vector<std::vector<Map_object_type>> town_map(town_rows, std::vector<Map_object_type>(town_cols, Road));
    town_map[EGO_INITIAL_ROW][EGO_INITIAL_COL] = Ego;
    town_map[2][6] = Tokyo_Tower;
    town_map[4][9] = Woven_Office;
    town_map[10][3] = Gas_Station;
    return town_map;
}

void display_map(const std::vector<std::vector<Map_object_type>> &town_map)
{
    for (int row = 0; row < town_map.size(); row++)
    {
        for (int col = 0; col < town_map[0].size(); col++)
        {
            switch (town_map[row][col])
            {
            case Ego:
                std::cout << "E"
                          << " ";
                break;
            case Road:
                std::cout << "-"
                          << " ";
                break;
            case Tokyo_Tower:
                std::cout << "T"
                          << " ";
                break;
            case Woven_Office:
                std::cout << "W"
                          << " ";
                break;
            case Gas_Station:
                std::cout << "G"
                          << " ";
                break;
            default:
                break;
            }
        }
        std::cout << std::endl;
    }
}

bool map_boundary_check(const Car_State &state)
{
    // Boundaries of the map.
    const int west_boundary = 0;
    const int east_boundary = MAX_TOWN_MAP_COL - 1;
    const int south_boundary = MAX_TOWN_MAP_ROW - 1;
    const int north_boundary = 0;
    if (state.position.col < west_boundary && state.direction == West)
    {
        std::cout << "You cannot exceed west boundary. Please decelerate or turn to the direction other than west." << std::endl;
        return false;
    }
    if (state.position.col > east_boundary && state.direction == East)
    {
        std::cout << "You cannot exceed east boundary. Please decelerate or turn to the direction other than east." << std::endl;
        return false;
    }
    if (state.position.row > south_boundary && state.direction == South)
    {
        std::cout << "You cannot exceed south boundary. Please decelerate or turn to the direction other than south." << std::endl;
        return false;
    }
    if (state.position.row < north_boundary && state.direction == North)
    {
        std::cout << "You cannot exceed north boundary. Please decelerate or turn to the direction other than north." << std::endl;
        return false;
    }
    return true;
}

void accelerate(Car_State &state)
{
    if (state.velocity < MAX_SPEED_LIMIT)
    {
        state.velocity++;
    }
    else
    {
        std::cout << "You have reached max speed limit. You cannot accelerate." << std::endl;
    }
}

void decelerate(Car_State &state)
{
    if (state.velocity > MIN_SPEED_LIMIT)
    {
        state.velocity--;
    }
    else
    {
        std::cout << "You have reached min speed limit. You cannot decelerate." << std::endl;
    }
}

void turn_right(Car_State &state)
{
    switch (state.direction)
    {
    case West:
        state.direction = North;
        break;
    case East:
        state.direction = South;
        break;
    case South:
        state.direction = West;
        break;
    case North:
        state.direction = East;
        break;
    default:
        break;
    }
}

void turn_left(Car_State &state)
{
    switch (state.direction)
    {
    case West:
        state.direction = South;
        break;
    case East:
        state.direction = North;
        break;
    case South:
        state.direction = East;
        break;
    case North:
        state.direction = West;
        break;
    default:
        break;
    }
}

void continue_straight(Car_State &state)
{
    Car_State next_state = state;
    bool result = true;
    switch (state.direction)
    {
    case West:
        next_state.position.col -= state.velocity;
        result = map_boundary_check(next_state);
        if (result)
        {
            state.position.col -= state.velocity;
            state.gas_amount -= state.velocity;
        }
        break;
    case East:
        next_state.position.col += state.velocity;
        result = map_boundary_check(next_state);
        if (result)
        {
            state.position.col += state.velocity;
            state.gas_amount -= state.velocity;
        }
        break;
    case South:
        next_state.position.row += state.velocity;
        result = map_boundary_check(next_state);
        if (result)
        {
            state.position.row += state.velocity;
            state.gas_amount -= state.velocity;
        }
        break;
    case North:
        next_state.position.row -= state.velocity;
        result = map_boundary_check(next_state);
        if (result)
        {
            state.position.row -= state.velocity;
            state.gas_amount -= state.velocity;
        }
        break;
    default:
        break;
    }
}

void stop(Car_State &state)
{
    state.velocity = 0;
}

void update_game(const Car_State &prev_state, Car_State &state, std::vector<std::vector<Map_object_type>> &town_map)
{
    if (prev_state.position.row == state.position.row && prev_state.position.col == state.position.col)
    {
        return;
    }
    if (town_map[prev_state.position.row][prev_state.position.col] == Ego)
    {
        town_map[prev_state.position.row][prev_state.position.col] = Road;
    }
    if (town_map[state.position.row][state.position.col] == Tokyo_Tower)
    {
        std::cout << "The ego is inside Tokyo Tower." << std::endl;
    }
    else if (town_map[state.position.row][state.position.col] == Woven_Office)
    {
        std::cout << "The ego is inside Woven Office." << std::endl;
    }
    else if (town_map[state.position.row][state.position.col] == Gas_Station)
    {
        std::cout << "The ego is inside Gas Station. Gas amount becomes full!" << std::endl;
        state.gas_amount = GAS_FULL_AMOUNT;
    }
    else
    {
        town_map[state.position.row][state.position.col] = Ego;
    }
}

void user_input_validity_check(const std::string user_input)
{
    for (auto cmd : VALID_COMMANDS)
    {
        if (cmd == user_input)
        {
            return;
        }
    }
    std::cout << "Your input "
              << "'" << user_input << "' is not valid." << std::endl;
    std::cout << "Please input one of the following valid commands." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (auto cmd : VALID_COMMANDS)
    {
        std::cout << cmd << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

void user_input_process(const std::string user_input, Car_State &state, std::vector<std::vector<Map_object_type>> &town_map)
{
    if (user_input == VALID_COMMANDS[0])
    {
        turn_left(state);
    }
    else if (user_input == VALID_COMMANDS[1])
    {
        turn_right(state);
    }
    else if (user_input == VALID_COMMANDS[2])
    {
        Car_State prev_state = state;
        continue_straight(state);
        update_game(prev_state, state, town_map);
    }
    else if (user_input == VALID_COMMANDS[3])
    {
        accelerate(state);
    }
    else if (user_input == VALID_COMMANDS[4])
    {
        decelerate(state);
    }
    else if (user_input == VALID_COMMANDS[5])
    {
        stop(state);
    }
}

void remained_gas_amount_check(const float gas_amount)
{
    if (gas_amount < GAS_FULL_AMOUNT / 2 && gas_amount > 0)
    {
        std::cout << "Gasoline is less than half. Please go to gas station as soon as possible." << std::endl;
    }
    else if (gas_amount <= 0)
    {
        std::cout << "Your gasoline is empty. Game Over." << std::endl;
        std::exit(0);
    }
}

float calc_object_distance_from_ego(const Position &ego_position, const Map_object_type &object, const std::vector<std::vector<Map_object_type>> &town_map)
{
    float distance = 0.0;
    for (int row = 0; row < town_map.size(); row++)
    {
        for (int col = 0; col < town_map[0].size(); col++)
        {
            if (town_map[row][col] == object)
            {
                distance = std::sqrt(std::pow((ego_position.row - row), 2) + std::pow((ego_position.col - col), 2));
            }
        }
    }
    return distance;
}

std::tuple<float, std::string> get_closest_landmark(const Position &ego_position, const std::vector<std::vector<Map_object_type>> &town_map)
{
    float tokyo_tower_distance = calc_object_distance_from_ego(ego_position, Tokyo_Tower, town_map);
    float woven_office_distance = calc_object_distance_from_ego(ego_position, Woven_Office, town_map);
    if (tokyo_tower_distance <= woven_office_distance)
    {
        return std::make_tuple(tokyo_tower_distance, "Tokyo Tower (T)");
    }
    return std::make_tuple(woven_office_distance, "Woven Office (W)");
}

void display_info(const Car_State &car_state, const std::vector<std::vector<Map_object_type>> &town_map)
{
    // Display direction.
    std::cout << "Ego car (E) direction: ";
    switch (car_state.direction)
    {
    case West:
        std::cout << "West, ←" << std::endl;
        break;
    case East:
        std::cout << "East, →" << std::endl;
        break;
    case South:
        std::cout << "South, ↓" << std::endl;
        break;
    case North:
        std::cout << "North, ↑" << std::endl;
        break;
    default:
        break;
    }
    // Display position.
    std::cout << "Ego car (E) position: ";
    std::cout << "row = " << car_state.position.row << ", ";
    std::cout << "col = " << car_state.position.col << std::endl;
    // Display speed.
    std::cout << "Ego car (E) speed: ";
    std::cout << car_state.velocity << std::endl;
    // Display gasoline amount.
    std::cout << "Ego car (E) remaining gasoline is ";
    std::cout << car_state.gas_amount << std::endl;
    // Display Landmark.
    std::cout << "Closest landmark is ";
    auto [distance, landmark_name] = get_closest_landmark(car_state.position, town_map);
    std::cout << landmark_name << ". ";
    std::cout << "Distance is " << distance << std::endl;
}

int main()
{
    Car_State car_state;
    std::vector<std::vector<Map_object_type>> town_map = init_map();
    while (1)
    {
        remained_gas_amount_check(car_state.gas_amount);
        display_info(car_state, town_map);
        display_map(town_map);

        std::string user_input;
        getline(std::cin, user_input);
        user_input_validity_check(user_input);
        if (user_input == VALID_COMMANDS[6])
        {
            break;
        }
        user_input_process(user_input, car_state, town_map);
    }

    return 0;
}