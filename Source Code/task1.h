#ifndef TEST_H
#define TEST_H

#include "unistd.h"
#include "header.h"

class task1 : public userProcess
{
public:
    task1(Memory* m);
    ~task1();
    int runhelper11();
    int runhelper21();
    void run();
    string programintroduction;
    void terminate();

};

void* runHelper1(void* arg);
void* runHelper2(void* arg);
#endif // TASK_H
