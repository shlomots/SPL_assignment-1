#ifndef HM1_CUSTOMER_H
#define HM1_CUSTOMER_H

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone() =0;
    std::string getName() const;
    int getId() const;
    std::vector<int> sort(const std::vector<Workout> &workout_options);
    virtual ~Customer() = default;
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer( std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);//this heppens inside trainer.
    std::string toString() const;
    Customer* clone() override;
    ~SweatyCustomer() = default;
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone() override;
    ~CheapCustomer() = default;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone() override;
    ~HeavyMuscleCustomer() = default;
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone() override;
    ~FullBodyCustomer() = default;
private:
};




#endif //HM1_CUSTOMER_H
