
#include "../Include/Action.h"
#include "../Include/Studio.h"
#include "../Include/Trainer.h"

extern Studio* backup;

using namespace std;

//BaseAction
BaseAction::BaseAction() : errorMsg(),status(){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = " ERROR: " + errorMsg;
    std::cout <<errorMsg <<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList){}

void OpenTrainer::act(Studio &studio) {
    Trainer* ourTrainer = studio.getTrainer(trainerId); //this trainer is going to be deleted in closeall
    //errors
    if(studio.getNumOfTrainers() <= trainerId){
        error("Workout session does not exist or is already open");
    }
    else if(ourTrainer->isOpen()){
        error("Workout session does not exist or is already open");
    }
    //add the customers to the trainer
    else{
        ourTrainer->openTrainer();
        for (int i = 0; i < customers.size() && i < ourTrainer->getCapacity(); ++i) {
            ourTrainer->addCustomer(customers[i]->clone()); //we dont want same resources from different classes
        }
        complete();
    }
    setString();
    if(getStatus()==ERROR){//delete the unnecessary customers.
        for (int i = 0; i < customers.size(); ++i) {
            delete customers[i];
            customers.clear();
        }
    }
    studio.addAction(this);
}

void OpenTrainer::setString() { //create the string for actionLog
    actString = "open " + std::to_string(trainerId);
    for (int i = 0; i < customers.size(); ++i) {
        actString += " " + customers[i]->getName() + "," + customers[i]->toString();
    }
}

std::string OpenTrainer::toString() const {//prints what the user entered.
    std::string output = actString;
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *OpenTrainer::clone() const {
    OpenTrainer* pa1= new OpenTrainer(*this);//call the copy constructor
    return pa1;
}

//destructor
OpenTrainer::~OpenTrainer(){
    for (int i = 0; i < customers.size(); ++i) {
        delete customers[i];
    }
    customers.clear();
}


//copy constructor
//this class has resources so we need a copy constructor
OpenTrainer::OpenTrainer(const OpenTrainer &other):trainerId(other.trainerId),actString(other.actString) {
    for (int i = 0; i < other.customers.size(); ++i) {
        customers.push_back(other.customers[i]->clone());
    }
}

//Order
Order::Order(int id) : trainerId(id){}

void Order::act(Studio &studio) {
    BaseAction* ourAction= this;//pointer.
    studio.addAction(ourAction);
    Trainer* ourTrainer = studio.getTrainer(trainerId);
    if(studio.getNumOfTrainers() <= trainerId){
        error("Trainer does not exist or is not open");
    }
    else if (!ourTrainer->isOpen()){
        error("Trainer does not exist or is not open");
    }
    else{
        std::vector<Customer*> customers = ourTrainer->getCustomers();//vector with pointers will be deleted in the trainer's destructor
        std::vector<Workout> workoutList = studio.getWorkoutOptions();
        for (int i = 0; i < customers.size(); ++i) {
            ourTrainer->order(customers[i]->getId(),customers[i]->order(studio.getWorkoutOptions()),workoutList);
        }
        std::vector<OrderPair> orderList = ourTrainer->getOrders();
        std::string orders;
        for (int j = 0; j < orderList.size(); ++j) {//create the string output.
            OrderPair tmp = orderList[j];
            std::string customerName;
            for (int i = 0; i < customers.size(); ++i) {
                if (customers[i]->getId()==tmp.first){
                    customerName = customers[i]->getName();
                }
            }
            orders += customerName +" Is Doing "+ tmp.second.getName() +"\n";
        }
        std::cout<< orders;
        complete();
    }
}

std::string Order::toString() const {
    std:: string output= "order "+ to_string(trainerId);
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *Order::clone() const {
    Order* pa1 = new Order(*this);
    return pa1;
}


//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId){}

void MoveCustomer::act(Studio &studio) {
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
    Trainer* source = studio.getTrainer(srcTrainer);
    Trainer* destination = studio.getTrainer(dstTrainer);//these resources are deleted in the studio
    if (studio.getNumOfTrainers()<=srcTrainer || !source->isOpen()){//if trainer doesn't exist or isn't open.
        error("Cannot move customer");
        return;
    }
    Customer* ourCustomer;
    bool exist = false;
    for (int i = 0; i < source->getCustomers().size(); ++i) {//search for the customer
        if (source->getCustomers()[i]->getId()==id){
            exist = true;
            ourCustomer = source->getCustomers()[i];
            break;
        }
    }
    if (!exist){//if the customer doesn't exist in the source trainer.
        error("Cannot move customer");
        return;
    }
    //if destination trainer doesn't exist or he isn't open or he doesn't have the capacity.
    if (studio.getNumOfTrainers()<=dstTrainer || !destination->isOpen() || destination->getCapacity() == destination->getCustomers().size()){
        error("Cannot move customer");
        return;
    }
    destination->addCustomer(ourCustomer);//add the customer to the destination trainer.
    destination->order(id, ourCustomer->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());//also update the new customer order.
    source->removeCustomer(id);
    if (source->getCustomers().size()==0){//if source trainer is out of customers, close his session.
        Close* closeTmp = new Close(srcTrainer);//this is instance of class close and it will add the close action and print it
        closeTmp->act(studio);
    }
    complete();
}

std::string MoveCustomer::toString() const {
    std::string output = "move " + to_string(srcTrainer) +" "+ to_string(dstTrainer) +" "+ to_string(id);
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *MoveCustomer::clone() const {
    MoveCustomer* pa1 = new MoveCustomer(*this);
    return pa1;
}


//Close
Close::Close(int id) : trainerId(id){}

void Close::act(Studio &studio) {
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
    Trainer* ourTrainer = studio.getTrainer(trainerId);//pointer here will be deleted in studio
    if (studio.getNumOfTrainers() <= trainerId || !ourTrainer->isOpen()){//trainer doesn't exist or he isn't open.
        error("Trainer does not exist or is not open");
    }
    else{
        ourTrainer->closeTrainer();//close the trainer
        std::string output = "Trainer " + to_string(trainerId) +" closed. Salary "+ to_string(ourTrainer->getSalary()) +"NIS";//print what they want.
        std::cout <<output <<std::endl;
        complete();
    }
}

std::string Close::toString() const {
    std::string output= "close "  + to_string(trainerId);
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}


BaseAction *Close::clone() const {
    Close* pa1 =new Close(*this);
    return pa1;
}


//CloseAll
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    Close* closeTmp;
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
        Trainer* tmp = studio.getTrainer(i);
        if (tmp->isOpen()){
            closeTmp = new Close(i);//this is instance of class close and it will close the trainer and print what we need.
            closeTmp->act(studio);//close the trainer.
        }
        tmp = nullptr;
    }
    studio.close();
    studio.clean();
    complete();
}

std::string CloseAll::toString() const {
    std::string output="closeall";
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return "output";
}

BaseAction *CloseAll::clone() const {
    CloseAll* pa1 =new CloseAll(*this);
    return pa1;
}


//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
    std::vector<Workout> workoutOptions = studio.getWorkoutOptions();
    std::string options;
    for (int i = 0; i < workoutOptions.size(); ++i) {//go throgh the workout options and print one by one.
        std::string typeToString = typeString(workoutOptions[i].getType());//function gettype will convert the TYPE into a string to print.
        options+= workoutOptions[i].getName() +", "+ typeToString +", " + to_string(workoutOptions[i].getPrice()) +"\n";
    }
    std::cout <<options;
    complete();
}

