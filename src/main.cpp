#include "../Include/Studio.h"
#include <iostream>

using namespace std;

Studio* backup = nullptr;

static OpenTrainer* getOpenTrainer(std::string actString, Studio &studio){
    std::string trainerId;
    trainerId += actString[5];
    int j = 6;
    while (actString[j] != ' '){
        trainerId += actString[j];
        j++;
    }
    std::vector<Customer*> customerList;
    Customer* tmpCustomer;
    for (int i = j+1; i < actString.length(); ++i) {
        std::string name;
        while (i<actString.length() && actString[i] != ','){
            name += actString.substr(i,1);
            i++;
        }
        if (actString.substr(i+1,3) == "swt"){
            tmpCustomer = new SweatyCustomer(name,studio.getCustomerId());
        }
        if (actString.substr(i+1,3) == "chp"){
            tmpCustomer = new CheapCustomer(name, studio.getCustomerId());
        }
        if (actString.substr(i+1,3) == "mcl"){
            tmpCustomer = new HeavyMuscleCustomer(name, studio.getCustomerId());
        }
        if (actString.substr(i+1,3) == "fbd"){
            tmpCustomer = new FullBodyCustomer(name, studio.getCustomerId());
        }
        customerList.push_back(tmpCustomer);
        i = i+4; //next name
    }
    if (studio.getTrainer(stoi(trainerId))->isOpen()){
        studio.deleteId(customerList.size());
    }
    return new OpenTrainer(std::stoi(trainerId), customerList);
}

static Order* getOrder(std::string actString){
    return new Order(std::stoi(actString.substr(6)));;
}

static MoveCustomer* getMoveCustomer(std::string actString){
    std::string sTrainerId;
    sTrainerId += actString[5];
    int j = 6;
    while (actString[j] != ' '){
        sTrainerId += actString[j];
        j++;
    }
    std::string dTrainerId;
    j++;
    dTrainerId += actString[j];
    j++;
    while (actString[j] != ' '){
        dTrainerId += actString[j];
        j++;
    }
    j++;
    std::string customerId;
    customerId += actString[j];
    j++;
    for (j; j < actString.length(); ++j) {
        customerId += actString[j];
    }
    return new MoveCustomer(std::stoi(sTrainerId), std::stoi(dTrainerId), std::stoi(customerId));
}

static Close* getClose(std::string actString){
    return new Close(std::stoi(actString.substr(6)));;
}

static PrintWorkoutOptions* getPrintWorkout(){
    return new PrintWorkoutOptions();
}

static PrintTrainerStatus* getPrintTrainer(std::string actString){
    return new PrintTrainerStatus(std::stoi(actString.substr(7)));
}

static PrintActionsLog* getPrintActionsLog(){
    return new PrintActionsLog();
}

static BackupStudio* getBackup(){
    return new BackupStudio();
}

static RestoreStudio* getRestore(){
    return new RestoreStudio;
}

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();

    std::string actString;
    std::getline(cin, actString);
    while(actString != "closeall"){
        BaseAction* action;
        if (actString.substr(0,4) == "open"){
            action = getOpenTrainer(actString, studio);
        }
        else if (actString.substr(0,5) == "order"){
            action = getOrder(actString);
        }
        else if (actString.substr(0,4) == "move"){
            action = getMoveCustomer(actString);
        }
        else if (actString.substr(0,5) == "close"){
            action = getClose(actString);
        }
        else if (actString.substr(0,7) == "workout"){
            action = getPrintWorkout();
        }
        else if (actString.substr(0,6) == "status"){
            action = getPrintTrainer(actString);
        }
        else if (actString == "log"){
            action = getPrintActionsLog();
        }
        else if (actString == "backup"){
            action = getBackup();
        }
        else{
            action = getRestore();
        }

        action->act(studio);
        std::getline(cin, actString);
    }
    CloseAll closeAll;
    closeAll.act(studio); //call the studio's destructor

    if(backup!=nullptr){

        delete backup; //call the studio's destructor
        backup = nullptr;
    }


    return 0;
}
