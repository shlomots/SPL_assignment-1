
#ifndef HM1_WORKOUT_H
#define HM1_WORKOUT_H


#include <string>

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    ~Workout() = default;
private:
    const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};




#endif //HM1_WORKOUT_H
