#include "task2.h"
#include "QProcess"
using namespace std;

task2::task2(Memory* m):
    systemProcess("task2.exe", 1002, m){
    programintroduction = "This is a test task exe file.";

};

int task2::runhelper() {

    QProcess *pro = new QProcess();
    QString strPath = "C:/wmh/snake"; //需要随每个电脑变化
    pro->startDetached(strPath);
    return 1;
}

void* runHelper(void* arg) {
    task2* ts =  (task2*) arg;
    ts->runhelper();
    return 0;
}


void task2::run() {
    systemcall(runHelper, this);
}

task2::~task2(){
    task2::terminate();
}

void task2::terminate(){
    this->programintroduction = "";
    systemProcess::terminate();
}
