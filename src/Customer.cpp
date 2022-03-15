
#include "../Include/Customer.h"

//Customer
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id){}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

std::vector<int> Customer::sort(const std::vector<Workout> &workout_options) {
    //create vector of the workout id's
    std::vector<int> output;
    for (int i = 0; i < workout_options.size(); ++i) {
        output.push_back(i);
    }
    //insertion sort
    int key;
    int j;
    for (int i = 1; i < output.size(); ++i) {
        key = output[i];
        j=i-1;
        while(j>=0 && workout_options[j].getPrice()>workout_options[key].getPrice()){
            output[j+1] = output[j];
            j = j-1;
        }
        output[j+1] = key;
    }
    return output;
}


//SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name,id){}

Customer *SweatyCustomer::clone() {
    SweatyCustomer* customer = new SweatyCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> customerOrder;
    //order all the cardio activities
    for (int i = 0; i < workout_options.size(); ++i) {
        if (workout_options[i].getType()==CARDIO){
            customerOrder.push_back(i);
        }
    }
    return customerOrder;
}

std::string SweatyCustomer::toString() const {
    return "swt";
}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name,id){}

Customer *CheapCustomer::clone() {
    CheapCustomer* customer = new CheapCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    //search for the cheapest activity
    int minPrice = workout_options[0].getPrice();
    int keyMin = 0;
    for (int i = 1; i < workout_options.size(); ++i) {
        if (workout_options[i].getPrice()<minPrice){
            minPrice = workout_options[i].getPrice();
            keyMin = i;
        }
    }
    std::vector<int> customerOrder;
    customerOrder.push_back(keyMin);
    return customerOrder;
}

std::string CheapCustomer::toString() const {
    return "chp";
}

//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name,id){}

Customer *HeavyMuscleCustomer::clone() {
    HeavyMuscleCustomer* customer = new HeavyMuscleCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> customerOrder;
    std::vector<int> sortedWorkout = sort(workout_options);
    //search all the anaerobic activities from the most expensive to the cheapest
    for (int i = sortedWorkout.size()-1; i >=0; i=i-1) {
        if (workout_options[sortedWorkout[i]].getType()==ANAEROBIC){
            customerOrder.push_back(workout_options[sortedWorkout[i]].getId());
        }
    }
    return customerOrder;
}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name,id){}

Customer *FullBodyCustomer::clone() {
    FullBodyCustomer* customer = new FullBodyCustomer(this->getName(),this->getId());
    return customer;
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> customerOrder;
    std::vector<int> sortedWorkout = sort(workout_options);
    //takes the cheapest cardio activity
    for (int i = 0; i < sortedWorkout.size(); ++i) {
        if (workout_options[sortedWorkout[i]].getType()==CARDIO){
            customerOrder.push_back(workout_options[sortedWorkout[i]].getId());
            break;
        }
    }
    //then takes the most expensive mixed activity
    for (int i = sortedWorkout.size()-1; i >=0 ; i=i-1) {
        if (workout_options[sortedWorkout[i]].getType()==MIXED){
            customerOrder.push_back(workout_options[sortedWorkout[i]].getId());
            break;
        }
    }
    //takes the cheapest anaerobic activity
    for (int i = 0; i < sortedWorkout.size(); ++i) {
        if (workout_options[sortedWorkout[i]].getType()==ANAEROBIC){
            customerOrder.push_back(workout_options[sortedWorkout[i]].getId());
            break;
        }
    }
    return customerOrder;
}

std::string FullBodyCustomer::toString() const {
    return "fbd";
}
