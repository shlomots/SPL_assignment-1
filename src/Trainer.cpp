
#include <vector>
#include "../Include/Trainer.h"

//constructor
Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false), salary(0), customersList(), orderList() {}


int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if (id == customersList[i]->getId()) {
            //delete customersList[i]; //we dont need to delete from the heap, we use this function to move the customer to other trainer
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    //create a new orderpair vector, and add the activities that remain in the session.
    std::vector<OrderPair> newOrderList;
    for (int j = 0; j < orderList.size(); ++j) {
        if (orderList[j].first==id){
            salary -= orderList[j].second.getPrice(); //remove the price of this workout
        }
        else{
            newOrderList.push_back(orderList[j]);
        }
    }
    orderList = std::move(newOrderList);
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    //add the orderpair's of the customer.
    for (int i = 0; i < workout_ids.size(); ++i) {
        OrderPair tmp(customer_id,workout_options[workout_ids[i]]);
        orderList.push_back(tmp);
        salary += workout_options[workout_ids[i]].getPrice();
    }
}

void Trainer::closeTrainer() {
    if (open){
        open = false;
        while(!customersList.empty()){
            delete customersList[customersList.size()-1];
            customersList.pop_back();
        }
        while (!orderList.empty()){
            orderList.pop_back();
        }
    }
}

Customer *Trainer::getCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if(customersList[i]->getId()==id){
            return customersList[i];
        }
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

void Trainer::clean() {
    open = false;
    for (int i = 0; i < customersList.size(); ++i) {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
}

//copy constructor
Trainer::Trainer(const Trainer &other):capacity(other.capacity),open(other.open),salary(other.salary) {
    for (int i = 0; i < other.customersList.size() ; ++i) {//deep copy
        Customer* c1=other.customersList[i]->clone();
        this->customersList.emplace_back(c1);
    }
    for (int i = 0; i < other.orderList.size(); ++i) {
        this->orderList.emplace_back(other.orderList[i]);
    }
}

//destructor
Trainer::~Trainer() {
    this->clean();
}

//copy assignment operator
Trainer &Trainer::operator=(const Trainer &other) {
    if (this!= &other){
        //first clean this object
        this->clean();
        //copy the data from other
        open = other.open;
        capacity = other.capacity;
        salary = other.salary;
        for (int i = 0; i < other.customersList.size(); ++i) {
            customersList.push_back(other.customersList[i]->clone());
        }
        for (int i = 0; i < other.orderList.size(); ++i) {
            orderList.push_back(other.orderList[i]);
        }
    }
    return *this;
}

//move constructor
Trainer::Trainer(Trainer &&other) : open(other.open), capacity(other.capacity), salary(other.capacity),
customersList(std::move(other.customersList)), orderList(){
    for (int i = 0; i < other.orderList.size(); ++i) {
        orderList.push_back(other.orderList[i]);
    }
    other.orderList.clear();
}

//move assignment operator
Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other){
        open = other.open;
        capacity = other.capacity;
        salary = other.salary;
        for (int i = 0; i < customersList.size(); ++i) {
            delete customersList[i];
        }
        customersList = std::move(other.customersList);
        orderList.clear();
        for (int i = 0; i < other.orderList.size(); ++i) {
            orderList.push_back(other.orderList[i]);
        }
        other.orderList.clear();
    }
    return *this;
}












