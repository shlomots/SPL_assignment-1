
#ifndef HM1_STUDIO_H
#define HM1_STUDIO_H

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

class Studio{
public:
    //constructors
    Studio();
    Studio(const std::string &configFilePath);
    //RULE OF FIVE
    Studio(const Studio &other);//copy constructor
    ~Studio();//destructor
    Studio& operator=(const Studio &other);//copy assignment operator
    Studio(Studio&& other);//move constructor
    Studio& operator=(Studio&& other);//move assignment operator

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void close();
    void addAction(BaseAction* baseAction);
    void clean();
    int getCustomerId();
    void deleteId(int count);

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customerId;
};


#endif //HM1_STUDIO_H
