#include "task1.h"

using namespace std;

task1::task1(Memory* m):
    userProcess("task1.cpp", 1001, m){
    programintroduction = "This is a test task file, it will out put step 1 and step 2 repetitively";

};

int task1::runhelper21() {
    cout <<  "step21 " <<endl;
    sleep(2);
    cout << "step22 " <<endl;
    return 1;
}
int task1::runhelper11() {
    cout <<  "step11" <<endl;
    sleep(2);
    cout << "step12" <<endl;
    return 1;
}

void* runHelper1(void* arg) {
    task1* ts =  (task1*) arg;
    ts->runhelper11();
    return 0;
}

void* runHelper2(void* arg) {
    task1* ts =  (task1*) arg;
    ts->runhelper21();
    return 0;
}

void task1::run() {
    createJob(1,1,runHelper1,this);
    createJob(1,1,runHelper2,this);
}

task1::~task1(){
    task1::terminate();
}

void task1::terminate(){
    this->programintroduction = "";
    userProcess::terminate();
}