std::string PrintWorkoutOptions::typeString(WorkoutType type) {
    if (type == ANAEROBIC){
        return "Anaerobic";
    }
    else if (type == CARDIO){
        return "Cardio";
    }
    else{
        return "Mixed";
    }
}

std::string PrintWorkoutOptions::toString() const {
    std::string output="workout_options";
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *PrintWorkoutOptions::clone() const {
    PrintWorkoutOptions* pa1 = new PrintWorkoutOptions(*this);
    return pa1;
}


//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id){}

void PrintTrainerStatus::act(Studio &studio) {
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
    Trainer* ourTrainer = studio.getTrainer(trainerId);
    std::string status;
    if (!ourTrainer->isOpen()){
        status += "Trainer " + to_string(trainerId) +" status: closed";
    }
    else{
        status += "Trainer " + to_string(trainerId) +" status: open\n" + "Customers:\n";
        std::vector<Customer*> customerList = ourTrainer->getCustomers();
        for (int i = 0; i < customerList.size(); ++i) {
            status += to_string(customerList[i]->getId()) +" "+ customerList[i]->getName() +"\n";
        }
        status += "orders:\n";
        std::vector<OrderPair> orders = ourTrainer->getOrders();
        for (int j = 0; j < orders.size(); ++j) {
            status += orders[j].second.getName() +" "+ to_string(orders[j].second.getPrice()) +"NIS "+ to_string(orders[j].first) +"\n";
        }
        status += "Current Trainer's Salary: " + to_string(ourTrainer->getSalary()) +"NIS";
    }
    std::cout <<status <<std::endl;
    complete();
}

std::string PrintTrainerStatus::toString() const {
    std::string output="status " + to_string(trainerId);
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *PrintTrainerStatus::clone() const {
    PrintTrainerStatus* pa1= new PrintTrainerStatus(*this);
    return pa1;
}


//PrintActionLog
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> actionLog = studio.getActionsLog();
    std::string output;
    for (int i = 0; i <actionLog.size() ; ++i) {//create the string output we want from the function.
        output += actionLog[i]->toString() +"\n";
    }
    std::cout<< output;
    BaseAction* ourAction= this;
    studio.addAction(ourAction);//we want the log action in, after the output string.
    complete();
}

std::string PrintActionsLog::toString() const {
    std::string output="log";
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *PrintActionsLog::clone() const {
    PrintActionsLog* pa1 = new PrintActionsLog(*this);
    return pa1;
}

//backup studio
BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
    if (backup != nullptr){
        delete backup;
        backup = nullptr;
    }
    backup = new Studio(studio); //studio's copy constructor
    complete();
}

std::string BackupStudio::toString() const {
    std::string output= "backup";
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *BackupStudio::clone() const {
    BackupStudio* pa1 = new BackupStudio(*this);
    return pa1;
}


//RestoreStudio
RestoreStudio::RestoreStudio() {};

void RestoreStudio::act(Studio &studio) {
    if(backup== nullptr){
        error("No backup available");
    }else{
        studio=*backup; //studio's copy assignment operator
        complete();
    }
    BaseAction* ourAction= this;
    studio.addAction(ourAction);
}

std::string RestoreStudio::toString() const {
    std::string output="restore";
    if (getStatus() == COMPLETED){
        output += " Completed";
    } else{
        output += getErrorMsg();
    }
    return output;
}

BaseAction *RestoreStudio::clone() const {
    RestoreStudio* pa1 = new RestoreStudio(*this);
    return pa1;
}














