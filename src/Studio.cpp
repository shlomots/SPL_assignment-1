
#include <vector>
#include <string>
#include "../Include/Studio.h"
#include <fstream>
#include <sstream>

Studio::Studio() : open(false), trainers(), actionsLog(), workout_options(), customerId(0){}

Studio::Studio(const std::string &configFilePath): workout_options() {
    open=true;
    customerId=0;
    //make the file to read from.
    std::ifstream file(configFilePath);
    std::string buf;
    int numOfTrainers;
    int i=0;
    int workoutId = 0;
    while(getline(file,buf)){
        if(buf[0]=='#'||buf[0]=='\0'){//if line begins with # or it's a line, go to the next line.
            continue;
        }
        else if(i==0){
            numOfTrainers= std::stoi(buf.substr(0));//get the number of trainers and turn it to an int.
        }else if(i==1){
            int pos=0;
            int end;
            while(buf[pos]!='\0'&&trainers.size()<=numOfTrainers){
                end=buf.find(',',pos);
                std::string cap= buf.substr(pos,end-pos);//create the substring needed.
                trainers.push_back(new Trainer(std::stoi(cap)));
                pos=end+1;
            }
        }else if(i>=2){
            int pos=0;
            int end;
            //find workout name
            end=buf.find(',',pos);
            std::string workoutName= buf.substr(pos,end-pos);
            pos=end+2;
            //find workout type
            end=buf.find(',',pos);
            std::string workoutType=buf.substr(pos,end-pos);
            pos=end+2;
            //find workout price
            int price=stoi(buf.substr(pos));
            WorkoutType type;
            if(workoutType== "Anaerobic"){
                type=ANAEROBIC;
            }
            else if(workoutType== "Cardio"){
                type=CARDIO;
            }
            else{
                type=MIXED;
            }
            workout_options.push_back(Workout(workoutId,workoutName,price,type));
            workoutId++;
        }
        i++;
    }
}

void Studio::start() {
    open = true;
    std::cout <<"Studio is now open!\n";
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid) {
    return trainers[tid];
}

std::vector<Workout>& Studio::getWorkoutOptions(){
    return workout_options;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

void Studio::addAction(BaseAction *baseAction) {
    actionsLog.push_back(baseAction);
}

void Studio::close() {
    open = false;
}

void Studio::clean() {
    if (this != nullptr){
        open=false;
        for (int i = 0; i < trainers.size(); ++i) {
            delete trainers[i];
        }
        trainers.clear();
        workout_options.clear();//no resources
        for (int i = 0; i < actionsLog.size(); ++i) {
            delete actionsLog[i];
        }
        actionsLog.clear();
    }
}

int Studio::getCustomerId() {
    int output = customerId;
    customerId++;
    return output;
}

void Studio::deleteId(int count) {
    customerId -=count;
}


//Copy constructor
Studio::Studio(const Studio &other):open(other.open),workout_options(other.workout_options), customerId(other.customerId){
    for (int i = 0; i < other.trainers.size() ; ++i) {
        trainers.emplace_back(new Trainer(*other.trainers[i]));  //calls the trainer's copy constructor

    }
    for (int i = 0; i < other.actionsLog.size(); ++i) {
        actionsLog.emplace_back(other.actionsLog[i]->clone());//call the right action's copy constructor
    }
}

//Copy assignment operator
Studio &Studio::operator=(const Studio &other) {
    if (this != &other){
        //first clean our studio.
        this->clean();
        //now put the new data in.
        open=other.open;
        for (int i = 0; i < other.trainers.size(); ++i) {
            Trainer* pt1 = new Trainer(*other.trainers[i]);//created address in which we put a copy of the trainer we want.
            trainers.push_back(pt1);
        }
        for (int i = 0; i < other.workout_options.size(); ++i) {
            workout_options.push_back(Workout(other.workout_options[i]));
        }
        for (int i = 0; i < other.actionsLog.size(); ++i) {
            BaseAction* pb1= other.actionsLog[i]->clone(); //
            actionsLog.push_back(pb1);
        }
    }
    return *this;
}

//destructor
Studio::~Studio() {
    this->clean();
}

//move constructor
Studio::Studio(Studio &&other): open(other.open), workout_options(std::move(other.workout_options)),
                                trainers(std::move(other.trainers)), actionsLog(std::move(other.actionsLog)) {}

//move assignment operator
Studio &Studio::operator=(Studio &&other) {
    //we delete ourselves and return what we got.
    if (this != &other){
        open=other.open;
        workout_options = std::move(other.workout_options);
        for (int i = 0; i < trainers.size(); ++i) {
            delete trainers[i];
        }
        trainers = std::move(other.trainers);
        for (int i = 0; i < actionsLog.size(); ++i) {
            delete actionsLog[i];
        }
        actionsLog = std::move(other.actionsLog);
    }
    return *this;
}


