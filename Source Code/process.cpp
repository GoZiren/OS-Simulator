#include "header.h"



//Process
Process::Process(string name, int pid, Memory* m)
{
    this->pid = pid;
    this->name = name;
    changeStatus(Ready);
    this->memory = m;
    this->memory_occupied = 0;

}


Process::~Process() {
    terminate();
}

//virtual run
void Process::run(){
    changeStatus(Running);
}
//virtual terminate
void Process::terminate() {
    changeStatus(Terminated);
}

void Process::changeStatus(Status status) {
    this->status = status;
}

userProcess::userProcess(std::string name,int pid, Memory* m):
    Process(name,pid,m){
    this->type = Sys;
};


userProcess::~userProcess(){
    userProcess::terminate();
}

void userProcess::run(){
    //empty
}


void userProcess::terminate() {
    changeStatus(Terminated);
    //clear the joblist
    this->memory_occupied = 0;

    //release the memory,
    this->memory->releaseMemory(this);
    this->jobList.clear();
    //this->memory->showMemory(); //memory可能已经被清空了，在关闭程序时

    //清空指针
    this->memory = NULL;
    //this->window = NULL;
}

void userProcess::createJob(int memoryID, job * pjob) {
    assert(this->status != Terminated);
    //pjob->belongProcess = this;
    changeStatus(Running);
    threadpool_add_job(pjob, threadpool);
}

void userProcess::createJob(int memoryID, int priority, void *(*callback_function)(void *arg), void *arg) {
    assert(callback_function != NULL);
    //申请内存
    struct job *pjob = (struct job*) calloc(1, sizeof(struct job));
    pjob->size = sizeof (job);
    this->memory_occupied+=pjob->jobsize();

    pjob->callback_function = callback_function;
    pjob->arg = arg;
    pjob->priority = priority;
    pjob->belongProcess = this;


    this->memory->requestMemory(this,pjob, pjob->jobsize());

    //this->memory->showMemory();

//    this->window->updateM(this->memory->getTable());

    //在joblist中加入一个job
    jobList.push_back(pjob);
    createJob(memoryID, pjob);
}


systemProcess::systemProcess(std::string name,int pid, Memory* m)
    :Process(name,pid,m){
    this->type = Sys;
};

systemProcess::~systemProcess(){
    terminate();
}

void systemProcess::run(){
    //empty
}

void systemProcess::systemcall(void *(*callback_function)(void *arg), void *arg){
    assert(callback_function != NULL);
    //申请内存
    struct job *pjob = (struct job*) calloc(1, sizeof(struct job));
    pjob->size = sizeof (job);
    this->memory_occupied+=pjob->jobsize();

    pjob->callback_function = callback_function;
    pjob->arg = arg;
    pjob->belongProcess = this;

    this->memory->requestMemory(this,pjob, pjob->jobsize());
    jobList.push_back(pjob); //<-这里加上
    //this->memory->showMemory();

    //运行函数
    (*(pjob->callback_function))(pjob->arg);

    //释放内存
    //this->memory->releaseMemory(this,pjob);  //这里删掉


    //pjob->belongProcess->memory_occupied -= pjob->jobsize();
}

void systemProcess::terminate(){
    changeStatus(Terminated);
    //clear the joblist
    this->memory_occupied = 0;

    //release the memory,
    this->memory->releaseMemory(this);
    this->jobList.clear();
    //this->memory->showMemory();
    this->memory = NULL;

    //this->window = NULL;
}

int job::jobsize(){
    return this->size;
}
