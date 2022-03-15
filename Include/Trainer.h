#ifndef HM1_TRAINER_H
#define HM1_TRAINER_H

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    //RULE OF FIVE
    Trainer(const Trainer &other);//copy constructor
    ~Trainer();//destructor
    Trainer& operator=(const Trainer &other);//copy assignment operator
    Trainer(Trainer&& other);//move constructor
    Trainer& operator=(Trainer&& other);//move assignment operator
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void clean();
private:
    int capacity;
    bool open;
    int salary;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};

#endif //HM1_TRAINER_H
