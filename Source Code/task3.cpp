#include "task3.h"



using namespace std;

task3::task3(Memory* m, calculator* window):
    systemProcess("Task3.cpp", 1, m){
    programintroduction = "This is a test task file, it will out put step 1 and step 2 repetitively";
    this->window3 = window;
};





void task3::run() {
    this->window3->show();
    //systemcall(runHelperT3, this);

    struct job *pjob = (struct job*) calloc(1, sizeof(struct job));
    pjob->size = sizeof (job);
    this->memory_occupied+=pjob->jobsize();
    pjob->belongProcess = this;

    this->memory->requestMemory(this,pjob, pjob->jobsize());
}

task3::~task3(){
    task3::terminate();
}

void task3::terminate(){
    this->programintroduction = "";
    systemProcess::terminate();
}
