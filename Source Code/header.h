#include<iostream>
#include<string.h>  //memset()
#include<stdio.h> //printf()
#pragma once
#include<assert.h>  //assert()
//#include "mainwindow.h"
#include <QString>

using namespace std;

#define SIZE  20 //Table Size

//描述区块大小的情况
#define INITIALSIZE  10  //初始分区大小
#define INCREMENT  30    //分区增量

#include <pthread.h>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FCB.h"   //<-是应该include FCB吗？


//Prototype
class Memory;
class Process;

//threadpool class
struct threadpool {
    int thread_num;                   //线程池中开启线程的个数
    int queue_max_num;                //队列中最大job的个数
    struct job *head;                 //指向job的头指针
    struct job *tail;                 //指向job的尾指针
    pthread_t *pthreads;              //线程池中所有线程的pthread_t
    pthread_mutex_t mutex;            //互斥信号量
    pthread_cond_t queue_empty;       //队列为空的条件变量
    pthread_cond_t queue_not_empty;   //队列不为空的条件变量
    pthread_cond_t queue_not_full;    //队列不为满的条件变量
    int queue_cur_num;                //队列当前的job个数
    int queue_close;                  //队列是否已经关闭
    int pool_close;                   //线程池是否已经关闭
};


struct threadpool *threadpool_init(int thread_num, int queue_max_num);

int threadpool_add_job(struct job*pjob, struct threadpool *pool);

int threadpool_destroy(struct threadpool *pool);

void *threadpool_function(void *arg);





//job structure
struct job {
    void * (*callback_function)(void *arg);
    void *arg;
    struct job *next;
    Process *belongProcess;           //记录父process
    int priority;                     //小的priority排在前面
    int size;
    int jobsize();
};




//Process
enum Status {
    Ready, Running, Terminated
};

enum processType{
    Sys, User
};


class Process
{
public:
    //constructor and destructor
    Process(std::string name,int pid, Memory* m);
    virtual ~Process();

    //process start and terminate
    void virtual run();
    void virtual terminate();

    //change the status of the process
    void changeStatus(Status status);

    Status status; //status of the process
    std::string name; //name of the process

    std::vector<struct job *> jobList;  // the list that record all the jobs
    processType type; //system Process or user Process
    Memory* memory;//pointing to the memory
    threadpool* threadpool;//pointing to the thread pool


    int memory_occupied = 0; //记录使用大小
    int pid;
    //MainWindow *window; //指向更新的页面

};


// the process of the system, usually cannot be
class systemProcess : public Process{
public:

    systemProcess(std::string name,int pid, Memory* m);
    ~systemProcess();

    virtual void run();
    virtual void terminate();

    //the process
    void systemcall(void *(*callback_function)(void *arg), void *arg);
    //int systemcall(void *(*callback_function));

};

class userProcess : public Process{
public:
    userProcess(std::string name,int pid, Memory* m);


    ~userProcess();

    virtual void run();
    virtual void terminate();

    void createJob(int memoryID, int priority,  void *(*callback_function)(void *arg), void *arg);
    void createJob(int memoryID, job * pjob);
};


class Memory{
public:
    /* Class Constructor*/
    /* Usage: Memory()
     * ---------------
     * Initialize Memoey Class with pre-specified Capacity and Size
     */
    Memory();

    /* Class Destructor */
    ~Memory();

    /*
     * Function: findspace
     * Usage: int pos = findSpace(job_length)
     * ---------------------------------------
     * 传入需要的内存大小，返回找到的内存块序号
     */
    int findSpace(int job_length);

    /*
     * Function: requestMemory()
     * Usage: requestMemory(process, job_length)
     * ------------------------------------
     * 传入process和job的指针以及这个job所需要的内存大小，实现占用
     */
    void requestMemory(Process * process, job * job, int job_length);

    /*
     * Function: releaseMemory()
     * Usage: releaseMemory(process, job)
     *        releaseMemory(process)
     * ------------------------------------
     * 释放内存。如果只传入process就释放process占用的所有job的内存
     * 传入process和job就释放job占用的内存
     */

    void releaseMemory(Process * process, job * job);

    void releaseMemory(Process * process);

    void mergeWithNext(int pos);

    /*
     * For debugging
     */
    void showMemory();

    /*
     * Function: release()
     * Usage: getTable()
     * -------------------------
     * 返回以string记录的 Memorytable
     */
    QString getTable();

private:
    struct Frame{
        int ini_pos;
        char* addr;
        int size;
        bool occupy;  //0: free, 1:occupied
        Process * process;
        job * job;
    };

    struct Table {
        Frame *data;
        int size;
    };

    Table memoryTable;
};




