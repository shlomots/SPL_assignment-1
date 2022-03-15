
#include "../Include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id),name(std::move(w_name)),price(w_price),type(w_type) {};

int Workout::getId() const {
    return id;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}
